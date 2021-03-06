#ifndef __RENDERING_LIGHT_BUILDER_H__
#define __RENDERING_LIGHT_BUILDER_H__

#include "Rendering.h"

#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"

#include "Utility/Builder.h"

namespace rendering
{
	namespace lighting
	{
		template <class T>
		class LightBuilder : public utility::Builder<T>
		{
			/* ==================== Static variables and functions begin ==================== */
			/* ==================== Static variables and functions end ==================== */

			/* ==================== Constructors and destructors begin ==================== */
		public:
			/// <summary> Light builder constructor. </summary>
			/// <param name="defaultPosition"> The default position of the light to be built. </param>
			/// <param name="defaultRotation">
			/// The quaternion representing the default rotation (orientation) of the light to be built.
			/// </param>
			/// <param name="defaultColor"> The default color of the light to be built. </param>
			/// <param name="defaultIntensity"> The default intensity of the light to be built. </param>
			/// <param name="defaultShadowInfoFlipFacesEnabled">
			/// The simple <code>bool</code> flag indicating whether the light to be created is supposed to cast shadows that use face flipping mechanism.
			/// </param>
			/// <param name="defaultShadowInfoShadowMapSizeAsPowerOf2">
			/// The integer value representing the default size of the shadow map to be used by the light to be built.
			/// More precisely, the specified value is the exponent in the equation <code>2^n</code> which is then used
			/// to calculate tha actual size of the shadow map.
			/// </param>
			/// <param name="defaultShadowInfoShadowSoftness">
			/// The simple floating-point value representing the softness of the shadows that the light to be built will cast.
			/// </param>
			/// <param name="defaultShadowInfoLightBleedingReductionFactor">
			/// The simple floating-point value representing the light-bleeding effect reduction factor.
			/// </param>
			/// <param name="defaultShadowInfoMinVariance">
			/// The simple floating-point value indicating the threshold in the VSM algorithm (Variance Shadow Mapping) when
			/// determining whether given point is in shadow from the specific light perspective or not.
			/// </param>
			LightBuilder(const math::Vector3D& defaultPosition, const math::Quaternion& defaultRotation, const Color& defaultColor, math::Real defaultIntensity,
				bool defaultShadowInfoFlipFacesEnabled, int defaultShadowInfoShadowMapSizeAsPowerOf2, math::Real defaultShadowInfoShadowSoftness, math::Real defaultShadowInfoLightBleedingReductionFactor,
				math::Real defaultShadowInfoMinVariance, int shaderId, int terrainShaderId, int noShadowShaderId, int noShadowTerrainShaderId) :
				utility::Builder<T>(),
				m_defaultPos(defaultPosition),
				m_defaultRot(defaultRotation),
				m_defaultColor(defaultColor),
				m_defaultIntensity(defaultIntensity),
				m_defaultShadowInfoFlipFacesEnabled(defaultShadowInfoFlipFacesEnabled),
				m_defaultShadowInfoShadowMapSizeAsPowerOf2(defaultShadowInfoShadowMapSizeAsPowerOf2),
				m_defaultShadowInfoShadowSoftness(defaultShadowInfoShadowSoftness),
				m_defaultShadowInfoLightBleedingReductionFactor(defaultShadowInfoLightBleedingReductionFactor),
				m_defaultShadowInfoMinVariance(defaultShadowInfoMinVariance),
				m_pos(m_defaultPos),
				m_rot(m_defaultRot),
				m_color(m_defaultColor),
				m_intensity(m_defaultIntensity),
				m_isShadowInfoFlipFacesEnabled(m_defaultShadowInfoFlipFacesEnabled),
				m_shadowInfoShadowMapSizeAsPowerOf2(m_defaultShadowInfoShadowMapSizeAsPowerOf2),
				m_shadowInfoShadowSoftness(m_defaultShadowInfoShadowSoftness),
				m_shadowInfoLightBleedingReductionFactor(m_defaultShadowInfoLightBleedingReductionFactor),
				m_shadowInfoMinVariance(m_defaultShadowInfoMinVariance),
				m_shaderId(shaderId),
				m_terrainShaderId(terrainShaderId),
				m_noShadowShaderId(noShadowShaderId),
				m_noShadowTerrainShaderId(noShadowTerrainShaderId)
			{
			}

			/// <summary> Light builder destructor. </summary>
			virtual ~LightBuilder()
			{
			}

