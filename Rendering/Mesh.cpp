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

/* ==================== MeshData class implementation begin ==================== */
Rendering::MeshData::MeshData(GLsizei size) :
	m_vao(0),
	m_buffers({ {0, 0, 0, 0, 0, 0, 0} }),
	m_size(size)
{
	glGenVertexArrays(1, &m_vao);
	CHECK_CONDITION_EXIT_ALWAYS_RENDERING(m_vao != 0, Utility::Logging::CRITICAL, "VAO has not been created successfully. Its ID is still 0.");
	DELOCUST_LOG_RENDERING("Created mesh data: ", *this);
}


Rendering::MeshData::~MeshData(void)
{
	for (auto bufferItr = m_buffers.begin(); bufferItr != m_buffers.end(); ++bufferItr)
	{
		if ((*bufferItr) != 0)
		{
			glDeleteBuffers(1, &(*bufferItr));
			*bufferItr = 0;
		}
	}
	if (m_vao != 0)
	{
		glDeleteVertexArrays(1, &m_vao);
		m_vao = 0;
	}
}

Rendering::MeshData::MeshData(MeshData&& meshData) :
	m_vao(std::move(meshData.m_vao)),
	m_buffers(std::move(meshData.m_buffers)),
	m_size(std::move(meshData.m_size))
{
	meshData.m_vao = 0;
	meshData.m_buffers.fill(0);
}

GLuint Rendering::MeshData::CreateVBO(MeshBufferTypes::MeshBufferType buffer)
{
	Rendering::CheckErrorCode(__FUNCTION__, "Started VBO creation");
	glGenBuffers(1, &m_buffers[buffer]);
	DEBUG_LOG_RENDERING("Created VBO at index ", buffer, " for mesh data \"", *this, "\"");
	Rendering::CheckErrorCode(__FUNCTION__, "Finished VBO creation");
	return m_buffers[buffer];
}
/* ==================== MeshData class implementation end ==================== */

/* ==================== Mesh class implementation begin ==================== */
Rendering::Mesh::Mesh(GLenum mode /* = GL_TRIANGLES */) :
#ifdef STORE_MESH_FILE_NAME
	m_fileName(""),
#endif
	m_mode(mode),
	m_meshData(nullptr)
{
}

Rendering::Mesh::Mesh(int* indices, int indicesCount, int verticesCount, Math::Vector3D* positions, Math::Vector2D* textureCoordinates /* = nullptr */,
	Math::Vector3D* normals /* = nullptr */, Math::Vector3D* tangents /* = nullptr */, Math::Vector3D* bitangents /* = nullptr */, bool calcNormalsEnabled /* = false */, GLenum mode /* = GL_TRIANGLES */) :
#ifdef STORE_MESH_FILE_NAME
	m_fileName(""),
#endif
	m_mode(mode),
	m_meshData(nullptr)
{
	AddVertices(positions, textureCoordinates, normals, tangents, bitangents, verticesCount, indices, indicesCount, calcNormalsEnabled);
}

Rendering::Mesh::Mesh(Math::Vector2D* screenPositions, Math::Vector2D* textureCoordinates, unsigned int verticesCount, GLenum mode /* = GL_TRIANGLE_STRIP */) :
#ifdef STORE_MESH_FILE_NAME
	m_fileName(""),
#endif
	m_mode(mode),
	m_meshData(nullptr)
{
	CHECK_CONDITION_EXIT_RENDERING(verticesCount > 0, Utility::Logging::ERR, "Cannot create a mesh. Specified number of vertices is not greater than 0 (", verticesCount, ")");
	CHECK_CONDITION_EXIT_RENDERING(screenPositions != nullptr, Utility::Logging::ERR, "Cannot create a mesh. Specified positions array is nullptr.");
	AddVertices(screenPositions, textureCoordinates, verticesCount);
}

Rendering::Mesh::Mesh(const std::string& fileName, GLenum mode /* = GL_TRIANGLES */) :
#ifdef STORE_MESH_FILE_NAME
	m_fileName(fileName),
