#include "StdAfx.h"
#include "Mesh.h"
//#include "CoreEngine.h"
//#include "OBJModel.h"
#include "Math\Vector.h"
#include "Math\FloatingPoint.h"
#include "stb_image.h"

#include "Utility\Utility.h"
#include "Utility\IConfig.h"
#include "Utility\ILogger.h"
#include "Utility\Time.h"
#include "Math\ISort.h"

#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

#include <unordered_set>
#include <fstream>

#ifndef MEASURE_MESH_TIME_ENABLED
#undef START_TIMER
#undef RESET_TIMER
#undef STOP_TIMER
#endif

/* static */ std::map<std::string, Rendering::MeshData*> Rendering::Mesh::meshResourceMap;

Rendering::MeshData::MeshData() :
	m_vbo(0),
	m_ibo(0),
	m_size(0)
{
	glGenBuffers(1, &m_vbo);
}

Rendering::MeshData::MeshData(GLsizei indexSize) :
	m_vbo(0),
	m_ibo(0),
	m_size(indexSize)
{
	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_ibo);
}


Rendering::MeshData::~MeshData(void)
{
	if (m_vbo)
	{
		glDeleteBuffers(1, &m_vbo);
	}
	if (m_ibo)
	{
		glDeleteBuffers(1, &m_ibo);
	}
}

Rendering::Mesh::Mesh(GLenum mode /* = GL_TRIANGLES */) :
	m_fileName(""),
	m_mode(mode),
	m_meshData(NULL)
{
}

Rendering::Mesh::Mesh(Vertex* vertices, int verticesCount, int* indices, int indicesCount, bool calcNormalsEnabled /* = true */, GLenum mode /* = GL_TRIANGLES */) :
	m_fileName(""),
	m_mode(mode),
	m_meshData(NULL)
{
	AddVertices(vertices, verticesCount, indices, indicesCount, calcNormalsEnabled);
}

Rendering::Mesh::Mesh(const std::string& fileName, GLenum mode /* = GL_TRIANGLES */) :
	m_fileName(fileName),
	m_mode(mode),
	m_meshData(NULL)
{
}

Rendering::Mesh::~Mesh(void)
{
	ASSERT(m_meshData != NULL);
	if (m_meshData == NULL)
	{
		WARNING_LOG("Destructing the mesh aborted. Mesh data is already NULL.");
	}
	m_meshData->RemoveReference();
	if (!m_meshData->IsReferenced())
	{
		if (m_fileName.length() > 0)
		{
			meshResourceMap.erase(m_fileName);
		}
		SAFE_DELETE(m_meshData);
	}
}

