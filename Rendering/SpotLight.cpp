#include "StdAfx.h"
#include "SpotLight.h"
#include "Shader.h"
#include "ShadowInfo.h"
#include "Utility\IConfig.h"
#include "Math\Matrix.h"

/* static */ bool Rendering::Lighting::SpotLight::spotLightsEnabled = true;

Rendering::Lighting::SpotLight::SpotLight(Math::Transform& transform, const Rendering::Color& color, Math::Real intensity, const Rendering::Attenuation& attenuation,
	const Math::Angle& viewAngle, int shadowMapSizeAsPowerOf2, Math::Real shadowSoftness, Math::Real lightBleedingReductionAmount, Math::Real minVariance,
	const Shader& shader, const Shader& terrainShader, const Shader& noShadowShader, const Shader& noShadowTerrainShader) :
	PointLight(transform, color, intensity, attenuation, shader, terrainShader, noShadowShader, noShadowTerrainShader),
	m_cutoff((viewAngle / 2).Cos())
{
	m_isShadowingEnabled = (shadowMapSizeAsPowerOf2 != 0); // shadowMapSizeAsPowerOf2 == 0 means the light doesn't cast shadows at all
	if (m_isShadowingEnabled)
	{
		Math::Real spotLightProjectionNearPlane = GET_CONFIG_VALUE_RENDERING("spotLightProjectionNearPlane", 0.1f);
		Math::Matrix4D projectionMatrix(viewAngle, REAL_ONE /* because shadow maps are supposed to be squares */, spotLightProjectionNearPlane, m_range);
		SetShadowInfo(projectionMatrix, false, shadowMapSizeAsPowerOf2, shadowSoftness, lightBleedingReductionAmount, minVariance);
		CHECK_CONDITION_EXIT_RENDERING(m_shadowInfo != NULL, Utility::Logging::CRITICAL, "Cannot initialize spot light. Shadow info is NULL.");
	}
}


Rendering::Lighting::SpotLight::~SpotLight(void)
{
}

bool Rendering::Lighting::SpotLight::IsEnabled() const
{
	if (!spotLightsEnabled)
	{
		return false;
	}
	return BaseLight::IsEnabled();
}