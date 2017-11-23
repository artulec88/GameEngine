#include "StdAfx.h"
#include "Mesh.h"
//#include "CoreEngine.h"
//#include "OBJModel.h"
#include "Math/Vector.h"
#include "Math/FloatingPoint.h"
#include "stb_image.h"

#include "Utility/IConfig.h"
#include "Utility/ILogger.h"
#include "Utility/Time.h"

#include <unordered_set>

#ifndef MEASURE_MESH_TIME_ENABLED
#undef START_TIMER
#undef RESET_TIMER
#undef STOP_TIMER
#endif

#define UNBIND_BUFFER(buffer) glBindBuffer(buffer, 0);

/* ==================== MeshData class implementation begin ==================== */
rendering::MeshData::MeshData(GLsizei size) :
	m_vao(0),
	m_buffers({ {0, 0, 0, 0, 0, 0, 0} }),
	m_size(size)
{
	glGenVertexArrays(1, &m_vao);
	CHECK_CONDITION_EXIT_ALWAYS_RENDERING(m_vao != 0, utility::logging::CRITICAL, "VAO has not been created successfully. Its ID is still 0.");
	DELOCUST_LOG_RENDERING("Created mesh data: ", *this);
}


rendering::MeshData::~MeshData()
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

//rendering::MeshData::MeshData(MeshData&& meshData) :
//	m_vao(std::move(meshData.m_vao)),
//	m_buffers(std::move(meshData.m_buffers)),
//	m_size(std::move(meshData.m_size))
//{
//	meshData.m_vao = 0;
//	meshData.m_buffers.fill(0);
//}

GLuint rendering::MeshData::CreateVbo(mesh_buffer_types::MeshBufferType buffer)
{
	CheckErrorCode(__FUNCTION__, "Started VBO creation");
	glGenBuffers(1, &m_buffers[buffer]);
	DELOCUST_LOG_RENDERING("Created VBO at index ", buffer, " for mesh data \"", *this, "\"");
	CheckErrorCode(__FUNCTION__, "Finished VBO creation");
	return m_buffers[buffer];
}

void rendering::MeshData::ReplaceVbo(mesh_buffer_types::MeshBufferType buffer, void* data, int dataCount, int singleDataEntrySize, int singleDataComponentsCount)
{
	CheckErrorCode(__FUNCTION__, "Started VBO data replacement function");
	CHECK_CONDITION_EXIT_RENDERING(m_buffers[buffer] != 0, Utility::Logging::EMERGENCY,
		"Cannot replace data in the mesh data buffer. The buffer has not been intialized.");

	// TODO: Check special cases such as mesh_buffer_types::INDEX and mesh_buffer_types::INSTANCE.
	Bind();
	switch (buffer)
	{
	case mesh_buffer_types::INDEX:
		break;
	case mesh_buffer_types::POSITIONS:
		//glDeleteBuffers(1, &m_buffers[mesh_buffer_types::POSITIONS]);
		//glGenBuffers(1, &m_buffers[mesh_buffer_types::POSITIONS]);
		glBindBuffer(GL_ARRAY_BUFFER, m_buffers[mesh_buffer_types::POSITIONS]);
		glBufferData(GL_ARRAY_BUFFER, dataCount * singleDataEntrySize, data, GL_STATIC_DRAW);
		glVertexAttribPointer(mesh_attribute_locations::POSITIONS, singleDataComponentsCount, GL_FLOAT, GL_FALSE, 0, nullptr);
		glEnableVertexAttribArray(mesh_attribute_locations::POSITIONS);
		m_size = dataCount;
		break;
	case mesh_buffer_types::TEXTURE_COORDINATES:
		//glDeleteBuffers(1, &m_buffers[mesh_buffer_types::TEXTURE_COORDINATES]);
		//glGenBuffers(1, &m_buffers[mesh_buffer_types::TEXTURE_COORDINATES]);
		glBindBuffer(GL_ARRAY_BUFFER, m_buffers[mesh_buffer_types::TEXTURE_COORDINATES]);
		glBufferData(GL_ARRAY_BUFFER, dataCount * singleDataEntrySize, data, GL_STATIC_DRAW);
		glVertexAttribPointer(mesh_attribute_locations::TEXTURE_COORDINATES, singleDataComponentsCount, GL_FLOAT, GL_FALSE, 0, nullptr);
		glEnableVertexAttribArray(mesh_attribute_locations::TEXTURE_COORDINATES);
		break;
	case mesh_buffer_types::NORMALS:
		break;
	case mesh_buffer_types::TANGENTS:
		break;
	case mesh_buffer_types::BITANGENTS:
		break;
	case mesh_buffer_types::INSTANCE:
		break;
	case mesh_buffer_types::COUNT:
	default:
		ERROR_LOG_RENDERING("Invalid buffer specified: ", buffer, ".");
	}
	Unbind();
	rendering::CheckErrorCode(__FUNCTION__, "Finished VBO data replacement function");
}
/* ==================== MeshData class implementation end ==================== */

/* ==================== Mesh class implementation begin ==================== */
rendering::Mesh::Mesh(GLenum mode /* = GL_TRIANGLES */) :
	m_mode(mode),
	m_meshData(nullptr)
{
}

rendering::Mesh::Mesh(int* indices, int indicesCount, int verticesCount, math::Vector3D* positions, math::Vector2D* textureCoordinates /* = nullptr */,
	math::Vector3D* normals /* = nullptr */, math::Vector3D* tangents /* = nullptr */, math::Vector3D* bitangents /* = nullptr */, bool calcNormalsEnabled /* = false */, GLenum mode /* = GL_TRIANGLES */) :
	m_mode(mode),
	m_meshData(nullptr)
{
	AddVertices(positions, textureCoordinates, normals, tangents, bitangents, verticesCount, indices, indicesCount, calcNormalsEnabled);
}

rendering::Mesh::Mesh(math::Vector2D* screenPositions, math::Vector2D* textureCoordinates, unsigned int verticesCount, GLenum mode) :
	m_mode(mode),
	m_meshData(nullptr)
{
	CHECK_CONDITION_EXIT_RENDERING(verticesCount > 0, Utility::Logging::ERR, "Cannot create a mesh. Specified number of vertices is not greater than 0 (", verticesCount, ")");
	CHECK_CONDITION_EXIT_RENDERING(screenPositions != nullptr, Utility::Logging::ERR, "Cannot create a mesh. Specified positions array is nullptr.");
	AddVertices(screenPositions, textureCoordinates, verticesCount);
}

rendering::Mesh::~Mesh()
{
}

//rendering::Mesh::Mesh(Mesh&& mesh) :
//	m_mode(std::move(mesh.m_mode)),
//	m_meshData(std::move(mesh.m_meshData)) // http://stackoverflow.com/questions/29643974/using-stdmove-with-stdshared-ptr
//{
//#ifdef STORE_MESH_FILE_NAME
//	DELOCUST_LOG_RENDERING("Mesh \"", m_fileName, "\" moved.");
//#else
//	DELOCUST_LOG_RENDERING("Mesh move constructor");
//#endif
//}