void Rendering::Mesh::Initialize()
{
	if (m_meshData != NULL)
	{
		DEBUG_LOG("Mesh data already initialized");
		return;
	}

	if (m_fileName.empty() || m_fileName.compare("") == 0) // TODO: Are these conditions the same?
	{
		ERROR_LOG("Mesh data cannot be initialized. File name is not specified");
	}

	std::string name = m_fileName;
	const char *tmp = strrchr(name.c_str(), '\\');
	if (tmp != NULL)
	{
		name.assign(tmp + 1);
	}
	//std::string extension = name.substr(name.find_last_of(".") + 1);
	//DELOCUST_LOG("Extension is = \"%s\"", extension.c_str());

	std::map<std::string, MeshData*>::const_iterator itr = meshResourceMap.find(m_fileName);
	if (itr != meshResourceMap.end()) // the mesh has been already loaded
	{
		INFO_LOG("Model \"%s\" is already loaded. Using already loaded mesh data.", name.c_str());
		m_meshData = itr->second;
		m_meshData->AddReference();
		return;
	}
#ifdef MEASURE_MESH_TIME_ENABLED
	Utility::Timing::Timer timer;
	timer.Start();
#endif
	INFO_LOG("Loading model from file \"%s\"", name.c_str());

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(("C:\\Users\\aosesik\\Documents\\Visual Studio 2015\\Projects\\GameEngine\\Models\\" + m_fileName).c_str(),
		aiProcess_Triangulate | /* aiProcess_FlipWindingOrder | */
		aiProcess_GenSmoothNormals |
		aiProcess_FlipUVs |
		aiProcess_CalcTangentSpace);

	CHECK_CONDITION_EXIT(scene != NULL, Utility::Critical, "Error while loading a mesh \"%s\"", name.c_str());
	CHECK_CONDITION_EXIT((scene->mMeshes != NULL) && (scene->mNumMeshes > 0), Utility::Critical,
		"Incorrect number of meshes loaded- %d- check the model \"%s\". One of the possible solutions is to check whether the model has any additional lines at the end.",
		scene->mNumMeshes, name.c_str());

	const aiMesh* model = scene->mMeshes[0];
	std::vector<Vertex> vertices;
	std::vector<Math::Vector3D> positions;
	std::vector<int> indices;

	//Math::Vector3D tangentsSum;
	const aiVector3D aiZeroVector(REAL_ZERO, REAL_ZERO, REAL_ZERO);
	for (unsigned int i = 0; i < model->mNumVertices; ++i)
	{
		const aiVector3D* pPos = &(model->mVertices[i]);
		const aiVector3D* pNormal = &(model->mNormals[i]);
		const aiVector3D* pTexCoord = model->HasTextureCoords(0) ? &(model->mTextureCoords[0][i]) : &aiZeroVector;
		const aiVector3D* pTangent = model->HasTangentsAndBitangents() ? &(model->mTangents[i]) : &aiZeroVector;
		if (pTangent == NULL)
		{
			ERROR_LOG("Tangent calculated incorrectly for the mesh file name \"%s\"", name.c_str());
			pTangent = &aiZeroVector;
		}
		//const aiVector3D* pBitangent = model->HasTangentsAndBitangents() ? &(model->mBitangents[i]) : &aiZeroVector;
		//if (pBitangent == NULL)
		//{
		//	ERROR_LOG("Bitangent calculated incorrectly");
		//	pBitangent = &aiZeroVector;
		//}

		Math::Vector3D vertexPos(pPos->x, pPos->y, pPos->z);
		positions.push_back(vertexPos);
		Math::Vector2D vertexTexCoord(pTexCoord->x, pTexCoord->y);
		Math::Vector3D vertexNormal(pNormal->x, pNormal->y, pNormal->z);
		Math::Vector3D vertexTangent(pTangent->x, pTangent->y, pTangent->z);
		//Math::Vector3D vertexBitangent(pBitangent->x, pBitangent->y, pBitangent->z);
		
		//tangentsSum += vertexTangent;

		//Vertex vertex(vertexPos, vertexTexCoord, vertexNormal, vertexTangent, vertexBitangent);
		Vertex vertex(vertexPos, vertexTexCoord, vertexNormal, vertexTangent);
		vertices.push_back(vertex);
	}

	//Math::Vector3D averageTangent = (tangentsSum / model->mNumVertices).Normalized();
	//CRITICAL_LOG("Average tangent for mesh %s = %s", name.c_str(), averageTangent.ToString().c_str());

	for (unsigned int i = 0; i < model->mNumFaces; ++i)
	{
		const aiFace& face = model->mFaces[i];
		CHECK_CONDITION_ALWAYS(face.mNumIndices == 3, Utility::Warning, "The face has %d indices when only triangle faces are supported.", face.mNumIndices);
		indices.push_back(face.mIndices[0]);
		indices.push_back(face.mIndices[1]);
		indices.push_back(face.mIndices[2]);
	}
	SavePositions(positions); // used by TerrainMesh to save the positions. For Mesh instances it does nothing as it is not necessary to store them.
	AddVertices(&vertices[0], vertices.size(), (int*)&indices[0], indices.size(), false);

	meshResourceMap.insert(std::pair<std::string, MeshData*>(m_fileName, m_meshData));

#ifdef MEASURE_MESH_TIME_ENABLED
	timer.Stop();
	INFO_LOG("Loading model took %s", timer.GetTimeSpan().ToString().c_str());
#endif
}

