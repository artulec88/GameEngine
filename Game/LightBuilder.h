#ifndef __GAME_LIGHT_BUILDER_H__
#define __GAME_LIGHT_BUILDER_H__

#include "Def.h"

#include "Engine\ShaderFactory.h"
#include "Engine\TextureFactory.h"

#include "Rendering\DirectionalLight.h"
#include "Rendering\PointLight.h"
#include "Rendering\SpotLight.h"

#include "Utility\Builder.h"
#include "Utility\Builder_impl.h"

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
		explicit LightBuilder(const Engine::ShaderFactory& shaderFactory, const Engine::TextureFactory& textureFactory);
		virtual ~LightBuilder(void);

		LightBuilder(const LightBuilder& lightBuilder) = delete;
		LightBuilder(LightBuilder&& lightBuilder) = delete;
		LightBuilder& operator=(const LightBuilder& lightBuilder) = delete;
		LightBuilder& operator=(LightBuilder&& lightBuilder) = delete;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		virtual void BuildPart1();
		virtual void BuildPart2();
		void SetLightIndex(int lightIndex);
	protected:
		virtual void SetupLightShaders() = 0;
		virtual void SetupLightParams() = 0;
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	protected:
		int m_lightIndex;
		std::string m_lightIndexStr;
		const Engine::ShaderFactory& m_shaderFactory;
		const Engine::TextureFactory& m_textureFactory;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class LightBuilder<T> */

	class DirectionalLightBuilder : public LightBuilder<Rendering::Lighting::DirectionalLight>
	{
		/* ==================== Static variables and functions begin ==================== */
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		explicit DirectionalLightBuilder(const Engine::ShaderFactory& shaderFactory, const Engine::TextureFactory& textureFactory) :
			LightBuilder(shaderFactory, textureFactory)
		{
		}
		virtual ~DirectionalLightBuilder(void)
		{
		}
		DirectionalLightBuilder(DirectionalLightBuilder& directionalLightBuilder) = delete;
		DirectionalLightBuilder(DirectionalLightBuilder&& directionalLightBuilder) = delete;
		DirectionalLightBuilder& operator=(const DirectionalLightBuilder& directionalLightBuilder) = delete;
		DirectionalLightBuilder& operator=(DirectionalLightBuilder&& directionalLightBuilder) = delete;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		virtual void SetupLightShaders();
		virtual void SetupLightParams();
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
		/* ==================== Non-static member variables end ==================== */
	}; /* end class DirectionalLightBuilder */

	class PointLightBuilder : public LightBuilder<Rendering::Lighting::PointLight>
	{
		/* ==================== Static variables and functions begin ==================== */
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		explicit PointLightBuilder(const Engine::ShaderFactory& shaderFactory, const Engine::TextureFactory& textureFactory);
		virtual ~PointLightBuilder(void) { };
		PointLightBuilder(PointLightBuilder& pointLightBuilder) = delete;
		PointLightBuilder(PointLightBuilder&& pointLightBuilder) = delete;
		PointLightBuilder& operator=(const PointLightBuilder& pointLightBuilder) = delete;
		PointLightBuilder& operator=(PointLightBuilder&& pointLightBuilder) = delete;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		virtual void SetupLightShaders();
		virtual void SetupLightParams();
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
		/* ==================== Non-static member variables end ==================== */
	}; /* end class PointLightBuilder */

	class SpotLightBuilder : public LightBuilder<Rendering::Lighting::SpotLight>
	{
		/* ==================== Static variables and functions begin ==================== */
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		explicit SpotLightBuilder(const Engine::ShaderFactory& shaderFactory, const Engine::TextureFactory& textureFactory);
		virtual ~SpotLightBuilder(void) { };
		SpotLightBuilder(SpotLightBuilder& spotLightBuilder) = delete;
		SpotLightBuilder(SpotLightBuilder&& spotLightBuilder) = delete;
		SpotLightBuilder& operator=(const SpotLightBuilder& spotLightBuilder) = delete;
		SpotLightBuilder& operator=(SpotLightBuilder&& spotLightBuilder) = delete;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		virtual void SetupLightShaders();
		virtual void SetupLightParams();
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
		/* ==================== Non-static member variables end ==================== */
	}; /* end class SpotLightBuilder */
} /* end namespace Game */

#endif /* __GAME_LIGHT_BUILDER_H__ */