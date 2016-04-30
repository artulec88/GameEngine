#include "StdAfx.h"
#include "Mesh.h"
//#include "CoreEngine.h"
//#include "OBJModel.h"
#include "Math\Vector.h"
#include "Math\Interpolation.h"
#include "Math\FloatingPoint.h"
#include "stb_image.h"

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

#define UNBIND_BUFFER(buffer) glBindBuffer(buffer, 0);

#define INDEX_BUFFER 0
#define POSITIONS_BUFFER 1
#define TEXTURE_COORDINATES_BUFFER 2
#define NORMALS_BUFFER 3
#define TANGENTS_BUFFER 4
#define BITANGENTS_BUFFER 5
#define INSTANCE_BUFFER 6

#define POSITION_ATTRIBUTE_LOCATION 0
#define TEXTURE_COORDINATES_ATTRIBUTE_LOCATION 1
#define NORMALS_ATTRIBUTE_LOCATION 2
#define TANGENTS_ATTRIBUTE_LOCATION 3
#define BITANGENTS_ATTRIBUTE_LOCATION 4

/* static */ std::map<std::string, Rendering::MeshData*> Rendering::Mesh::meshResourceMap;

//Rendering::MeshData::MeshData() :
//	m_vao(0),
//	m_vbos(),
//	m_ibo(0),
//	m_size(0)
//{
//	GLuint vao;
//	glGenVertexArrays(1, &m_vao);
//}

Rendering::MeshData::MeshData(GLsizei indexSize) :
	m_vao(0),
	m_size(indexSize)
{
	memset(m_buffers, NULL, sizeof(m_buffers)); // fill the buffers with zeros (or NULLs)
	DELOCUST_LOG("Created mesh data %s", ToString().c_str());
}


Rendering::MeshData::~MeshData(void)
{
	//ERROR_LOG("Deleting mesh data %s", ToString().c_str());
	//if (m_buffers[0] != 0) // TODO: What if m_buffers = [0, a, b, c, d]? This is possible if INDEX_BUFFER is not used.
	//{
	//	glDeleteBuffers(sizeof(m_buffers), m_buffers);
	//}
	//if (m_ibo)
	//{
	//	glDeleteBuffers(1, &m_ibo);
	//}
	for (int i = 0; i < MESH_DATA_BUFFERS_COUNT; ++i)
	{
		if (m_buffers[i] != 0)
		{
			glDeleteBuffers(1, &m_buffers[i]);
		}
	}
	if (m_vao != 0)
	{
		glDeleteVertexArrays(1, &m_vao);
		m_vao = 0;
	}
}

void Rendering::MeshData::CreateVAO()
{
	Rendering::CheckErrorCode(__FUNCTION__, "Started VAO creation");
	glGenVertexArrays(1, &m_vao);
	CHECK_CONDITION_EXIT_ALWAYS(m_vao != 0, Utility::Critical, "VAO has not been created successfully. Its ID is still 0.");
	Rendering::CheckErrorCode(__FUNCTION__, "Finished VAO creation");
}

void Rendering::MeshData::CreateVBO(int index)
{
	Rendering::CheckErrorCode(__FUNCTION__, "Started VBO creation");
	glGenBuffers(1, &m_buffers[index]);
	DEBUG_LOG("Created VBO at index %d for mesh data \"%s\"", index, ToString().c_str());
	Rendering::CheckErrorCode(__FUNCTION__, "Finished VBO creation");
}

//void Rendering::MeshData::CreateIBO()
//{
	// TODO: Check if m_ibo is already created
	//glGenBuffers(1, &m_ibo);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
	//EMERGENCY_LOG("Created IBO for mesh data %s", ToString().c_str());
//}

std::string Rendering::MeshData::ToString() const
{
	std::stringstream ss("VAO = ");
	ss << m_vao << "; VBOs = [";
	for (int i = 0; i < MESH_DATA_BUFFERS_COUNT; ++i)
	{
		ss << m_buffers[i];
		if (i + 1 < MESH_DATA_BUFFERS_COUNT)
		{
			ss << "; ";
		}
	}
	ss << "] ";
	return ss.str();
}

Rendering::Mesh::Mesh(GLenum mode /* = GL_TRIANGLES */) :
	m_fileName(""),
	m_mode(mode),
	m_meshData(NULL)
{
}

Rendering::Mesh::Mesh(Math::Vector3D* positions, int verticesCount,
	int* indices, int indicesCount, bool calcNormalsEnabled /* = true */, GLenum mode /* = GL_TRIANGLES */) :
	m_fileName(""),
	m_mode(mode),
	m_meshData(NULL)
{
	AddVertices(positions, NULL, NULL, NULL, NULL, verticesCount, indices, indicesCount, calcNormalsEnabled);
}

Rendering::Mesh::Mesh(Math::Vector3D* positions, Math::Vector2D* textureCoordinates, int verticesCount,
	int* indices, int indicesCount, bool calcNormalsEnabled /* = true */, GLenum mode /* = GL_TRIANGLES */) :
	m_fileName(""),
	m_mode(mode),
	m_meshData(NULL)
{
	AddVertices(positions, textureCoordinates, NULL, NULL, NULL, verticesCount, indices, indicesCount, calcNormalsEnabled);
}

Rendering::Mesh::Mesh(Math::Vector3D* positions, Math::Vector2D* textureCoordinates, Math::Vector3D* normals, int verticesCount,
	int* indices, int indicesCount, bool calcNormalsEnabled /* = true */, GLenum mode /* = GL_TRIANGLES */) :
	m_fileName(""),
	m_mode(mode),
	m_meshData(NULL)
{
	AddVertices(positions, textureCoordinates, normals, NULL, NULL, verticesCount, indices, indicesCount, calcNormalsEnabled);
}