void Rendering::Mesh::AddVertices(Vertex* vertices, size_t verticesCount, const int* indices, size_t indicesCount, bool calcNormalsEnabled /* = true */)
{
#ifdef DELOCUST_ENABLED
	for (size_t i = 0; i < verticesCount; ++i)
	{
		DELOCUST_LOG("vertex[%d]:\n\tPos:\t%s\n\tTex:\t%s\n\tNormal:\t%s", i, vertices[i].m_pos.ToString().c_str(), vertices[i].m_texCoord.ToString().c_str(), vertices[i].m_normal.ToString().c_str());
	}
	for (size_t i = 0; i < indicesCount; ++i)
	{
		DELOCUST_LOG("index[%d]: %d", i, indices[i]);
	}
#endif
	m_meshData = new MeshData(static_cast<GLsizei>(indicesCount)); // TODO: size_t is bigger than GLsizei, so errors will come if indicesCount > 2^32.

	if (calcNormalsEnabled)
	{
		CalcNormals(vertices, verticesCount, indices, indicesCount);
	}
	//if (calcTangents)
	//{
	//	this->CalcTangents(vertices, verticesCount);
	//}

	CHECK_CONDITION_EXIT(meshData != NULL, Critical, "Mesh data instance is NULL");
	glBindBuffer(GL_ARRAY_BUFFER, m_meshData->GetVBO());
	glBufferData(GL_ARRAY_BUFFER, verticesCount * sizeof(Vertex), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_meshData->GetIBO());
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesCount * sizeof(int), indices, GL_STATIC_DRAW);
}

//void Rendering::Mesh::CalcIndices(Vertex* vertices, size_t verticesCount, std::vector<Vertex>& indexedVertices, std::vector<int>& indices) const
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

//bool Rendering::Mesh::GetSimilarVertexIndex(const Vertex& vertex, const std::vector<Vertex>& indexedVertices, int& index) const
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

void Rendering::Mesh::Draw() const
{
	CHECK_CONDITION_EXIT(meshData != NULL, Critical, "Mesh data instance is NULL");

	BindBuffers();

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0); // positions
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)sizeof(Math::Vector3D)); // texture coordinates
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(sizeof(Math::Vector3D) + sizeof(Math::Vector2D))); // normals
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(sizeof(Math::Vector3D) + sizeof(Math::Vector2D) + sizeof(Math::Vector3D))); // tangents
	//glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(sizeof(Math::Vector3D) + sizeof(Math::Vector2D) + sizeof(Math::Vector3D) + sizeof(Math::Vector3D))); // bitangents

	glDrawElements(m_mode, m_meshData->GetSize(), GL_UNSIGNED_INT, 0);

	UnbindBuffers();
}

void Rendering::Mesh::BindBuffers() const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_meshData->GetVBO());
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_meshData->GetIBO());

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	//glEnableVertexAttribArray(4);
}

void Rendering::Mesh::UnbindBuffers() const
{
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
	//glDisableVertexAttribArray(4);
}

bool Rendering::Mesh::Compare(const Mesh& mesh) const
{
	CHECK_CONDITION_RETURN(m_meshData != NULL && mesh.m_meshData != NULL, false, Utility::Error, "Cannot compare two meshes' VBOs, because mesh(-es) data is/are NULL.");
	return m_meshData->GetVBO() < mesh.m_meshData->GetVBO();
}

void Rendering::Mesh::CalcNormals(Vertex* vertices, size_t verticesCount, const int* indices, size_t indicesCount) const
{
	// TODO: The value 3 for iterationStep works ok only for mode equal to GL_TRIANGLES.
	// For different modes (GL_QUADS, GL_LINES) this iterationStep variable will be incorrect
	const int iterationStep = 3; // we are iterating through faces which are triangles (each triangle has 3 vertices)
	for(int i = 0; i < indicesCount; i += iterationStep)
	{
		int i0 = indices[i];
		int i1 = indices[i + 1];
		int i2 = indices[i + 2];
		
		Math::Vector3D v1 = vertices[i1].m_pos - vertices[i0].m_pos;
		Math::Vector3D v2 = vertices[i2].m_pos - vertices[i0].m_pos;
		Math::Vector3D normalVec = v1.Cross(v2).Normalized();
		
		vertices[i0].m_normal += normalVec;
		vertices[i1].m_normal += normalVec;
		vertices[i2].m_normal += normalVec;
	}
	
	for(int i = 0; i < verticesCount; i++)
	{
		vertices[i].m_normal = vertices[i].m_normal.Normalized();
	}
}

