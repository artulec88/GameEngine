#include "StdAfx.h"
#include "Mesh.h"
//#include "OBJModel.h"
#include "Math\Vector.h"
#include "Math\FloatingPoint.h"

#include "Utility\Utility.h"
#include "Utility\ILogger.h"
#include "Math\ISort.h"

#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

#ifdef MEASURE_TIME_ENABLED
#include <time.h>
#endif
#include <fstream>

using namespace Rendering;
using namespace Utility;
using namespace std;
using namespace Math;

/* static */ std::map<std::string, MeshData*> Mesh::meshResourceMap;

MeshData::MeshData(int indexSize) :
	vbo(0),
	ibo(0),
	size(indexSize)
{
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ibo);
}


MeshData::~MeshData(void)
{
	if (vbo)
	{
		glDeleteBuffers(1, &vbo);
	}
	if (ibo)
	{
		glDeleteBuffers(1, &ibo);
	}
}

Mesh::Mesh(Vertex* vertices, int verticesCount, int* indices, int indicesCount, bool calcNormalsEnabled /* = true */, GLenum mode /* = GL_TRIANGLES */) :
	fileName(),
	mode(mode),
	meshData(NULL)
{
	AddVertices(vertices, verticesCount, indices, indicesCount, calcNormalsEnabled);
}

//Mesh::Mesh(Vertex* vertices, int verticesCount, bool calcNormalsEnabled /* = true */, GLenum mode /* = GL_TRIANGLES */) :
//	fileName(),
//	mode(mode),
//	meshData(NULL)
//{
//	std::vector<Vertex> indexedVerticesVector;
//	std::vector<int> indicesVector;
//	CalcIndices(vertices, verticesCount, indexedVerticesVector, indicesVector);
//	if (indexedVerticesVector.empty())
//	{
//		LOG(Emergency, LOGPLACE, "The vector with indexed vertices is empty");
//		exit(EXIT_FAILURE);
//	}
//	if (indicesVector.empty())
//	{
//		LOG(Emergency, LOGPLACE, "The vector with vertices indices is empty");
//		exit(EXIT_FAILURE);
//	}
//	AddVertices(&indexedVerticesVector[0], indexedVerticesVector.size(), &indicesVector[0], indicesVector.size(), calcNormalsEnabled);
//}

Mesh::Mesh(const std::string& fileName, GLenum mode /* = GL_TRIANGLES */) :
	fileName(fileName),
	mode(mode),
	meshData(NULL)
{
}

Mesh::~Mesh(void)
{
	ASSERT(meshData != NULL);
	if (meshData == NULL)
	{
		LOG(Utility::Warning, LOGPLACE, "Mesh data is already NULL");
		return;
	}
	meshData->RemoveReference();
	if (! meshData->IsReferenced())
	{
		if (fileName.length() > 0)
		{
			meshResourceMap.erase(fileName);
		}
		SAFE_DELETE(meshData);
	}
}

