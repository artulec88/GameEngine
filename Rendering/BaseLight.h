#ifndef __RENDERING_BASE_LIGHT_H__
#define __RENDERING_BASE_LIGHT_H__

#include "Rendering.h"
#include "ShadowInfo.h"
#include "Color.h"

#include "Math/Vector.h"
#include "Math/Transform.h"
#ifdef ANT_TWEAK_BAR_ENABLED
#include "AntTweakBar/include/AntTweakBar.h"
#endif

namespace rendering
{
	class Shader;

	namespace lighting
	{

		class BaseLight
		{
			/* ==================== Static variables and functions begin ==================== */
			/* ==================== Static variables and functions end ==================== */

			/* ==================== Constructors and destructors begin ==================== */
		public:
			/// <summary>
			/// Base light constructor.
			/// </summary>
			/// <param name="transform"> The light transform. </param>
			/// <param name="color"> The color of the light. </param>
			/// <param name="intensity"> The intensity of the light. </param>
			RENDERING_API BaseLight(const math::Transform& transform, const Color& color, math::Real intensity, int shaderId,
				int terrainShaderId, int noShadowShaderId, int noShadowTerrainShaderId, bool isShadowingEnabled = false);

			/// <summary>The base light destructor.</summary>
			RENDERING_API virtual ~BaseLight();

			/// <summary> Base light copy constructor. </summary>
			/// <param name="baseLight"> The reference to the base light to copy construct from. </param>
			RENDERING_API BaseLight(const BaseLight& baseLight) = delete;

			/// <summary> Base light move constructor. </summary>
			/// <param name="baseLight"> The r-value reference to the base light to move construct from. </param>
			RENDERING_API BaseLight(BaseLight&& baseLight) = default;

			/// <summary> Base light copy assignment operator. </summary>
			/// <param name="baseLight"> The reference to the base light to copy assign from. </param>
			/// <returns> The reference to the newly copy-assigned base light. </returns>
			BaseLight& operator=(const BaseLight& baseLight) = delete;

			/// <summary> Base light move assignment operator. </summary>
			/// <param name="baseLight"> The r-value reference to the base light to move assign from. </param>
			/// <returns> The reference to the newly move-assigned base light. </returns>
			BaseLight& operator=(BaseLight&& baseLight) = default;
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
			/// <summary> Gets the current color of the light. </summary>
			/// <returns> Current color of the light. </returns>
			RENDERING_API const Color& GetColor() const { return m_color; }

			/// <summary> Sets new color for the light. </summary>
			/// <param name="color"> The new color for the light. </param>
			RENDERING_API void SetColor(const Color& color) { m_color = color; }

			/// <summary> Gets the current intensity of the light. </summary>
			/// <returns> Current intensity of the light. </returns>
			RENDERING_API math::Real GetIntensity() const { return m_intensity; }

			/// <summary> Sets new intensity for the light. </summary>
			/// <param name="intensity"> New intensity for the light. </param>
			RENDERING_API void SetIntensity(const math::Real intensity) { m_intensity = intensity; }

			/// <summary> Gives information whether light casts shadows or not. </summary>
			/// <returns> <code>true</code> if the light casts shadows and <code>false</code> otherwise. </returns>
			RENDERING_API bool IsShadowingEnabled() const { return m_isShadowingEnabled; }

			/// <summary> Gives information whether the light is enabled or not. </summary>
			/// <returns> <code>true</code> if the light is enabled and <code>false</code> otherwise. </returns>
			RENDERING_API bool IsEnabled() const { return m_isEnabled; }

			/// <summary> Gets the current light transform. </summary>
			/// <returns> Light transform reference. </returns>
			RENDERING_API math::Transform& GetTransform() { return m_transform; }

			/// <summary> Gets the current light transform. </summary>
			/// <returns> Constant light transform reference. </returns>
			RENDERING_API const math::Transform& GetTransform() const { return m_transform; }

			RENDERING_API int GetShaderId() const { return m_shaderId; }
			RENDERING_API int GetTerrainShaderId() const { return m_terrainShaderId; }
			RENDERING_API int GetNoShadowShaderId() const { return m_noShadowShaderId; }
			RENDERING_API int GetNoShadowTerrainShaderId() const { return m_noShadowTerrainShaderId; }

			RENDERING_API const ShadowInfo* GetShadowInfo() const { return m_shadowInfo.get(); }

			RENDERING_API virtual ShadowCameraTransform CalcShadowCameraTransform(const math::Vector3D& cameraPos, const math::Quaternion& cameraRot) const;

			RENDERING_API void SetTransform(const math::Transform& transform) { m_transform = transform; }
			RENDERING_API void SetIsEnabled(const bool isEnabled) { m_isEnabled = isEnabled; }
			RENDERING_API void SetIsShadowingEnabled(const bool isShadowingEnabled) { m_isShadowingEnabled = isShadowingEnabled; }
#ifdef ANT_TWEAK_BAR_ENABLED
			RENDERING_API virtual void InitializeTweakBar(TwBar* lightsBar);
#endif
		protected:
			void SetShadowInfo(const math::Matrix4D& projection, bool flipFacesEnabled, int shadowMapSizeAsPowerOf2,
				math::Real shadowSoftness, math::Real lightBleedingReductionFactor, math::Real minVariance);
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
			/// <summary> The base light transform. </summary>
			math::Transform m_transform;

			/// <summary>The light color.</summary>
			Color m_color;

			/// <summary>The light intensity.</summary>
			math::Real m_intensity;

			/// <summary>The ID of the default shader for the light.</summary>
			int m_shaderId;

			/// <summary>The ID of the terrain shader for the light.</summary>
			int m_terrainShaderId;

			/// <summary> The ID of the default shader with no shadow calculation for the light. </summary>
			int m_noShadowShaderId;

			/// <summary> The ID of the terrain shader with no shadow calculation for the light. </summary>
			int m_noShadowTerrainShaderId;

			/// <summary>The information about the shadow that the light casts.</summary>
			std::unique_ptr<ShadowInfo> m_shadowInfo;

			/// <summary>The information whether the light is enabled (the value <code>true</code>) or not (the value <code>false</code>).</summary>
			bool m_isEnabled;

			/// <summary>The information about whether the light casts shadows (the value <code>true</code>) or not (the value <code>false</code>).</summary>
			bool m_isShadowingEnabled;
			/* ==================== Non-static member variables end ==================== */
		}; /* end class BaseLight */

	} /* end namespace lighting */

} /* end namespace rendering */

#endif /* __RENDERING_BASE_LIGHT_H__ */