void Rendering::Mesh::CalcTangents(Vertex* vertices, size_t verticesCount) const
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
		Math::Vector3D deltaPos1 = v1.m_pos - v0.m_pos;
		Math::Vector3D deltaPos2 = v2.m_pos - v0.m_pos;

		// UV delta
		Math::Vector2D deltaUV1 = v1.m_texCoord - v0.m_texCoord;
		Math::Vector2D deltaUV2 = v2.m_texCoord - v0.m_texCoord;

		Math::Real r = REAL_ONE / (deltaUV1.Cross(deltaUV2));
		Math::Vector3D tangent = (deltaPos1 * deltaUV2.GetY() - deltaPos2 * deltaUV1.GetY()) * r;
		Math::Vector3D bitangent = (deltaPos2 * deltaUV1.GetX() - deltaPos1 * deltaUV2.GetX()) * r;

		// Set the same tangent for all three vertices of the triangle. They will be merged later, during indexing.
		v0.m_tangent = tangent;
		v1.m_tangent = tangent;
		v2.m_tangent = tangent;

		// Set the same bitangent for all three vertices of the triangle. They will be merged later, during indexing.
		//v0.m_bitangent = bitangent;
		//v1.m_bitangent = bitangent;
		//v2.m_bitangent = bitangent;
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

Rendering::BillboardMesh::BillboardMesh(const Math::Vector3D& worldPosition) :
	Mesh(GL_POINTS),
	m_worldPosition(worldPosition)
{
	m_meshData = new MeshData();
	glBindBuffer(GL_ARRAY_BUFFER, m_meshData->GetVBO());
	glBufferData(GL_ARRAY_BUFFER, sizeof(Math::Vector3D), &m_worldPosition, GL_STATIC_DRAW);
}

Rendering::BillboardMesh::~BillboardMesh()
{
}

void Rendering::BillboardMesh::Draw() const
{
	CHECK_CONDITION_EXIT(m_meshData != NULL, Critical, "Mesh data instance is NULL");

	glBindBuffer(GL_ARRAY_BUFFER, m_meshData->GetVBO());

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0); // world position
	glDrawArrays(m_mode, 0, 1);
	glDisableVertexAttribArray(0);
}

Rendering::GuiMesh::GuiMesh(const Math::Vector2D* positions, unsigned int positionsCount) :
	Mesh(GL_TRIANGLE_STRIP),
	m_positionsCount(positionsCount)
{
	CHECK_CONDITION_EXIT(positionsCount > 0, Utility::Error, "Cannot create a mesh. Specified number of positions is not greater than 0");
	CHECK_CONDITION_EXIT(positions != NULL, Utility::Error, "Cannot create a mesh. Specified positions array is NULL.");
	m_meshData = new MeshData(positionsCount);
	glBindBuffer(GL_ARRAY_BUFFER, m_meshData->GetVBO());
	glBufferData(GL_ARRAY_BUFFER, positionsCount * sizeof(Math::Vector2D), positions, GL_STATIC_DRAW);
}

Rendering::GuiMesh::~GuiMesh()
{
}

void Rendering::GuiMesh::Draw() const
{
	CHECK_CONDITION_EXIT(m_meshData != NULL, Critical, "Mesh data instance is NULL");

	glBindBuffer(GL_ARRAY_BUFFER, m_meshData->GetVBO());

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Math::Vector2D), (GLvoid*)0); // positions
	glDrawArrays(m_mode, 0, m_positionsCount * 2);
	glDisableVertexAttribArray(0);
}

/* static */ const float Rendering::TerrainMesh::MAX_HEIGHT = 40.0f;
/* static */ const float Rendering::TerrainMesh::MAX_PIXEL_COLOR = 255.0f;

Rendering::TerrainMesh::TerrainMesh(const std::string& fileName, GLenum mode /* = GL_TRIANGLES */) :
	Mesh(fileName, mode),
	m_x(REAL_ZERO),
	m_z(REAL_ZERO),
	m_headPositionHeightAdjustment(GET_CONFIG_VALUE("headPositionHeightAdjustment", 2.5f))
#ifdef HEIGHTMAP_KD_TREE
	,m_positions(NULL),
	m_positionsCount(0),
	m_kdTree(NULL),
	m_kdTreeSamples(GET_CONFIG_VALUE("kdTreeSamples", 8))
#elif defined HEIGHTMAP_HEIGHTS
	,m_heightMapWidth(0),
	m_heightMapHeight(0),
	m_heights(NULL)
#endif
{
}

Rendering::TerrainMesh::TerrainMesh(Math::Real gridX, Math::Real gridZ, const std::string& heightMapFileName, GLenum mode /* = GL_TRIANGLES */) :
	Mesh(mode),
	m_x(gridX),
	m_z(gridZ),
	m_headPositionHeightAdjustment(GET_CONFIG_VALUE("headPositionHeightAdjustment", 2.5f))
