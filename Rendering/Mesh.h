#pragma once

#include "Rendering.h"
#include "Vertex.h"

#include "Math\Matrix.h"
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
	MeshData(size_t indexSize);
	virtual ~MeshData(void);
private:
	MeshData(const MeshData& meshData) {} // don't implement
	void operator=(const MeshData& meshData) {} // don't implement
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	unsigned int GetVBO() const { return m_vbo; }
	unsigned int GetIBO() const { return m_ibo; }
	size_t GetSize() const { return m_size; }
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
private:
	/// <summary> Vertex buffer object. </summary>
	unsigned int m_vbo;
	
	/// <summary> Index buffer object. </summary>
	unsigned int m_ibo;
	
	/// <summary> The size. </summary>
	size_t m_size;
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
	//RENDERING_API Mesh(Vertex* vertices, int verticesCount, bool calcNormalsEnabled = true, GLenum mode = GL_TRIANGLES);
	RENDERING_API virtual ~Mesh(void);
private: // disable copy constructor and assignment operator
	Mesh(Mesh& mesh) {}
	void operator=(Mesh& mesh) {}
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	RENDERING_API void Initialize();
	RENDERING_API void Draw() const;
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
private:
	std::string m_fileName;
	GLenum m_mode;
	MeshData* m_meshData;
/* ==================== Non-static member variables end ==================== */

}; /* end class Mesh */

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
	RENDERING_API Math::Real GetHeightAt(const Math::Vector2D& xz);
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