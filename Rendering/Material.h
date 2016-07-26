#ifndef __RENDERING_MATERIAL_H__
#define __RENDERING_MATERIAL_H__

#include "Rendering.h"
#include "Texture.h"
#include "MappedValues.h"

#include "Math\Vector.h"

#include <string>
#include <map>

namespace Rendering
{

	/// <summary>
	/// The representation of the material that can be applied on a mesh.
	/// </summary>
	class Material
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
		const Math::Real& GetReal(const std::string& realValueName) const { return m_mappedValues.GetReal(realValueName); }
		const Math::Vector3D& GetVec3D(const std::string& vectorName) const { return m_mappedValues.GetVec3D(vectorName); }
		const Math::Vector4D& GetVec4D(const std::string& vectorName) const { return m_mappedValues.GetVec4D(vectorName); }
		const Texture* GetTexture(const std::string& textureName) const { return m_mappedValues.GetTexture(textureName); }

		void SetReal(const std::string& realValueName, Math::Real realValue) { m_mappedValues.SetReal(realValueName, realValue); }
		void SetVector3D(const std::string& vectorName, const Math::Vector3D& vec3D) { m_mappedValues.SetVector3D(vectorName, vec3D); }
		void SetVector4D(const std::string& vectorName, const Math::Vector4D& vec4D) { return m_mappedValues.SetVector4D(vectorName, vec4D); }

		RENDERING_API void SetAdditionalTexture(Texture* texture, const std::string& textureName);
#ifdef ANT_TWEAK_BAR_ENABLED
		RENDERING_API virtual void InitializeTweakBar(TwBar* tweakBar, const char* groupName);
#endif
		bool HasMultipleTextures() const { return m_hasMultipleTextures; }
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	private:
		MappedValues m_mappedValues;
		bool m_hasMultipleTextures;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class Material */

} /* end namespace Rendering */

#endif /* __RENDERING_MATERIAL_H__ */