#ifndef __RENDERING_MESH_H__
#define __RENDERING_MESH_H__

#include "Rendering.h"

#include "Math\Matrix.h"
#include "Math\Vector.h"
#include "Utility\ReferenceCounter.h"
#include "Utility\ILogger.h"

#include <string>
#include <map>
#include <vector>

//#define HEIGHTMAP_BRUTE_FORCE
//#define HEIGHTMAP_KD_TREE
#define HEIGHTMAP_HEIGHTS

#ifdef HEIGHTMAP_KD_TREE
#include "Math\KDTree.h"
#endif

//#define TEXTURE_ATLAS_OFFSET_CALCULATION
//#define MEASURE_MESH_TIME_ENABLED

#define MESH_DATA_BUFFERS_COUNT 7

// Type cast conversion to make Math::Vector3D possible to use in std::unordered_set object.
namespace std
{
	template <>
	struct hash<Math::Vector3D>
	{
		std::size_t operator()(const Math::Vector3D& vector) const
		{
			return vector.to_size_t();
		}
	};
}

namespace Rendering
{
	class MeshData : public Utility::ReferenceCounter
	{
		/* ==================== Static variables and functions begin ==================== */
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		/// <summary>
		/// Simple mesh data constructor without index buffer object.
		/// </summary>
		//MeshData();
		MeshData(GLsizei indexSize);
		virtual ~MeshData(void);
	private:
		MeshData(const MeshData& meshData); // don't implement
		void operator=(const MeshData& meshData); // don't implement
	/* ==================== Constructors and destructors end ==================== */

	/* ==================== Non-static member functions begin ==================== */
	public:
		GLuint GetVAO() const { return m_vao; }

		void CreateVAO();

		inline void Bind() const
		{
			Rendering::CheckErrorCode(__FUNCTION__, "Started mesh data binding");
			//WARNING_LOG("Binding mesh data %s", ToString().c_str());
			CHECK_CONDITION_EXIT_ALWAYS(m_vao != 0, Utility::Critical, "Trying to bind the VAO with id=0");
			glBindVertexArray(m_vao);
			Rendering::CheckErrorCode(__FUNCTION__, "Finished mesh data binding");
		}

		inline void Unbind() const
		{
			Rendering::CheckErrorCode(__FUNCTION__, "Started mesh data unbinding");
			//WARNING_LOG("Unbinding mesh data %s", ToString().c_str());
			//int index = 0;
			//for (std::vector<GLuint>::const_iterator vboItr = m_vbos.begin(); vboItr != m_vbos.end(); ++vboItr, ++index)
			//{
			//	glDisableVertexAttribArray(index);
			//}
			glBindVertexArray(0);
			Rendering::CheckErrorCode(__FUNCTION__, "Finished mesh data unbinding");
		}

		bool HasVBO(unsigned int index) const
		{
			CHECK_CONDITION_EXIT_ALWAYS(index >= 0 && index < MESH_DATA_BUFFERS_COUNT, Utility::Critical, "Cannot access buffer at index %d. Mesh data = \"%s\"", index, ToString().c_str());
			return m_buffers[index] != 0;
		}

		/// <summary>
		/// Returns the handle for the vertex buffer object under specified index.
		/// </summary>
		/// <param name="index"> The index of the VBO we want to get ID of </param>
		/// <returns> A handle to the vertex buffer object. </returns>
		GLuint GetVBO(unsigned int index) const
		{
			CHECK_CONDITION_EXIT_ALWAYS(index >= 0 && index < MESH_DATA_BUFFERS_COUNT, Utility::Critical, "Cannot access buffer at index %d. Mesh data = \"%s\"", index, ToString().c_str());
			CHECK_CONDITION_EXIT_ALWAYS(m_buffers[index] != 0, Utility::Critical, "The buffer under index %d is 0. Mesh data = \"%s\"", index, ToString().c_str());
			return m_buffers[index];
		}

		//size_t GetVBOsCount() const { return m_vbos.size(); }

		/// <summary>
		/// Creates the collection of VBOs and stores it in the vector of available VBOs.
		/// </summary>
		void CreateVBO(int index);

		/// <summary>
		/// Creates IBO.
		/// </summary>
		//void CreateIBO();

		/// <summary>
		/// Returns the handle for the index buffer object.
		/// </summary>
		/// <returns> A handle to the index buffer object. </returns>
		//GLuint GetIBO() const { return m_ibo; }

		/// <summary>
		/// Returns the size of the mesh.
		/// </summary>
		/// <returns> The size of the mesh. </returns>
		GLsizei GetSize() const { return m_size; }

		std::string ToString() const;
	/* ==================== Non-static member functions end ==================== */

	/* ==================== Non-static member variables begin ==================== */
	private:
		/// <summary>
		/// Vertex array object.
		/// </summary>
		GLuint m_vao;

		/// <summary>
		/// Vertex buffer objects. A handle to data representing the whole mesh (positions, texture coordinates, normals, etc.).
		/// </summary>
		GLuint m_buffers[MESH_DATA_BUFFERS_COUNT];
	