			/// <summary> Light builder copy constructor. </summary>
			/// <param name="lightBuilder"> The reference to the light builder to copy construct from. </param>
			LightBuilder(const LightBuilder& lightBuilder) = delete;

			/// <summary> Light builder move constructor. </summary>
			/// <param name="lightBuilder"> The r-value reference to the light builder to move construct from. </param>
			LightBuilder(LightBuilder&& lightBuilder) = delete;

			/// <summary> Light builder copy assignment operator. </summary>
			/// <param name="lightBuilder"> The reference to the light builder to copy assign from. </param>
			/// <returns> The reference to the newly copy-assigned light builder. </returns>
			LightBuilder& operator=(const LightBuilder& lightBuilder) = delete;

			/// <summary> Light builder move assignment operator. </summary>
			/// <param name="lightBuilder"> The r-value reference to the light builder to move assign from. </param>
			/// <returns> The reference to the newly move-assigned light builder. </returns>
			LightBuilder& operator=(LightBuilder&& lightBuilder) = delete;
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
			void SetDefault() override
			{
				m_pos = m_defaultPos;
				m_rot = m_defaultRot;
				m_color = m_defaultColor;
				m_intensity = m_defaultIntensity;
				m_isShadowInfoFlipFacesEnabled = m_defaultShadowInfoFlipFacesEnabled;
				m_shadowInfoShadowMapSizeAsPowerOf2 = m_defaultShadowInfoShadowMapSizeAsPowerOf2;
				m_shadowInfoShadowSoftness = m_defaultShadowInfoShadowSoftness;
				m_shadowInfoLightBleedingReductionFactor = m_defaultShadowInfoLightBleedingReductionFactor;
				m_shadowInfoMinVariance = m_defaultShadowInfoMinVariance;
			}

			LightBuilder<T>& SetPosition(const math::Vector3D& position)
			{
				return SetPosition(position.x, position.y, position.z);
			}
			LightBuilder<T>& SetPosition(math::Real posX, math::Real posY, math::Real posZ)
			{
				m_pos.Set(posX, posY, posZ);
				return *this;
			}
			LightBuilder<T>& SetColor(color_ids::ColorId colorId)
			{
				m_color = Color(colorId);
				return *this;
			}
			LightBuilder<T>& SetColor(math::Real red, math::Real green, math::Real blue, math::Real alpha = REAL_ONE)
			{
				m_color.Set(red, green, blue, alpha);
				return *this;
			}
			LightBuilder<T>& SetColor(const Color& color)
			{
				m_color = color;
				return *this;
			}
			LightBuilder<T>& SetIntensity(math::Real intensity)
			{
				m_intensity = intensity;
				return *this;
			}
			LightBuilder<T>& SetIsShadowInfoFlipFacesEnabled(bool isShadowInfoFlipFacesEnabled)
			{
				m_isShadowInfoFlipFacesEnabled = isShadowInfoFlipFacesEnabled;
				return *this;
			}
			LightBuilder<T>& SetShadowInfoShadowMapSizeAsPowerOf2(int shadowInfoShadowMapSizeAsPowerOf2)
			{
				m_shadowInfoShadowMapSizeAsPowerOf2 = shadowInfoShadowMapSizeAsPowerOf2;
				return *this;
			}
			LightBuilder<T>& SetShadowInfoShadowSoftness(math::Real shadowInfoShadowSoftness)
			{
				m_shadowInfoShadowSoftness = shadowInfoShadowSoftness;
				return *this;
			}
			LightBuilder<T>& SetShadowInfoLightBleedingReductionFactor(math::Real shadowInfoLightBleedingReductionFactor)
			{
				m_shadowInfoLightBleedingReductionFactor = shadowInfoLightBleedingReductionFactor;
				return *this;
			}
			LightBuilder<T>& SetShadowInfoMinVariance(math::Real shadowInfoMinVariance)
			{
				m_shadowInfoMinVariance = shadowInfoMinVariance;
				return *this;
			}
		protected:
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
			const math::Vector3D m_defaultPos; // TODO: Make it static constexpr in the future.
			const math::Quaternion m_defaultRot; // TODO: Make it static constexpr in the future.
			const Color m_defaultColor; // TODO: Make it static constexpr in the future.
			const math::Real m_defaultIntensity; // TODO: Make it static constexpr in the future.
			const bool m_defaultShadowInfoFlipFacesEnabled; // TODO: Make it static constexpr in the future.
			const int m_defaultShadowInfoShadowMapSizeAsPowerOf2; // TODO: Make it static constexpr in the future.
			const math::Real m_defaultShadowInfoShadowSoftness; // TODO: Make it static constexpr in the future.
			const math::Real m_defaultShadowInfoLightBleedingReductionFactor; // TODO: Make it static constexpr in the future.
			const math::Real m_defaultShadowInfoMinVariance; // TODO: Make it static constexpr in the future.