Rendering::Mesh::Mesh(Math::Vector3D* positions, Math::Vector2D* textureCoordinates, Math::Vector3D* normals, Math::Vector3D* tangents, int verticesCount,
	int* indices, int indicesCount, bool calcNormalsEnabled /* = true */, GLenum mode /* = GL_TRIANGLES */) :
	m_fileName(""),
	m_mode(mode),
	m_meshData(NULL)
{
	AddVertices(positions, textureCoordinates, normals, tangents, NULL, verticesCount, indices, indicesCount, calcNormalsEnabled);
}

Rendering::Mesh::Mesh(Math::Vector3D* positions, Math::Vector2D* textureCoordinates, Math::Vector3D* normals, Math::Vector3D* tangents, Math::Vector3D* bitangents, int verticesCount,
	int* indices, int indicesCount, bool calcNormalsEnabled /* = true */, GLenum mode /* = GL_TRIANGLES */) :
	m_fileName(""),
	m_mode(mode),
	m_meshData(NULL)
{
	AddVertices(positions, textureCoordinates, normals, tangents, bitangents, verticesCount, indices, indicesCount, calcNormalsEnabled);
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
	Rendering::CheckErrorCode(__FUNCTION__, "Started Mesh initialization");
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
		DEBUG_LOG("Model \"%s\" is already loaded. Using already loaded mesh data.", name.c_str());
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
		aiProcess_CalcTangentSpace );

	CHECK_CONDITION_EXIT(scene != NULL, Utility::Critical, "Error while loading a mesh \"%s\"", name.c_str());
	CHECK_CONDITION_EXIT((scene->mMeshes != NULL) && (scene->mNumMeshes > 0), Utility::Critical,
		"Incorrect number of meshes loaded- %d- check the model \"%s\". One of the possible solutions is to check whether the model has any additional lines at the end.",
		scene->mNumMeshes, name.c_str());

	const aiMesh* model = scene->mMeshes[0];
	std::vector<Math::Vector3D> positions;
	positions.reserve(model->mNumVertices);
	std::vector<Math::Vector2D> textureCoordinates;
	textureCoordinates.reserve(model->mNumVertices);
	std::vector<Math::Vector3D> normals;
	normals.reserve(model->mNumVertices);
	std::vector<Math::Vector3D> tangents;
	tangents.reserve(model->mNumVertices);
	std::vector<Math::Vector3D> bitangents;
	bitangents.reserve(model->mNumVertices);

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

		//Math::Vector3D vertexBitangent(pBitangent->x, pBitangent->y, pBitangent->z);

		positions.emplace_back(pPos->x, pPos->y, pPos->z);
		textureCoordinates.emplace_back(pTexCoord->x, pTexCoord->y);
		normals.emplace_back(pNormal->x, pNormal->y, pNormal->z);
		tangents.emplace_back(pTangent->x, pTangent->y, pTangent->z);
	}

	std::vector<int> indices;
	indices.reserve(model->mNumFaces * 3 /* each face is a triangle and has 3 vertices */);
	for (unsigned int i = 0; i < model->mNumFaces; ++i)
	{
		const aiFace& face = model->mFaces[i];
		CHECK_CONDITION_ALWAYS(face.mNumIndices == 3, Utility::Warning, "The face has %d indices when only triangle faces are supported.", face.mNumIndices);
		indices.push_back(face.mIndices[0]);
		indices.push_back(face.mIndices[1]);
		indices.push_back(face.mIndices[2]);
	}
	SavePositions(positions); // used by TerrainMesh to save the positions. For Mesh instances it does nothing as it is not necessary to store them.
	AddVertices(&positions[0], &textureCoordinates[0], &normals[0], &tangents[0], NULL, positions.size(), (int*)&indices[0], indices.size(), false);

	meshResourceMap.insert(std::pair<std::string, MeshData*>(m_fileName, m_meshData));

#ifdef MEASURE_MESH_TIME_ENABLED
	timer.Stop();
	INFO_LOG("Loading model took %s", timer.GetTimeSpan().ToString().c_str());
#endif
	Rendering::CheckErrorCode(__FUNCTION__, "Finished Mesh initialization");
}