void rendering::Mesh::AddVertices(math::Vector2D* positions, math::Vector2D* textureCoordinates, int verticesCount)
{
	rendering::CheckErrorCode(__FUNCTION__, "Started adding 2D vertices to the Mesh");
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
	m_meshData = std::make_shared<MeshData>(static_cast<GLsizei>(verticesCount)); // TODO: size_t is bigger than GLsizei, so errors will come if indicesCount > 2^32.

	CHECK_CONDITION_EXIT_RENDERING(m_meshData != nullptr, Utility::Logging::CRITICAL, "Mesh data instance is nullptr");
	m_meshData->Bind();
	m_meshData->CreateVbo(mesh_buffer_types::POSITIONS);
	glBindBuffer(GL_ARRAY_BUFFER, m_meshData->GetVbo(mesh_buffer_types::POSITIONS));
	glBufferData(GL_ARRAY_BUFFER, verticesCount * sizeof(math::Vector2D), positions, GL_STATIC_DRAW);
	glVertexAttribPointer(mesh_attribute_locations::POSITIONS, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(mesh_attribute_locations::POSITIONS);

	if (textureCoordinates != nullptr)
	{
		m_meshData->CreateVbo(mesh_buffer_types::TEXTURE_COORDINATES);
		glBindBuffer(GL_ARRAY_BUFFER, m_meshData->GetVbo(mesh_buffer_types::TEXTURE_COORDINATES));
		glBufferData(GL_ARRAY_BUFFER, verticesCount * sizeof(math::Vector2D), textureCoordinates, GL_STATIC_DRAW);
		glVertexAttribPointer(mesh_attribute_locations::TEXTURE_COORDINATES, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
		glEnableVertexAttribArray(mesh_attribute_locations::TEXTURE_COORDINATES);
	}

	m_meshData->Unbind();
	rendering::CheckErrorCode(__FUNCTION__, "Finished adding 2D vertices to the Mesh");
}

void rendering::Mesh::FillBuffer(mesh_buffer_types::MeshBufferType buffer, mesh_attribute_locations::MeshAttributeLocation attributeLocation, math::Real* data, unsigned int dataCount)
{
	rendering::CheckErrorCode(__FUNCTION__, "Started adding new values to the Mesh");
#ifdef DELOCUST_ENABLED
	for (size_t i = 0; i < positionsCount; ++i)
	{
		DELOCUST_LOG_RENDERING("positions[", i, "] = ", positions[i]);
	}
#endif
	CHECK_CONDITION_EXIT_RENDERING(m_meshData != nullptr, Utility::Logging::CRITICAL, "Mesh data instance is nullptr");
	CHECK_CONDITION_RENDERING(!m_meshData->HasVBO(buffer), Utility::Logging::WARNING, "Filling buffer that is already initialized.");
	m_meshData->Bind();
	if (!m_meshData->HasVbo(buffer))
	{
		m_meshData->CreateVbo(buffer);
	}
	glBindBuffer(GL_ARRAY_BUFFER, m_meshData->GetVbo(buffer));
	glBufferData(GL_ARRAY_BUFFER, dataCount * sizeof(math::Real), data, GL_STATIC_DRAW);
	glVertexAttribPointer(attributeLocation, 3, GL_FLOAT, GL_FALSE, 0, nullptr); // TODO: The hard-coded values here are the possible reason for adding additional parameters to the function.
	glEnableVertexAttribArray(attributeLocation);
	m_meshData->Unbind();
	rendering::CheckErrorCode(__FUNCTION__, "Finished adding new values to the Mesh");
}

void rendering::Mesh::AddVertices(math::Vector3D* positions, math::Vector2D* textureCoordinates, math::Vector3D* normals, math::Vector3D* tangents, math::Vector3D* bitangents, int verticesCount, int* indices, int indicesCount, bool calcNormalsEnabled)
{
	rendering::CheckErrorCode(__FUNCTION__, "Started adding 3D vertices to the Mesh");
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
	glBindBuffer(GL_ARRAY_BUFFER, m_meshData->CreateVbo(mesh_buffer_types::POSITIONS));
	glBufferData(GL_ARRAY_BUFFER, verticesCount * sizeof(math::Vector3D), positions, GL_STATIC_DRAW);
	glVertexAttribPointer(mesh_attribute_locations::POSITIONS, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(mesh_attribute_locations::POSITIONS);

	if (textureCoordinates != nullptr)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_meshData->CreateVbo(mesh_buffer_types::TEXTURE_COORDINATES));
		glBufferData(GL_ARRAY_BUFFER, verticesCount * sizeof(math::Vector2D), textureCoordinates, GL_STATIC_DRAW);
		glVertexAttribPointer(mesh_attribute_locations::TEXTURE_COORDINATES, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
		glEnableVertexAttribArray(mesh_attribute_locations::TEXTURE_COORDINATES);
	}

	if (normals != nullptr)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_meshData->CreateVbo(mesh_buffer_types::NORMALS));
		glBufferData(GL_ARRAY_BUFFER, verticesCount * sizeof(math::Vector3D), normals, GL_STATIC_DRAW);
		glVertexAttribPointer(mesh_attribute_locations::NORMALS, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
		glEnableVertexAttribArray(mesh_attribute_locations::NORMALS);
	}

	if (tangents != nullptr)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_meshData->CreateVbo(mesh_buffer_types::TANGENTS));
		glBufferData(GL_ARRAY_BUFFER, verticesCount * sizeof(math::Vector3D), tangents, GL_STATIC_DRAW);
		glVertexAttribPointer(mesh_attribute_locations::TANGENTS, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
		glEnableVertexAttribArray(mesh_attribute_locations::TANGENTS);
	}

	if (bitangents != nullptr)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_meshData->CreateVbo(mesh_buffer_types::BITANGENTS));
		glBufferData(GL_ARRAY_BUFFER, verticesCount * sizeof(math::Vector3D), bitangents, GL_STATIC_DRAW);
		glVertexAttribPointer(mesh_attribute_locations::BITANGENTS, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
		glEnableVertexAttribArray(mesh_attribute_locations::BITANGENTS);
	}

	if (indices != nullptr)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_meshData->CreateVbo(mesh_buffer_types::INDEX));
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesCount * sizeof(int), indices, GL_STATIC_DRAW);
	}

	m_meshData->Unbind();
	rendering::CheckErrorCode(__FUNCTION__, "Finished adding 3D vertices to the Mesh");
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

