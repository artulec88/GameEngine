#pragma once

#include "Rendering.h"
#include "MeshData.h"
#include "Vertex.h"

//#include "Utility\ReferenceCounter.h"

#include <string>
#include <map>

namespace Rendering
{

class RENDERING_API Mesh// : public Utility::ReferenceCounter
{
/* ==================== Static variables begin ==================== */
private:
	static std::map<std::string, MeshData*> meshResourceMap;
/* ==================== Static variables end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	Mesh(const std::string& fileName);
	Mesh(Vertex* vertices, int verticesCount, int* indices, int indicesCount, bool calcNormalsEnabled = true);
	virtual ~Mesh(void);
private: // disable copy constructor and assignment operator
	Mesh(Mesh& mesh) {}
	void operator=(Mesh& mesh) {}
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	void Draw() const;
protected:
	void AddVertices(Vertex* vertices, int verticesCount, const int* indices, int indicesCount, bool calcNormalsEnabled = true);
	void CalcNormals(Vertex* vertices, int verticesCount, const int* indices, int indicesCount) const;
	void CalcTangents(Vertex* vertices, int verticesCount) const;
/* ==================== Non-static member functions end ==================== */


/* ==================== Non-static member variables begin ==================== */
private:
	std::string fileName;
	MeshData* meshData;
/* ==================== Non-static member variables end ==================== */

}; /* end class Mesh */

} /* end namespace Rendering */