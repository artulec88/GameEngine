#pragma once

#include "Rendering.h"
#include "Texture.h"
#include "MappedValues.h"

#include "Math\Vector.h"

#include <string>
#include <map>

namespace Rendering
{

class Material : public MappedValues
{
/* ==================== Constructors and destructors begin ==================== */
public:
	RENDERING_API Material(Texture* diffuseTexture, Math::Real specularIntensity = REAL_ONE, Math::Real specularPower = 8.0f, Texture* normalMap = NULL,
		Texture* displacementMap = NULL, Math::Real displacementScale = REAL_ZERO, Math::Real displacementOffset = REAL_ZERO);
	RENDERING_API Material(Texture* texture, const std::string& textureName);
	RENDERING_API virtual ~Material(void) { };
private:
	Material(const Material& material) {}
	void operator=(const Material& material) {}
/* ==================== Constructors and destructors end ==================== */


/* ==================== Non-static member functions begin ==================== */
public:
	RENDERING_API void SetAdditionalTexture(Texture* texture, const std::string& textureName);
	bool HasMultipleTextures() const { return m_hasMultipleTextures; }
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
private:
	bool m_hasMultipleTextures;
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
}; /* end class Material */

} /* end namespace Rendering */