void Mesh::Initialize()
{
	if (meshData != NULL)
	{
		LOG(Utility::Debug, LOGPLACE, "Mesh data already initialized");
		return;
	}

	if (fileName.empty())
	{
		LOG(Utility::Error, LOGPLACE, "Mesh data cannot be initialized. File name is not specified");
	}

	std::string name = fileName;
	const char *tmp = strrchr(name.c_str(), '\\');
	if (tmp != NULL)
	{
		name.assign(tmp + 1);
	}
	//std::string extension = name.substr(name.find_last_of(".") + 1);
	//LOG(Utility::Delocust, LOGPLACE, "Extension is = \"%s\"", extension.c_str());

	std::map<std::string, MeshData*>::const_iterator itr = meshResourceMap.find(fileName);
	if (itr != meshResourceMap.end()) // the mesh has been already loaded
	{
		LOG(Info, LOGPLACE, "Model \"%s\" is already loaded. Using already loaded mesh data.", name.c_str());
		meshData = itr->second;
		meshData->AddReference();
		return;
	}

#ifdef MEASURE_TIME_ENABLED
	clock_t begin = clock();
#endif

	LOG(Info, LOGPLACE, "Loading model from file \"%s\"", name.c_str());

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(fileName.c_str(),
		aiProcess_Triangulate |
		aiProcess_GenSmoothNormals |
		aiProcess_FlipUVs |
		aiProcess_CalcTangentSpace);

	if (scene == NULL)
	{
		LOG(Critical, LOGPLACE, "Error while loading a mesh \"%s\"", name.c_str());
		exit(EXIT_FAILURE);
	}
	if ((scene->mMeshes == NULL) || (scene->mNumMeshes < 1))
	{
		LOG(Emergency, LOGPLACE, "Incorrect number of meshes loaded (%d). Check the model.", scene->mNumMeshes);
		LOG(Info, LOGPLACE, "One of the possible solutions is to check whether the model has an additional line at the end");
		exit(EXIT_FAILURE);
	}

	const aiMesh* model = scene->mMeshes[0];
	std::vector<Vertex> vertices;
	std::vector<int> indices;

	const aiVector3D aiZeroVector(0.0f, 0.0f, 0.0f);
	for (unsigned int i = 0; i < model->mNumVertices; ++i)
	{
		const aiVector3D* pPos = &(model->mVertices[i]);
		const aiVector3D* pNormal = &(model->mNormals[i]);
		const aiVector3D* pTexCoord = model->HasTextureCoords(0) ? &(model->mTextureCoords[0][i]) : &aiZeroVector;
		const aiVector3D* pTangent = model->HasTangentsAndBitangents() ? &(model->mTangents[i]) : &aiZeroVector;
		if (pTangent == NULL)
		{
			LOG(Error, LOGPLACE, "Tangent calculated incorrectly");
			pTangent = &aiZeroVector;
		}

		Math::Vector3D vertexPos(pPos->x, pPos->y, pPos->z);
		Math::Vector2D vertexTexCoord(pTexCoord->x, pTexCoord->y);
		Math::Vector3D vertexNormal(pNormal->x, pNormal->y, pNormal->z);
		Math::Vector3D vertexTangent(pTangent->x, pTangent->y, pTangent->z);

		Vertex vertex(vertexPos, vertexTexCoord, vertexNormal, vertexTangent);
		vertices.push_back(vertex);
	}

	for (unsigned int i = 0; i < model->mNumFaces; ++i)
	{
		const aiFace& face = model->mFaces[i];
		ASSERT(face.mNumIndices == 3);
		indices.push_back(face.mIndices[0]);
		indices.push_back(face.mIndices[1]);
		indices.push_back(face.mIndices[2]);
	}
	SavePositions(vertices); // used by TerrainMesh to save the positions. For Mesh instances it does nothing as it is not necessary to store these positions
	AddVertices(&vertices[0], vertices.size(), (int*)&indices[0], indices.size(), false);

	meshResourceMap.insert(std::pair<std::string, MeshData*>(fileName, meshData));

#ifdef MEASURE_TIME_ENABLED
	clock_t end = clock();
	LOG(Info, LOGPLACE, "Loading a model took %.2f [ms]", 1000.0 * static_cast<double>(end - begin) / (CLOCKS_PER_SEC));
#endif
}

void Mesh::AddVertices(Vertex* vertices, int verticesCount, const int* indices, int indicesCount, bool calcNormalsEnabled /* = true */)
{
	meshData = new MeshData(indicesCount);

	if (calcNormalsEnabled)
	{
		this->CalcNormals(vertices, verticesCount, indices, indicesCount);
	}
	//if (calcTangents)
	//{
	//	this->CalcTangents(vertices, verticesCount);
	//}

	ASSERT(meshData != NULL);
	if (meshData == NULL)
	{
		LOG(Critical, LOGPLACE, "Mesh data instance is NULL");
		exit(EXIT_FAILURE);
	}
	glBindBuffer(GL_ARRAY_BUFFER, meshData->GetVBO());
	glBufferData(GL_ARRAY_BUFFER, verticesCount * sizeof(Vertex), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshData->GetIBO());
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesCount * sizeof(int), indices, GL_STATIC_DRAW);
}

