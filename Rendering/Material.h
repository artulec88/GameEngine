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
	/// <summary>
	/// Creates a new material based on the given parameters.
	/// </summary>
	/// <param name="diffuseTexture">The diffuse texture to be used by the material.</param>
	/// <param name="specularIntensity">The measure of how intensive the reflection of the material is.
	/// For example, the metal reflect more light, whereas wood reflects almost no light.</param>
	/// <param name="specularPower>Defines the "width" of the reflection, in common sense.
	/// When a small value is specified the reflection is just a tiny focused beam and otherwise a wide-spreaded reflection.</param>
	/// <param name="normalMap>The normal map for the material.</param>
	/// <param name="displacementMap">The dispacement map of the material.</param>
	/// <param name="displacementScale">The displacement scale.</param>
	/// <param name="displacementOffset">The displacement offset.</param>
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
/* ==================== Non-static member variables end ==================== */
}; /* end class Material */

} /* end namespace Rendering */