#endif
	m_mode(mode),
	m_meshData(nullptr)
{
	Rendering::CheckErrorCode(__FUNCTION__, "Started Mesh initialization");
	CHECK_CONDITION_RENDERING(!fileName.empty(), Utility::Logging::ERR, "Mesh data cannot be initialized. File name is not specified");

#ifdef MEASURE_MESH_TIME_ENABLED
	Utility::Timing::Timer timer;
	timer.Start();
#endif
	INFO_LOG_RENDERING("Loading model from file \"", fileName, "\"");

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(fileName.c_str(),
		aiProcess_Triangulate | /* aiProcess_FlipWindingOrder | */
		aiProcess_GenSmoothNormals |
		aiProcess_FlipUVs |
		aiProcess_CalcTangentSpace);

	CHECK_CONDITION_EXIT_RENDERING(scene != nullptr, Utility::Logging::CRITICAL, "Error while loading a mesh \"", fileName, "\"");
	CHECK_CONDITION_EXIT_RENDERING((scene->mMeshes != nullptr) && (scene->mNumMeshes > 0), Utility::Logging::CRITICAL,
		"Incorrect number of meshes loaded- ", scene->mNumMeshes, "- check the model \"", fileName,
		"\". One of the possible solutions is to check whether the model has any additional lines at the end.");

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
		if (pTangent == nullptr)
		{
			ERROR_LOG_RENDERING("Tangent calculated incorrectly for the mesh file name \"", fileName, "\"");
			pTangent = &aiZeroVector;
		}
		//const aiVector3D* pBitangent = model->HasTangentsAndBitangents() ? &(model->mBitangents[i]) : &aiZeroVector;
		//if (pBitangent == nullptr)
		//{
		//	ERROR_LOG_RENDERING("Bitangent calculated incorrectly");
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
		CHECK_CONDITION_ALWAYS_RENDERING(face.mNumIndices == 3, Utility::Logging::WARNING, "The face has ", face.mNumIndices, " indices when only triangle faces are supported.");
		indices.push_back(face.mIndices[0]);
		indices.push_back(face.mIndices[1]);
		indices.push_back(face.mIndices[2]);
	}
	SavePositions(positions); // used by TerrainMesh to save the positions. For Mesh instances it does nothing as it is not necessary to store them.
	AddVertices(&positions[0], &textureCoordinates[0], &normals[0], &tangents[0], nullptr, static_cast<int>(positions.size()), (int*)&indices[0], static_cast<int>(indices.size()), false);

#ifdef MEASURE_MESH_TIME_ENABLED
	timer.Stop();
	INFO_LOG_RENDERING("Loading model took ", timer.GetTimeSpan().ToString());
#endif
	Rendering::CheckErrorCode(__FUNCTION__, "Finished Mesh initialization");
}

Rendering::Mesh::~Mesh(void)
{
}

Rendering::Mesh::Mesh(Mesh&& mesh) :
#ifdef STORE_MESH_FILE_NAME
	m_fileName(std::move(mesh.m_fileName)),
#endif
	m_mode(std::move(mesh.m_mode)),
	m_meshData(std::move(mesh.m_meshData)) // http://stackoverflow.com/questions/29643974/using-stdmove-with-stdshared-ptr
{
#ifdef STORE_MESH_FILE_NAME
	DELOCUST_LOG_RENDERING("Mesh \"", m_fileName, "\" moved.");
#else
	DELOCUST_LOG_RENDERING("Mesh move constructor");
#endif
}