//void Mesh::CalcIndices(Vertex* vertices, int verticesCount, std::vector<Vertex>& indexedVertices, std::vector<int>& indices) const
//{
//	/**
//	 * TODO: Improve this code as described here:
//	 * http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-9-vbo-indexing/
//	 * and in the Tutorial9 solution.
//	 */
//	for (int i = 0; i < verticesCount; ++i)
//	{
//		int index;
//		bool found = GetSimilarVertexIndex(vertices[i], indexedVertices, index);
//		if (found)
//		{
//			indices.push_back(index);
//		}
//		else
//		{
//			indexedVertices.push_back(Vertex(vertices[i]));
//			indices.push_back(static_cast<int>(indexedVertices.size() - 1));
//		}
//	}
//}

//bool Mesh::GetSimilarVertexIndex(const Vertex& vertex, const std::vector<Vertex>& indexedVertices, int& index) const
//{
//	// Lame linear search
//	for (unsigned int i = 0; i < indexedVertices.size(); ++i)
//	{
//		if (vertex == indexedVertices[i])
//		{
//			index = i;
//			return true;
//		}
//	}
//
//	// No other vertex could be used instead.
//	// Looks like we'll have to add it to the VBO.
//	return false;
//}

void Mesh::Draw() const
{
	ASSERT(meshData != NULL);
	if (meshData == NULL)
	{
		LOG(Critical, LOGPLACE, "Mesh data instance is NULL");
		exit(EXIT_FAILURE);
	}

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);

	glBindBuffer(GL_ARRAY_BUFFER, meshData->GetVBO());
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)sizeof(Vector3D));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(sizeof(Vector3D) + sizeof(Vector2D)));
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(sizeof(Vector3D) + sizeof(Vector2D) + sizeof(Vector3D)));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshData->GetIBO());
	glDrawElements(mode /* GL_TRIANGLES / GL_LINES */, meshData->GetSize(), GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
}

void Mesh::CalcNormals(Vertex* vertices, int verticesCount, const int* indices, int indicesCount) const
{
	// TODO: The value 3 for iterationStep works ok only for mode equal to GL_TRIANGLES.
	// For different modes (GL_QUADS, GL_LINES) this iterationStep variable will be incorrect
	const int iterationStep = 3; // we are iterating through faces which are triangles (each triangle has 3 vertices)
	for(int i = 0; i < indicesCount; i += iterationStep)
	{
		int i0 = indices[i];
		int i1 = indices[i + 1];
		int i2 = indices[i + 2];
		
		Vector3D v1 = vertices[i1].pos - vertices[i0].pos;
		Vector3D v2 = vertices[i2].pos - vertices[i0].pos;
		Vector3D normalVec = v1.Cross(v2).Normalized();
		
		vertices[i0].normal += normalVec;
		vertices[i1].normal += normalVec;
		vertices[i2].normal += normalVec;
	}
	
	for(int i = 0; i < verticesCount; i++)
	{
		vertices[i].normal = vertices[i].normal.Normalized();
	}
}

