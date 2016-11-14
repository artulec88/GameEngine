#ifndef __RENDERING_LIGHT_BUILDER_H__
#define __RENDERING_LIGHT_BUILDER_H__

#include "Rendering.h"

#include "ShaderIDs.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"

#include "Utility\Builder.h"
#include "Utility\IConfig.h"

#include <string>

namespace Rendering
{
	template <class T>
	class LightBuilder : public Utility::Builder<T>
	{
		/* ==================== Static variables and functions begin ==================== */
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		LightBuilder(const Math::Vector3D& defaultPosition, const Math::Quaternion& defaultRotation, const Color& defaultColor, Math::Real defaultIntensity,
			bool defaultShadowInfoFlipFacesEnabled, int defaultShadowInfoShadowMapSizeAsPowerOf2, Math::Real defaultShadowInfoShadowSoftness, Math::Real defaultShadowInfoLightBleedingReductionFactor,
			Math::Real defaultShadowInfoMinVariance) :
			Utility::Builder<T>(),
			M_DEFAULT_POS(defaultPosition),
			M_DEFAULT_ROT(defaultRotation),
			M_DEFAULT_COLOR(defaultColor),
			M_DEFAULT_INTENSITY(defaultIntensity),
			M_DEFAULT_SHADOW_INFO_FLIP_FACES_ENABLED(defaultShadowInfoFlipFacesEnabled),
			M_DEFAULT_SHADOW_INFO_SHADOW_MAP_SIZE_AS_POWER_OF_2(defaultShadowInfoShadowMapSizeAsPowerOf2),
			M_DEFAULT_SHADOW_INFO_SHADOW_SOFTNESS(defaultShadowInfoShadowSoftness),
			M_DEFAULT_SHADOW_INFO_LIGHT_BLEEDING_REDUCTION_FACTOR(defaultShadowInfoLightBleedingReductionFactor),
			M_DEFAULT_SHADOW_INFO_MIN_VARIANCE(defaultShadowInfoMinVariance),
			m_pos(M_DEFAULT_POS),
			m_rot(M_DEFAULT_ROT),
			m_color(M_DEFAULT_COLOR),
			m_intensity(M_DEFAULT_INTENSITY),
			m_isShadowInfoFlipFacesEnabled(M_DEFAULT_SHADOW_INFO_FLIP_FACES_ENABLED),
			m_shadowInfoShadowMapSizeAsPowerOf2(M_DEFAULT_SHADOW_INFO_SHADOW_MAP_SIZE_AS_POWER_OF_2),
			m_shadowInfoShadowSoftness(M_DEFAULT_SHADOW_INFO_SHADOW_SOFTNESS),
			m_shadowInfoLightBleedingReductionFactor(M_DEFAULT_SHADOW_INFO_LIGHT_BLEEDING_REDUCTION_FACTOR),
			m_shadowInfoMinVariance(M_DEFAULT_SHADOW_INFO_MIN_VARIANCE),
			m_shaderID(ShaderIDs::INVALID),
			m_terrainShaderID(ShaderIDs::INVALID),
			m_noShadowShaderID(ShaderIDs::INVALID),
			m_noShadowTerrainShaderID(ShaderIDs::INVALID)
		{
		}
		virtual ~LightBuilder(void)
		{
		}

		LightBuilder(const LightBuilder& lightBuilder) = delete;
		LightBuilder(LightBuilder&& lightBuilder) = delete;
		LightBuilder& operator=(const LightBuilder& lightBuilder) = delete;
		LightBuilder& operator=(LightBuilder&& lightBuilder) = delete;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		virtual void Build() override
		{
			BuildShaders();
		}

