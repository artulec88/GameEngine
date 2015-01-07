#pragma once

#include "Rendering.h"
#include "MeshData.h"
#include "Vertex.h"

//#include "Utility\ReferenceCounter.h"

#include <string>
#include <map>
#include <vector>

namespace Rendering
{

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
	RENDERING_API Mesh(Vertex* vertices, int verticesCount, bool calcNormalsEnabled = true, GLenum mode = GL_TRIANGLES);
	RENDERING_API virtual ~Mesh(void);
private: // disable copy constructor and assignment operator
	Mesh(Mesh& mesh) {}
	void operator=(Mesh& mesh) {}
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	RENDERING_API void Draw() const;
protected:
	RENDERING_API void AddVertices(Vertex* vertices, int verticesCount, const int* indices, int indicesCount, bool calcNormalsEnabled = true);
	RENDERING_API void CalcNormals(Vertex* vertices, int verticesCount, const int* indices, int indicesCount) const;
	RENDERING_API void CalcTangents(Vertex* vertices, int verticesCount) const;
	void CalcIndices(Vertex* vertices, int verticesCount, std::vector<Vertex>& indexedVertices, std::vector<int>& indices) const;
	bool GetSimilarVertexIndex(const Vertex& vertex, const std::vector<Vertex>& indexedVertices, int& index) const;
/* ==================== Non-static member functions end ==================== */


/* ==================== Non-static member variables begin ==================== */
private:
	std::string fileName;
	GLenum mode;
	MeshData* meshData;
/* ==================== Non-static member variables end ==================== */

}; /* end class Mesh */

} /* end namespace Rendering */