void Mesh::CalcTangents(Vertex* vertices, int verticesCount) const
{
	// TODO: The value 3 for iterationStep works ok only for mode equal to GL_TRIANGLES.
	// For different modes (Gl_QUADS, GL_LINES) this iterationStep variable will be incorrect
	const int iterationStep = 3; // each face has three vertices
	for (int i = 0; i < verticesCount - iterationStep + 1; i += iterationStep)
	{
		// shortcut for vertices
		Vertex& v0 = vertices[i];
		Vertex& v1 = vertices[i + 1];
		Vertex& v2 = vertices[i + 2];

		// Edges of the triangle: position delta
		Math::Vector3D deltaPos1 = v1.pos - v0.pos;
		Math::Vector3D deltaPos2 = v2.pos - v0.pos;

		// UV delta
		Math::Vector2D deltaUV1 = v1.texCoord - v0.texCoord;
		Math::Vector2D deltaUV2 = v2.texCoord - v0.texCoord;

		Math::Real r = static_cast<Math::Real>(1.0f) / (deltaUV1.GetX() * deltaUV2.GetY() - deltaUV1.GetY() * deltaUV2.GetX());
		Math::Vector3D tangent = (deltaPos1 * deltaUV2.GetY() - deltaPos2 * deltaUV1.GetY()) * r;
		Math::Vector3D bitangent = (deltaPos2 * deltaUV1.GetX() - deltaPos1 * deltaUV2.GetX()) * r;

		// Set the same tangent for all three vertices of the triangle. They will be merged later, during indexing.
		v0.tangent = tangent;
		v1.tangent = tangent;
		v2.tangent = tangent;

		// Set the same bitangent for all three vertices of the triangle. They will be merged later, during indexing.
		//v0.bitangent = bitangent;
		//v1.bitangent = bitangent;
		//v2.bitangent = bitangent;
	}

	// See "http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-13-normal-mapping/#Going_further"
	//for (int i = 0; i < verticesCount; ++i)
	//{
	//	Math::Vector3D& normal = vertices[i].normal;
	//	Math::Vector3D& tangent = vertices[i].tangent;
	//	//Math::Vector3D& bitangent = vertices[i].bitangent;

	//	// Gram-Schmidt orthogonalization
	//	tangent -= normal * normal.Dot(tangent);
	//	tangent.Normalize();

	//	// Calculate handedness
	//	//Math::Vector3D cross = normal.Cross(tangent);
	//	//if (cross.Dot(bitangent) < 0)
	//	//{
	//	//	tangent.Negate();
	//	//}
	//}
}

TerrainMesh::TerrainMesh(const std::string& fileName, GLenum mode /* = GL_TRIANGLES */) :
	Mesh(fileName, mode),
	positions(NULL),
	positionsCount(0),
	lastX(REAL_ZERO),
	lastY(REAL_ZERO),
	lastZ(REAL_ZERO)
#ifdef HEIGHTMAP_SORT_TABLE
	,lastClosestPositionIndex(0)
#elif defined HEIGHTMAP_KD_TREE
	,kdTree(NULL)
#endif
{
}

TerrainMesh::~TerrainMesh(void)
{
	SAFE_DELETE_JUST_TABLE(positions);
#ifdef HEIGHTMAP_KD_TREE
	SAFE_DELETE(kdTree);
#endif
}

/**
 * Performs the k-NN search in the 2-dimensional space in order to find the k closest points to the given point (xz).
 * See also: http://en.wikipedia.org/wiki/Nearest_neighbor_search
 */