		virtual void SetDefault()
		{
			m_pos = M_DEFAULT_POS;
			m_rot = M_DEFAULT_ROT;
			m_color = M_DEFAULT_COLOR;
			m_intensity = M_DEFAULT_INTENSITY;
			m_isShadowInfoFlipFacesEnabled = M_DEFAULT_SHADOW_INFO_FLIP_FACES_ENABLED;
			m_shadowInfoShadowMapSizeAsPowerOf2 = M_DEFAULT_SHADOW_INFO_SHADOW_MAP_SIZE_AS_POWER_OF_2;
			m_shadowInfoShadowSoftness = M_DEFAULT_SHADOW_INFO_SHADOW_SOFTNESS;
			m_shadowInfoLightBleedingReductionFactor = M_DEFAULT_SHADOW_INFO_LIGHT_BLEEDING_REDUCTION_FACTOR;
			m_shadowInfoMinVariance = M_DEFAULT_SHADOW_INFO_MIN_VARIANCE;
		}

		LightBuilder<T>& SetPosition(const Math::Vector3D& position)
		{
			return SetPosition(position.GetX(), position.GetY(), position.GetZ());
		}
		LightBuilder<T>& SetPosition(Math::Real posX, Math::Real posY, Math::Real posZ)
		{
			m_pos.Set(posX, posY, posZ);
			return *this;
		}
		LightBuilder<T>& SetColor(ColorNames::ColorName colorName)
		{
			m_color = Color(colorName);
			return *this;
		}
		LightBuilder<T>& SetColor(Math::Real red, Math::Real green, Math::Real blue, Math::Real alpha = REAL_ONE)
		{
			m_color.Set(red, green, blue, alpha);
			return *this;
		}
		LightBuilder<T>& SetColor(const Color& color)
		{
			m_color = color;
			return *this;
		}
		LightBuilder<T>& SetIntensity(Math::Real intensity)
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
		LightBuilder<T>& SetShadowInfoShadowSoftness(Math::Real shadowInfoShadowSoftness)
		{
			m_shadowInfoShadowSoftness = shadowInfoShadowSoftness;
			return *this;
		}
		LightBuilder<T>& SetShadowInfoLightBleedingReductionFactor(Math::Real shadowInfoLightBleedingReductionFactor)
		{
			m_shadowInfoLightBleedingReductionFactor = shadowInfoLightBleedingReductionFactor;
			return *this;
		}
		LightBuilder<T>& SetShadowInfoMinVariance(Math::Real shadowInfoMinVariance)
		{
			m_shadowInfoMinVariance = shadowInfoMinVariance;
			return *this;
		}
	protected:
		virtual void BuildShaders() = 0;
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	protected:
		const Math::Vector3D M_DEFAULT_POS; // TODO: Make it static constexpr in the future.
		const Math::Quaternion M_DEFAULT_ROT; // TODO: Make it static constexpr in the future.
		const Color M_DEFAULT_COLOR; // TODO: Make it static constexpr in the future.
		const Math::Real M_DEFAULT_INTENSITY; // TODO: Make it static constexpr in the future.
		const bool M_DEFAULT_SHADOW_INFO_FLIP_FACES_ENABLED; // TODO: Make it static constexpr in the future.
		const int M_DEFAULT_SHADOW_INFO_SHADOW_MAP_SIZE_AS_POWER_OF_2; // TODO: Make it static constexpr in the future.
		const Math::Real M_DEFAULT_SHADOW_INFO_SHADOW_SOFTNESS; // TODO: Make it static constexpr in the future.
		const Math::Real M_DEFAULT_SHADOW_INFO_LIGHT_BLEEDING_REDUCTION_FACTOR; // TODO: Make it static constexpr in the future.
		const Math::Real M_DEFAULT_SHADOW_INFO_MIN_VARIANCE; // TODO: Make it static constexpr in the future.

		Math::Vector3D m_pos;
		Math::Quaternion m_rot;
		Rendering::Color m_color;
		Math::Real m_intensity;

		bool m_isShadowInfoFlipFacesEnabled;
		int m_shadowInfoShadowMapSizeAsPowerOf2;
		Math::Real m_shadowInfoShadowSoftness;
		Math::Real m_shadowInfoLightBleedingReductionFactor;
		Math::Real m_shadowInfoMinVariance;

		int m_shaderID;
		int m_terrainShaderID;
		int m_noShadowShaderID;
		int m_noShadowTerrainShaderID;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class LightBuilder<T> */

