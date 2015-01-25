#pragma once

#include "Rendering.h"
#include "Vertex.h"

#include "Math\Matrix.h"
#include "Utility\ReferenceCounter.h"

#include <string>
#include <map>
#include <vector>

//#define HEIGHTMAP_BRUTE_FORCE
#define HEIGHTMAP_SORT_TABLE
//#define HEIGHTMAP_KNN

namespace Rendering
{

class MeshData : public Utility::ReferenceCounter
{
/* ==================== Non-static member variables begin ==================== */
private:
	/**
	 * @brief Vertex buffer object
	 */
	unsigned int vbo;
	/**
	 * @brief Index buffer objects
	 *
	 * An array of integers.
	 */
	unsigned int ibo; // index buffer object
	int size; // the size
/* ==================== Non-static member variables end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	MeshData(int indexSize);
	virtual ~MeshData(void);
private:
	MeshData(const MeshData& meshData) {} // don't implement
	void operator=(const MeshData& meshData) {} // don't implement
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	unsigned int GetVBO() const { return vbo; }
	unsigned int GetIBO() const { return ibo; }
	int GetSize() const { return size; }
/* ==================== Non-static member functions end ==================== */
}; /* end class MeshData */

class Mesh// : public Utility::ReferenceCounter
{
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
protected:
	void AddVertices(Vertex* vertices, int verticesCount, const int* indices, int indicesCount, bool calcNormalsEnabled = true);
	void CalcNormals(Vertex* vertices, int verticesCount, const int* indices, int indicesCount) const;
	void CalcTangents(Vertex* vertices, int verticesCount) const;
	//void CalcIndices(Vertex* vertices, int verticesCount, std::vector<Vertex>& indexedVertices, std::vector<int>& indices) const;
	//bool GetSimilarVertexIndex(const Vertex& vertex, const std::vector<Vertex>& indexedVertices, int& index) const;
	virtual void SavePositions(const std::vector<Vertex>& vertices) { /* does nothing*/ };
/* ==================== Non-static member functions end ==================== */


/* ==================== Non-static member variables begin ==================== */
private:
	std::string fileName;
	GLenum mode;
	MeshData* meshData;
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
	virtual void SavePositions(const std::vector<Vertex>& vertices);
/* ==================== Non-static member functions end ==================== */


/* ==================== Non-static member variables begin ==================== */
private:
	Math::Vector3D* positions;
	int positionsCount;
	Math::Real lastX, lastY, lastZ;
#ifdef HEIGHTMAP_SORT_TABLE
	int lastClosestPositionIndex;
#endif
/* ==================== Non-static member variables end ==================== */

}; /* end class TerrainMesh */

} /* end namespace Rendering */