			math::Vector3D m_pos;
			math::Quaternion m_rot;
			Color m_color;
			math::Real m_intensity;

			bool m_isShadowInfoFlipFacesEnabled;
			int m_shadowInfoShadowMapSizeAsPowerOf2;
			math::Real m_shadowInfoShadowSoftness;
			math::Real m_shadowInfoLightBleedingReductionFactor;
			math::Real m_shadowInfoMinVariance;

			int m_shaderId;
			int m_terrainShaderId;
			int m_noShadowShaderId;
			int m_noShadowTerrainShaderId;
			/* ==================== Non-static member variables end ==================== */
		}; /* end class LightBuilder<T> */

		/// <summary>
		/// Directional light builder.
		/// </summary>
		class DirectionalLightBuilder : public LightBuilder<DirectionalLight>
		{
			/* ==================== Static variables and functions begin ==================== */
			/* ==================== Static variables and functions end ==================== */

			/* ==================== Constructors and destructors begin ==================== */
		public:
			/// <summary> Directional light builder constructor. </summary>
			RENDERING_API DirectionalLightBuilder();

			/// <summary> Directional light builder destructor. </summary>
			RENDERING_API virtual ~DirectionalLightBuilder();

			/// <summary> Directional light builder copy constructor. </summary>
			/// <param name="directionalLightBuilder"> The directional light builder to copy construct from. </param>
			DirectionalLightBuilder(const DirectionalLightBuilder& directionalLightBuilder) = delete;

			/// <summary> Directional light builder move constructor. </summary>
			/// <param name="directionalLightBuilder"> The directional light builder to move construct from. </param>
			DirectionalLightBuilder(DirectionalLightBuilder&& directionalLightBuilder) = delete;

			/// <summary> Directional light builder copy assignment operator. </summary>
			/// <param name="directionalLightBuilder"> The directional light builder to copy assign from. </param>
			/// <returns> The newly copy-assigned directional light builder. </returns>
			DirectionalLightBuilder& operator=(const DirectionalLightBuilder& directionalLightBuilder) = delete;

			/// <summary> Directional light builder move assignment operator. </summary>
			/// <param name="directionalLightBuilder"> The directional light builder to move assign from. </param>
			/// <returns> The newly move-assigned directional light builder. </returns>
			DirectionalLightBuilder& operator=(DirectionalLightBuilder&& directionalLightBuilder) = delete;
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
		protected:
			void SetDefault() override;
			DirectionalLight Build() override
			{
				return DirectionalLight(math::Transform(m_pos, m_rot), m_color, m_intensity, m_shaderId, m_terrainShaderId, m_noShadowShaderId, m_noShadowTerrainShaderId,
					m_isShadowInfoFlipFacesEnabled, m_shadowInfoShadowMapSizeAsPowerOf2, m_shadowInfoShadowSoftness, m_shadowInfoLightBleedingReductionFactor,
					m_shadowInfoMinVariance, m_halfShadowArea);
			}
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
		private:
			const math::Real m_defaultHalfShadowArea; // TODO: Make it a static constexpr in the future.

			math::Real m_halfShadowArea;
			/* ==================== Non-static member variables end ==================== */
		}; /* end class DirectionalLightBuilder */


		/// <summary>
		/// Point light builder.
		/// </summary>
		class PointLightBuilder : public LightBuilder<PointLight>
		{
			/* ==================== Static variables and functions begin ==================== */
			/* ==================== Static variables and functions end ==================== */

			/* ==================== Constructors and destructors begin ==================== */
		public:
			/// <summary> Point light builder constructor. </summary>
			RENDERING_API PointLightBuilder();

			/// <summary> Point light builder destructor. </summary>
			RENDERING_API virtual ~PointLightBuilder()
			{
			}

			/// <summary> Point light builder copy constructor. </summary>
			/// <param name="pointLightBuilder"> The point light builder to copy construct from. </param>
			PointLightBuilder(PointLightBuilder& pointLightBuilder) = delete;

