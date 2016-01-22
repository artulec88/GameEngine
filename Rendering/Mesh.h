#pragma once

#include "Rendering.h"
#include "Vertex.h"

#include "Math\Matrix.h"
#include "Math\Vector.h"
#include "Utility\ReferenceCounter.h"

#include <string>
#include <map>
#include <vector>

//#define HEIGHTMAP_BRUTE_FORCE
//#define HEIGHTMAP_SORT_TABLE
#define HEIGHTMAP_KD_TREE

#ifdef HEIGHTMAP_KD_TREE
#include "Math\KDTree.h"
#endif

#define MEASURE_TIME_ENABLED

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
	MeshData();
	MeshData(GLsizei indexSize);
	virtual ~MeshData(void);
private:
	MeshData(const MeshData& meshData) {} // don't implement
	void operator=(const MeshData& meshData) {} // don't implement
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	/// <summary>
	/// Returns the handle for the vertex buffer object.
	/// </summary>
	/// <returns> A handle to the vertex buffer object. </returns>
	GLuint GetVBO() const { return m_vbo; }

	/// <summary>
	/// Returns the handle for the index buffer object.
	/// </summary>
	/// <returns> A handle to the index buffer object. </returns>
	GLuint GetIBO() const { return m_ibo; }

	/// <summary>
	/// Returns the size of the mesh.
	/// </summary>
	/// <returns> The size of the mesh. </returns>
	GLsizei GetSize() const { return m_size; }
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
private:
	/// <summary>
	/// Vertex buffer object. A handle to vertices making up the whole mesh.
	/// </summary>
	GLuint m_vbo;
	
	/// <summary>
	/// Index buffer object.
	/// </summary>
	GLuint m_ibo;
	
	/// <summary>
	/// The size. It represents how much data there is in the vertex buffer object.
	/// </summary>
	GLsizei m_size;
/* ==================== Non-static member variables end ==================== */
}; /* end class MeshData */

class Mesh// : public Utility::ReferenceCounter
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
	RENDERING_API Mesh(Vertex* vertices, int verticesCount, int* indices, int indicesCount, bool calcNormalsEnabled = true, GLenum mode = GL_TRIANGLES);
	RENDERING_API virtual ~Mesh(void);
protected:
	Mesh(GLenum mode = GL_TRIANGLES);
private: // disable copy constructor and assignment operator
	Mesh(Mesh& mesh) {}
	void operator=(Mesh& mesh) {}
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	void Initialize();
	virtual void Draw() const;
	void BindBuffers() const;
	void UnbindBuffers() const;
protected:
	void AddVertices(Vertex* vertices, size_t verticesCount, const int* indices, size_t indicesCount, bool calcNormalsEnabled = true);
	void CalcNormals(Vertex* vertices, size_t verticesCount, const int* indices, size_t indicesCount) const;
	void CalcTangents(Vertex* vertices, size_t verticesCount) const;
	//void CalcIndices(Vertex* vertices, size_t verticesCount, std::vector<Vertex>& indexedVertices, std::vector<int>& indices) const;
	//bool GetSimilarVertexIndex(const Vertex& vertex, const std::vector<Vertex>& indexedVertices, int& index) const;
	virtual void SavePositions(const std::vector<Math::Vector3D>& positions) { /* does nothing*/ };
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
	RENDERING_API BillboardMesh(const Math::Vector3D& worldPosition);
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
	Math::Vector3D m_worldPosition;
/* ==================== Non-static member variables end ==================== */
}; /* end class BillboardMesh */

/// <summary>
/// The mesh that is going to be rendered on the screen.
/// </summary>
class GuiMesh : public Mesh
{
/* ==================== Constructors and destructors begin ==================== */
public:
	GuiMesh(const Math::Vector2D* positions, unsigned int positionsCount);
	virtual ~GuiMesh(void);
private: // disable copy constructor and assignment operator
	GuiMesh(GuiMesh& mesh);
	void operator=(GuiMesh& mesh);
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	virtual void Draw() const;
/* ==================== Non-static member functions end ==================== */


/* ==================== Non-static member variables begin ==================== */
private:
	unsigned int m_positionsCount;
/* ==================== Non-static member variables end ==================== */
}; /* end class GuiMesh */

class TerrainMesh : public Mesh
{
/* ==================== Static variables begin ==================== */
/* ==================== Static variables end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	RENDERING_API TerrainMesh(const std::string& fileName, GLenum mode = GL_TRIANGLES);
	virtual ~TerrainMesh(void);
private: // disable copy constructor and assignment operator
	//TerrainMesh(TerrainMesh& terrainMesh) : Mesh(terrainMesh) {}
	//void operator=(TerrainMesh& terrainMesh) : Mesh(terrainMesh) {}
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	RENDERING_API Math::Real GetHeightAt(const Math::Vector2D& xz, bool headPositionHeightAdjustmentEnabled = false);
	RENDERING_API void TransformPositions(const Math::Matrix4D& transformationMatrix);
protected:
	virtual void SavePositions(const std::vector<Math::Vector3D>& positions);
/* ==================== Non-static member functions end ==================== */


/* ==================== Non-static member variables begin ==================== */
private:
	const Math::Real m_headPositionHeightAdjustment;
	Math::Vector3D* m_positions;
	size_t m_positionsCount;
	Math::Real m_lastX, m_lastY, m_lastZ;
#ifdef HEIGHTMAP_SORT_TABLE
	int m_lastClosestPositionIndex;
#elif defined HEIGHTMAP_KD_TREE
	const int m_kdTreeSamples;
	Math::KDTree* m_kdTree;
#endif
/* ==================== Non-static member variables end ==================== */
}; /* end class TerrainMesh */

} /* end namespace Rendering */