#ifdef HEIGHTMAP_KD_TREE
	, m_positions(NULL),
	m_positionsCount(0),
	m_kdTree(NULL),
	m_kdTreeSamples(GET_CONFIG_VALUE("kdTreeSamples", 8))
#elif defined HEIGHTMAP_HEIGHTS
	, m_heightMapWidth(0),
	m_heightMapHeight(0),
	m_heights(NULL)
#endif
{
	/* Loading heightmap begin */
	std::string name = heightMapFileName;
	const char *tmp = strrchr(heightMapFileName.c_str(), '\\');
	if (tmp != NULL)
	{
		name.assign(tmp + 1);
	}
	int heightMapWidth, heightMapHeight, bytesPerPixel;
	//unsigned char* data = stbi_load((Core::CoreEngine::GetCoreEngine()->GetTexturesDirectory() + fileName).c_str(), &x, &y, &bytesPerPixel, 4 /* req_comp */);
	unsigned char* heightMapData = stbi_load(("C:\\Users\\aosesik\\Documents\\Visual Studio 2015\\Projects\\GameEngine\\Textures\\" + heightMapFileName).c_str(),
		&heightMapWidth, &heightMapHeight, &bytesPerPixel, 1 /* we only care about one RED component for now (the heightmap is grayscale) */);
	CHECK_CONDITION_EXIT(m_heightMapData != NULL, Utility::Error, "Unable to load terrain height map from the file \"%s\"", name.c_str());
	//for (int i = 0; i < m_heightMapWidth; ++i)
	//{
	//	for (int j = 0; j < m_heightMapHeight; ++j)
	//	{
	//		CRITICAL_LOG("HeightMap[%d][%d] = %d", i, j, m_heightMapData[i * m_heightMapWidth + j]);
	//	}
	//}
	/* Loading heightmap finished */

	const int VERTEX_COUNT = heightMapHeight; // The number of vertices along each side of the single terrain tile. It is equal to the height of the height map image.
#ifdef HEIGHTMAP_KD_TREE
	m_positionsCount = VERTEX_COUNT * VERTEX_COUNT;
#else
	m_heights = new Math::Real[VERTEX_COUNT * VERTEX_COUNT];
#endif
	//const int INDICES_COUNT = 6 * (VERTEX_COUNT - 1) * (VERTEX_COUNT - 1); // The number of indices.

	const int vertexCountMinusOne = VERTEX_COUNT - 1;
	std::vector<Math::Vector3D> positions;
	std::vector<Vertex> vertices;
	for (int i = vertexCountMinusOne; i >= 0; --i)
	{
		const Math::Real iReal = static_cast<Math::Real>(i);
		for (int j = 0; j < VERTEX_COUNT; ++j)
		{
			const Math::Real jReal = static_cast<Math::Real>(j);
			Math::Real terrainHeight = GetHeightAt(j, i, heightMapData, heightMapWidth, heightMapHeight);
			Math::Vector3D position(jReal / vertexCountMinusOne * SIZE, terrainHeight, iReal / vertexCountMinusOne * SIZE);
			positions.push_back(position);
			Math::Vector2D texCoord(jReal / vertexCountMinusOne, iReal / vertexCountMinusOne);
			Math::Vector3D normal = CalculateNormal(j, i, heightMapData, heightMapWidth, heightMapHeight);
			Math::Vector3D tangent(REAL_ONE, REAL_ZERO, REAL_ZERO); // TODO: Calculate tangent
			vertices.push_back(Vertex(position, texCoord, normal,tangent));
		}
	}
	stbi_image_free(heightMapData);
	
	std::vector<int> indices;
	for (int gz = 0; gz < vertexCountMinusOne; ++gz)
	{
		for (int gx = 0; gx < vertexCountMinusOne; ++gx)
		{
			int topLeft = (gz * VERTEX_COUNT) + gx;
			int topRight = topLeft + 1;
			int bottomLeft = ((gz + 1) * VERTEX_COUNT) + gx;
			int bottomRight = bottomLeft + 1;
			indices.push_back(topLeft);
			indices.push_back(topRight);
			indices.push_back(bottomLeft);
			indices.push_back(bottomLeft);
			indices.push_back(topRight);
			indices.push_back(bottomRight);
		}
	}
	
//#ifdef DELOCUST_ENABLED
	//int i = 0;
	//for (std::vector<Vertex>::const_iterator vertexItr = vertices.begin(); vertexItr != vertices.end(); ++vertexItr, ++i)
	//{
	//	CRITICAL_LOG("vertex[%d]:\n\tPos:\t%s\n\tTex:\t%s\n\tNormal:\t%s", i, vertexItr->m_pos.ToString().c_str(), vertexItr->m_texCoord.ToString().c_str(), vertexItr->m_normal.ToString().c_str());
	//}
	//for (size_t i = 0; i < INDICES_COUNT; ++i)
	//{
	//	ERROR_LOG("index[%d]: %d", i, indices[i]);
	//}
//#endif

	SavePositions(positions);
	AddVertices(&vertices[0], vertices.size(), &indices[0], indices.size(), false);
}