			/// <summary> Point light builder move constructor. </summary>
			/// <param name="pointLightBuilder"> The point light builder to move construct from. </param>
			PointLightBuilder(PointLightBuilder&& pointLightBuilder) = delete;

			/// <summary> Point light builder copy assignment operator. </summary>
			/// <param name="pointLightBuilder"> The point light builder to copy assign from. </param>
			/// <returns> The reference to the newly copy-assigned point light builder. </returns>
			PointLightBuilder& operator=(const PointLightBuilder& pointLightBuilder) = delete;

			/// <summary> Point light builder move assignment operator. </summary>
			/// <param name="pointLightBuilder"> The point light builder to move assign from. </param>
			/// <returns> The reference to the newly move-assigned point light builder. </returns>
			PointLightBuilder& operator=(PointLightBuilder&& pointLightBuilder) = delete;
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
			void SetDefault() override;
			PointLight Build() override
			{
				return PointLight(math::Transform(m_pos, m_rot), m_color, m_intensity, m_shaderId, m_terrainShaderId, m_noShadowShaderId, m_noShadowTerrainShaderId,
					m_attenuation);
			}
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
		private:
			const Attenuation m_defaultPointLightAttenuation; // TODO: Make it static and constexpr

			Attenuation m_attenuation;
			/* ==================== Non-static member variables end ==================== */
		}; /* end class PointLightBuilder */


		/// <summary>
		/// Spot light builder.
		/// </summary>
		class SpotLightBuilder : public LightBuilder<SpotLight>
		{
			/* ==================== Static variables and functions begin ==================== */
			/* ==================== Static variables and functions end ==================== */

			/* ==================== Constructors and destructors begin ==================== */
		public:
			/// <summary> Spot light builder constructor. </summary>
			RENDERING_API SpotLightBuilder();

			/// <summary> Spot light builder destructor. </summary>
			RENDERING_API virtual ~SpotLightBuilder()
			{
			}

			/// <summary> Spot light builder copy constructor. </summary>
			/// <param name="spotLightBuilder"> The spot light builder to copy construct from. </param>
			SpotLightBuilder(SpotLightBuilder& spotLightBuilder) = delete;

			/// <summary> Spot light builder move constructor. </summary>
			/// <param name="spotLightBuilder"> The spot light builder to move construct from. </param>
			SpotLightBuilder(SpotLightBuilder&& spotLightBuilder) = delete;

			/// <summary> Spot light builder copy assignment operator. </summary>
			/// <param name="spotLightBuilder"> The spot light builder to copy assign from. </param>
			/// <returns> The reference to the newly copy-assigned spot light builder. </returns>
			SpotLightBuilder& operator=(const SpotLightBuilder& spotLightBuilder) = delete;

			/// <summary> Spot light builder move assignment operator. </summary>
			/// <param name="spotLightBuilder"> The spot light builder to move assign from. </param>
			/// <returns> The reference to the newly move-assigned spot light builder. </returns>
			SpotLightBuilder& operator=(SpotLightBuilder&& spotLightBuilder) = delete;
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
			void SetDefault() override;
			SpotLight Build() override
			{
				return SpotLight(math::Transform(m_pos, m_rot), m_color, m_intensity, m_shaderId, m_terrainShaderId, m_noShadowShaderId, m_noShadowTerrainShaderId,
					m_attenuation, m_shadowInfoProjectionNearPlane, m_isShadowInfoFlipFacesEnabled, m_shadowInfoShadowMapSizeAsPowerOf2, m_shadowInfoShadowSoftness,
					m_shadowInfoLightBleedingReductionFactor, m_shadowInfoMinVariance, m_viewAngle);
			}
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
		private:
			const Attenuation m_defaultSpotLightAttenuation; // TODO: Make it static constexpr in the future.
			const math::Angle m_defaultSpotLightViewAngle; // TODO: Make it static constexpr in the future.
			const math::Real m_defaultSpotLightShadowInfoProjectionNearPlane; // TODO: Make it static constexpr in the future.

			Attenuation m_attenuation;
			math::Angle m_viewAngle;
			math::Real m_shadowInfoProjectionNearPlane;
			/* ==================== Non-static member variables end ==================== */
		}; /* end class SpotLightBuilder */
	} /* end namespace lighting */
} /* end namespace rendering */

#endif /* __RENDERING_LIGHT_BUILDER_H__ */