Math::Real TerrainMesh::GetHeightAt(const Math::Vector2D& xz)
{
//#ifdef MEASURE_TIME_ENABLED
//	clock_t begin = clock();
//#endif
#ifdef HEIGHTMAP_BRUTE_FORCE
	if (AlmostEqual(xz.GetX(), lastX) && AlmostEqual(xz.GetY() /* in this case GetY() returns Z */, lastZ))
	{
		return lastY;
	}

	/**
	 * TODO: Calculate the y value. Add additional parameter to GetHeightAt function which indicates the interpolation method.
	 * This interpolation method would be used for situations when the pair (x, z) is not found in the this->positions table.
	 */
	/**
	 * TODO: Optimization!!!
	 */
	const int SAMPLES = 4;
	Math::Vector3D closestPositions [SAMPLES];
	Math::Real closestPositionsDistances [SAMPLES];
	for (int i = 0; i < SAMPLES; ++i)
	{
		closestPositions[i].SetX(REAL_MAX);
		closestPositions[i].SetY(REAL_MAX);
		closestPositions[i].SetZ(REAL_MAX);
		closestPositionsDistances[i] = (closestPositions[i].GetXZ() - xz).LengthSquared();
	}
	Math::Real y = REAL_ZERO;
	bool foundPerfectMatch = false;
	for (int i = 0; i < positionsCount; ++i)
	{
		//if (i % 10000 == 0)
		//{
		//	LOG(Utility::Debug, LOGPLACE, "i = %d", i);
		//}


		/**
		 * Checking the closestPositions table and adding new position if distance(positions[i], (x, z)) < closestPositions[SAMPLES]
		 */
		Math::Real distance = (positions[i].GetXZ() - xz).LengthSquared();
		//LOG(Utility::Info, LOGPLACE, "distance = %.2f", distance);
		if (AlmostEqual(distance, REAL_ZERO))
		{
			y = positions[i].GetY();
			foundPerfectMatch = true;
			break;
		}
		int index = SAMPLES;
		for (; index > 0; --index)
		{
			if (distance >= closestPositionsDistances[index - 1])
			{
				break;
			}
		}
		//LOG(Utility::Info, LOGPLACE, "index = %d", index);
		if (index < SAMPLES)
		{
			for (int j = SAMPLES - 1; j > index; --j)
			{
				closestPositionsDistances[j] = closestPositionsDistances[j - 1];
				closestPositions[j] = closestPositions[j - 1];
			}
			closestPositionsDistances[index] = distance;
			closestPositions[index] = positions[i];
		}
	}

	//LOG(Utility::Info, LOGPLACE, "closestPositions = { %s, %s, %s, %s }", closestPositions[0].ToString().c_str(),
	//	closestPositions[1].ToString().c_str(), closestPositions[2].ToString().c_str(), closestPositions[3].ToString().c_str());

	if (!foundPerfectMatch)
	{
		y = REAL_ZERO;
		for (int i = 0; i < SAMPLES; ++i)
		{
			y += closestPositions[i].GetY();
		}
		y /= static_cast<Math::Real>(SAMPLES);
	}

	lastX = xz.GetX();
	lastZ = xz.GetY(); // in this case GetY() returns Z

	y += 2.0f; // head position
	lastY = y;

	//LOG(Utility::Notice, LOGPLACE, "Height %.2f returned for position \"%s\"", y, xz.ToString().c_str());
#elif defined HEIGHTMAP_SORT_TABLE
	if (AlmostEqual(xz.GetX(), lastX) && AlmostEqual(xz.GetY() /* in this case GetY() returns Z */, lastZ))
	{
		return lastY;
	}

	/**
	 * TODO: Calculate the y value. Add additional parameter to GetHeightAt function which indicates the interpolation method.
	 * This interpolation method would be used for situations when the pair (x, z) is not found in the this->positions table.
	 */
	/**
	 * TODO: Optimization!!!
	 */
	const int SAMPLES = 4;
	Math::Vector3D closestPositions [SAMPLES];
	Math::Real closestPositionsDistances [SAMPLES];
	for (int i = 0; i < SAMPLES; ++i)
	{
		closestPositions[i].SetX(REAL_MAX);
		closestPositions[i].SetY(REAL_MAX);
		closestPositions[i].SetZ(REAL_MAX);
		closestPositionsDistances[i] = (closestPositions[i].GetXZ() - xz).LengthSquared();
	}
	Math::Real y = REAL_ZERO;

	int lowIndex = lastClosestPositionIndex, highIndex = lastClosestPositionIndex + 1;
	while (true)
	{
		Math::Real distanceForLowIndex = positions[lowIndex].LengthSquared();
		Math::Real distanceForHighIndex = positions[highIndex].LengthSquared();
		--lowIndex;
		++highIndex;
	}

	lastX = xz.GetX();
	lastZ = xz.GetY(); // in this case GetY() returns Z

	y += 1.5f; // head position adjustment
	lastY = y;

	//LOG(Utility::Notice, LOGPLACE, "Height %.2f returned for position \"%s\"", y, xz.ToString().c_str());
#endif

//#ifdef MEASURE_TIME_ENABLED
//	clock_t end = clock();
//	LOG(Info, LOGPLACE, "Camera's height calculation took %.2f [us]", 1000000.0 * static_cast<double>(end - begin) / (CLOCKS_PER_SEC));
//#endif

	return y;
}

