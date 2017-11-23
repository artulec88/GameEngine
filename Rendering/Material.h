#ifndef __RENDERING_MATERIAL_H__
#define __RENDERING_MATERIAL_H__

#include "Rendering.h"
#include "Texture.h"
#include "MappedValues.h"

#include "Math/Vector.h"

#include <string>

namespace rendering
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
		/// <param name="specularIntensity">
		/// The measure of how intensive the reflection of the material is.
		/// For example, the metal reflect more light, whereas wood reflects almost no light.
		/// </param>
		/// <param name="specularPower">
		/// Defines the "width" of the reflection, in common sense.
		/// When a small value is specified the reflection is just a tiny focused beam and otherwise a wide-spreaded reflection.
		/// </param>
		/// <param name="normalMap">The normal map for the material.</param>
		/// <param name="displacementMap">The dispacement map of the material.</param>
		/// <param name="displacementScale">The displacement scale.</param>
		/// <param name="displacementOffset">The displacement offset.</param>
		RENDERING_API Material(const Texture* diffuseTexture, math::Real specularIntensity, math::Real specularPower, const Texture* normalMap,
			const Texture* displacementMap, math::Real displacementScale = REAL_ZERO, math::Real displacementOffset = REAL_ZERO);

		/// <summary>
		/// Material constructor that takes a <paramref name="texture"/> with name <paramref name="textureName"/>.
		/// </summary>
		/// <param name="texture"> The texture the material will use. </param>
		/// <param name="textureName"> The name of the texture for texture-lookup mechanism. </param>
		RENDERING_API Material(const Texture* texture, const std::string& textureName);

		/// <summary> Material destructor. </summary>
		RENDERING_API ~Material();

		/// <summary> Material copy constructor. </summary>
		/// <param name="material"> The material to copy construct from. </param>
		Material(const Material& material) = delete;

		/// <summary> Material move constructor. </summary>
		/// <param name="material"> The material to move construct from. </param>
		Material(Material&& material) = delete;

		/// <summary> Material copy assignment operator. </summary>
		/// <param name="material"> The material to copy assign from. </param>
		/// <returns> The newly copy-assigned material. </returns>
		Material& operator=(const Material& material) = delete;
		
		/// <summary> Material move assignment operator. </summary>
		/// <param name="material"> The material to move assign from. </param>
		/// <returns> The newly move-assigned material. </returns>
		Material& operator=(Material&& material) = delete;
		/* ==================== Constructors and destructors end ==================== */


		/* ==================== Non-static member functions begin ==================== */
		const math::Real& GetReal(const std::string& realValueName) const { return m_mappedValues.GetReal(realValueName); }
		const math::Vector3D& GetVec3D(const std::string& vectorName) const { return m_mappedValues.GetVec3D(vectorName); }
		const math::Vector4D& GetVec4D(const std::string& vectorName) const { return m_mappedValues.GetVec4D(vectorName); }
		const Texture* GetTexture(const std::string& textureName) const { return m_mappedValues.GetTexture(textureName); }

		void SetReal(const std::string& realValueName, math::Real realValue) { m_mappedValues.SetReal(realValueName, realValue); }
		void SetVector3D(const std::string& vectorName, const math::Vector3D& vec3D) { m_mappedValues.SetVector3D(vectorName, vec3D); }
		void SetVector4D(const std::string& vectorName, const math::Vector4D& vec4D) { return m_mappedValues.SetVector4D(vectorName, vec4D); }

		RENDERING_API void SetAdditionalTexture(const Texture* texture, const std::string& textureName);
#ifdef ANT_TWEAK_BAR_ENABLED
		RENDERING_API void InitializeTweakBar(TwBar* tweakBar, const char* groupName);
#endif

		/// <summary> Gives information whether or not the material uses multiple textures. </summary>
		/// <returns> <code>true</code> when material uses multiple textures (at least two) and <code>false</code> otherwise. </returns>
		bool HasMultipleTextures() const noexcept { return m_hasMultipleTextures; }
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	private:
		/// <summary>
		/// Mapped values container.
		/// </summary>
		MappedValues m_mappedValues;
		
		/// <summary>
		/// Indicates whether the material uses multiple textures (value <code>true</code>) or not (value <code>false</code>).
		/// </summary>
		bool m_hasMultipleTextures;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class Material */

} /* end namespace rendering */

#endif /* __RENDERING_MATERIAL_H__ */