void Rendering::Mesh::AddVertices(Math::Vector2D* positions, Math::Vector2D* textureCoordinates, int verticesCount)
{
	Rendering::CheckErrorCode(__FUNCTION__, "Started adding 2D vertices to the Mesh");
#ifdef DELOCUST_ENABLED
	for (size_t i = 0; i < verticesCount; ++i)
	{
		DELOCUST_LOG_RENDERING("vertex[", i, "]:\n\tPos:\t", positions[i].ToString(), "\n\tTex:\t", textureCoordinates[i].ToString(), "\n\tNormal:\t", normals[i].ToString());
	}
	for (size_t i = 0; i < indicesCount; ++i)
	{
		DELOCUST_LOG_RENDERING("index[", i, "]: ", indices[i]);
	}
#endif
	m_meshData = std::make_shared<MeshData>(static_cast<GLsizei>(verticesCount)); // TODO: size_t is bigger than GLsizei, so errors will come if indicesCount > 2^32.

	CHECK_CONDITION_EXIT_RENDERING(m_meshData != nullptr, Utility::Logging::CRITICAL, "Mesh data instance is nullptr");
	m_meshData->Bind();
	m_meshData->CreateVBO(MeshBufferTypes::POSITIONS);
	glBindBuffer(GL_ARRAY_BUFFER, m_meshData->GetVBO(MeshBufferTypes::POSITIONS));
	glBufferData(GL_ARRAY_BUFFER, verticesCount * sizeof(Math::Vector2D), positions, GL_STATIC_DRAW);
	glVertexAttribPointer(MeshAttributeLocations::POSITIONS, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(MeshAttributeLocations::POSITIONS);

	if (textureCoordinates != nullptr)
	{
		m_meshData->CreateVBO(MeshBufferTypes::TEXTURE_COORDINATES);
		glBindBuffer(GL_ARRAY_BUFFER, m_meshData->GetVBO(MeshBufferTypes::TEXTURE_COORDINATES));
		glBufferData(GL_ARRAY_BUFFER, verticesCount * sizeof(Math::Vector2D), textureCoordinates, GL_STATIC_DRAW);
		glVertexAttribPointer(MeshAttributeLocations::TEXTURE_COORDINATES, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(MeshAttributeLocations::TEXTURE_COORDINATES);
	}

	m_meshData->Unbind();
	Rendering::CheckErrorCode(__FUNCTION__, "Finished adding 2D vertices to the Mesh");
}

void Rendering::Mesh::FillBuffer(MeshBufferTypes::MeshBufferType buffer, MeshAttributeLocations::MeshAttributeLocation attributeLocation, Math::Real* data, unsigned int dataCount)
{
	Rendering::CheckErrorCode(__FUNCTION__, "Started adding new values to the Mesh");
#ifdef DELOCUST_ENABLED
	for (size_t i = 0; i < positionsCount; ++i)
	{
		DELOCUST_LOG_RENDERING("positions[", i, "] = ", positions[i]);
	}
#endif
	CHECK_CONDITION_EXIT_RENDERING(m_meshData != nullptr, Utility::Logging::CRITICAL, "Mesh data instance is nullptr");
	CHECK_CONDITION_RENDERING(!m_meshData->HasVBO(buffer), Utility::Logging::WARNING, "Filling buffer that is already initialized.");
	m_meshData->Bind();
	if (!m_meshData->HasVBO(buffer))
	{
		m_meshData->CreateVBO(buffer);
	}
	glBindBuffer(GL_ARRAY_BUFFER, m_meshData->GetVBO(buffer));
	glBufferData(GL_ARRAY_BUFFER, dataCount * sizeof(Math::Real), data, GL_STATIC_DRAW);
	glVertexAttribPointer(attributeLocation, 3, GL_FLOAT, GL_FALSE, 0, 0); // TODO: The hard-coded values here are the possible reason for adding additional parameters to the function.
	glEnableVertexAttribArray(attributeLocation);
	m_meshData->Unbind();
	Rendering::CheckErrorCode(__FUNCTION__, "Finished adding new values to the Mesh");
}

void Rendering::Mesh::AddVertices(Math::Vector3D* positions, Math::Vector2D* textureCoordinates, Math::Vector3D* normals, Math::Vector3D* tangents, Math::Vector3D* bitangents, int verticesCount, int* indices, int indicesCount, bool calcNormalsEnabled)
{
	Rendering::CheckErrorCode(__FUNCTION__, "Started adding 3D vertices to the Mesh");
#ifdef DELOCUST_ENABLED
	for (size_t i = 0; i < verticesCount; ++i)
	{
		DELOCUST_LOG_RENDERING("vertex[", i, "]:\n\tPos:\t", positions[i], "\n\tTex:\t", textureCoordinates[i], "\n\tNormal:\t", normals[i]);
	}
	for (size_t i = 0; i < indicesCount; ++i)
	{
		DELOCUST_LOG_RENDERING("index[", i, "]: ", indices[i]);
	}
#endif
	m_meshData = std::make_shared<MeshData>(static_cast<GLsizei>((indices != nullptr) ? indicesCount : verticesCount)); // TODO: size_t is bigger than GLsizei, so errors will come if indicesCount > 2^32.

	if (calcNormalsEnabled)
	{
		CalcNormals(normals, positions, verticesCount, indices, indicesCount);
	}
	//if (calcTangents)
	//{
	//	CalcTangents(vertices, verticesCount);
	//}

	CHECK_CONDITION_EXIT_RENDERING(m_meshData != nullptr, Utility::Logging::CRITICAL, "Mesh data instance is nullptr");
	m_meshData->Bind();
	glBindBuffer(GL_ARRAY_BUFFER, m_meshData->CreateVBO(MeshBufferTypes::POSITIONS));
	glBufferData(GL_ARRAY_BUFFER, verticesCount * sizeof(Math::Vector3D), positions, GL_STATIC_DRAW);
	glVertexAttribPointer(MeshAttributeLocations::POSITIONS, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(MeshAttributeLocations::POSITIONS);

	if (textureCoordinates != nullptr)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_meshData->CreateVBO(MeshBufferTypes::TEXTURE_COORDINATES));
		glBufferData(GL_ARRAY_BUFFER, verticesCount * sizeof(Math::Vector2D), textureCoordinates, GL_STATIC_DRAW);
		glVertexAttribPointer(MeshAttributeLocations::TEXTURE_COORDINATES, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(MeshAttributeLocations::TEXTURE_COORDINATES);
	}

	if (normals != nullptr)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_meshData->CreateVBO(MeshBufferTypes::NORMALS));
		glBufferData(GL_ARRAY_BUFFER, verticesCount * sizeof(Math::Vector3D), normals, GL_STATIC_DRAW);
		glVertexAttribPointer(MeshAttributeLocations::NORMALS, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(MeshAttributeLocations::NORMALS);
	}

	if (tangents != nullptr)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_meshData->CreateVBO(MeshBufferTypes::TANGENTS));
		glBufferData(GL_ARRAY_BUFFER, verticesCount * sizeof(Math::Vector3D), tangents, GL_STATIC_DRAW);
		glVertexAttribPointer(MeshAttributeLocations::TANGENTS, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(MeshAttributeLocations::TANGENTS);
	}

	if (bitangents != nullptr)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_meshData->CreateVBO(MeshBufferTypes::BITANGENTS));
		glBufferData(GL_ARRAY_BUFFER, verticesCount * sizeof(Math::Vector3D), bitangents, GL_STATIC_DRAW);
		glVertexAttribPointer(MeshAttributeLocations::BITANGENTS, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(MeshAttributeLocations::BITANGENTS);
	}

	if (indices != nullptr)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_meshData->CreateVBO(MeshBufferTypes::INDEX));
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
	CHECK_CONDITION_EXIT_RENDERING(m_meshData != nullptr, Utility::Logging::CRITICAL, "Mesh data instance is nullptr");

	m_meshData->Bind();

	if (m_meshData->HasVBO(MeshBufferTypes::INDEX))
	{
		//CRITICAL_LOG_RENDERING("Drawing elements for mesh ", m_fileName);
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
/* ==================== Mesh class implementation end ==================== */

/* ==================== BillboardMesh class implementation begin ==================== */
Rendering::BillboardMesh::BillboardMesh(Math::Real* modelMatricesValues, unsigned int billboardsCount, unsigned int billboardDataLength) :
	Mesh(GL_POINTS),
	m_billboardsCount(billboardsCount)
{
	Math::Vector3D zeroVector(REAL_ZERO, REAL_ZERO, REAL_ZERO);
	AddVertices(&zeroVector, nullptr, nullptr, nullptr, nullptr, 1, nullptr, 0, false);

	CHECK_CONDITION_EXIT_RENDERING(billboardsCount > 0, Utility::Logging::ERR, "Cannot create a billboard mesh. Specified number of billboards is not greater than 0 (", billboardsCount, ")");
	CHECK_CONDITION_EXIT_RENDERING(billboardDataLength > 0, Utility::Logging::ERR, "Cannot create a billboard mesh. Specified billboard data length is not greater than 0 (", billboardDataLength, ")");

	m_meshData->Bind();
	glBindBuffer(GL_ARRAY_BUFFER, m_meshData->CreateVBO(MeshBufferTypes::INSTANCE));
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

Rendering::BillboardMesh::BillboardMesh(BillboardMesh&& billboardMesh) :
	Mesh(std::move(billboardMesh)),
	m_billboardsCount(std::move(billboardMesh.m_billboardsCount))
{
}

void Rendering::BillboardMesh::Draw() const
{
	CHECK_CONDITION_EXIT_RENDERING(m_meshData != nullptr, Utility::Logging::CRITICAL, "Mesh data instance is nullptr");
	m_meshData->Bind();

	glDrawArraysInstanced(m_mode, 0, 1, m_billboardsCount);

	m_meshData->Unbind();
}
/* ==================== BillboardMesh class implementation end ==================== */


/* ==================== InstanceMesh class implementation begin ==================== */
Rendering::InstanceMesh::InstanceMesh(Math::Vector2D* positions, unsigned int positionsCount, unsigned int maxParticlesCount, unsigned int instanceDataLength) :
	Mesh(GL_TRIANGLE_STRIP),
	m_positionsCount(positionsCount),
	m_maxParticlesCount(maxParticlesCount),
	m_instanceDataLength(instanceDataLength)
{
	CHECK_CONDITION_EXIT_RENDERING(positionsCount > 0, Utility::Logging::ERR, "Cannot create a mesh. Specified number of positions is not greater than 0 (", positionsCount, ")");
	CHECK_CONDITION_EXIT_RENDERING(m_maxParticlesCount > 0, Utility::Logging::ERR, "Cannot create a mesh. Specified maximum number of particles is not greater than 0 (", m_maxParticlesCount, ")");
	CHECK_CONDITION_EXIT_RENDERING(m_instanceDataLength > 0, Utility::Logging::ERR, "Cannot create a instance mesh. Specified instance data length is not greater than 0 (", instanceDataLength, ").");
	
	AddVertices(positions, nullptr, positionsCount);

	m_meshData->Bind();
	m_meshData->CreateVBO(MeshBufferTypes::INSTANCE); // instanced attributes will be stored in this VBO
	glBindBuffer(GL_ARRAY_BUFFER, m_meshData->GetVBO(MeshBufferTypes::INSTANCE));
	glBufferData(GL_ARRAY_BUFFER, sizeof(Math::Real) * m_maxParticlesCount * m_instanceDataLength, nullptr, GL_STREAM_DRAW);
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
#ifdef TEXTURE_ATLAS_OFFSET_CALCULATION
	glVertexAttribPointer(5 /* TEXTURE_ATLAS_OFFSETS_LOCATION */, 4, GL_FLOAT, GL_FALSE, m_instanceDataLength * sizeof(Math::Real), (GLvoid*)(16 * sizeof(Math::Real)));
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

Rendering::InstanceMesh::InstanceMesh(InstanceMesh&& instanceMesh) :
	Mesh(std::move(instanceMesh)),
	m_positionsCount(instanceMesh.m_positionsCount),
	m_maxParticlesCount(instanceMesh.m_maxParticlesCount),
	m_instanceDataLength(instanceMesh.m_instanceDataLength)
{
}

void Rendering::InstanceMesh::Draw(Math::Real* data, unsigned int dataSize, unsigned int particlesCount) const
{
	CHECK_CONDITION_EXIT_RENDERING(m_meshData != nullptr, Utility::Logging::CRITICAL, "Mesh data instance is nullptr");

	// Updating the instance VBO begin
	//m_meshData->Bind();
	glBindBuffer(GL_ARRAY_BUFFER, m_meshData->GetVBO(MeshBufferTypes::INSTANCE));
	glBufferData(GL_ARRAY_BUFFER, sizeof(Math::Real) * m_maxParticlesCount * m_instanceDataLength, data, GL_STREAM_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Math::Real) * dataSize, data);
	//m_meshData->Unbind();
	// Updating the instance VBO end

	m_meshData->Bind();
	
	glDrawArraysInstanced(m_mode, 0, m_positionsCount /* * 2 */, particlesCount);

	m_meshData->Unbind();
}
/* ==================== InstanceMesh class implementation end ==================== */


/* ==================== TerrainMesh class implementation begin ==================== */
Rendering::TerrainMesh::TerrainMesh(const std::string& fileName, GLenum mode /* = GL_TRIANGLES */) :
	Mesh(fileName, mode),
	m_x(0),
	m_z(0),
	m_vertexCount(0)
#ifdef HEIGHTS_KD_TREE
	,m_positions(),
	m_kdTree(nullptr),
	m_kdTreeSamples(GET_CONFIG_VALUE_RENDERING("kdTreeSamples", 8))
#elif defined HEIGHTS_HEIGHTMAP
	, m_heights(),
	m_gridSquareSize(0)
#endif
{
	DEBUG_LOG_RENDERING("Terrain mesh has been created.");
}

Rendering::TerrainMesh::TerrainMesh(int gridX, int gridZ, const std::string& heightMapFileName, GLenum mode /* = GL_TRIANGLES */) :
	Mesh(mode),
	m_x(gridX),
	m_z(gridZ),
	m_vertexCount(0)
#ifdef HEIGHTS_KD_TREE
	, m_positions(),
	m_kdTree(nullptr),
	m_kdTreeSamples(GET_CONFIG_VALUE_RENDERING("kdTreeSamples", 8))
#elif defined HEIGHTS_HEIGHTMAP
	, m_heightMapWidth(0),
	m_heightMapHeight(0),
	m_heights(),
	m_gridSquareSize(0)
#endif
{
	DEBUG_LOG_RENDERING("Terrain mesh construction has started.");

	/* Loading heightmap begin */
	std::string name = heightMapFileName;
	const char *tmp = strrchr(heightMapFileName.c_str(), '\\');
	if (tmp != nullptr)
	{
		name.assign(tmp + 1);
	}
	int bytesPerPixel;
	unsigned char* heightMapData = stbi_load(("C:\\Users\\aosesik\\Documents\\Visual Studio 2015\\Projects\\GameEngine\\Textures\\" + heightMapFileName).c_str(),
		&m_heightMapWidth, &m_heightMapHeight, &bytesPerPixel, 1 /* we only care about one RED component for now (the heightmap is grayscale) */);
	CHECK_CONDITION_EXIT_RENDERING(heightMapData != nullptr, Utility::Logging::ERR, "Unable to load terrain height map from the file \"", name, "\"");
	CHECK_CONDITION_RENDERING(m_heightMapWidth < 32768 && m_heightMapHeight < 32768, Utility::Logging::EMERGENCY, "The heightmap's size is too big to be used in the rendering engine.");
	//for (int i = 0; i < heightMapWidth; ++i)
	//{
	//	for (int j = 0; j < heightMapHeight; ++j)
	//	{
	//		CRITICAL_LOG_RENDERING("HeightMap[", i * heightMapWidth + j, "] ([", i, "][", j, "]) = ", heightMapData[i * heightMapWidth + j]);
	//	}
	//}
	/* Loading heightmap finished */

	m_vertexCount = m_heightMapHeight; // The number of vertices along each side of the single terrain tile. It is equal to the height of the height map image.
	const int vertexCountMinusOne = m_vertexCount - 1;
#ifdef HEIGHTS_KD_TREE
	//m_vertexCount = VERTEX_COUNT * VERTEX_COUNT;
#else
	m_heights.reserve(m_heightMapWidth * m_heightMapHeight);
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
			//DEBUG_LOG_RENDERING("counter = ", positions.size(), "; x = ", x, "; z = ", z, "; Position = ", position.ToString());
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
		//CRITICAL_LOG_RENDERING("vertex[", i, "]:\n\tPos:\t", vertexItr->m_pos.ToString(), "\n\tTex:\t", vertexItr->m_texCoord.ToString(), "\n\tNormal:\t", vertexItr->m_normal.ToString());
	//}
	//for (size_t i = 0; i < INDICES_COUNT; ++i)
	//{
	//	ERROR_LOG_RENDERING("index[", i, "]: ", indices[i]);
	//}
//#endif

	SavePositions(positions);
	AddVertices(&positions[0], &textureCoordinates[0], &normals[0], &tangents[0], nullptr, static_cast<int>(positions.size()), &indices[0], static_cast<int>(indices.size()), false);

	DEBUG_LOG_RENDERING("Terrain mesh has been created.");
}

Rendering::TerrainMesh::TerrainMesh(int gridX, int gridZ, const Math::HeightsGenerator& heightsGenerator, int vertexCount, GLenum mode /* = GL_TRIANGLES */) :
	Mesh(mode),
	m_x(gridX),
	m_z(gridZ),
	m_vertexCount(vertexCount)
#ifdef HEIGHTS_KD_TREE
	, m_positions(m_vertexCount),
	m_kdTree(nullptr),
	m_kdTreeSamples(GET_CONFIG_VALUE_RENDERING("kdTreeSamples", 8))
#elif defined HEIGHTS_HEIGHTMAP
	, m_heightMapWidth(vertexCount),
	m_heightMapHeight(vertexCount),
	m_heights(m_heightMapWidth * m_heightMapHeight),
	m_gridSquareSize(0)
#endif
{
	DEBUG_LOG_RENDERING("Terrain mesh construction has started.");
	const int vertexCountMinusOne = m_vertexCount - 1;
#ifdef HEIGHTS_KD_TREE
	//m_vertexCount = VERTEX_COUNT * VERTEX_COUNT;
#else
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
			Math::Real terrainHeight = CalculateHeightAt(x, z, heightsGenerator);
			//DEBUG_LOG_RENDERING("counter = ", positions.size(), "; x = ", x, "; z = ", z, "; Position = ", position.ToString());
			positions.emplace_back(xReal / vertexCountMinusOne * SIZE, terrainHeight, zReal / vertexCountMinusOne * SIZE);
			textureCoordinates.emplace_back(xReal / vertexCountMinusOne, zReal / vertexCountMinusOne);
			normals.push_back(CalculateNormal(x, z, heightsGenerator));
			tangents.emplace_back(REAL_ONE, REAL_ZERO, REAL_ZERO); // TODO: Calculate tangent
		}
	}

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
	//	CRITICAL_LOG_RENDERING("vertex[", i, "]:\n\tPos:\t", vertexItr->m_pos.ToString(), "\n\tTex:\t", vertexItr->m_texCoord.ToString(), "\n\tNormal:\t", vertexItr->m_normal.ToString());
	//}
	//for (size_t i = 0; i < INDICES_COUNT; ++i)
	//{
	//	ERROR_LOG_RENDERING("index[", i, "]: ", indices[i]);
	//}
	//#endif

	SavePositions(positions);
	AddVertices(&positions[0], &textureCoordinates[0], &normals[0], &tangents[0], nullptr, static_cast<int>(positions.size()), &indices[0], static_cast<int>(indices.size()), false);
	DEBUG_LOG_RENDERING("Terrain mesh has been created.");
}