		/// <summary>
		/// Index buffer object.
		/// </summary>
		//GLuint m_ibo;
	
		/// <summary>
		/// The size. It represents how much data there is in the vertex buffer object.
		/// </summary>
		GLsizei m_size;
	/* ==================== Non-static member variables end ==================== */
	}; /* end class MeshData */

class Mesh
{
public:
	class MeshComparator
	{
	public:
		bool operator() (const Mesh& lhs, const Mesh& rhs) const
		{
			return lhs.Compare(rhs);
		}
	}; // end class MeshComparator

/* ==================== Static variables begin ==================== */
private:
	static std::map<std::string, MeshData*> meshResourceMap;
/* ==================== Static variables end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	RENDERING_API Mesh(const std::string& fileName, GLenum mode = GL_TRIANGLES);
	RENDERING_API Mesh(Math::Vector3D* positions, int verticesCount, int* indices, int indicesCount, bool calcNormalsEnabled = true, GLenum mode = GL_TRIANGLES);
	RENDERING_API Mesh(Math::Vector3D* positions, Math::Vector2D* textureCoordinates, int verticesCount, int* indices, int indicesCount, bool calcNormalsEnabled = true, GLenum mode = GL_TRIANGLES);
	RENDERING_API Mesh(Math::Vector3D* positions, Math::Vector2D* textureCoordinates, Math::Vector3D* normals, int verticesCount, int* indices, int indicesCount, bool calcNormalsEnabled = true, GLenum mode = GL_TRIANGLES);
	RENDERING_API Mesh(Math::Vector3D* positions, Math::Vector2D* textureCoordinates, Math::Vector3D* normals, Math::Vector3D* tangents, int verticesCount, int* indices, int indicesCount, bool calcNormalsEnabled = true, GLenum mode = GL_TRIANGLES);
	RENDERING_API Mesh(Math::Vector3D* positions, Math::Vector2D* textureCoordinates, Math::Vector3D* normals, Math::Vector3D* tangents, Math::Vector3D* bitangents, int verticesCount, int* indices, int indicesCount, bool calcNormalsEnabled = true, GLenum mode = GL_TRIANGLES);
	RENDERING_API virtual ~Mesh(void);
protected:
	Mesh(GLenum mode = GL_TRIANGLES);
private: // disable copy constructor and assignment operator
	Mesh(Mesh& mesh) {}
	void operator=(Mesh& mesh) {}
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	RENDERING_API void Initialize(); // TODO: Try to remove it, the initialization should be handled in the constructor
	virtual void Draw() const;
protected:
	void AddVertices(Math::Vector2D* positions, Math::Vector2D* textureCoordinates, int verticesCount);
	void AddVertices(Math::Vector3D* positions, Math::Vector2D* textureCoordinates, Math::Vector3D* normals, Math::Vector3D* tangents, Math::Vector3D* bitangents, int verticesCount, int* indices, int indicesCount, bool calcNormalsEnabled);
	void CalcNormals(Math::Vector3D*& normals, Math::Vector3D* positions, size_t verticesCount, const int* indices, size_t indicesCount) const;
	void CalcTangents(Math::Vector3D*& tangents, Math::Vector3D* positions, Math::Vector2D* textureCoordinates, size_t verticesCount) const;
	//void CalcIndices(Vertex* vertices, size_t verticesCount, std::vector<Vertex>& indexedVertices, std::vector<int>& indices) const;
	//bool GetSimilarVertexIndex(const Vertex& vertex, const std::vector<Vertex>& indexedVertices, int& index) const;
	virtual void SavePositions(const std::vector<Math::Vector3D>& positions) { /* does nothing*/ }; // TODO: Remove this function
	bool Compare(const Mesh& mesh) const;
/* ==================== Non-static member functions end ==================== */


/* ==================== Non-static member variables begin ==================== */
protected:
	std::string m_fileName;
	GLenum m_mode;
	MeshData* m_meshData;
/* ==================== Non-static member variables end ==================== */

}; /* end class Mesh */

/// <summary>
/// The billboard mesh that is going to be rendered on the screen.
/// </summary>
class BillboardMesh : public Mesh
{
/* ==================== Static variables begin ==================== */
/* ==================== Static variables end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	RENDERING_API BillboardMesh(Math::Real* modelMatricesValues, unsigned int billboardsCount, unsigned int billboardDataLength);
	RENDERING_API virtual ~BillboardMesh(void);
private: // disable copy constructor and assignment operator
	BillboardMesh(BillboardMesh& billboardMesh);
	void operator=(BillboardMesh& billboardMesh);
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	virtual void Draw() const;
/* ==================== Non-static member functions end ==================== */


/* ==================== Non-static member variables begin ==================== */
private:
	unsigned int m_billboardsCount;
/* ==================== Non-static member variables end ==================== */
}; /* end class BillboardMesh */