void TerrainMesh::SavePositions(const std::vector<Vertex>& vertices)
{
#ifdef HEIGHTMAP_BRUTE_FORCE
	positionsCount = vertices.size();
	LOG(Utility::Info, LOGPLACE, "Terrain consists of %d positions", positionsCount);
	positions = new Math::Vector3D[positionsCount];
	for (unsigned int i = 0; i < positionsCount; ++i)
	{
		positions[i] = vertices[i].pos;
	}
#elif defined HEIGHTMAP_SORT_TABLE
	LOG(Utility::Info, LOGPLACE, "Terrain consists of %d positions", vertices.size());
	std::vector<Math::Vector3D> uniquePositions;
	for (unsigned int i = 0; i < vertices.size(); ++i)
	{
		bool isPositionUnique = true;
		for (unsigned int j = 0; j < uniquePositions.size(); ++j)
		{
			if (uniquePositions[j] == vertices[i].pos)
			{
				isPositionUnique = false;
				break;
				//LOG(Utility::Emergency, LOGPLACE, "Positions %d and %d are equal (%s == %s)",
				//	i, j, positions[i].ToString().c_str(), positions[j].ToString().c_str());
			}
		}
		if (isPositionUnique)
		{
			uniquePositions.push_back(vertices[i].pos);
		}
	}

	//std::sort

	ISort::GetSortingObject(ISort::QUICK_SORT)->Sort(&uniquePositions[0], uniquePositions.size(), COMPONENT_X);

	positionsCount = uniquePositions.size();;
	LOG(Utility::Info, LOGPLACE, "Terrain consists of %d unique positions", positionsCount);
	positions = new Math::Vector3D[positionsCount];
	for (unsigned int i = 0; i < positionsCount; ++i)
	{
		positions[i] = uniquePositions[i];
	}
#elif defined HEIGHTMAP_KD_TREE
	LOG(Utility::Info, LOGPLACE, "Terrain consists of %d positions", vertices.size());
	std::vector<Math::Vector3D> uniquePositions;
	for (unsigned int i = 0; i < vertices.size(); ++i)
	{
		bool isPositionUnique = true;
		for (unsigned int j = 0; j < uniquePositions.size(); ++j)
		{
			if (uniquePositions[j] == vertices[i].pos)
			{
				isPositionUnique = false;
				break;
				//LOG(Utility::Emergency, LOGPLACE, "Positions %d and %d are equal (%s == %s)",
				//	i, j, positions[i].ToString().c_str(), positions[j].ToString().c_str());
			}
		}
		if (isPositionUnique)
		{
			uniquePositions.push_back(vertices[i].pos);
		}
	}

	//std::sort

	ISort::GetSortingObject(ISort::QUICK_SORT)->Sort(&uniquePositions[0], uniquePositions.size(), COMPONENT_X);

	positionsCount = uniquePositions.size();;
	LOG(Utility::Info, LOGPLACE, "Terrain consists of %d unique positions", positionsCount);
	positions = new Math::Vector3D[positionsCount];
	for (unsigned int i = 0; i < positionsCount; ++i)
	{
		positions[i] = uniquePositions[i];
	}
#endif

	/**
	 * TODO: Think of a nice way to store positions so that access time in GetHeightAt(x, z) is optimized. Maybe a Binary Tree? Maybe a k-d tree?
	 * Maybe find the minimum and maximum value for "Y" component of all positions and then use bucket sort (http://en.wikipedia.org/wiki/Bucket_sort)
	 * based on "Y" values?
	 */
}

void TerrainMesh::TransformPositions(const Math::Matrix4D& transformationMatrix)
{
	for (int i = 0; i < positionsCount; ++i)
	{
		std::string oldPos = positions[i].ToString();
		positions[i] = transformationMatrix * positions[i];
		if ((i % 1000 == 0) || (i == positionsCount - 1))
		{
			LOG(Utility::Info, LOGPLACE, "%d) Old position = %s. New Position = %s", i, oldPos.c_str(), positions[i].ToString().c_str());
		}
	}
	LOG(Utility::Info, LOGPLACE, "Transformation matrix = \n%s", transformationMatrix.ToString().c_str());
}