Rendering::TerrainMesh::~TerrainMesh(void)
{
#ifdef HEIGHTS_KD_TREE
#elif defined HEIGHTS_HEIGHTMAP
	//SAFE_DELETE_WHOLE_TABLE(m_heights, m_vertexCount);
#endif
}

int Rendering::TerrainMesh::GetHeightMapIndex(int x, int z) const
{
	return (m_heightMapWidth * m_heightMapHeight) - ((z + 1) * m_heightMapWidth) + x;
}

Math::Real Rendering::TerrainMesh::CalculateHeightAt(int x, int z, unsigned char* heightMapData)
{
	// TODO: Range checking
	CHECK_CONDITION_RETURN_RENDERING(x >= 0 && x < m_heightMapWidth && z >= 0 && z < m_heightMapHeight, REAL_ZERO,
		Utility::Logging::ERR, "Cannot determine the height of the terrain on (", x, ", ", z, ") position. It is out of range.");
	const int heightMapIndex = GetHeightMapIndex(x, z);
	CHECK_CONDITION_RENDERING(heightMapIndex >= 0 && heightMapIndex < m_heightMapWidth * m_heightMapHeight, Utility::Logging::ERR,
		"The heightmap index calculation is incorrect. Calculated index (", heightMapIndex, ") is out of range [0; ", m_heightMapWidth * m_heightMapHeight, ")");
	//DEBUG_LOG_RENDERING("Heightmap index for [", x, ", ", z, "] = ", heightMapIndex);
	Math::Real height = static_cast<Math::Real>(heightMapData[heightMapIndex]);
	height = ((height / MAX_PIXEL_COLOR) - 0.5f) * 2.0f * HEIGHTMAP_MAX_HEIGHT;
#ifdef HEIGHTS_HEIGHTMAP
	m_heights[heightMapIndex] = height;
#endif
	return height;
}

