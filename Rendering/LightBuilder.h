#ifndef __RENDERING_LIGHT_BUILDER_H__
#define __RENDERING_LIGHT_BUILDER_H__

#include "Rendering.h"

#include "ShaderFactory.h"
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
		LightBuilder(const Rendering::ShaderFactory& shaderFactory, const Math::Vector3D& defaultPosition, const Math::Quaternion& defaultRotation, const Color& defaultColor, Math::Real defaultIntensity) :
			Utility::Builder<T>(),
			m_shaderFactory(shaderFactory),
			M_DEFAULT_POS(defaultPosition),
			M_DEFAULT_ROT(defaultRotation),
			M_DEFAULT_COLOR(defaultColor),
			M_DEFAULT_INTENSITY(defaultIntensity),
			m_pos(M_DEFAULT_POS),
			m_rot(M_DEFAULT_ROT),
			m_color(M_DEFAULT_COLOR),
			m_intensity(M_DEFAULT_INTENSITY),
			m_shader(nullptr),
			m_terrainShader(nullptr),
			m_noShadowShader(nullptr),
			m_noShadowTerrainShader(nullptr)
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
		virtual void BuildParts() override
		{
			BuildLightParams();
			//BuildShadowInfo();
			BuildShaders();
		}

		virtual void SetDefault()
		{
			m_pos = M_DEFAULT_POS;
			m_rot = M_DEFAULT_ROT;
			m_color = M_DEFAULT_COLOR;
			m_intensity = M_DEFAULT_INTENSITY;
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
	protected:
		virtual void BuildLightParams() = 0;
		//virtual void BuildShadowInfo() = 0;
		virtual void BuildShaders() = 0;
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	protected:
		const Rendering::ShaderFactory& m_shaderFactory;

		const Math::Vector3D M_DEFAULT_POS; // TODO: Make it static constexpr in the future.
		const Math::Quaternion M_DEFAULT_ROT; // TODO: Make it static constexpr in the future.
		const Color M_DEFAULT_COLOR; // TODO: Make it static constexpr in the future.
		const Math::Real M_DEFAULT_INTENSITY; // TODO: Make it static constexpr in the future.

		Math::Vector3D m_pos;
		Math::Quaternion m_rot;
		Rendering::Color m_color;
		Math::Real m_intensity;

		const Shader* m_shader;
		const Shader* m_terrainShader;
		const Shader* m_noShadowShader;
		const Shader* m_noShadowTerrainShader;
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
		/// <param name="shaderFactory"> The shader factory. </param>
		RENDERING_API DirectionalLightBuilder(const Rendering::ShaderFactory& shaderFactory);

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
		virtual void BuildLightParams() override;
		virtual void BuildShaders() override;
		virtual void SetDefault() override;
		virtual Lighting::DirectionalLight Get() override
		{
			return Lighting::DirectionalLight(Math::Transform(m_pos, m_rot), m_color, m_intensity, m_shader, m_terrainShader, m_noShadowShader, m_noShadowTerrainShader);
		}
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	private:
		//const Math::Vector3D M_DEFAULT_DIRECTIONAL_LIGHT_POS; // TODO: Make it a static constexpr in the future.
		//const Math::Angle M_DEFAULT_DIRECTIONAL_LIGHT_ROTATION_ANGLE_X; // TODO: Make it a static constexpr in the future.
		//const Math::Angle M_DEFAULT_DIRECTIONAL_LIGHT_ROTATION_ANGLE_Y; // TODO: Make it a static constexpr in the future.
		//const Math::Angle M_DEFAULT_DIRECTIONAL_LIGHT_ROTATION_ANGLE_Z; // TODO: Make it a static constexpr in the future.
		const Math::Real M_DEFAULT_HALF_SHADOW_AREA; // TODO: Make it a static constexpr in the future.
		const int M_DEFAULT_SHADOW_MAP_SIZE_AS_POWER_OF_2; // TODO: Make it a static constexpr in the future.
		const Math::Real M_DEFAULT_SHADOW_SOFTNESS; // TODO: Make it a static constexpr in the future.
		const Math::Real M_DEFAULT_LIGHT_BLEEDING_REDUCTION_FACTOR; // TODO: Make it a static constexpr in the future.
		const Math::Real M_DEFAULT_MIN_VARIANCE; // TODO: Make it a static constexpr in the future.

		Math::Real m_halfShadowArea;
		int m_shadowMapSizeAsPowerOf2;
		Math::Real m_shadowSoftness;
		Math::Real m_lightBleedingReductionFactor;
		Math::Real m_minVariance;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class DirectionalLightBuilder */

	class PointLightBuilder : public LightBuilder<Rendering::Lighting::PointLight>
	{
		/* ==================== Static variables and functions begin ==================== */
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		RENDERING_API PointLightBuilder(const Rendering::ShaderFactory& shaderFactory);
		RENDERING_API virtual ~PointLightBuilder(void) { };
		PointLightBuilder(PointLightBuilder& pointLightBuilder) = delete;
		PointLightBuilder(PointLightBuilder&& pointLightBuilder) = delete;
		PointLightBuilder& operator=(const PointLightBuilder& pointLightBuilder) = delete;
		PointLightBuilder& operator=(PointLightBuilder&& pointLightBuilder) = delete;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		virtual void BuildLightParams() override;
		//virtual void BuildShadowParams() override;
		virtual void BuildShaders() override;
		virtual void SetDefault() override;
		virtual Lighting::PointLight Get() override
		{
			return Lighting::PointLight(Math::Transform(m_pos, m_rot), m_color, m_intensity, m_shader, m_terrainShader, m_noShadowShader, m_noShadowTerrainShader);
		}
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	private:
		const Rendering::Attenuation M_DEFAULT_POINT_LIGHT_ATTENUATION; // TODO: Make it static and constexpr

		Rendering::Attenuation m_attenuation;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class PointLightBuilder */

	class SpotLightBuilder : public LightBuilder<Rendering::Lighting::SpotLight>
	{
		/* ==================== Static variables and functions begin ==================== */
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		RENDERING_API SpotLightBuilder(const Rendering::ShaderFactory& shaderFactory);
		RENDERING_API virtual ~SpotLightBuilder(void) { };
		SpotLightBuilder(SpotLightBuilder& spotLightBuilder) = delete;
		SpotLightBuilder(SpotLightBuilder&& spotLightBuilder) = delete;
		SpotLightBuilder& operator=(const SpotLightBuilder& spotLightBuilder) = delete;
		SpotLightBuilder& operator=(SpotLightBuilder&& spotLightBuilder) = delete;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		virtual void BuildLightParams() override;
		//virtual void BuildShadowParams() override;
		virtual void BuildShaders() override;
		virtual void SetDefault() override;
		virtual Lighting::SpotLight Get() override
		{
			return Lighting::SpotLight(Math::Transform(m_pos, m_rot), m_color, m_intensity, m_shader, m_terrainShader, m_noShadowShader, m_noShadowTerrainShader);
		}
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	private:
		const Rendering::Attenuation M_DEFAULT_SPOT_LIGHT_ATTENUATION; // TODO: Make it static constexpr in the future.
		const Math::Angle M_DEFAULT_SPOT_LIGHT_VIEW_ANGLE; // TODO: Make it static constexpr in the future.
		const int M_DEFAULT_SPOT_LIGHT_SHADOW_MAP_SIZE_AS_POWER_OF_2; // TODO: Make it static constexpr in the future.
		const Math::Real M_DEFAULT_SPOT_LIGHT_PROJECTION_NEAR_PLANE; // TODO: Make it static constexpr in the future.
		const Math::Real M_DEFAULT_SPOT_LIGHT_SHADOW_SOFTNESS; // TODO: Make it static constexpr in the future.
		const Math::Real M_DEFAULT_SPOT_LIGHT_LIGHT_BLEEDING_REDUCTION_FACTOR; // TODO: Make it static constexpr in the future.
		const Math::Real M_DEFAULT_SPOT_LIGHT_MIN_VARIANCE; // TODO: Make it static constexpr in the future.

		Rendering::Attenuation m_attenuation;
		Math::Angle m_viewAngle;
		int m_shadowMapSizeAsPowerOf2;
		Math::Real m_nearPlane;
		Math::Real m_shadowSoftness;
		Math::Real m_lightBleedingReductionFactor;
		Math::Real m_minVariance;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class SpotLightBuilder */
} /* end namespace Rendering */

#endif /* __RENDERING_LIGHT_BUILDER_H__ */