void rendering::Mesh::Draw() const
{
	CheckErrorCode(__FUNCTION__, "Started drawing the Mesh");
	CHECK_CONDITION_EXIT_RENDERING(m_meshData != nullptr, Utility::Logging::CRITICAL, "Mesh data instance is nullptr");

	m_meshData->Bind();

	if (m_meshData->HasVbo(mesh_buffer_types::INDEX))
	{
		//CRITICAL_LOG_RENDERING("Drawing elements for mesh ", m_fileName);
		glDrawElements(m_mode, m_meshData->GetSize() /* * sizeof(int) */, GL_UNSIGNED_INT, nullptr);
		//glDrawArrays(m_mode, 0, m_meshData->GetSize() /* * sizeof(float) */);
	}
	else
	{
		glDrawArrays(m_mode, 0, m_meshData->GetSize() /* * sizeof(float) */);
	}

	m_meshData->Unbind();
	CheckErrorCode(__FUNCTION__, "Finished drawing the Mesh");
}

void rendering::Mesh::ReplaceData(mesh_buffer_types::MeshBufferType buffer, void* data, int dataCount, int singleDataEntrySize, int singleDataComponentsCount)
{
	CheckErrorCode(__FUNCTION__, "Started replacing data in the Mesh");
	//CRITICAL_LOG_RENDERING("Replacing data in the mesh. The functionality has not been tested yet.");
	CHECK_CONDITION_RETURN_VOID_RENDERING(m_meshData->HasVBO(buffer), Utility::Logging::EMERGENCY,
		"Cannot replace data in buffer ", buffer, " because this buffer has not been initialized at all.");
	m_meshData->ReplaceVbo(buffer, data, dataCount, singleDataEntrySize, singleDataComponentsCount);
	CheckErrorCode(__FUNCTION__, "Finished replacing data in the Mesh");
}

void* rendering::Mesh::GetBufferData(mesh_buffer_types::MeshBufferType buffer, int* bufferEntriesCount) const
{
	CHECK_CONDITION_EXIT_RENDERING(m_meshData->HasVBO(buffer), Utility::Logging::EMERGENCY,
		"Retrieving data for the buffer type ", buffer, " cannot be performed. The buffer is 0.");

	//m_meshData->Bind();
	glBindBuffer(GL_ARRAY_BUFFER, m_meshData->GetVbo(buffer));

	const auto bufferSize = GetBufferSize(buffer);
	void* data = glMapBufferRange(GL_ARRAY_BUFFER, 0, bufferSize, GL_MAP_READ_BIT);
	glUnmapBuffer(GL_ARRAY_BUFFER);

	if (bufferEntriesCount != nullptr)
	{
		switch (buffer)
		{
		case mesh_buffer_types::POSITIONS:
		case mesh_buffer_types::NORMALS:
		case mesh_buffer_types::TANGENTS:
		case mesh_buffer_types::BITANGENTS:
			*bufferEntriesCount = bufferSize / sizeof(math::Vector3D);
			break;
		case mesh_buffer_types::TEXTURE_COORDINATES:
			*bufferEntriesCount = bufferSize / sizeof(math::Vector2D);
			break;
		case mesh_buffer_types::INDEX:
			*bufferEntriesCount = bufferSize / sizeof(int);
			break;
		case mesh_buffer_types::INSTANCE:
			*bufferEntriesCount = bufferSize / sizeof(math::Real);
			break;
		default:
			ERROR_LOG_RENDERING("Invalid buffer type (", buffer, ") specified. Cannot determine the number of entries in the buffer.");
			*bufferEntriesCount = 0;
		}
	}
	return data;
}

void rendering::Mesh::CalcNormals(math::Vector3D*& normals, math::Vector3D* positions, size_t verticesCount, const int* indices, size_t indicesCount) const
{
	// TODO: The value 3 for iterationStep works ok only for mode equal to GL_TRIANGLES.
	// For different modes (GL_QUADS, GL_LINES) this iterationStep variable will be incorrect
	const auto iterationStep = 3; // we are iterating through faces which are triangles (each triangle has 3 vertices)
	for(auto i = 0; i < indicesCount; i += iterationStep)
	{
		const auto i0 = indices[i];
		const auto i1 = indices[i + 1];
		const auto i2 = indices[i + 2];
		
		const auto v1 = positions[i1] - positions[i0];
		const auto v2 = positions[i2] - positions[i0];
		const auto normalVec = v1.Cross(v2).Normalized();
		
		normals[i0] += normalVec;
		normals[i1] += normalVec;
		normals[i2] += normalVec;
	}
	
	for(auto i = 0; i < verticesCount; i++)
	{
		normals[i].Normalize();
	}
}