Math::Real Rendering::TerrainMesh::CalculateHeightAt(int x, int z, const Math::HeightsGenerator& heightsGenerator)
{
	// TODO: Range checking
	CHECK_CONDITION_RETURN_RENDERING(x >= 0 && x < m_heightMapWidth && z >= 0 && z < m_heightMapHeight, REAL_ZERO,
		Utility::Logging::ERR, "Cannot determine the height of the terrain on (", x, ", ", z, ") position. It is out of range.");
	const int heightMapIndex = GetHeightMapIndex(x, z);
	CHECK_CONDITION_RENDERING(heightMapIndex >= 0 && heightMapIndex < m_heightMapWidth * m_heightMapHeight, Utility::Logging::ERR,
		"The heightmap index calculation is incorrect. Calculated index (", heightMapIndex, ") is out of range [0; ", m_heightMapWidth * m_heightMapHeight, ")");
	//DEBUG_LOG_RENDERING("Heightmap index for [", x, ", ", z, "] = ", heightMapIndex);
	Math::Real height = heightsGenerator.GenerateHeight(static_cast<Math::Real>(x), static_cast<Math::Real>(z));
#ifdef HEIGHTS_HEIGHTMAP
	m_heights[heightMapIndex] = height;
#endif
	return height;
}

Math::Vector3D Rendering::TerrainMesh::CalculateNormal(int x, int z, unsigned char* heightMapData)
{
	Math::Real heightLeft = (x - 1) >= 0 ? CalculateHeightAt(x - 1, z, heightMapData) : REAL_ZERO;
	Math::Real heightRight = (x + 1) < m_heightMapWidth ? CalculateHeightAt(x + 1, z, heightMapData) : REAL_ZERO;
	Math::Real heightDown = (z - 1) >= 0 ? CalculateHeightAt(x, z - 1, heightMapData) : REAL_ZERO;
	Math::Real heightUp = (z + 1) < m_heightMapHeight ? CalculateHeightAt(x, z + 1, heightMapData) : REAL_ZERO;
	Math::Vector3D normal(heightLeft - heightRight, 2.0f, heightDown - heightUp);
	normal.Normalize();
	return normal;
}

