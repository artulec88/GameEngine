#pragma once

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
	Math::Vector3D m_pos;
	Math::Vector2D m_texCoord;
	Math::Vector3D m_normal;
	Math::Vector3D m_tangent;
	//Math::Vector3D m_bitangent;

	Vertex(const Math::Vector3D& position,
		const Math::Vector2D& texCoord = Math::Vector2D(REAL_ZERO, REAL_ZERO),
		const Math::Vector3D& normal = Math::Vector3D(REAL_ZERO, REAL_ZERO, REAL_ZERO),
		const Math::Vector3D& tangent = Math::Vector3D(REAL_ZERO, REAL_ZERO, REAL_ZERO)/*,
		const Math::Vector3D& bitangent = Math::Vector3D(REAL_ZERO, REAL_ZERO, REAL_ZERO)*/);

	bool operator==(const Vertex& vertex) const;
	bool operator!=(const Vertex& vertex) const;

#ifdef _DEBUG
	std::string ToString() const;
#endif
}; /* end struct Vertex */

} /* end namespace Rendering */