/// <summary>
/// The simple GUI mesh that is going to be rendered on the screen.
/// </summary>
class GuiMesh : public Mesh
{
/* ==================== Constructors and destructors begin ==================== */
public:
	GuiMesh(Math::Vector2D* positions, unsigned int positionsCount);
	virtual ~GuiMesh(void);
private: // disable copy constructor and assignment operator
	GuiMesh(GuiMesh& mesh);
	void operator=(GuiMesh& mesh);
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	//virtual void Draw() const;
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
private:
	unsigned int m_positionsCount;
/* ==================== Non-static member variables end ==================== */
}; /* end class GuiMesh */

/// <summary>
/// The instance mesh that is used by particles.
/// </summary>
class InstanceMesh : public Mesh
{
/* ==================== Non-static member functions begin ==================== */
private:
/* ==================== Non-static member functions end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	InstanceMesh(Math::Vector2D* positions, unsigned int positionsCount, unsigned int maxParticlesCount, unsigned int instanceDataLength);
	virtual ~InstanceMesh(void);
private: // disable copy constructor and assignment operator
	InstanceMesh(InstanceMesh& mesh);
	void operator=(InstanceMesh& mesh);
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	unsigned int GetInstanceDataLength() const { return m_instanceDataLength; }
	void Draw(Math::Real* data, unsigned int dataSize, unsigned int particlesCount) const;
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
private:
	unsigned int m_positionsCount;
	unsigned int m_maxParticlesCount;
	unsigned int m_instanceDataLength;
/* ==================== Non-static member variables end ==================== */
}; /* end class InstanceMesh */

class TerrainMesh : public Mesh
{
/* ==================== Static variables begin ==================== */
private:
	/// <summary> The size of the single squared-size terrain tile. </summary>
	static const int SIZE;
	/// <summary> The maximal height of the terrain.
	/// The negative value represents the minimal height of the terrain. In other words, the terrain height always lies in range [-MAX_HEIGHT; MAX_HEIGHT].
	/// </summary>
	static const float MAX_HEIGHT;
	/// <summary> The maximum value for color of the single pixel in the height map. </summary>
	static const float MAX_PIXEL_COLOR;
/* ==================== Static variables end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	RENDERING_API TerrainMesh(const std::string& fileName, GLenum mode = GL_TRIANGLES);
	RENDERING_API TerrainMesh(Math::Real gridX, Math::Real gridZ, const std::string& heightMapFileName, GLenum mode = GL_TRIANGLES);
	RENDERING_API virtual ~TerrainMesh(void);
private: // disable copy constructor and assignment operator
	TerrainMesh(TerrainMesh& terrainMesh);
	void operator=(TerrainMesh& terrainMesh);
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	RENDERING_API Math::Real GetHeightAt(const Math::Vector2D& xz, bool headPositionHeightAdjustmentEnabled = false) const;
	RENDERING_API Math::Real GetHeightAt(Math::Real x, Math::Real y, bool headPositionHeightAdjustmentEnabled = false) const;
	RENDERING_API void TransformPositions(const Math::Matrix4D& transformationMatrix);
protected:
	virtual void SavePositions(const std::vector<Math::Vector3D>& positions);
private:
	int GetHeightMapIndex(int x, int z) const;
	Math::Real CalculateHeightAt(int x, int z, unsigned char* heightMapData) const;
	Math::Vector3D CalculateNormal(int x, int z, unsigned char* heightMapData) const;
/* ==================== Non-static member functions end ==================== */


/* ==================== Non-static member variables begin ==================== */
private:
	/// <summary> The position of the single terrain tile along the X axis. </summary>
	Math::Real m_x;
	/// <summary> The position of the single terrain tile along the Z axis. </summary>
	Math::Real m_z;

	const Math::Real m_headPositionHeightAdjustment;
	size_t m_vertexCount;
#ifdef HEIGHTMAP_KD_TREE
	Math::Vector3D* m_positions;
	const int m_kdTreeSamples;
	Math::KDTree* m_kdTree;
#elif defined HEIGHTMAP_HEIGHTS
	int m_heightMapWidth, m_heightMapHeight;
	Math::Real* m_heights;
	Math::Real m_gridSquareSize;
#endif
/* ==================== Non-static member variables end ==================== */
}; /* end class TerrainMesh */

/// <summary>
/// The text mesh that is going to be rendered on the screen.
/// </summary>
class TextMesh : public Mesh
{
/* ==================== Static variables begin ==================== */
/* ==================== Static variables end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	RENDERING_API TextMesh(Math::Vector2D* screenPositions, Math::Vector2D* textureCoordinates, int verticesCount, GLenum mode = GL_TRIANGLES);
	RENDERING_API virtual ~TextMesh(void);
private: // disable copy constructor and assignment operator
	TextMesh(TextMesh& textMesh);
	void operator=(TextMesh& textMesh);
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	virtual void Draw() const;
	void ReplaceData(Math::Vector2D* screenPositions, Math::Vector2D* textureCoordinates, int verticesCount);
/* ==================== Non-static member functions end ==================== */


/* ==================== Non-static member variables begin ==================== */
private:
/* ==================== Non-static member variables end ==================== */
}; /* end class TextMesh */

} /* end namespace Rendering */

#endif // __RENDERING_MESH_H__