	/// <summary>
	/// Directional light builder.
	/// </summary>
	class DirectionalLightBuilder : public LightBuilder<Rendering::Lighting::DirectionalLight>
	{
		/* ==================== Static variables and functions begin ==================== */
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		/// <summary> Directional light builder constructor. </summary>
		RENDERING_API DirectionalLightBuilder();

		/// <summary> Directional light builder destructor. </summary>
		RENDERING_API virtual ~DirectionalLightBuilder(void);

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
		virtual void BuildShaders() override;
		virtual void SetDefault() override;
		virtual Lighting::DirectionalLight Get() override
		{
			return Lighting::DirectionalLight(Math::Transform(m_pos, m_rot), m_color, m_intensity, m_shaderID, m_terrainShaderID, m_noShadowShaderID, m_noShadowTerrainShaderID,
				m_isShadowInfoFlipFacesEnabled, m_shadowInfoShadowMapSizeAsPowerOf2, m_shadowInfoShadowSoftness, m_shadowInfoLightBleedingReductionFactor,
				m_shadowInfoMinVariance, m_halfShadowArea);
		}
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	private:
		const Math::Real M_DEFAULT_HALF_SHADOW_AREA; // TODO: Make it a static constexpr in the future.

		Math::Real m_halfShadowArea;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class DirectionalLightBuilder */


	/// <summary>
	/// Point light builder.
	/// </summary>
	class PointLightBuilder : public LightBuilder<Rendering::Lighting::PointLight>
	{
		/* ==================== Static variables and functions begin ==================== */
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		/// <summary> Point light builder constructor. </summary>
		RENDERING_API PointLightBuilder();

		/// <summary> Point light builder destructor. </summary>
		RENDERING_API virtual ~PointLightBuilder(void)
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
	public:
		virtual void BuildShaders() override;
		virtual void SetDefault() override;
		virtual Lighting::PointLight Get() override
		{
			return Lighting::PointLight(Math::Transform(m_pos, m_rot), m_color, m_intensity, m_shaderID, m_terrainShaderID, m_noShadowShaderID, m_noShadowTerrainShaderID,
				m_attenuation);
		}
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	private:
		const Rendering::Attenuation M_DEFAULT_POINT_LIGHT_ATTENUATION; // TODO: Make it static and constexpr

		Rendering::Attenuation m_attenuation;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class PointLightBuilder */


	/// <summary>
	/// Spot light builder.
	/// </summary>
	class SpotLightBuilder : public LightBuilder<Rendering::Lighting::SpotLight>
	{
		/* ==================== Static variables and functions begin ==================== */
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		/// <summary> Spot light builder constructor. </summary>
		RENDERING_API SpotLightBuilder();

		/// <summary> Spot light builder destructor. </summary>
		RENDERING_API virtual ~SpotLightBuilder(void)
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
	public:
		virtual void BuildShaders() override;
		virtual void SetDefault() override;
		virtual Lighting::SpotLight Get() override
		{
			return Lighting::SpotLight(Math::Transform(m_pos, m_rot), m_color, m_intensity, m_shaderID, m_terrainShaderID, m_noShadowShaderID, m_noShadowTerrainShaderID,
				m_attenuation, m_shadowInfoProjectionNearPlane, m_isShadowInfoFlipFacesEnabled, m_shadowInfoShadowMapSizeAsPowerOf2, m_shadowInfoShadowSoftness,
				m_shadowInfoLightBleedingReductionFactor, m_shadowInfoMinVariance, m_viewAngle);
		}
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	private:
		const Rendering::Attenuation M_DEFAULT_SPOT_LIGHT_ATTENUATION; // TODO: Make it static constexpr in the future.
		const Math::Angle M_DEFAULT_SPOT_LIGHT_VIEW_ANGLE; // TODO: Make it static constexpr in the future.
		const Math::Real M_DEFAULT_SPOT_LIGHT_SHADOW_INFO_PROJECTION_NEAR_PLANE; // TODO: Make it static constexpr in the future.

		Rendering::Attenuation m_attenuation;
		Math::Angle m_viewAngle;
		Math::Real m_shadowInfoProjectionNearPlane;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class SpotLightBuilder */
} /* end namespace Rendering */

#endif /* __RENDERING_LIGHT_BUILDER_H__ */