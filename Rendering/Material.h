#pragma once

#include "Texture.h"

#include "Math\Vector.h"

namespace Rendering
{

struct Material
{
/* ==================== Non-static member variables begin ==================== */
	Texture* texture;
	Math::Vector3D color;
	Math::Real specularIntensity;
	Math::Real specularPower;
/* ==================== Non-static member variables end ==================== */

/* ==================== Constructors and destructors begin ==================== */
	Material(Texture* texture = NULL,
		const Math::Vector3D& color = Math::Vector3D(1.0, 1.0, 1.0),
		Math::Real specularIntensity = 2,
		Math::Real specularPower = 32);

	~Material(void);
/* ==================== Constructors and destructors end ==================== */
}; /* end class Material */

} /* end namespace Rendering */