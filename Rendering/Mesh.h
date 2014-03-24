#pragma once

#include "Rendering.h"
#include "Vertex.h"
#include <string>

namespace Rendering
{

class RENDERING_API Mesh
{
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

/* ==================== Constructors and destructors begin ==================== */
public:
	Mesh(Vertex* vertices, int vertSize, unsigned short* indices, int indexSize, bool calcNormalsEnabled = true);
	Mesh(const std::string& fileName);
	virtual ~Mesh(void);
private: // disable copy constructor and assignment operator
	Mesh(Mesh& mesh);
	void operator=(Mesh& mesh);
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	void Draw() const;
protected:
	// TODO: Remove this function in the future. Create a nice design for loading meshes from external files
	/**
	 * This is a temporary function. It will be removed once the model loader is finished
	 */
	void LoadFromFile(const std::string& fileName);
	void AddVertices(Vertex* vertices, int vertSize, unsigned short* indices, int indexSize, bool calcNormalsEnabled = true);
	void CalcNormals(Vertex* vertices, int vertSize, unsigned short* indices, int indexSize);
/* ==================== Non-static member functions end ==================== */

}; /* end class Mesh */

} /* end namespace Rendering */