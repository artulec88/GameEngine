#pragma once

#include "Rendering.h"
#include "Texture.h"
#include "MappedValues.h"

#include "Math\Vector.h"

#include <map>

namespace Rendering
{

class Material : public MappedValues
{
/* ==================== Non-static member variables begin ==================== */
private:
	//Texture* texture;
	//Math::Vector3D color;
	/**
	 * @brief Specular intensity
	 *
	 * Measures the intensity of the reflection from the material (metal reflects more light, whereas wood reflects almost no light)
	 */
	//Math::Real specularIntensity;
	/**
	 * @brief Specular power
	 *
	 * Defines the "width" of the reflection, in common sense.
	 * When small it is just a tiny focused beam and otherwise a wide-spreaded reflection
	 */
	//Math::Real specularPower;

/* ==================== Non-static member variables end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	RENDERING_API Material(Texture* diffuse, Math::Real specularIntensity = 1, Math::Real specularPower = 8,
		Texture* normalMap = NULL,
		Texture* displacementMap = NULL, Math::Real displacementScale = 0.0f, Math::Real displacementOffset = 0.0f); // TODO: Do not use hard-coded values
	RENDERING_API virtual ~Material(void) { };
private:
	Material(const Material& material) {}
	void operator=(const Material& material) {}
/* ==================== Constructors and destructors end ==================== */


/* ==================== Non-static member functions begin ==================== */
public:
	RENDERING_API Texture* GetDiffuseTexture() const { return GetTexture("diffuse"); }
	RENDERING_API Math::Real GetSpecularIntensity() const { return GetReal("specularIntensity"); }
	RENDERING_API Math::Real GetSpecularPower() const { return GetReal("specularPower"); }
/* ==================== Non-static member functions end ==================== */
}; /* end class Material */

} /* end namespace Rendering */