void rendering::Mesh::CalcTangents(math::Vector3D*& tangents, math::Vector3D* positions, math::Vector2D* textureCoordinates, size_t verticesCount) const
{
	// TODO: The value 3 for iterationStep works ok only for mode equal to GL_TRIANGLES.
	// For different modes (Gl_QUADS, GL_LINES) this iterationStep variable will be incorrect
	const auto iterationStep = 3; // each face has three vertices
	for (auto i = 0; i < verticesCount - iterationStep + 1; i += iterationStep)
	{
		// Edges of the triangle: position delta
		const auto deltaPos1 = positions[i + 1] - positions[i];
		const auto deltaPos2 = positions[i + 2] - positions[i];

		// UV delta
		const auto deltaUv1 = textureCoordinates[i + 1] - textureCoordinates[i];
		const auto deltaUv2 = textureCoordinates[i + 2] - textureCoordinates[i];

		const auto r = REAL_ONE / (deltaUv1.Cross(deltaUv2));
		const auto tangent = (deltaPos1 * deltaUv2.y - deltaPos2 * deltaUv1.y) * r;
		//const auto bitangent = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x) * r;

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
	//	math::Vector3D& normal = vertices[i].normal;
	//	math::Vector3D& tangent = vertices[i].tangent;
	//	//math::Vector3D& bitangent = vertices[i].bitangent;

	//	// Gram-Schmidt orthogonalization
	//	tangent -= normal * normal.Dot(tangent);
	//	tangent.Normalize();

	//	// Calculate handedness
	//	//math::Vector3D cross = normal.Cross(tangent);
	//	//if (cross.Dot(bitangent) < 0)
	//	//{
	//	//	tangent.Negate();
	//	//}
	//}
}
/* ==================== Mesh class implementation end ==================== */

/* ==================== BillboardMesh class implementation begin ==================== */
rendering::BillboardMesh::BillboardMesh(math::Real* modelMatricesValues, unsigned int billboardsCount, unsigned int billboardDataLength) :
	Mesh(GL_POINTS),
	m_billboardsCount(billboardsCount)
{
	math::Vector3D zeroVector(REAL_ZERO, REAL_ZERO, REAL_ZERO);
	AddVertices(&zeroVector, nullptr, nullptr, nullptr, nullptr, 1, nullptr, 0, false);

	CHECK_CONDITION_EXIT_RENDERING(billboardsCount > 0, Utility::Logging::ERR, "Cannot create a billboard mesh. Specified number of billboards is not greater than 0 (", billboardsCount, ")");
	CHECK_CONDITION_EXIT_RENDERING(billboardDataLength > 0, Utility::Logging::ERR, "Cannot create a billboard mesh. Specified billboard data length is not greater than 0 (", billboardDataLength, ")");

	m_meshData->Bind();
	glBindBuffer(GL_ARRAY_BUFFER, m_meshData->CreateVbo(mesh_buffer_types::INSTANCE));
	glBufferData(GL_ARRAY_BUFFER, sizeof(math::Real) * m_billboardsCount * billboardDataLength, modelMatricesValues, GL_STATIC_DRAW);
	glVertexAttribPointer(1 /* MVP_MATRIX_COLUMN_1_LOCATION */, 4, GL_FLOAT, GL_FALSE, billboardDataLength * sizeof(math::Real), nullptr);
	glEnableVertexAttribArray(1 /* MVP_MATRIX_COLUMN_1_LOCATION */);
	glVertexAttribDivisor(1 /* MVP_MATRIX_COLUMN_1_LOCATION */, 1);
	glVertexAttribPointer(2 /* MVP_MATRIX_COLUMN_2_LOCATION */, 4, GL_FLOAT, GL_FALSE, billboardDataLength * sizeof(math::Real), (GLvoid*)(4 * sizeof(math::Real)));
	glEnableVertexAttribArray(2 /* MVP_MATRIX_COLUMN_2_LOCATION */);
	glVertexAttribDivisor(2 /* MVP_MATRIX_COLUMN_2_LOCATION */, 1);
	glVertexAttribPointer(3 /* MVP_MATRIX_COLUMN_3_LOCATION */, 4, GL_FLOAT, GL_FALSE, billboardDataLength * sizeof(math::Real), (GLvoid*)(8 * sizeof(math::Real)));
	glEnableVertexAttribArray(3 /* MVP_MATRIX_COLUMN_3_LOCATION */);
	glVertexAttribDivisor(3 /* MVP_MATRIX_COLUMN_3_LOCATION */, 1);
	glVertexAttribPointer(4 /* MVP_MATRIX_COLUMN_4_LOCATION */, 4, GL_FLOAT, GL_FALSE, billboardDataLength * sizeof(math::Real), (GLvoid*)(12 * sizeof(math::Real)));
	glEnableVertexAttribArray(4 /* MVP_MATRIX_COLUMN_4_LOCATION */);
	glVertexAttribDivisor(4 /* MVP_MATRIX_COLUMN_4_LOCATION */, 1);

	m_meshData->Unbind();
}

rendering::BillboardMesh::~BillboardMesh()
{
}

//rendering::BillboardMesh::BillboardMesh(BillboardMesh&& billboardMesh) :
//	Mesh(std::move(billboardMesh)),
//	m_billboardsCount(std::move(billboardMesh.m_billboardsCount))
//{
//}

void rendering::BillboardMesh::Draw() const
{
	CHECK_CONDITION_EXIT_RENDERING(m_meshData != nullptr, Utility::Logging::CRITICAL, "Mesh data instance is nullptr");
	m_meshData->Bind();

	glDrawArraysInstanced(m_mode, 0, 1, m_billboardsCount);

	m_meshData->Unbind();
}
/* ==================== BillboardMesh class implementation end ==================== */


/* ==================== InstanceMesh class implementation begin ==================== */
rendering::InstanceMesh::InstanceMesh(math::Vector2D* positions, unsigned int positionsCount, unsigned int maxParticlesCount, unsigned int instanceDataLength) :
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
	m_meshData->CreateVbo(mesh_buffer_types::INSTANCE); // instanced attributes will be stored in this VBO
	glBindBuffer(GL_ARRAY_BUFFER, m_meshData->GetVbo(mesh_buffer_types::INSTANCE));
	glBufferData(GL_ARRAY_BUFFER, sizeof(math::Real) * m_maxParticlesCount * m_instanceDataLength, nullptr, GL_STREAM_DRAW);
	glVertexAttribPointer(1 /* MVP_MATRIX_COLUMN_1_LOCATION */, 4, GL_FLOAT, GL_FALSE, m_instanceDataLength * sizeof(math::Real), nullptr);
	glEnableVertexAttribArray(1 /* MVP_MATRIX_COLUMN_1_LOCATION */);
	glVertexAttribDivisor(1 /* MVP_MATRIX_COLUMN_1_LOCATION */, 1);
	glVertexAttribPointer(2 /* MVP_MATRIX_COLUMN_2_LOCATION */, 4, GL_FLOAT, GL_FALSE, m_instanceDataLength * sizeof(math::Real), (GLvoid*)(4 * sizeof(math::Real)));
	glEnableVertexAttribArray(2 /* MVP_MATRIX_COLUMN_2_LOCATION */);
	glVertexAttribDivisor(2 /* MVP_MATRIX_COLUMN_2_LOCATION */, 1);
	glVertexAttribPointer(3 /* MVP_MATRIX_COLUMN_3_LOCATION */, 4, GL_FLOAT, GL_FALSE, m_instanceDataLength * sizeof(math::Real), (GLvoid*)(8 * sizeof(math::Real)));
	glEnableVertexAttribArray(3 /* MVP_MATRIX_COLUMN_3_LOCATION */);
	glVertexAttribDivisor(3 /* MVP_MATRIX_COLUMN_3_LOCATION */, 1);
	glVertexAttribPointer(4 /* MVP_MATRIX_COLUMN_4_LOCATION */, 4, GL_FLOAT, GL_FALSE, m_instanceDataLength * sizeof(math::Real), (GLvoid*)(12 * sizeof(math::Real)));
	glEnableVertexAttribArray(4 /* MVP_MATRIX_COLUMN_4_LOCATION */);
	glVertexAttribDivisor(4 /* MVP_MATRIX_COLUMN_4_LOCATION */, 1);
#ifdef TEXTURE_ATLAS_OFFSET_CALCULATION
	glVertexAttribPointer(5 /* TEXTURE_ATLAS_OFFSETS_LOCATION */, 4, GL_FLOAT, GL_FALSE, m_instanceDataLength * sizeof(math::Real), (GLvoid*)(16 * sizeof(math::Real)));
	glEnableVertexAttribArray(5 /* TEXTURE_ATLAS_OFFSETS_LOCATION */);
	glVertexAttribDivisor(5 /* TEXTURE_ATLAS_OFFSETS_LOCATION */, 1);
	glVertexAttribPointer(6 /* BLEND_FACTOR_LOCATION */, 1, GL_FLOAT, GL_FALSE, m_instanceDataLength * sizeof(math::Real), (GLvoid*)(20 * sizeof(math::Real)));
	glEnableVertexAttribArray(6 /* BLEND_FACTOR_LOCATION */);
	glVertexAttribDivisor(6 /* BLEND_FACTOR_LOCATION */, 1);
#else
	glVertexAttribPointer(5 /* LIFE_STAGE_FACTOR_LOCATION */, 1, GL_FLOAT, GL_FALSE, m_instanceDataLength * sizeof(math::Real), (GLvoid*)(16 * sizeof(math::Real)));
	glEnableVertexAttribArray(5 /* LIFE_STAGE_FACTOR_LOCATION */);
	glVertexAttribDivisor(5 /* LIFE_STAGE_FACTOR_LOCATION */, 1);
#endif

	m_meshData->Unbind();
}

