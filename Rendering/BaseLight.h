#ifndef __RENDERING_BASE_LIGHT_H__
#define __RENDERING_BASE_LIGHT_H__

#include "Rendering.h"
#include "ShadowInfo.h"
#include "Color.h"

#include "Math\Vector.h"
#include "Math\Transform.h"
#ifdef ANT_TWEAK_BAR_ENABLED
#include "AntTweakBarTypes.h"
#include "AntTweakBar\include\AntTweakBar.h"
#endif

namespace Rendering
{
	class Shader;

	namespace Lighting
	{

		class BaseLight
		{
			/* ==================== Static variables and functions begin ==================== */
			/* ==================== Static variables and functions end ==================== */

			/* ==================== Constructors and destructors begin ==================== */
		public:
			/// <summary>The constructor.</summary>
			/// <param name="transform">The transform of the light.</param>
			/// <param name="color">The color of the light.</param>
			/// <param name="intensity">The intensity of the light.</param>
			/// <remarks><code>explicit</code> keyword is used to prevent implicit conversions between <code>Color</code> objects and <code>BaseLight</code>.</remarks>
			RENDERING_API BaseLight(Math::Transform& transform, const Color& color, Math::Real intensity, const Shader& shader,
				const Shader& terrainShader, const Shader& noShadowShader, const Shader& noShadowTerrainShader);

			/// <summary>The destructor.</summary>
			RENDERING_API virtual ~BaseLight(void);
			
			BaseLight(const BaseLight& baseLight) = delete; // Copy constructor
			BaseLight(BaseLight&& baseLight) = delete; // Move constructor
			BaseLight& operator=(const BaseLight& baseLight) = delete; // Copy assignment operator
			BaseLight& operator=(BaseLight&& baseLight) = delete; // Move assignment operator
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
		public:
			//virtual void Update(Math::Real delta);
			//virtual void Render(Shader* shader, Renderer* renderer);
			const Color& GetColor() const { return m_color; }
			Math::Real GetIntensity() const { return m_intensity; }
			RENDERING_API inline const Shader& GetShader() const { return m_shader; }
			RENDERING_API inline const Shader& GetTerrainShader() const { return m_terrainShader; }
			RENDERING_API inline const Shader& GetNoShadowShader() const { return m_noShadowShader; }
			RENDERING_API inline const Shader& GetNoShadowTerrainShader() const { return m_noShadowTerrainShader; }
			RENDERING_API inline const ShadowInfo* GetShadowInfo() const { return m_shadowInfo.get(); }
			virtual bool IsEnabled() const { return m_isEnabled; }

			bool IsShadowingEnabled() const { return m_isShadowingEnabled; }

			virtual ShadowCameraTransform CalcShadowCameraTransform(const Math::Vector3D& cameraPos, const Math::Quaternion& cameraRot);
			Math::Transform& GetTransform() { return m_transform; }
			const Math::Transform& GetTransform() const { return m_transform; }

			RENDERING_API void SetColor(const Color& color) { m_color = color; }
			RENDERING_API void SetIntensity(Math::Real intensity) { m_intensity = intensity; }
			RENDERING_API void SetIsEnabled(bool isEnabled) { m_isEnabled = isEnabled; }
			void SetShadowInfo(const Math::Matrix4D& projection, bool flipFacesEnabled, int shadowMapSizeAsPowerOf2,
				Math::Real shadowSoftness = REAL_ONE, Math::Real lightBleedingReductionAmount = static_cast<Math::Real>(0.2f),
				Math::Real minVariance = static_cast<Math::Real>(0.00002f));

#ifdef ANT_TWEAK_BAR_ENABLED
			virtual void InitializeTweakBar(TwBar* lightsBar);
#endif
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
		protected:
			/// <summary> The base light transform. </summary>
			Math::Transform& m_transform;

			/// <summary>The light color.</summary>
			Color m_color;

			/// <summary>The light intensity.</summary>
			Math::Real m_intensity;

			/// <summary>The default shader for the light.</summary>
			const Shader& m_shader; // TODO: Assuming we have 10 point lights then we have also 10 instances of the same Shader object. It would be better to store just a reference or a pointer to a globally accessible shader (e.g. ShaderFactory?).

			/// <summary>The terrain shader for the light.</summary>
			const Shader& m_terrainShader;

			/// <summary> The default shader with no shadow calculation for the light. </summary>
			const Shader& m_noShadowShader;

			/// <summary> The terrain shader with no shadow calculation for the light. </summary>
			const Shader& m_noShadowTerrainShader;

			/// <summary>The information about the shadow that the light casts.</summary>
			std::unique_ptr<ShadowInfo> m_shadowInfo;

			/// <summary>The information whether the light is enabled or not.</summary>
			bool m_isEnabled;

			/// <summary>The information about whether the light casts shadows or not.</summary>
			bool m_isShadowingEnabled;
			/* ==================== Non-static member variables end ==================== */
		}; /* end class BaseLight */

	} /* end namespace Lighting */

} /* end namespace Rendering */

#endif /* __RENDERING_BASE_LIGHT_H__ */