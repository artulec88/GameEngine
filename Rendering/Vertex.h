#pragma once

#include "Rendering.h"

/* MATH begin */
#include "Math\Vector.h"
/* MATH end */

#include <string>

namespace Rendering
{

struct RENDERING_API Vertex
{
	Math::Vector3D pos;
	Math::Vector2D texCoord;
	Math::Vector3D normal;
	Math::Vector3D tangent;
	//Math::Vector3D bitangent;

	Vertex(const Math::Vector3D& position,
		const Math::Vector2D& texCoord = Math::Vector2D(REAL_ZERO, REAL_ZERO),
		const Math::Vector3D& normal = Math::Vector3D(REAL_ZERO, REAL_ZERO, REAL_ZERO),
		const Math::Vector3D& tangent = Math::Vector3D(REAL_ZERO, REAL_ZERO, REAL_ZERO));

	bool operator==(const Vertex& vertex) const;
	bool operator!=(const Vertex& vertex) const;

	std::string ToString() const;
}; /* end struct Vertex */

} /* end namespace Rendering */