Rendering::TerrainMesh::~TerrainMesh(void)
{
#ifdef HEIGHTMAP_KD_TREE
	SAFE_DELETE_JUST_TABLE(m_positions);
	SAFE_DELETE(m_kdTree);
#endif
}

Math::Real Rendering::TerrainMesh::GetHeightAt(int x, int z, unsigned char* heightMapData, int heightMapWidth, int heightMapHeight) const
{
	// TODO: Range checking
	CHECK_CONDITION_RETURN(x >= 0 && x < heightMapWidth && z >= 0 && z < heightMapHeight, REAL_ZERO,
		Utility::Error, "Cannot determine the height of the terrain on (%d, %d) position. It is out of range.", x, z);
	Math::Real height = static_cast<Math::Real>(heightMapData[x * heightMapWidth + z]);
	height = ((height / MAX_PIXEL_COLOR) - 0.5f) * 2.0f * MAX_HEIGHT;
#ifdef HEIGHTMAP_HEIGHTS
	m_heights[x * heightMapWidth + z] = height;
#endif
	return height;
}

Math::Vector3D Rendering::TerrainMesh::CalculateNormal(int x, int z, unsigned char* heightMapData, int heightMapWidth, int heightMapHeight) const
{
	Math::Real heightLeft = GetHeightAt(x - 1, z, heightMapData, heightMapWidth, heightMapHeight);
	Math::Real heightRight = GetHeightAt(x + 1, z, heightMapData, heightMapWidth, heightMapHeight);
	Math::Real heightDown = GetHeightAt(x, z - 1, heightMapData, heightMapWidth, heightMapHeight);
	Math::Real heightUp = GetHeightAt(x, z + 1, heightMapData, heightMapWidth, heightMapHeight);
	Math::Vector3D normal(heightLeft - heightRight, 2.0f, heightDown - heightUp);
	normal.Normalize();
	return normal;
}

Math::Real Rendering::TerrainMesh::GetHeightAt(const Math::Vector2D& xz, bool headPositionHeightAdjustmentEnabled /* = false */) const
{
	return GetHeightAt(xz.GetX(), xz.GetY(), headPositionHeightAdjustmentEnabled);
}

/**
 * Performs the k-NN search in the 2-dimensional space in order to find the k closest points to the given point (xz).
 * See also: http://en.wikipedia.org/wiki/Nearest_neighbor_search
 */
Math::Real Rendering::TerrainMesh::GetHeightAt(Math::Real x, Math::Real z, bool headPositionHeightAdjustmentEnabled /* = false */) const
{
#ifdef MEASURE_MESH_TIME_ENABLED
	Utility::Timing::Timer timer;
	timer.Start();
#endif
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
	Math::Vector3D closestPositions[SAMPLES];
	Math::Real closestPositionsDistances[SAMPLES];
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
		//	DEBUG_LOG("i = %d", i);
		//}


		/**
		* Checking the closestPositions table and adding new position if distance(positions[i], (x, z)) < closestPositions[SAMPLES]
		*/
		Math::Real distance = (positions[i].GetXZ() - xz).LengthSquared();
		//INFO_LOG("distance = %.2f", distance);
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
		//INFO_LOG("index = %d", index);
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

	//INFO_LOG("closestPositions = { %s, %s, %s, %s }", closestPositions[0].ToString().c_str(), closestPositions[1].ToString().c_str(),
	//	closestPositions[2].ToString().c_str(), closestPositions[3].ToString().c_str());

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

	if (headPositionHeightAdjustmentEnabled)
	{
		y += m_headPositionHeightAdjustment; // head position adjustment
	}
	lastY = y;

	//NOTICE_LOG("Height %.2f returned for position \"%s\"", y, xz.ToString().c_str());
#elif defined HEIGHTMAP_KD_TREE
	Math::Real y = m_kdTree->SearchNearestValue(x, z);
	if (headPositionHeightAdjustmentEnabled)
	{
		y += m_headPositionHeightAdjustment; // head position adjustment
	}
	//DEBUG_LOG("Height %.2f returned for position \"%s\"", y, xz.ToString().c_str());
#elif defined HEIGHTMAP_HEIGHTS
	Math::Real y = m_heights[0];
#endif

#ifdef MEASURE_MESH_TIME_ENABLED
	timer.Stop();
	DEBUG_LOG("Camera's height calculation took %s", timer.GetTimeSpan(Utility::Timing::MICROSECOND).ToString().c_str());
#endif

	return y;
}