void Rendering::Mesh::AddVertices(Math::Vector2D* positions, Math::Vector2D* textureCoordinates, int verticesCount)
{
	Rendering::CheckErrorCode(__FUNCTION__, "Started adding 2D vertices to the Mesh");
#ifdef DELOCUST_ENABLED
	for (size_t i = 0; i < verticesCount; ++i)
	{
		DELOCUST_LOG("vertex[%d]:\n\tPos:\t%s\n\tTex:\t%s\n\tNormal:\t%s", i, positions[i].ToString().c_str(), textureCoordinates[i].ToString().c_str(), normals[i].ToString().c_str());
	}
	for (size_t i = 0; i < indicesCount; ++i)
	{
		DELOCUST_LOG("index[%d]: %d", i, indices[i]);
	}
#endif
	m_meshData = new MeshData(static_cast<GLsizei>(verticesCount)); // TODO: size_t is bigger than GLsizei, so errors will come if indicesCount > 2^32.

	CHECK_CONDITION_EXIT(m_meshData != NULL, Critical, "Mesh data instance is NULL");
	m_meshData->CreateVAO();
	m_meshData->Bind();
	m_meshData->CreateVBO(POSITIONS_BUFFER);
	glBindBuffer(GL_ARRAY_BUFFER, m_meshData->GetVBO(POSITIONS_BUFFER));
	glBufferData(GL_ARRAY_BUFFER, verticesCount * sizeof(Math::Vector2D), positions, GL_STATIC_DRAW);
	glVertexAttribPointer(POSITION_ATTRIBUTE_LOCATION, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(POSITION_ATTRIBUTE_LOCATION);

	if (textureCoordinates != NULL)
	{
		m_meshData->CreateVBO(TEXTURE_COORDINATES_BUFFER);
		glBindBuffer(GL_ARRAY_BUFFER, m_meshData->GetVBO(TEXTURE_COORDINATES_BUFFER));
		glBufferData(GL_ARRAY_BUFFER, verticesCount * sizeof(Math::Vector2D), textureCoordinates, GL_STATIC_DRAW);
		glVertexAttribPointer(TEXTURE_COORDINATES_ATTRIBUTE_LOCATION, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(TEXTURE_COORDINATES_ATTRIBUTE_LOCATION);
	}

	m_meshData->Unbind();
	Rendering::CheckErrorCode(__FUNCTION__, "Finished adding 2D vertices to the Mesh");
}

void Rendering::Mesh::AddVertices(Math::Vector3D* positions, Math::Vector2D* textureCoordinates, Math::Vector3D* normals, Math::Vector3D* tangents, Math::Vector3D* bitangents, int verticesCount, int* indices, int indicesCount, bool calcNormalsEnabled)
{
	Rendering::CheckErrorCode(__FUNCTION__, "Started adding 3D vertices to the Mesh");
#ifdef DELOCUST_ENABLED
	for (size_t i = 0; i < verticesCount; ++i)
	{
		DELOCUST_LOG("vertex[%d]:\n\tPos:\t%s\n\tTex:\t%s\n\tNormal:\t%s", i, positions[i].ToString().c_str(), textureCoordinates[i].ToString().c_str(), normals[i].ToString().c_str());
	}
	for (size_t i = 0; i < indicesCount; ++i)
	{
		DELOCUST_LOG("index[%d]: %d", i, indices[i]);
	}
#endif
	m_meshData = new MeshData(static_cast<GLsizei>((indices != NULL) ? indicesCount : verticesCount)); // TODO: size_t is bigger than GLsizei, so errors will come if indicesCount > 2^32.

	if (calcNormalsEnabled)
	{
		CalcNormals(normals, positions, verticesCount, indices, indicesCount);
	}
	//if (calcTangents)
	//{
	//	this->CalcTangents(vertices, verticesCount);
	//}

	CHECK_CONDITION_EXIT(m_meshData != NULL, Critical, "Mesh data instance is NULL");
	m_meshData->CreateVAO();
	m_meshData->Bind();
	m_meshData->CreateVBO(POSITIONS_BUFFER);
	glBindBuffer(GL_ARRAY_BUFFER, m_meshData->GetVBO(POSITIONS_BUFFER));
	glBufferData(GL_ARRAY_BUFFER, verticesCount * sizeof(Math::Vector3D), positions, GL_STATIC_DRAW);
	glVertexAttribPointer(POSITION_ATTRIBUTE_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(POSITION_ATTRIBUTE_LOCATION);

	if (textureCoordinates != NULL)
	{
		m_meshData->CreateVBO(TEXTURE_COORDINATES_BUFFER);
		glBindBuffer(GL_ARRAY_BUFFER, m_meshData->GetVBO(TEXTURE_COORDINATES_BUFFER));
		glBufferData(GL_ARRAY_BUFFER, verticesCount * sizeof(Math::Vector2D), textureCoordinates, GL_STATIC_DRAW);
		glVertexAttribPointer(TEXTURE_COORDINATES_ATTRIBUTE_LOCATION, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(TEXTURE_COORDINATES_ATTRIBUTE_LOCATION);
	}

	if (normals != NULL)
	{
		m_meshData->CreateVBO(NORMALS_BUFFER);
		glBindBuffer(GL_ARRAY_BUFFER, m_meshData->GetVBO(NORMALS_BUFFER));
		glBufferData(GL_ARRAY_BUFFER, verticesCount * sizeof(Math::Vector3D), normals, GL_STATIC_DRAW);
		glVertexAttribPointer(NORMALS_ATTRIBUTE_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(NORMALS_ATTRIBUTE_LOCATION);
	}

	if (tangents != NULL)
	{
		m_meshData->CreateVBO(TANGENTS_BUFFER);
		glBindBuffer(GL_ARRAY_BUFFER, m_meshData->GetVBO(TANGENTS_BUFFER));
		glBufferData(GL_ARRAY_BUFFER, verticesCount * sizeof(Math::Vector3D), tangents, GL_STATIC_DRAW);
		glVertexAttribPointer(TANGENTS_ATTRIBUTE_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(TANGENTS_ATTRIBUTE_LOCATION);
	}

	if (bitangents != NULL)
	{
		m_meshData->CreateVBO(BITANGENTS_BUFFER);
		glBindBuffer(GL_ARRAY_BUFFER, m_meshData->GetVBO(BITANGENTS_BUFFER));
		glBufferData(GL_ARRAY_BUFFER, verticesCount * sizeof(Math::Vector3D), bitangents, GL_STATIC_DRAW);
		glVertexAttribPointer(BITANGENTS_ATTRIBUTE_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(BITANGENTS_ATTRIBUTE_LOCATION);
	}

	if (indices != NULL)
	{
		m_meshData->CreateVBO(INDEX_BUFFER);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_meshData->GetVBO(INDEX_BUFFER));
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesCount * sizeof(int), indices, GL_STATIC_DRAW);
	}

	m_meshData->Unbind();
	Rendering::CheckErrorCode(__FUNCTION__, "Finished adding 3D vertices to the Mesh");
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
	Rendering::CheckErrorCode(__FUNCTION__, "Started drawing the Mesh");
	CHECK_CONDITION_EXIT(m_meshData != NULL, Critical, "Mesh data instance is NULL");

	m_meshData->Bind();

	if (m_meshData->HasVBO(INDEX_BUFFER))
	{
		//CRITICAL_LOG("Drawing elements for mesh %s", m_fileName.c_str());
		glDrawElements(m_mode, m_meshData->GetSize() /* * sizeof(int) */, GL_UNSIGNED_INT, 0);
		//glDrawArrays(m_mode, 0, m_meshData->GetSize() /* * sizeof(float) */);
	}
	else
	{
		glDrawArrays(m_mode, 0, m_meshData->GetSize() /* * sizeof(float) */);
	}

	m_meshData->Unbind();
	Rendering::CheckErrorCode(__FUNCTION__, "Finished drawing the Mesh");
}

bool Rendering::Mesh::Compare(const Mesh& mesh) const
{
	CHECK_CONDITION_RETURN(m_meshData != NULL && mesh.m_meshData != NULL, false, Utility::Error, "Cannot compare two meshes' VAOs, because mesh(-es) data is/are NULL.");
	return m_meshData->GetVAO() < mesh.m_meshData->GetVAO();
}

void Rendering::Mesh::CalcNormals(Math::Vector3D*& normals, Math::Vector3D* positions, size_t verticesCount, const int* indices, size_t indicesCount) const
{
	// TODO: The value 3 for iterationStep works ok only for mode equal to GL_TRIANGLES.
	// For different modes (GL_QUADS, GL_LINES) this iterationStep variable will be incorrect
	const int iterationStep = 3; // we are iterating through faces which are triangles (each triangle has 3 vertices)
	for(int i = 0; i < indicesCount; i += iterationStep)
	{
		int i0 = indices[i];
		int i1 = indices[i + 1];
		int i2 = indices[i + 2];
		
		Math::Vector3D v1 = positions[i1] - positions[i0];
		Math::Vector3D v2 = positions[i2] - positions[i0];
		Math::Vector3D normalVec = v1.Cross(v2).Normalized();
		
		normals[i0] += normalVec;
		normals[i1] += normalVec;
		normals[i2] += normalVec;
	}
	
	for(int i = 0; i < verticesCount; i++)
	{
		normals[i].Normalize();
	}
}

void Rendering::Mesh::CalcTangents(Math::Vector3D*& tangents, Math::Vector3D* positions, Math::Vector2D* textureCoordinates, size_t verticesCount) const
{
	// TODO: The value 3 for iterationStep works ok only for mode equal to GL_TRIANGLES.
	// For different modes (Gl_QUADS, GL_LINES) this iterationStep variable will be incorrect
	const int iterationStep = 3; // each face has three vertices
	for (int i = 0; i < verticesCount - iterationStep + 1; i += iterationStep)
	{
		// Edges of the triangle: position delta
		Math::Vector3D deltaPos1 = positions[i + 1] - positions[i];
		Math::Vector3D deltaPos2 = positions[i + 2] - positions[i];

		// UV delta
		Math::Vector2D deltaUV1 = textureCoordinates[i + 1] - textureCoordinates[i];
		Math::Vector2D deltaUV2 = textureCoordinates[i + 2] - textureCoordinates[i];

		Math::Real r = REAL_ONE / (deltaUV1.Cross(deltaUV2));
		Math::Vector3D tangent = (deltaPos1 * deltaUV2.GetY() - deltaPos2 * deltaUV1.GetY()) * r;
		Math::Vector3D bitangent = (deltaPos2 * deltaUV1.GetX() - deltaPos1 * deltaUV2.GetX()) * r;

		// Set the same tangent for all three vertices of the triangle. They will be merged later, during indexing.
		tangents[i] = tangent;
		tangents[i + 1] = tangent;
		tangents[i + 2] = tangent;

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

Rendering::BillboardMesh::BillboardMesh(Math::Real* modelMatricesValues, unsigned int billboardsCount, unsigned int billboardDataLength) :
	Mesh(GL_POINTS),
	m_billboardsCount(billboardsCount)
{
	AddVertices(&Math::Vector3D(0.0f, 0.0f, 0.0f), NULL, NULL, NULL, NULL, 1, NULL, 0, false);

	CHECK_CONDITION_EXIT(billboardsCount > 0, Utility::Error, "Cannot create a billboard mesh. Specified number of billboards is not greater than 0");
	CHECK_CONDITION_EXIT(billboardDataLength > 0, Utility::Error, "Cannot create a billboard mesh. Specified billboard data length is not greater than 0");

	m_meshData->Bind();
	m_meshData->CreateVBO(INSTANCE_BUFFER); // instanced attributes will be stored in this VBO
	glBindBuffer(GL_ARRAY_BUFFER, m_meshData->GetVBO(INSTANCE_BUFFER));
	glBufferData(GL_ARRAY_BUFFER, sizeof(Math::Real) * m_billboardsCount * billboardDataLength, modelMatricesValues, GL_STATIC_DRAW);
	glVertexAttribPointer(1 /* MVP_MATRIX_COLUMN_1_LOCATION */, 4, GL_FLOAT, GL_FALSE, billboardDataLength * sizeof(Math::Real), (GLvoid*)0);
	glEnableVertexAttribArray(1 /* MVP_MATRIX_COLUMN_1_LOCATION */);
	glVertexAttribDivisor(1 /* MVP_MATRIX_COLUMN_1_LOCATION */, 1);
	glVertexAttribPointer(2 /* MVP_MATRIX_COLUMN_2_LOCATION */, 4, GL_FLOAT, GL_FALSE, billboardDataLength * sizeof(Math::Real), (GLvoid*)(4 * sizeof(Math::Real)));
	glEnableVertexAttribArray(2 /* MVP_MATRIX_COLUMN_2_LOCATION */);
	glVertexAttribDivisor(2 /* MVP_MATRIX_COLUMN_2_LOCATION */, 1);
	glVertexAttribPointer(3 /* MVP_MATRIX_COLUMN_3_LOCATION */, 4, GL_FLOAT, GL_FALSE, billboardDataLength * sizeof(Math::Real), (GLvoid*)(8 * sizeof(Math::Real)));
	glEnableVertexAttribArray(3 /* MVP_MATRIX_COLUMN_3_LOCATION */);
	glVertexAttribDivisor(3 /* MVP_MATRIX_COLUMN_3_LOCATION */, 1);
	glVertexAttribPointer(4 /* MVP_MATRIX_COLUMN_4_LOCATION */, 4, GL_FLOAT, GL_FALSE, billboardDataLength * sizeof(Math::Real), (GLvoid*)(12 * sizeof(Math::Real)));
	glEnableVertexAttribArray(4 /* MVP_MATRIX_COLUMN_4_LOCATION */);
	glVertexAttribDivisor(4 /* MVP_MATRIX_COLUMN_4_LOCATION */, 1);

	m_meshData->Unbind();
}

Rendering::BillboardMesh::~BillboardMesh()
{
}

void Rendering::BillboardMesh::Draw() const
{
	CHECK_CONDITION_EXIT(m_meshData != NULL, Critical, "Mesh data instance is NULL");
	m_meshData->Bind();

	glDrawArraysInstanced(m_mode, 0, 1, m_billboardsCount);

	m_meshData->Unbind();
}

Rendering::GuiMesh::GuiMesh(Math::Vector2D* positions, unsigned int positionsCount) :
	Mesh(GL_TRIANGLE_STRIP),
	m_positionsCount(positionsCount)
{
	CHECK_CONDITION_EXIT(positionsCount > 0, Utility::Error, "Cannot create a mesh. Specified number of positions is not greater than 0");
	CHECK_CONDITION_EXIT(positions != NULL, Utility::Error, "Cannot create a mesh. Specified positions array is NULL.");
	AddVertices(positions, NULL, positionsCount);
}

Rendering::GuiMesh::~GuiMesh()
{
}

//void Rendering::GuiMesh::Draw() const
//{
//	CHECK_CONDITION_EXIT(m_meshData != NULL, Critical, "Mesh data instance is NULL");
//
//	m_meshData->Bind();
//
//	glDrawArrays(m_mode, 0, m_positionsCount * 2);
//
//	m_meshData->Unbind();
//}

Rendering::InstanceMesh::InstanceMesh(Math::Vector2D* positions, unsigned int positionsCount, unsigned int maxParticlesCount, unsigned int instanceDataLength) :
	Mesh(GL_TRIANGLE_STRIP),
	m_positionsCount(positionsCount),
	m_maxParticlesCount(maxParticlesCount),
	m_instanceDataLength(instanceDataLength)
{
	CHECK_CONDITION_EXIT(positionsCount > 0, Utility::Error, "Cannot create a mesh. Specified number of positions is not greater than 0");
	CHECK_CONDITION_EXIT(particlesCount > 0, Utility::Error, "Cannot create a mesh. Specified number of particles is not greater than 0");
	
	AddVertices(positions, NULL, positionsCount);

	m_meshData->Bind();
	m_meshData->CreateVBO(INSTANCE_BUFFER); // instanced attributes will be stored in this VBO
	glBindBuffer(GL_ARRAY_BUFFER, m_meshData->GetVBO(INSTANCE_BUFFER));
	glBufferData(GL_ARRAY_BUFFER, sizeof(Math::Real) * m_maxParticlesCount * m_instanceDataLength, NULL, GL_STREAM_DRAW);
	glVertexAttribPointer(1 /* MVP_MATRIX_COLUMN_1_LOCATION */, 4, GL_FLOAT, GL_FALSE, m_instanceDataLength * sizeof(Math::Real), (GLvoid*)0);
	glEnableVertexAttribArray(1 /* MVP_MATRIX_COLUMN_1_LOCATION */);
	glVertexAttribDivisor(1 /* MVP_MATRIX_COLUMN_1_LOCATION */, 1);
	glVertexAttribPointer(2 /* MVP_MATRIX_COLUMN_2_LOCATION */, 4, GL_FLOAT, GL_FALSE, m_instanceDataLength * sizeof(Math::Real), (GLvoid*)(4 * sizeof(Math::Real)));
	glEnableVertexAttribArray(2 /* MVP_MATRIX_COLUMN_2_LOCATION */);
	glVertexAttribDivisor(2 /* MVP_MATRIX_COLUMN_2_LOCATION */, 1);
	glVertexAttribPointer(3 /* MVP_MATRIX_COLUMN_3_LOCATION */, 4, GL_FLOAT, GL_FALSE, m_instanceDataLength * sizeof(Math::Real), (GLvoid*)(8 * sizeof(Math::Real)));
	glEnableVertexAttribArray(3 /* MVP_MATRIX_COLUMN_3_LOCATION */);
	glVertexAttribDivisor(3 /* MVP_MATRIX_COLUMN_3_LOCATION */, 1);
	glVertexAttribPointer(4 /* MVP_MATRIX_COLUMN_4_LOCATION */, 4, GL_FLOAT, GL_FALSE, m_instanceDataLength * sizeof(Math::Real), (GLvoid*)(12 * sizeof(Math::Real)));
	glEnableVertexAttribArray(4 /* MVP_MATRIX_COLUMN_4_LOCATION */);
	glVertexAttribDivisor(4 /* MVP_MATRIX_COLUMN_4_LOCATION */, 1);
	glVertexAttribPointer(5 /* TEXTURE_ATLAS_OFFSETS_LOCATION */, 4, GL_FLOAT, GL_FALSE, m_instanceDataLength * sizeof(Math::Real), (GLvoid*)(16 * sizeof(Math::Real)));
#ifdef TEXTURE_ATLAS_OFFSET_CALCULATION
	glEnableVertexAttribArray(5 /* TEXTURE_ATLAS_OFFSETS_LOCATION */);
	glVertexAttribDivisor(5 /* TEXTURE_ATLAS_OFFSETS_LOCATION */, 1);
	glVertexAttribPointer(6 /* BLEND_FACTOR_LOCATION */, 1, GL_FLOAT, GL_FALSE, m_instanceDataLength * sizeof(Math::Real), (GLvoid*)(20 * sizeof(Math::Real)));
	glEnableVertexAttribArray(6 /* BLEND_FACTOR_LOCATION */);
	glVertexAttribDivisor(6 /* BLEND_FACTOR_LOCATION */, 1);
#else
	glVertexAttribPointer(5 /* LIFE_STAGE_FACTOR_LOCATION */, 1, GL_FLOAT, GL_FALSE, m_instanceDataLength * sizeof(Math::Real), (GLvoid*)(16 * sizeof(Math::Real)));
	glEnableVertexAttribArray(5 /* LIFE_STAGE_FACTOR_LOCATION */);
	glVertexAttribDivisor(5 /* LIFE_STAGE_FACTOR_LOCATION */, 1);
#endif

	m_meshData->Unbind();
}

Rendering::InstanceMesh::~InstanceMesh()
{
}

void Rendering::InstanceMesh::Draw(Math::Real* data, unsigned int dataSize, unsigned int particlesCount) const
{
	CHECK_CONDITION_EXIT(m_meshData != NULL, Critical, "Mesh data instance is NULL");

	// Updating the instance VBO begin
	//m_meshData->Bind();
	glBindBuffer(GL_ARRAY_BUFFER, m_meshData->GetVBO(INSTANCE_BUFFER));
	glBufferData(GL_ARRAY_BUFFER, sizeof(Math::Real) * m_maxParticlesCount * m_instanceDataLength, data, GL_STREAM_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Math::Real) * dataSize, data);
	//m_meshData->Unbind();
	// Updating the instance VBO end

	m_meshData->Bind();
	
	glDrawArraysInstanced(m_mode, 0, m_positionsCount /* * 2 */, particlesCount);

	m_meshData->Unbind();
}

/* static */ const int Rendering::TerrainMesh::SIZE = 800;
/* static */ const float Rendering::TerrainMesh::MAX_HEIGHT = 5.0f;
/* static */ const float Rendering::TerrainMesh::MAX_PIXEL_COLOR = 255.0f;

Rendering::TerrainMesh::TerrainMesh(const std::string& fileName, GLenum mode /* = GL_TRIANGLES */) :
	Mesh(fileName, mode),
	m_x(REAL_ZERO),
	m_z(REAL_ZERO),
	m_headPositionHeightAdjustment(GET_CONFIG_VALUE("headPositionHeightAdjustment", 2.5f)),
	m_vertexCount(0)
#ifdef HEIGHTMAP_KD_TREE
	,m_positions(NULL),
	m_kdTree(NULL),
	m_kdTreeSamples(GET_CONFIG_VALUE("kdTreeSamples", 8))
#elif defined HEIGHTMAP_HEIGHTS
	, m_heights(NULL),
	m_gridSquareSize(0)
#endif
{
}

Rendering::TerrainMesh::TerrainMesh(Math::Real gridX, Math::Real gridZ, const std::string& heightMapFileName, GLenum mode /* = GL_TRIANGLES */) :
	Mesh(mode),
	m_x(gridX),
	m_z(gridZ),
	m_headPositionHeightAdjustment(GET_CONFIG_VALUE("headPositionHeightAdjustment", 2.5f)),
	m_vertexCount(0)
#ifdef HEIGHTMAP_KD_TREE
	, m_positions(NULL),
	m_kdTree(NULL),
	m_kdTreeSamples(GET_CONFIG_VALUE("kdTreeSamples", 8))
#elif defined HEIGHTMAP_HEIGHTS
	, m_heightMapWidth(0),
	m_heightMapHeight(0),
	m_heights(NULL),
	m_gridSquareSize(0)
#endif
{
	/* Loading heightmap begin */
	std::string name = heightMapFileName;
	const char *tmp = strrchr(heightMapFileName.c_str(), '\\');
	if (tmp != NULL)
	{
		name.assign(tmp + 1);
	}
	int bytesPerPixel;
	//unsigned char* data = stbi_load((Core::CoreEngine::GetCoreEngine()->GetTexturesDirectory() + fileName).c_str(), &x, &y, &bytesPerPixel, 4 /* req_comp */);
	unsigned char* heightMapData = stbi_load(("C:\\Users\\aosesik\\Documents\\Visual Studio 2015\\Projects\\GameEngine\\Textures\\" + heightMapFileName).c_str(),
		&m_heightMapWidth, &m_heightMapHeight, &bytesPerPixel, 1 /* we only care about one RED component for now (the heightmap is grayscale) */);
	CHECK_CONDITION_EXIT(m_heightMapData != NULL, Utility::Error, "Unable to load terrain height map from the file \"%s\"", name.c_str());
	CHECK_CONDITION(heightMapWidth < 32768 && heightMapHeight < 32768, Utility::Emergency, "The heightmap's size is too big to be used in the rendering engine.");
	//for (int i = 0; i < heightMapWidth; ++i)
	//{
	//	for (int j = 0; j < heightMapHeight; ++j)
	//	{
	//		CRITICAL_LOG("HeightMap[%d] ([%d][%d]) = %d", i * heightMapWidth + j, i, j, heightMapData[i * heightMapWidth + j]);
	//	}
	//}
	/* Loading heightmap finished */

	m_vertexCount = m_heightMapHeight; // The number of vertices along each side of the single terrain tile. It is equal to the height of the height map image.
	const int vertexCountMinusOne = m_vertexCount - 1;
#ifdef HEIGHTMAP_KD_TREE
	//m_vertexCount = VERTEX_COUNT * VERTEX_COUNT;
#else
	m_heights = new Math::Real[m_heightMapWidth * m_heightMapHeight];
	m_gridSquareSize = static_cast<Math::Real>(SIZE) / vertexCountMinusOne;
#endif
	//const int INDICES_COUNT = 6 * (VERTEX_COUNT - 1) * (VERTEX_COUNT - 1); // The number of indices.

	std::vector<Math::Vector3D> positions;
	positions.reserve(m_vertexCount);
	std::vector<Math::Vector2D> textureCoordinates;
	textureCoordinates.reserve(m_vertexCount);
	std::vector<Math::Vector3D> normals;
	normals.reserve(m_vertexCount);
	std::vector<Math::Vector3D> tangents;
	tangents.reserve(m_vertexCount);
	for (int z = vertexCountMinusOne; z >= 0; --z)
	{
		const Math::Real zReal = static_cast<Math::Real>(z);
		for (int x = 0; x < m_vertexCount; ++x)
		{
			const Math::Real xReal = static_cast<Math::Real>(x);
			Math::Real terrainHeight = CalculateHeightAt(x, z, heightMapData);
			//DEBUG_LOG("counter = %d; x = %d; z = %d; Position = %s", positions.size(), x, z, position.ToString().c_str());
			positions.emplace_back(xReal / vertexCountMinusOne * SIZE, terrainHeight, zReal / vertexCountMinusOne * SIZE);
			textureCoordinates.emplace_back(xReal / vertexCountMinusOne, zReal / vertexCountMinusOne);
			normals.push_back(CalculateNormal(x, z, heightMapData));
			tangents.emplace_back(REAL_ONE, REAL_ZERO, REAL_ZERO); // TODO: Calculate tangent
		}
	}
	stbi_image_free(heightMapData);
	
	std::vector<int> indices;
	indices.reserve(vertexCountMinusOne * vertexCountMinusOne * 6);
	for (int gz = 0; gz < vertexCountMinusOne; ++gz)
	{
		for (int gx = 0; gx < vertexCountMinusOne; ++gx)
		{
			int topLeft = (gz * m_vertexCount) + gx;
			int topRight = topLeft + 1;
			int bottomLeft = ((gz + 1) * m_vertexCount) + gx;
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
	AddVertices(&positions[0], &textureCoordinates[0], &normals[0], &tangents[0], NULL, positions.size(), &indices[0], indices.size(), false);
}

Rendering::TerrainMesh::~TerrainMesh(void)
{
#ifdef HEIGHTMAP_KD_TREE
	SAFE_DELETE_JUST_TABLE(m_positions);
	SAFE_DELETE(m_kdTree);
#elif defined HEIGHTMAP_HEIGHTS
	//SAFE_DELETE_WHOLE_TABLE(m_heights, m_vertexCount);
	SAFE_DELETE_JUST_TABLE(m_heights);
#endif
}

int Rendering::TerrainMesh::GetHeightMapIndex(int x, int z) const
{
	return (m_heightMapWidth * m_heightMapHeight) - ((z + 1) * m_heightMapWidth) + x;
}

Math::Real Rendering::TerrainMesh::CalculateHeightAt(int x, int z, unsigned char* heightMapData) const
{
	// TODO: Range checking
	CHECK_CONDITION_RETURN(x >= 0 && x < m_heightMapWidth && z >= 0 && z < m_heightMapHeight, REAL_ZERO,
		Utility::Error, "Cannot determine the height of the terrain on (%d, %d) position. It is out of range.", x, z);
	const int heightMapIndex = GetHeightMapIndex(x, z);
	CHECK_CONDITION(heightMapIndex >= 0 && heightMapIndex < m_heightMapWidth * m_heightMapHeight, Utility::Error,
		"The heightmap index calculation is incorrect. Calculated index (%d) is out of range [0; %d)", heightMapIndex, m_heightMapWidth * m_heightMapHeight);
	//DEBUG_LOG("Heightmap index for [%d, %d] = %d", x, z, heightMapIndex);
	Math::Real height = static_cast<Math::Real>(heightMapData[heightMapIndex]);
	height = ((height / MAX_PIXEL_COLOR) - 0.5f) * 2.0f * MAX_HEIGHT;
#ifdef HEIGHTMAP_HEIGHTS
	m_heights[heightMapIndex] = height;
#endif
	return height;
}

Math::Vector3D Rendering::TerrainMesh::CalculateNormal(int x, int z, unsigned char* heightMapData) const
{
	Math::Real heightLeft = (x - 1) >= 0 ? CalculateHeightAt(x - 1, z, heightMapData) : REAL_ZERO;
	Math::Real heightRight = (x + 1) < m_heightMapWidth ? CalculateHeightAt(x + 1, z, heightMapData) : REAL_ZERO;
	Math::Real heightDown = (z - 1) >= 0 ? CalculateHeightAt(x, z - 1, heightMapData) : REAL_ZERO;
	Math::Real heightUp = (z + 1) < m_heightMapHeight ? CalculateHeightAt(x, z + 1, heightMapData) : REAL_ZERO;
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
	Math::Real terrainX = x - m_x;
	Math::Real terrainZ = z - m_z;
	int gridX = Math::Floor(terrainX / m_gridSquareSize);
	int gridZ = Math::Floor(terrainZ / m_gridSquareSize);
	if (gridX < 0 || gridX >= m_vertexCount - 1 || gridZ < 0 || gridZ >= m_vertexCount - 1)
	{
		return REAL_ZERO;
	}
	Math::Real xCoord = fmod(terrainX, m_gridSquareSize) / m_gridSquareSize;
	Math::Real zCoord = fmod(terrainZ, m_gridSquareSize) / m_gridSquareSize;
	Math::Real y;
	if (xCoord <= (1.0f - zCoord))
	{
		y = Math::Interpolation::BarycentricInterpolation(0.0f, m_heights[GetHeightMapIndex(gridX, gridZ)], 0.0f,
			1.0f, m_heights[GetHeightMapIndex(gridX + 1, gridZ)], 0.0f,
			0.0f, m_heights[GetHeightMapIndex(gridX, gridZ + 1)], 1.0f,
			xCoord, zCoord);
		//for (int i = 2; i > -3; --i)
		//{
		//	CRITICAL_LOG("%d) Heights: [%d][%d] = %.2f; [%d][%d] = %.2f; [%d][%d] = %.2f; [%d][%d] = %.2f; [%d][%d] = %.2f",
		//		i, gridX - 2, gridZ + i, ((gridX - 2 < 0) || (gridZ + i < 0)) ? -999.0f : m_heights[gridX - 2][gridZ + i],
		//		gridX - 1, gridZ + i, ((gridX - 1 < 0) || (gridZ + i < 0)) ? -999.0f : m_heights[gridX - 1][gridZ + i],
		//		gridX, gridZ + i, ((gridX < 0) || (gridZ + i < 0)) ? -999.0f : m_heights[gridX][gridZ + i],
		//		gridX + 1, gridZ + i, ((gridX + 1 < 0) || (gridZ + i < 0)) ? -999.0f : m_heights[gridX + 1][gridZ + i],
		//		gridX + 2, gridZ + i, ((gridX + 2 < 0) || (gridZ + i < 0)) ? -999.0f : m_heights[gridX + 2][gridZ + i]);
		//}
		//EMERGENCY_LOG("Grid = [%d, %d]. Coords = [%.3f, %.3f]. Height[%d] = %.3f, Height[%d] = %.3f, Height[%d] = %.3f. Final height = %.3f",
		//	gridX, gridZ, xCoord, zCoord, GetHeightMapIndex(gridX, gridZ), m_heights[GetHeightMapIndex(gridX, gridZ)],
		//	GetHeightMapIndex(gridX + 1, gridZ), m_heights[GetHeightMapIndex(gridX + 1, gridZ)],
		//	GetHeightMapIndex(gridX, gridZ + 1), m_heights[GetHeightMapIndex(gridX, gridZ + 1)], y);
	}
	else
	{
		y = Math::Interpolation::BarycentricInterpolation(1.0f, m_heights[GetHeightMapIndex(gridX + 1, gridZ)], 0.0f,
			1.0f, m_heights[GetHeightMapIndex(gridX + 1, gridZ + 1)], 1.0f,
			0.0f, m_heights[GetHeightMapIndex(gridX, gridZ + 1)], 1.0f,
			xCoord, zCoord);
		//ERROR_LOG("Grid = [%d, %d]. Coords = [%.3f, %.3f]. Height[%d] = %.3f, Height[%d] = %.3f, Height[%d] = %.3f. Final height = %.3f",
		//	gridX, gridZ, xCoord, zCoord, GetHeightMapIndex(gridX + 1, gridZ), m_heights[GetHeightMapIndex(gridX + 1, gridZ)],
		//	GetHeightMapIndex(gridX + 1, gridZ + 1), m_heights[GetHeightMapIndex(gridX + 1, gridZ + 1)],
		//	GetHeightMapIndex(gridX, gridZ + 1), m_heights[GetHeightMapIndex(gridX, gridZ + 1)], y);
	}
	//y -= 0.35f;
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
	clock_t begin = clock(); // TODO: Replace with Utility::Timer. Use QueryPerformanceCounter() instead of clock() function when measuring time. It is more accurate.
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
	clock_t end = clock(); // TODO: Replace with Utility::Timer. Use QueryPerformanceCounter() instead of clock() function when measuring time. It is more accurate.
	DEBUG_LOG("Removing duplicates from the vector of positions took %.2f [ms]", 1000.0 * static_cast<double>(end - begin) / (CLOCKS_PER_SEC));
#endif

	//ISort::GetSortingObject(ISort::QUICK_SORT)->Sort(&uniquePositions[0], uniquePositions.size(), COMPONENT_Y);
	//INFO_LOG("The minimum value is %s", uniquePositions[0].ToString().c_str());
	//INFO_LOG("The maximum value is %s", uniquePositions[uniquePositions.size() - 1].ToString().c_str());

	m_vertexCount = uniquePositions.size();
	INFO_LOG("Terrain consists of %d unique positions", m_vertexCount);
	m_positions = new Math::Vector3D[m_vertexCount];
	for (int i = 0; i < m_vertexCount; ++i)
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
	for (int i = 0; i < m_vertexCount; ++i)
	{
		//std::string oldPos = positions[i].ToString();
		m_positions[i] = transformationMatrix * m_positions[i]; // FIXME: Check matrix multiplication
		//if ((i % 1000 == 0) || (i == m_vertexCount - 1))
		//{
		//	DELOCUST_LOG("%d) Old position = %s. New Position = %s", i, oldPos.c_str(), positions[i].ToString().c_str());
		//}
	}
	m_kdTree = new Math::KDTree(m_positions, m_vertexCount, m_kdTreeSamples);
#endif
}

Rendering::TextMesh::TextMesh(Math::Vector2D* screenPositions, Math::Vector2D* textureCoordinates, int verticesCount, GLenum mode /* = GL_TRIANGLES */) :
	Mesh(mode)
{
	AddVertices(screenPositions, textureCoordinates, verticesCount);
}

Rendering::TextMesh::~TextMesh(void)
{
}

void Rendering::TextMesh::Draw() const
{
	CHECK_CONDITION_EXIT(m_meshData != NULL, Critical, "Mesh data instance is NULL");

	m_meshData->Bind();
	glDrawArrays(m_mode, 0, m_meshData->GetSize());
	m_meshData->Unbind();
}

void Rendering::TextMesh::ReplaceData(Math::Vector2D* screenPositions, Math::Vector2D* textureCoordinates, int verticesCount)
{
	//CHECK_CONDITION_EXIT(m_meshData != NULL, Critical, "Mesh data instance is NULL");
	//glBindBuffer(GL_ARRAY_BUFFER, m_meshData->GetVBO());
	//glBufferSubData(GL_ARRAY_BUFFER, 0, screenVerticesCount * sizeof(Vertex2D), screenVertices);

	SAFE_DELETE(m_meshData);

	// TODO: Optimize this! Removing whole VAO and VBOs just to replace it with other VAO and VBOs seems unneccessary.

	AddVertices(screenPositions, textureCoordinates, verticesCount);
}