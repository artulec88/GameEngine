#pragma once

#include "Rendering.h"
#include "Texture.h"

#include "Math\Vector.h"

namespace Rendering
{

struct RENDERING_API Material
{
/* ==================== Non-static member variables begin ==================== */
	Texture* texture;
	Math::Vector3D color;
	/**
	 * @brief Specular intensity
	 *
	 * Measures the intensity of the reflection from the material (metal reflects more light, whereas wood reflects almost no light)
	 */
	Math::Real specularIntensity;
	/**
	 * @brief Specular power
	 *
	 * Defines the "width" of the reflection, in common sense.
	 * When small it is just a tiny focused beam and otherwise a wide-spreaded reflection
	 */
	Math::Real specularPower;
/* ==================== Non-static member variables end ==================== */

/* ==================== Constructors and destructors begin ==================== */
	Material(Texture* texture = NULL,
		const Math::Vector3D& color = Math::Vector3D(1.0, 1.0, 1.0),
		Math::Real specularIntensity = 2,
		Math::Real specularPower = 32);

	~Material(void);
/* ==================== Constructors and destructors end ==================== */


/* ==================== Non-static member functions begin ==================== */
	Math::Real GetSpecularIntensity() const;
	void SetSpecularIntensity(Math::Real specularIntensity);
	Math::Real GetSpecularPower() const;
	void SetSpecularPower(Math::Real specularPower);
/* ==================== Non-static member functions end ==================== */
}; /* end class Material */

} /* end namespace Rendering */