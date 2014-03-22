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
	//Math::Vector3D normal;

	Vertex(const Math::Vector3D& position, const Math::Vector2D& texCoord = Math::Vector2D(0.0, 0.0));
	//Vertex(Math::Vector3D& position, Math::Vector2D& texCoord, Math::Vector3D& normal);

	std::string ToString() const;
}; /* end struct Vertex */

} /* end namespace Rendering */