rendering::InstanceMesh::~InstanceMesh()
{
}

//rendering::InstanceMesh::InstanceMesh(InstanceMesh&& instanceMesh) :
//	Mesh(std::move(instanceMesh)),
//	m_positionsCount(instanceMesh.m_positionsCount),
//	m_maxParticlesCount(instanceMesh.m_maxParticlesCount),
//	m_instanceDataLength(instanceMesh.m_instanceDataLength)
//{
//}

void rendering::InstanceMesh::Draw(math::Real* data, unsigned int dataSize, unsigned int particlesCount) const
{
	CHECK_CONDITION_EXIT_RENDERING(m_meshData != nullptr, Utility::Logging::CRITICAL, "Mesh data instance is nullptr");

	// Updating the instance VBO begin
	//m_meshData->Bind();
	glBindBuffer(GL_ARRAY_BUFFER, m_meshData->GetVbo(mesh_buffer_types::INSTANCE));
	glBufferData(GL_ARRAY_BUFFER, sizeof(math::Real) * m_maxParticlesCount * m_instanceDataLength, data, GL_STREAM_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(math::Real) * dataSize, data);
	//m_meshData->Unbind();
	// Updating the instance VBO end

	m_meshData->Bind();
	
	glDrawArraysInstanced(m_mode, 0, m_positionsCount /* * 2 */, particlesCount);

	m_meshData->Unbind();
}
/* ==================== InstanceMesh class implementation end ==================== */