Math::Vector3D Rendering::TerrainMesh::CalculateNormal(int x, int z, const Math::HeightsGenerator& heightsGenerator)
{
	Math::Real heightLeft = (x - 1) >= 0 ? CalculateHeightAt(x - 1, z, heightsGenerator) : REAL_ZERO;
	Math::Real heightRight = (x + 1) < m_heightMapWidth ? CalculateHeightAt(x + 1, z, heightsGenerator) : REAL_ZERO;
	Math::Real heightDown = (z - 1) >= 0 ? CalculateHeightAt(x, z - 1, heightsGenerator) : REAL_ZERO;
	Math::Real heightUp = (z + 1) < m_heightMapHeight ? CalculateHeightAt(x, z + 1, heightsGenerator) : REAL_ZERO;
	Math::Vector3D normal(heightLeft - heightRight, 2.0f, heightDown - heightUp);
	normal.Normalize();
	return normal;
}

/**
 * Performs the k-NN search in the 2-dimensional space in order to find the k closest points to the given point (xz).
 * See also: http://en.wikipedia.org/wiki/Nearest_neighbor_search
 */
Math::Real Rendering::TerrainMesh::GetHeightAt(Math::Real x, Math::Real z) const
{
#ifdef MEASURE_MESH_TIME_ENABLED
	Utility::Timing::Timer timer;
	timer.Start();
#endif
#if defined HEIGHTS_KD_TREE
	Math::Real y = m_kdTree->SearchNearestValue(x, z);
	//DEBUG_LOG_RENDERING("Height ", y, " returned for position \"", xz.ToString(), "\"");
#elif defined HEIGHTS_HEIGHTMAP
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
			//CRITICAL_LOG_RENDERING(i, ") Heights: [", gridX - 2, "][", gridZ + i, "] = ", ((gridX - 2 < 0) || (gridZ + i < 0)) ? -999.0f : m_heights[gridX - 2][gridZ + i],
			//	"; [", gridX - 1, "][", gridZ + i, "] = ", ((gridX - 1 < 0) || (gridZ + i < 0)) ? -999.0f : m_heights[gridX - 1][gridZ + i],
			//	"; [", gridX, "][", gridZ + i, "] = ", ((gridX < 0) || (gridZ + i < 0)) ? -999.0f : m_heights[gridX][gridZ + i],
			//	"; [", gridX + 1, "][", gridZ + i, "] = ", ((gridX + 1 < 0) || (gridZ + i < 0)) ? -999.0f : m_heights[gridX + 1][gridZ + i],
			//	"; [", gridX + 2, "][", gridZ + i, "] = ", ((gridX + 2 < 0) || (gridZ + i < 0)) ? -999.0f : m_heights[gridX + 2][gridZ + i]);
		//}
		//EMERGENCY_LOG_RENDERING("Grid = [", gridX, ", ", gridZ, "]. Coords = [", xCoord, ", ", zCoord, "]. Height[", GetHeightMapIndex(gridX, gridZ), "] = ",
		//	m_heights[GetHeightMapIndex(gridX, gridZ)], ", Height[", GetHeightMapIndex(gridX + 1, gridZ), "] = ",
		//	m_heights[GetHeightMapIndex(gridX + 1, gridZ)], ", Height[", GetHeightMapIndex(gridX, gridZ + 1), "] = ",
		//	m_heights[GetHeightMapIndex(gridX, gridZ + 1)], ". Final height = ", y);
	}
	else
	{
		y = Math::Interpolation::BarycentricInterpolation(1.0f, m_heights[GetHeightMapIndex(gridX + 1, gridZ)], 0.0f,
			1.0f, m_heights[GetHeightMapIndex(gridX + 1, gridZ + 1)], 1.0f,
			0.0f, m_heights[GetHeightMapIndex(gridX, gridZ + 1)], 1.0f,
			xCoord, zCoord);
		//ERROR_LOG_RENDERING("Grid = [", gridX, ", ", gridZ, "]. Coords = [", xCoord, ", ", zCoord, "]. Height[", GetHeightMapIndex(gridX + 1, gridZ), "] = ",
		//	m_heights[GetHeightMapIndex(gridX + 1, gridZ)], ", Height[", GetHeightMapIndex(gridX + 1, gridZ + 1), "] = ",
		//	m_heights[GetHeightMapIndex(gridX + 1, gridZ + 1)], ", Height[", GetHeightMapIndex(gridX, gridZ + 1), "] = ",
		//	m_heights[GetHeightMapIndex(gridX, gridZ + 1)], ". Final height = ", y);
	}
	//y -= 0.35f;
