#ifndef __GAME_LIGHT_BUILDER_H__
#define __GAME_LIGHT_BUILDER_H__

#include "Def.h"

#include "Engine\GameManager.h"

#include "Rendering\ShaderFactory.h"
#include "Rendering\DirectionalLight.h"
#include "Rendering\PointLight.h"
#include "Rendering\SpotLight.h"

#include "Utility\Builder.h"

#include <string>

namespace Game
{
	template <class T>
	class LightBuilder : public Utility::Builder<T>
	{
		/* ==================== Static variables and functions begin ==================== */
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		LightBuilder(Engine::GameManager* gameManager, const Rendering::ShaderFactory& shaderFactory, T* light) :
			Utility::Builder<T>(light),
			m_lightIndex(0),
			m_lightIndexStr("0"),
			m_gameManager(gameManager),
			m_shaderFactory(shaderFactory),
			m_pos(REAL_ZERO, REAL_ZERO, REAL_ZERO),
			m_rot(),
			m_color(Rendering::ColorNames::WHITE),
			m_intensity(REAL_ZERO)
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
		virtual void BuildPart1() = 0;
		virtual void BuildPart2() = 0;
		virtual void BuildPart3() = 0;
		void SetLightIndex(int lightIndex)
		{
			m_lightIndex = lightIndex;
			m_lightIndexStr = std::to_string(lightIndex);
		}
	protected:
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	protected:
		int m_lightIndex;
		std::string m_lightIndexStr;
		Engine::GameManager* m_gameManager;
		const Rendering::ShaderFactory& m_shaderFactory;

		Math::Vector3D m_pos;
		Math::Quaternion m_rot;
		Rendering::Color m_color;
		Math::Real m_intensity;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class LightBuilder<T> */

	class DirectionalLightBuilder : public LightBuilder<Rendering::Lighting::DirectionalLight>
	{
		/* ==================== Static variables and functions begin ==================== */
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		DirectionalLightBuilder(Engine::GameManager* gameManager, const Rendering::ShaderFactory& shaderFactory,
			Rendering::Lighting::DirectionalLight* directionalLight);
		virtual ~DirectionalLightBuilder(void);
		DirectionalLightBuilder(DirectionalLightBuilder& directionalLightBuilder) = delete;
		DirectionalLightBuilder(DirectionalLightBuilder&& directionalLightBuilder) = delete;
		DirectionalLightBuilder& operator=(const DirectionalLightBuilder& directionalLightBuilder) = delete;
		DirectionalLightBuilder& operator=(DirectionalLightBuilder&& directionalLightBuilder) = delete;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		virtual void BuildPart1();
		virtual void BuildPart2();
		virtual void BuildPart3();
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	private:
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
		PointLightBuilder(Engine::GameManager* gameManager, const Rendering::ShaderFactory& shaderFactory,
			Rendering::Lighting::PointLight* pointLight);
		virtual ~PointLightBuilder(void) { };
		PointLightBuilder(PointLightBuilder& pointLightBuilder) = delete;
		PointLightBuilder(PointLightBuilder&& pointLightBuilder) = delete;
		PointLightBuilder& operator=(const PointLightBuilder& pointLightBuilder) = delete;
		PointLightBuilder& operator=(PointLightBuilder&& pointLightBuilder) = delete;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		virtual void BuildPart1();
		virtual void BuildPart2();
		virtual void BuildPart3();
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	private:
		const Math::Vector3D M_DEFAULT_POINT_LIGHT_POS;
		const Math::Angle M_DEFAULT_POINT_LIGHT_ROTATION_ANGLE_X;
		const Math::Angle M_DEFAULT_POINT_LIGHT_ROTATION_ANGLE_Y;
		const Math::Angle M_DEFAULT_POINT_LIGHT_ROTATION_ANGLE_Z;
		const Rendering::Color M_DEFAULT_POINT_LIGHT_COLOR;
		const Math::Real M_DEFAULT_POINT_LIGHT_INTENSITY;
		const Rendering::Attenuation M_DEFAULT_POINT_LIGHT_ATTENUATION;

		Rendering::Attenuation m_attenuation;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class PointLightBuilder */

	class SpotLightBuilder : public LightBuilder<Rendering::Lighting::SpotLight>
	{
		/* ==================== Static variables and functions begin ==================== */
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		SpotLightBuilder(Engine::GameManager* gameManager, const Rendering::ShaderFactory& shaderFactory,
			Rendering::Lighting::SpotLight* spotLight);
		virtual ~SpotLightBuilder(void) { };
		SpotLightBuilder(SpotLightBuilder& spotLightBuilder) = delete;
		SpotLightBuilder(SpotLightBuilder&& spotLightBuilder) = delete;
		SpotLightBuilder& operator=(const SpotLightBuilder& spotLightBuilder) = delete;
		SpotLightBuilder& operator=(SpotLightBuilder&& spotLightBuilder) = delete;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		virtual void BuildPart1();
		virtual void BuildPart2();
		virtual void BuildPart3();
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	private:
		const Math::Vector3D M_DEFAULT_SPOT_LIGHT_POS;
		const Math::Angle M_DEFAULT_SPOT_LIGHT_ROTATION_ANGLE_X;
		const Math::Angle M_DEFAULT_SPOT_LIGHT_ROTATION_ANGLE_Y;
		const Math::Angle M_DEFAULT_SPOT_LIGHT_ROTATION_ANGLE_Z;
		const Rendering::Color M_DEFAULT_SPOT_LIGHT_COLOR;
		const Math::Real M_DEFAULT_SPOT_LIGHT_INTENSITY;
		const Rendering::Attenuation M_DEFAULT_SPOT_LIGHT_ATTENUATION;
		const Math::Angle M_DEFAULT_SPOT_LIGHT_VIEW_ANGLE;
		const int M_DEFAULT_SPOT_LIGHT_SHADOW_MAP_SIZE_AS_POWER_OF_2;
		const Math::Real M_DEFAULT_SPOT_LIGHT_SHADOW_SOFTNESS;
		const Math::Real M_DEFAULT_SPOT_LIGHT_LIGHT_BLEEDING_REDUCTION_AMOUNT;
		const Math::Real M_DEFAULT_SPOT_LIGHT_MIN_VARIANCE;

		Rendering::Attenuation m_attenuation;
		Math::Angle m_viewAngle;
		int m_shadowMapSizeAsPowerOf2;
		Math::Real m_nearPlane;
		Math::Real m_shadowSoftness;
		Math::Real m_lightBleedingReductionFactor;
		Math::Real m_minVariance;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class SpotLightBuilder */
} /* end namespace Game */

#endif /* __GAME_LIGHT_BUILDER_H__ */