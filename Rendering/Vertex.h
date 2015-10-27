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

struct RENDERING_API Vertex
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
	/* ==================== Non-static member variables end ==================== */
}; /* end struct Vertex */

} /* end namespace Rendering */

#endif /* __RENDERING_VERTEX_H__ */