#endif

#ifdef MEASURE_MESH_TIME_ENABLED
	timer.Stop();
	DEBUG_LOG_RENDERING("Camera's height calculation took ", timer.GetTimeSpan(Utility::Timing::MICROSECOND).ToString());
#endif

	return y;
}

/**
 * TODO: See this page for a possible ways to optimize this function
 * (http://stackoverflow.com/questions/1041620/whats-the-most-efficient-way-to-erase-duplicates-and-sort-a-vector)
 */
void Rendering::TerrainMesh::SavePositions(const std::vector<Math::Vector3D>& positions)
{
#ifdef HEIGHTS_KD_TREE
#ifdef MEASURE_MESH_TIME_ENABLED
	clock_t begin = clock(); // TODO: Replace with Utility::Timer. Use QueryPerformanceCounter() instead of clock() function when measuring time. It is more accurate.
#endif
	DEBUG_LOG_RENDERING("Terrain consists of ", positions.size(), " positions");
	std::unordered_set<Math::Vector3D> verticesSet;
	for (unsigned int i = 0; i < positions.size(); ++i)
	{
		verticesSet.insert(positions[i]);
	}
	m_positions.assign(verticesSet.begin(), verticesSet.end()); // store only unique positions.
#ifdef MEASURE_MESH_TIME_ENABLED
	clock_t end = clock(); // TODO: Replace with Utility::Timer. Use QueryPerformanceCounter() instead of clock() function when measuring time. It is more accurate.
	DEBUG_LOG_RENDERING("Removing duplicates from the vector of positions took ", 1000.0 * static_cast<double>(end - begin) / (CLOCKS_PER_SEC), " [ms]", );
#endif

	//ISort::GetSortingObject(ISort::QUICK_SORT)->Sort(&m_positions[0], uniquePositions.size(), COMPONENT_Y);
	//INFO_LOG_RENDERING("The minimum value is ", m_positions[0].ToString());
	//INFO_LOG_RENDERING("The maximum value is ", m_positions[uniquePositions.size() - 1].ToString());

	m_vertexCount = m_positions.size();
	INFO_LOG_RENDERING("Terrain consists of ", m_vertexCount, " unique positions");
#endif

	/**
	 * TODO: Think of a nice way to store positions so that access time in GetHeightAt(x, z) is optimized. Maybe a Binary Tree? Maybe a k-d tree?
	 * Maybe find the minimum and maximum value for "Y" component of all positions and then use bucket sort (http://en.wikipedia.org/wiki/Bucket_sort)
	 * based on "Y" values?
	 */
}