/**
 * TODO: See this page for a possible ways to optimize this function
 * (http://stackoverflow.com/questions/1041620/whats-the-most-efficient-way-to-erase-duplicates-and-sort-a-vector)
 */
void Rendering::TerrainMesh::SavePositions(const std::vector<Math::Vector3D>& positions)
{
#ifdef HEIGHTMAP_KD_TREE
#ifdef MEASURE_MESH_TIME_ENABLED
	clock_t begin = clock(); // TODO: Replace with Utility::Timer
#endif
	DEBUG_LOG("Terrain consists of %d positions", positions.size());
	std::unordered_set<Math::Vector3D> verticesSet;
	for (unsigned int i = 0; i < positions.size(); ++i)
	{
		verticesSet.insert(positions[i]);
	}
	std::vector<Math::Vector3D> uniquePositions;
	uniquePositions.assign(verticesSet.begin(), verticesSet.end());
#ifdef MEASURE_MESH_TIME_ENABLED
	clock_t end = clock(); // TODO: Replace with Utility::Timer
	DEBUG_LOG("Removing duplicates from the vector of positions took %.2f [ms]", 1000.0 * static_cast<double>(end - begin) / (CLOCKS_PER_SEC));
#endif

	//ISort::GetSortingObject(ISort::QUICK_SORT)->Sort(&uniquePositions[0], uniquePositions.size(), COMPONENT_Y);
	//INFO_LOG("The minimum value is %s", uniquePositions[0].ToString().c_str());
	//INFO_LOG("The maximum value is %s", uniquePositions[uniquePositions.size() - 1].ToString().c_str());

	m_positionsCount = uniquePositions.size();
	INFO_LOG("Terrain consists of %d unique positions", m_positionsCount);
	m_positions = new Math::Vector3D[m_positionsCount];
	for (int i = 0; i < m_positionsCount; ++i)
	{
		m_positions[i] = uniquePositions[i];
	}
#endif

	/**
	 * TODO: Think of a nice way to store positions so that access time in GetHeightAt(x, z) is optimized. Maybe a Binary Tree? Maybe a k-d tree?
	 * Maybe find the minimum and maximum value for "Y" component of all positions and then use bucket sort (http://en.wikipedia.org/wiki/Bucket_sort)
	 * based on "Y" values?
	 */
}

void Rendering::TerrainMesh::TransformPositions(const Math::Matrix4D& transformationMatrix)
{
#ifdef HEIGHTMAP_KD_TREE
	DEBUG_LOG("Transformation matrix = \n%s", transformationMatrix.ToString().c_str());
	CHECK_CONDITION_EXIT(m_positions != NULL, Utility::Emergency, "Cannot transform the positions. The positions array is NULL.");
	for (int i = 0; i < m_positionsCount; ++i)
	{
		//std::string oldPos = positions[i].ToString();
		m_positions[i] = transformationMatrix * m_positions[i]; // FIXME: Check matrix multiplication
		//if ((i % 1000 == 0) || (i == positionsCount - 1))
		//{
		//	DELOCUST_LOG("%d) Old position = %s. New Position = %s", i, oldPos.c_str(), positions[i].ToString().c_str());
		//}
	}
	m_kdTree = new Math::KDTree(m_positions, m_positionsCount, m_kdTreeSamples);
#endif
}
