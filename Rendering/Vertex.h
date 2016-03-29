#ifndef __RENDERING_VERTEX_H__
#define __RENDERING_VERTEX_H__

#include "Rendering.h"

/* MATH begin */
#include "Math\Vector.h"
/* MATH end */

#ifdef _DEBUG
#include <string>
#endif

namespace Rendering
{

	struct Vertex2D
	{
	/* ==================== Constructors and destructors begin ==================== */
		Vertex2D(const Math::Vector2D& position,
			const Math::Vector2D& texCoord = Math::Vector2D(REAL_ZERO, REAL_ZERO));
	/* ==================== Constructors and destructors end ==================== */

	/* ==================== Non-static member functions begin ==================== */
		bool operator==(const Vertex2D& vertex2D) const;
		bool operator!=(const Vertex2D& vertex2D) const;

#ifdef _DEBUG
		std::string ToString() const;
#endif
	/* ==================== Non-static member functions end ==================== */

	/* ==================== Non-static member variables begin ==================== */
		Math::Vector2D m_pos;
		Math::Vector2D m_texCoord;
		// TODO: Memory layout- add additional padding variable or
		// use a table of Math::Vector3D objects with size 2 where [0], [1] store position and texture coordinates respectively.
	/* ==================== Non-static member variables end ==================== */
	}; /* end struct Vertex2D */

struct Vertex
{
	/* ==================== Constructors and destructors begin ==================== */
	Vertex(const Math::Vector3D& position,
		const Math::Vector2D& texCoord = Math::Vector2D(REAL_ZERO, REAL_ZERO),
		const Math::Vector3D& normal = Math::Vector3D(REAL_ZERO, REAL_ZERO, REAL_ZERO),
		const Math::Vector3D& tangent = Math::Vector3D(REAL_ZERO, REAL_ZERO, REAL_ZERO)/*,
		const Math::Vector3D& bitangent = Math::Vector3D(REAL_ZERO, REAL_ZERO, REAL_ZERO)*/);
	/* ==================== Constructors and destructors end ==================== */

	/* ==================== Non-static member functions begin ==================== */
	bool operator==(const Vertex& vertex) const;
	bool operator!=(const Vertex& vertex) const;

#ifdef _DEBUG
	std::string ToString() const;
#endif
	/* ==================== Non-static member functions end ==================== */

	/* ==================== Non-static member variables begin ==================== */
	Math::Vector3D m_pos;
	Math::Vector2D m_texCoord;
	Math::Vector3D m_normal;
	Math::Vector3D m_tangent;
	//Math::Vector3D m_bitangent;
	
	// TODO: Memory layout- add additional padding variable or
	// use a table of Math::Vector3D objects with size 4 where [0], [1], [2], [3] store position, texture coordinates, normals and tangents respectively.

	/* ==================== Non-static member variables end ==================== */
}; /* end struct Vertex */

} /* end namespace Rendering */

#endif /* __RENDERING_VERTEX_H__ */