/* ==================== TerrainMesh class implementation begin ==================== */
//Rendering::TerrainMesh::TerrainMesh(const std::string& fileName, GLenum mode /* = GL_TRIANGLES */) :
//	Mesh(fileName, mode),
//	m_x(0),
//	m_z(0),
//	m_vertexCount(0)
//#ifdef HEIGHTS_KD_TREE
//	,m_positions(),
//	m_kdTree(nullptr),
//	m_kdTreeSamples(GET_CONFIG_VALUE_RENDERING("kdTreeSamples", 8))
//#elif defined HEIGHTS_HEIGHTMAP
//	, m_heights(),
//	m_gridSquareSize(0)
//#endif
//{
//	DEBUG_LOG_RENDERING("Terrain mesh has been created.");
//}
//
//Rendering::TerrainMesh::TerrainMesh(int gridX, int gridZ, const std::string& heightMapFileName, GLenum mode /* = GL_TRIANGLES */) :
//	Mesh(mode),
//	m_x(gridX),
//	m_z(gridZ),
//	m_vertexCount(0)
//#ifdef HEIGHTS_KD_TREE
//	, m_positions(),
//	m_kdTree(nullptr),
//	m_kdTreeSamples(GET_CONFIG_VALUE_RENDERING("kdTreeSamples", 8))
//#elif defined HEIGHTS_HEIGHTMAP
//	, m_heightMapWidth(0),
//	m_heightMapHeight(0),
//	m_heights(),
//	m_gridSquareSize(0)
//#endif
//{
//	DEBUG_LOG_RENDERING("Terrain mesh construction has started.");
//
//	/* Loading heightmap begin */
//	std::string name = heightMapFileName;
//	const char *tmp = strrchr(heightMapFileName.c_str(), '\\');
//	if (tmp != nullptr)
//	{
//		name.assign(tmp + 1);
//	}
//	int bytesPerPixel;
//	unsigned char* heightMapData = stbi_load(heightMapFileName.c_str(), &m_heightMapWidth, &m_heightMapHeight, &bytesPerPixel,
//		1 /* we only care about one RED component for now (the heightmap is grayscale) */);
//	CHECK_CONDITION_EXIT_RENDERING(heightMapData != nullptr, Utility::Logging::ERR, "Unable to load terrain height map from the file \"", name, "\"");
//	CHECK_CONDITION_RENDERING(m_heightMapWidth < 32768 && m_heightMapHeight < 32768, Utility::Logging::EMERGENCY, "The heightmap's size is too big to be used in the rendering engine.");
//	//for (int i = 0; i < heightMapWidth; ++i)
//	//{
//	//	for (int j = 0; j < heightMapHeight; ++j)
//	//	{
//	//		CRITICAL_LOG_RENDERING("HeightMap[", i * heightMapWidth + j, "] ([", i, "][", j, "]) = ", heightMapData[i * heightMapWidth + j]);
//	//	}
//	//}
//	/* Loading heightmap finished */
//
//	m_vertexCount = m_heightMapHeight; // The number of vertices along each side of the single terrain tile. It is equal to the height of the height map image.
//	const int vertexCountMinusOne = m_vertexCount - 1;
//#ifdef HEIGHTS_KD_TREE
//	//m_vertexCount = VERTEX_COUNT * VERTEX_COUNT;
//#else
//	m_heights.reserve(m_heightMapWidth * m_heightMapHeight);
//	m_gridSquareSize = static_cast<math::Real>(SIZE) / vertexCountMinusOne;
//#endif
//	//const int INDICES_COUNT = 6 * (VERTEX_COUNT - 1) * (VERTEX_COUNT - 1); // The number of indices.
//
//	std::vector<math::Vector3D> positions;
//	positions.reserve(m_vertexCount);
//	std::vector<math::Vector2D> textureCoordinates;
//	textureCoordinates.reserve(m_vertexCount);
//	std::vector<math::Vector3D> normals;
//	normals.reserve(m_vertexCount);
//	std::vector<math::Vector3D> tangents;
//	tangents.reserve(m_vertexCount);
//	for (int z = vertexCountMinusOne; z >= 0; --z)
//	{
//		const math::Real zReal = static_cast<math::Real>(z);
//		for (int x = 0; x < m_vertexCount; ++x)
//		{
//			const math::Real xReal = static_cast<math::Real>(x);
//			math::Real terrainHeight = CalculateHeightAt(x, z, heightMapData);
//			//DEBUG_LOG_RENDERING("counter = ", positions.size(), "; x = ", x, "; z = ", z, "; Position = ", position);
//			positions.emplace_back(xReal / vertexCountMinusOne * SIZE, terrainHeight, zReal / vertexCountMinusOne * SIZE);
//			textureCoordinates.emplace_back(xReal / vertexCountMinusOne, zReal / vertexCountMinusOne);
//			normals.push_back(CalculateNormal(x, z, heightMapData));
//			tangents.emplace_back(REAL_ONE, REAL_ZERO, REAL_ZERO); // TODO: Calculate tangent
//		}
//	}
//	stbi_image_free(heightMapData);
//	
//	std::vector<int> indices;
//	indices.reserve(vertexCountMinusOne * vertexCountMinusOne * 6);
//	for (int gz = 0; gz < vertexCountMinusOne; ++gz)
//	{
//		for (int gx = 0; gx < vertexCountMinusOne; ++gx)
//		{
//			int topLeft = (gz * m_vertexCount) + gx;
//			int topRight = topLeft + 1;
//			int bottomLeft = ((gz + 1) * m_vertexCount) + gx;
//			int bottomRight = bottomLeft + 1;
//			indices.push_back(topLeft);
//			indices.push_back(topRight);
//			indices.push_back(bottomLeft);
//			indices.push_back(bottomLeft);
//			indices.push_back(topRight);
//			indices.push_back(bottomRight);
//		}
//	}
//	
////#ifdef DELOCUST_ENABLED
//	//int i = 0;
//	//for (std::vector<Vertex>::const_iterator vertexItr = vertices.begin(); vertexItr != vertices.end(); ++vertexItr, ++i)
//	//{
//		//CRITICAL_LOG_RENDERING("vertex[", i, "]:\n\tPos:\t", vertexItr->m_pos, "\n\tTex:\t", vertexItr->m_texCoord, "\n\tNormal:\t", vertexItr->m_normal);
//	//}
//	//for (size_t i = 0; i < INDICES_COUNT; ++i)
//	//{
//	//	ERROR_LOG_RENDERING("index[", i, "]: ", indices[i]);
//	//}
////#endif
//
//	SavePositions(positions);
//	AddVertices(&positions[0], &textureCoordinates[0], &normals[0], &tangents[0], nullptr, static_cast<int>(positions.size()), &indices[0], static_cast<int>(indices.size()), false);
//
//	DEBUG_LOG_RENDERING("Terrain mesh has been created.");
//}
//
//Rendering::TerrainMesh::TerrainMesh(int gridX, int gridZ, const math::HeightsGenerator& heightsGenerator, int vertexCount, GLenum mode /* = GL_TRIANGLES */) :
//	Mesh(mode),
//	m_x(gridX),
//	m_z(gridZ),
//	m_vertexCount(vertexCount)
//#ifdef HEIGHTS_KD_TREE
//	, m_positions(m_vertexCount),
//	m_kdTree(nullptr),
//	m_kdTreeSamples(GET_CONFIG_VALUE_RENDERING("kdTreeSamples", 8))
//#elif defined HEIGHTS_HEIGHTMAP
//	, m_heightMapWidth(vertexCount),
//	m_heightMapHeight(vertexCount),
//	m_heights(m_heightMapWidth * m_heightMapHeight),
//	m_gridSquareSize(0)
//#endif
//{
//	DEBUG_LOG_RENDERING("Terrain mesh construction has started.");
//	const int vertexCountMinusOne = m_vertexCount - 1;
//#ifdef HEIGHTS_KD_TREE
//	//m_vertexCount = VERTEX_COUNT * VERTEX_COUNT;
//#else
//	m_gridSquareSize = static_cast<math::Real>(SIZE) / vertexCountMinusOne;
//#endif
//	//const int INDICES_COUNT = 6 * (VERTEX_COUNT - 1) * (VERTEX_COUNT - 1); // The number of indices.
//
//	std::vector<math::Vector3D> positions;
//	positions.reserve(m_vertexCount);
//	std::vector<math::Vector2D> textureCoordinates;
//	textureCoordinates.reserve(m_vertexCount);
//	std::vector<math::Vector3D> normals;
//	normals.reserve(m_vertexCount);
//	std::vector<math::Vector3D> tangents;
//	tangents.reserve(m_vertexCount);
//	for (int z = vertexCountMinusOne; z >= 0; --z)
//	{
//		const math::Real zReal = static_cast<math::Real>(z);
//		for (int x = 0; x < m_vertexCount; ++x)
//		{
//			const math::Real xReal = static_cast<math::Real>(x);
//			math::Real terrainHeight = CalculateHeightAt(x, z, heightsGenerator);
//			//DEBUG_LOG_RENDERING("counter = ", positions.size(), "; x = ", x, "; z = ", z, "; Position = ", position);
//			positions.emplace_back(xReal / vertexCountMinusOne * SIZE, terrainHeight, zReal / vertexCountMinusOne * SIZE);
//			textureCoordinates.emplace_back(xReal / vertexCountMinusOne, zReal / vertexCountMinusOne);
//			normals.push_back(CalculateNormal(x, z, heightsGenerator));
//			tangents.emplace_back(REAL_ONE, REAL_ZERO, REAL_ZERO); // TODO: Calculate tangent
//		}
//	}
//
//	std::vector<int> indices;
//	indices.reserve(vertexCountMinusOne * vertexCountMinusOne * 6);
//	for (int gz = 0; gz < vertexCountMinusOne; ++gz)
//	{
//		for (int gx = 0; gx < vertexCountMinusOne; ++gx)
//		{
//			int topLeft = (gz * m_vertexCount) + gx;
//			int topRight = topLeft + 1;
//			int bottomLeft = ((gz + 1) * m_vertexCount) + gx;
//			int bottomRight = bottomLeft + 1;
//			indices.push_back(topLeft);
//			indices.push_back(topRight);
//			indices.push_back(bottomLeft);
//			indices.push_back(bottomLeft);
//			indices.push_back(topRight);
//			indices.push_back(bottomRight);
//		}
//	}
//
//	//#ifdef DELOCUST_ENABLED
//	//int i = 0;
//	//for (std::vector<Vertex>::const_iterator vertexItr = vertices.begin(); vertexItr != vertices.end(); ++vertexItr, ++i)
//	//{
//	//	CRITICAL_LOG_RENDERING("vertex[", i, "]:\n\tPos:\t", vertexItr->m_pos, "\n\tTex:\t", vertexItr->m_texCoord, "\n\tNormal:\t", vertexItr->m_normal);
//	//}
//	//for (size_t i = 0; i < INDICES_COUNT; ++i)
//	//{
//	//	ERROR_LOG_RENDERING("index[", i, "]: ", indices[i]);
//	//}
//	//#endif
//
//	SavePositions(positions);
//	AddVertices(&positions[0], &textureCoordinates[0], &normals[0], &tangents[0], nullptr, static_cast<int>(positions.size()), &indices[0], static_cast<int>(indices.size()), false);
//	DEBUG_LOG_RENDERING("Terrain mesh has been created.");
//}
//
//Rendering::TerrainMesh::~TerrainMesh(void)
//{
//#ifdef HEIGHTS_KD_TREE
//#elif defined HEIGHTS_HEIGHTMAP
//	//SAFE_DELETE_WHOLE_TABLE(m_heights, m_vertexCount);
//#endif
//}
//
//int Rendering::TerrainMesh::GetHeightMapIndex(int x, int z) const
//{
//	return (m_heightMapWidth * m_heightMapHeight) - ((z + 1) * m_heightMapWidth) + x;
//}
//
//math::Real Rendering::TerrainMesh::CalculateHeightAt(int x, int z, unsigned char* heightMapData)
//{
//	// TODO: Range checking
//	CHECK_CONDITION_RETURN_RENDERING(x >= 0 && x < m_heightMapWidth && z >= 0 && z < m_heightMapHeight, REAL_ZERO,
//		Utility::Logging::ERR, "Cannot determine the height of the terrain on (", x, ", ", z, ") position. It is out of range.");
//	const int heightMapIndex = GetHeightMapIndex(x, z);
//	CHECK_CONDITION_RENDERING(heightMapIndex >= 0 && heightMapIndex < m_heightMapWidth * m_heightMapHeight, Utility::Logging::ERR,
//		"The heightmap index calculation is incorrect. Calculated index (", heightMapIndex, ") is out of range [0; ", m_heightMapWidth * m_heightMapHeight, ")");
//	//DEBUG_LOG_RENDERING("Heightmap index for [", x, ", ", z, "] = ", heightMapIndex);
//	math::Real height = static_cast<math::Real>(heightMapData[heightMapIndex]);
//	height = ((height / MAX_PIXEL_COLOR) - 0.5f) * 2.0f * HEIGHTMAP_MAX_HEIGHT;
//#ifdef HEIGHTS_HEIGHTMAP
//	m_heights[heightMapIndex] = height;
//#endif
//	return height;
//}
//
//math::Real Rendering::TerrainMesh::CalculateHeightAt(int x, int z, const math::HeightsGenerator& heightsGenerator)
//{
//	// TODO: Range checking
//	CHECK_CONDITION_RETURN_RENDERING(x >= 0 && x < m_heightMapWidth && z >= 0 && z < m_heightMapHeight, REAL_ZERO,
//		Utility::Logging::ERR, "Cannot determine the height of the terrain on (", x, ", ", z, ") position. It is out of range.");
//	const int heightMapIndex = GetHeightMapIndex(x, z);
//	CHECK_CONDITION_RENDERING(heightMapIndex >= 0 && heightMapIndex < m_heightMapWidth * m_heightMapHeight, Utility::Logging::ERR,
//		"The heightmap index calculation is incorrect. Calculated index (", heightMapIndex, ") is out of range [0; ", m_heightMapWidth * m_heightMapHeight, ")");
//	//DEBUG_LOG_RENDERING("Heightmap index for [", x, ", ", z, "] = ", heightMapIndex);
//	math::Real height = heightsGenerator.GenerateHeight(static_cast<math::Real>(x), static_cast<math::Real>(z));
//#ifdef HEIGHTS_HEIGHTMAP
//	m_heights[heightMapIndex] = height;
//#endif
//	return height;
//}
//
//math::Vector3D Rendering::TerrainMesh::CalculateNormal(int x, int z, unsigned char* heightMapData)
//{
//	math::Real heightLeft = (x - 1) >= 0 ? CalculateHeightAt(x - 1, z, heightMapData) : REAL_ZERO;
//	math::Real heightRight = (x + 1) < m_heightMapWidth ? CalculateHeightAt(x + 1, z, heightMapData) : REAL_ZERO;
//	math::Real heightDown = (z - 1) >= 0 ? CalculateHeightAt(x, z - 1, heightMapData) : REAL_ZERO;
//	math::Real heightUp = (z + 1) < m_heightMapHeight ? CalculateHeightAt(x, z + 1, heightMapData) : REAL_ZERO;
//	math::Vector3D normal(heightLeft - heightRight, 2.0f, heightDown - heightUp);
//	normal.Normalize();
//	return normal;
//}
//
//math::Vector3D Rendering::TerrainMesh::CalculateNormal(int x, int z, const math::HeightsGenerator& heightsGenerator)
//{
//	math::Real heightLeft = (x - 1) >= 0 ? CalculateHeightAt(x - 1, z, heightsGenerator) : REAL_ZERO;
//	math::Real heightRight = (x + 1) < m_heightMapWidth ? CalculateHeightAt(x + 1, z, heightsGenerator) : REAL_ZERO;
//	math::Real heightDown = (z - 1) >= 0 ? CalculateHeightAt(x, z - 1, heightsGenerator) : REAL_ZERO;
//	math::Real heightUp = (z + 1) < m_heightMapHeight ? CalculateHeightAt(x, z + 1, heightsGenerator) : REAL_ZERO;
//	math::Vector3D normal(heightLeft - heightRight, 2.0f, heightDown - heightUp);
//	normal.Normalize();
//	return normal;
//}
//
///**
// * Performs the k-NN search in the 2-dimensional space in order to find the k closest points to the given point (xz).
// * See also: http://en.wikipedia.org/wiki/Nearest_neighbor_search
// */
//math::Real Rendering::TerrainMesh::GetHeightAt(math::Real x, math::Real z) const
//{
//#ifdef MEASURE_MESH_TIME_ENABLED
//	Utility::timing::Timer timer;
//	timer.Start();
//#endif
//#if defined HEIGHTS_KD_TREE
//	math::Real y = m_kdTree->SearchNearestValue(x, z);
//	//DEBUG_LOG_RENDERING("Height ", y, " returned for position \"", xz, "\"");
//#elif defined HEIGHTS_HEIGHTMAP
//	math::Real terrainX = x - m_x;
//	math::Real terrainZ = z - m_z;
//	int gridX = math::Floor(terrainX / m_gridSquareSize);
//	int gridZ = math::Floor(terrainZ / m_gridSquareSize);
//	if (gridX < 0 || gridX >= m_vertexCount - 1 || gridZ < 0 || gridZ >= m_vertexCount - 1)
//	{
//		return REAL_ZERO;
//	}
//	math::Real xCoord = fmod(terrainX, m_gridSquareSize) / m_gridSquareSize;
//	math::Real zCoord = fmod(terrainZ, m_gridSquareSize) / m_gridSquareSize;
//	math::Real y;
//	if (xCoord <= (1.0f - zCoord))
//	{
//		y = math::Interpolation::BarycentricInterpolation(0.0f, m_heights[GetHeightMapIndex(gridX, gridZ)], 0.0f,
//			1.0f, m_heights[GetHeightMapIndex(gridX + 1, gridZ)], 0.0f,
//			0.0f, m_heights[GetHeightMapIndex(gridX, gridZ + 1)], 1.0f,
//			xCoord, zCoord);
//		//for (int i = 2; i > -3; --i)
//		//{
//			//CRITICAL_LOG_RENDERING(i, ") Heights: [", gridX - 2, "][", gridZ + i, "] = ", ((gridX - 2 < 0) || (gridZ + i < 0)) ? -999.0f : m_heights[gridX - 2][gridZ + i],
//			//	"; [", gridX - 1, "][", gridZ + i, "] = ", ((gridX - 1 < 0) || (gridZ + i < 0)) ? -999.0f : m_heights[gridX - 1][gridZ + i],
//			//	"; [", gridX, "][", gridZ + i, "] = ", ((gridX < 0) || (gridZ + i < 0)) ? -999.0f : m_heights[gridX][gridZ + i],
//			//	"; [", gridX + 1, "][", gridZ + i, "] = ", ((gridX + 1 < 0) || (gridZ + i < 0)) ? -999.0f : m_heights[gridX + 1][gridZ + i],
//			//	"; [", gridX + 2, "][", gridZ + i, "] = ", ((gridX + 2 < 0) || (gridZ + i < 0)) ? -999.0f : m_heights[gridX + 2][gridZ + i]);
//		//}
//		//EMERGENCY_LOG_RENDERING("Grid = [", gridX, ", ", gridZ, "]. Coords = [", xCoord, ", ", zCoord, "]. Height[", GetHeightMapIndex(gridX, gridZ), "] = ",
//		//	m_heights[GetHeightMapIndex(gridX, gridZ)], ", Height[", GetHeightMapIndex(gridX + 1, gridZ), "] = ",
//		//	m_heights[GetHeightMapIndex(gridX + 1, gridZ)], ", Height[", GetHeightMapIndex(gridX, gridZ + 1), "] = ",
//		//	m_heights[GetHeightMapIndex(gridX, gridZ + 1)], ". Final height = ", y);
//	}
//	else
//	{
//		y = math::Interpolation::BarycentricInterpolation(1.0f, m_heights[GetHeightMapIndex(gridX + 1, gridZ)], 0.0f,
//			1.0f, m_heights[GetHeightMapIndex(gridX + 1, gridZ + 1)], 1.0f,
//			0.0f, m_heights[GetHeightMapIndex(gridX, gridZ + 1)], 1.0f,
//			xCoord, zCoord);
//		//ERROR_LOG_RENDERING("Grid = [", gridX, ", ", gridZ, "]. Coords = [", xCoord, ", ", zCoord, "]. Height[", GetHeightMapIndex(gridX + 1, gridZ), "] = ",
//		//	m_heights[GetHeightMapIndex(gridX + 1, gridZ)], ", Height[", GetHeightMapIndex(gridX + 1, gridZ + 1), "] = ",
//		//	m_heights[GetHeightMapIndex(gridX + 1, gridZ + 1)], ", Height[", GetHeightMapIndex(gridX, gridZ + 1), "] = ",
//		//	m_heights[GetHeightMapIndex(gridX, gridZ + 1)], ". Final height = ", y);
//	}
//	//y -= 0.35f;
//#endif
//
//#ifdef MEASURE_MESH_TIME_ENABLED
//	timer.Stop();
//	DEBUG_LOG_RENDERING("Camera's height calculation took ", timer.GetTimeSpan(Utility::timing::MICROSECOND).ToString());
//#endif
//
//	return y;
//}
//
///**
// * TODO: See this page for a possible ways to optimize this function
// * (http://stackoverflow.com/questions/1041620/whats-the-most-efficient-way-to-erase-duplicates-and-sort-a-vector)
// */
//void Rendering::TerrainMesh::SavePositions(const std::vector<math::Vector3D>& positions)
//{
//#ifdef HEIGHTS_KD_TREE
//#ifdef MEASURE_MESH_TIME_ENABLED
//	clock_t begin = clock(); // TODO: Replace with Utility::Timer. Use QueryPerformanceCounter() instead of clock() function when measuring time. It is more accurate.
//#endif
//	DEBUG_LOG_RENDERING("Terrain consists of ", positions.size(), " positions");
//	std::unordered_set<math::Vector3D> verticesSet;
//	for (unsigned int i = 0; i < positions.size(); ++i)
//	{
//		verticesSet.insert(positions[i]);
//	}
//	m_positions.assign(verticesSet.begin(), verticesSet.end()); // store only unique positions.
//#ifdef MEASURE_MESH_TIME_ENABLED
//	clock_t end = clock(); // TODO: Replace with Utility::Timer. Use QueryPerformanceCounter() instead of clock() function when measuring time. It is more accurate.
//	DEBUG_LOG_RENDERING("Removing duplicates from the vector of positions took ", 1000.0 * static_cast<double>(end - begin) / (CLOCKS_PER_SEC), " [ms]", );
//#endif
//
//	//ISort::GetSortingObject(ISort::QUICK_SORT)->Sort(&m_positions[0], uniquePositions.size(), COMPONENT_Y);
//	//INFO_LOG_RENDERING("The minimum value is ", m_positions[0]);
//	//INFO_LOG_RENDERING("The maximum value is ", m_positions[uniquePositions.size() - 1]);
//
//	m_vertexCount = m_positions.size();
//	INFO_LOG_RENDERING("Terrain consists of ", m_vertexCount, " unique positions");
//#endif
//
//	/**
//	 * TODO: Think of a nice way to store positions so that access time in GetHeightAt(x, z) is optimized. Maybe a Binary Tree? Maybe a k-d tree?
//	 * Maybe find the minimum and maximum value for "Y" component of all positions and then use bucket sort (http://en.wikipedia.org/wiki/Bucket_sort)
//	 * based on "Y" values?
//	 */
//}
//
//void Rendering::TerrainMesh::TransformPositions(const math::Matrix4D& transformationMatrix)
//{
//#ifdef HEIGHTS_KD_TREE
//	DEBUG_LOG_RENDERING("Transformation matrix = \n", transformationMatrix);
//	CHECK_CONDITION_EXIT_RENDERING(!m_positions.empty(), Utility::EMERGENCY, "Cannot transform the positions. The positions array is empty.");
//	for (int i = 0; i < m_vertexCount; ++i)
//	{
//		//std::string oldPos = positions[i];
//		m_positions[i] = transformationMatrix * m_positions[i]; // FIXME: Check matrix multiplication
//		//if ((i % 1000 == 0) || (i == m_vertexCount - 1))
//		//{
//		//	DELOCUST_LOG_RENDERING(i, ") Old position = ", oldPos, ". New Position = ", positions[i]);
//		//}
//	}
//	m_kdTree = std::make_unique<math::KDTree>(m_positions, m_vertexCount, m_kdTreeSamples);
//#endif
//}
/* ==================== TerrainMesh class implementation end ==================== */
