#include "StdAfx.h"
#include "LightBuilder.h"
#include "CoreEngine.h"
#include "Shader.h"
#include "Utility\IConfig.h"

using namespace Rendering;

LightBuilder::LightBuilder(void)
{
}


LightBuilder::~LightBuilder(void)
{
}

/* ==================== DirectionalLightBuilder implementation begin ==================== */
void DirectionalLightBuilder::BuildPart1()
{
	const Color defaultDirectionalLightColor(GET_CONFIG_VALUE("defaultDirectionalLightColorRed", REAL_ONE), GET_CONFIG_VALUE("defaultDirectionalLightColorGreen", REAL_ONE), GET_CONFIG_VALUE("defaultDirectionalLightColorBlue", REAL_ONE));
	const Math::Real defaultDirectionalLightIntensity(GET_CONFIG_VALUE("defaultDirectionalLightIntensity", REAL_ONE));

	Math::Real red = GET_CONFIG_VALUE("directionalLightColorRed", defaultDirectionalLightColor.GetRed());
	Math::Real green = GET_CONFIG_VALUE("directionalLightColorGreen", defaultDirectionalLightColor.GetGreen());
	Math::Real blue = GET_CONFIG_VALUE("directionalLightColorBlue", defaultDirectionalLightColor.GetBlue());
	Color color(red, green, blue);

	Math::Real intensity = GET_CONFIG_VALUE("directionalLightIntensity", defaultDirectionalLightIntensity);

	m_directionalLight = new DirectionalLight(color, intensity);
}

void DirectionalLightBuilder::BuildPart2()
{
	// Setting shaders
	m_directionalLight->SetShader(new Shader(GET_CONFIG_VALUE_STR("directionalLightShader", "forward-directional")));
	m_directionalLight->SetTerrainShader(new Shader(GET_CONFIG_VALUE_STR("directionalLightShaderTerrain", "forward-directional-terrain")));

	// Setting shadow info
	const Math::Real defaultDirectionalLightShadowArea = GET_CONFIG_VALUE("defaultDirectionalLightShadowArea", 80.0f);
	const int defaultDirectionalLightShadowMapSizeAsPowerOf2 = GET_CONFIG_VALUE("defaultDirectionalLightShadowMapSizeAsPowerOf2", 10);
	const Math::Real defaultDirectionalLightShadowSoftness = GET_CONFIG_VALUE("defaultDirectionalLightShadowSoftness", 1.0f);
	const Math::Real defaultDirectionalLightLightBleedingReductionAmount = GET_CONFIG_VALUE("defaultDirectionalLightLightBleedingReductionAmount", 0.2f);
	const Math::Real defaultDirectionalLightMinVariance = GET_CONFIG_VALUE("defaultDirectionalLightMinVariance", 0.00002f);

	Math::Real halfShadowArea = GET_CONFIG_VALUE("directionalLightShadowArea", defaultDirectionalLightShadowArea) / 2.0f;
	int shadowMapSizeAsPowerOf2 = GET_CONFIG_VALUE("directionalLightShadowMapSizeAsPowerOf2", defaultDirectionalLightShadowMapSizeAsPowerOf2);
	Math::Real shadowSoftness = GET_CONFIG_VALUE("directionalLightShadowSoftness", defaultDirectionalLightShadowSoftness);
	Math::Real lightBleedingReductionAmount = GET_CONFIG_VALUE("directionalLightLightBleedingReductionAmount", defaultDirectionalLightLightBleedingReductionAmount);
	Math::Real minVariance = GET_CONFIG_VALUE("directionalLightMinVariance", defaultDirectionalLightMinVariance);
	bool isShadowingEnabled = (shadowMapSizeAsPowerOf2 != 0); // shadowMapSizeAsPowerOf2 == 0 means the light doesn't cast shadows
	if (isShadowingEnabled)
	{
		Math::Matrix4D ortoMatrix = Math::Matrix4D::OrtographicProjection(-halfShadowArea, halfShadowArea, -halfShadowArea, halfShadowArea, -halfShadowArea, halfShadowArea);
		m_directionalLight->SetShadowInfo(new ShadowInfo(ortoMatrix, true, shadowMapSizeAsPowerOf2, shadowSoftness, lightBleedingReductionAmount, minVariance));
	}

	// Setting additional directional light information
	m_directionalLight->SetMaxIntensity(m_directionalLight->GetIntensity());
	m_directionalLight->SetSunlightDaytimeColor(m_directionalLight->GetColor());
	m_directionalLight->SetSunNearHorizonColor(Color(GET_CONFIG_VALUE("directionalLightNearHorizonColorRed", REAL_ONE), GET_CONFIG_VALUE("directionalLightNearHorizonColorGreen", 0.2f), GET_CONFIG_VALUE("directionalLightNearHorizonColorBlue", REAL_ZERO)));
	m_directionalLight->SetSunlightNighttimeColor(Color(GET_CONFIG_VALUE("directionalLightNighttimeColorRed", REAL_ZERO), GET_CONFIG_VALUE("directionalLightNighttimeColorGreen", REAL_ZERO), GET_CONFIG_VALUE("directionalLightNighttimeColorBlue", REAL_ZERO)));
}

void DirectionalLightBuilder::AddToEngine()
{
	CoreEngine::GetCoreEngine()->GetRenderer()->AddLight(m_directionalLight);
}
/* ==================== DirectionalLightBuilder implementation end ==================== */

/* ==================== PointLightBuilder implementation begin ==================== */
void PointLightBuilder::BuildPart1()
{
}

void PointLightBuilder::BuildPart2()
{
}

void PointLightBuilder::AddToEngine()
{
	CoreEngine::GetCoreEngine()->GetRenderer()->AddLight(m_pointLight);
}
/* ==================== PointLightBuilder implementation end ==================== */

/* ==================== SpotLightBuilder implementation begin ==================== */
void SpotLightBuilder::BuildPart1()
{
}

void SpotLightBuilder::BuildPart2()
{
}

void SpotLightBuilder::AddToEngine()
{
	CoreEngine::GetCoreEngine()->GetRenderer()->AddLight(m_spotLight);
}
/* ==================== SpotLightBuilder implementation end ==================== */
