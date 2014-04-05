#pragma once

#include "Rendering.h"

#include "Utility\ReferenceCounter.h"

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

} /* end namespace Rendering */