void Rendering::TerrainMesh::TransformPositions(const Math::Matrix4D& transformationMatrix)
{
#ifdef HEIGHTS_KD_TREE
	DEBUG_LOG_RENDERING("Transformation matrix = \n", transformationMatrix.ToString());
	CHECK_CONDITION_EXIT_RENDERING(!m_positions.empty(), Utility::EMERGENCY, "Cannot transform the positions. The positions array is empty.");
	for (int i = 0; i < m_vertexCount; ++i)
	{
		//std::string oldPos = positions[i].ToString();
		m_positions[i] = transformationMatrix * m_positions[i]; // FIXME: Check matrix multiplication
		//if ((i % 1000 == 0) || (i == m_vertexCount - 1))
		//{
		//	DELOCUST_LOG_RENDERING(i, ") Old position = ", oldPos, ". New Position = ", positions[i].ToString());
		//}
	}
	m_kdTree = std::make_unique<Math::KDTree>(m_positions, m_vertexCount, m_kdTreeSamples);
#endif
}
/* ==================== TerrainMesh class implementation end ==================== */

/* ==================== TextMesh class implementation begin ==================== */
Rendering::TextMesh::TextMesh(Math::Vector2D* screenPositions, Math::Vector2D* textureCoordinates, int verticesCount, GLenum mode /* = GL_TRIANGLES */) :
	Mesh(mode)
{
	AddVertices(screenPositions, textureCoordinates, verticesCount);
}

Rendering::TextMesh::~TextMesh(void)
{
}

Rendering::TextMesh::TextMesh(TextMesh&& textMesh) :
	Mesh(std::move(textMesh))
{
}

void Rendering::TextMesh::Draw() const
{
	CHECK_CONDITION_EXIT_RENDERING(m_meshData != nullptr, Utility::Logging::CRITICAL, "Mesh data instance is nullptr");

	m_meshData->Bind();
	glDrawArrays(m_mode, 0, m_meshData->GetSize());
	m_meshData->Unbind();
}

void Rendering::TextMesh::ReplaceData(Math::Vector2D* screenPositions, Math::Vector2D* textureCoordinates, int verticesCount)
{
	//CHECK_CONDITION_EXIT_RENDERING(m_meshData != nullptr, CRITICAL, "Mesh data instance is nullptr");
	//glBindBuffer(GL_ARRAY_BUFFER, m_meshData->GetVBO());
	//glBufferSubData(GL_ARRAY_BUFFER, 0, screenVerticesCount * sizeof(Vertex2D), screenVertices);

	// TODO: Optimize this! Removing whole VAO and VBOs just to replace it with other VAO and VBOs seems unneccessary.

	AddVertices(screenPositions, textureCoordinates, verticesCount);
}
/* ==================== TextMesh class implementation end ==================== */