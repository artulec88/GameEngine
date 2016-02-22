#include "StdAfx.h"
#include "DirectionalLight.h"
//#include "CoreEngine.h"
#include "Shader.h"
#include "ShadowInfo.h"
#include "Utility\IConfig.h"

using namespace Rendering::Lighting;

/* static */ bool DirectionalLight::directionalLightsEnabled = true;

DirectionalLight::DirectionalLight(const Rendering::Color& color /* = Color(REAL_ONE, REAL_ONE, REAL_ONE, REAL_ONE) */,
	Math::Real intensity /* = REAL_ONE */,
	Math::Real halfShadowArea /* = REAL_ONE */,
	int shadowMapSizeAsPowerOf2 /* = 0 */,
	Math::Real shadowSoftness /* = REAL_ONE */,
	Math::Real lightBleedingReductionAmount /* = static_cast<Math::Real>(0.2f) */,
	Math::Real minVariance /* = static_cast<Math::Real>(0.00002f) */) :
	BaseLightComponent(color, intensity),
	m_halfShadowArea(halfShadowArea)
{
	if ((shadowMapSizeAsPowerOf2 != 0) /* shadowMapSizeAsPowerOf2 == 0 means the light doesn't cast shadows */)
	{
		Math::Matrix4D ortographicProjectionMatrix(-halfShadowArea, halfShadowArea, -halfShadowArea, halfShadowArea, -halfShadowArea, halfShadowArea);
		SetShadowInfo(new ShadowInfo(ortographicProjectionMatrix, true, shadowMapSizeAsPowerOf2, shadowSoftness, lightBleedingReductionAmount, minVariance));
		CHECK_CONDITION_EXIT(m_shadowInfo != NULL, Utility::Critical, "Cannot initialize directional light. Shadow info is NULL.");
	}
}


DirectionalLight::~DirectionalLight(void)
{
}

//void DirectionalLight::InitializeShaders()
//{
//	SetShader(new Shader(GET_CONFIG_VALUE_STR("directionalLightShader", "forward-directional")));
//	CHECK_CONDITION_EXIT(m_shader != NULL, Utility::Critical, "Cannot initialize directional light. Shader is NULL.");
//
//	SetTerrainShader(new Shader(GET_CONFIG_VALUE_STR("directionalLightShaderTerrain", "forward-directional-terrain")));
//	CHECK_CONDITION_EXIT(m_terrainShader != NULL, Utility::Critical, "Cannot initialize directional light. Terrain shader is NULL.");
//}

bool DirectionalLight::IsEnabled() const
{
	if (!directionalLightsEnabled)
	{
		return false;
	}
	return BaseLightComponent::IsEnabled();
}

Rendering::ShadowCameraTransform DirectionalLight::CalcShadowCameraTransform(const Math::Vector3D& cameraPos, const Math::Quaternion& cameraRot)
{
	// This function in directional light allows the directional light to be casting shadows only in the area around the camera current position
	ShadowCameraTransform shadowCameraTransform(cameraPos + cameraRot.GetForward() * m_halfShadowArea, GetTransform().GetTransformedRot());

	/**
	 * The reoccurring shimmering is caused by the moving shadow camera by the value less than
	 * the shadow map texel size. If I move by, let's say, half the texel size, render the scene
	 * and generate the shadow map again, the objects aren't going to map to exactly the same texels
	 * in the shadow map. This causes approximation to be calculated slightly differently each frame.
	 * To fix the shimmering effect we have to make sure we only move by the multiple of the texel size.
	 */
	/* ==================== Fixing the shimmering effect begin ==================== */
	Math::Real shadowMapSize = static_cast<Math::Real>(1 << GetShadowInfo()->GetShadowMapSizeAsPowerOf2());
	Math::Real worldSpaceShadowMapTexelSize = (m_halfShadowArea * 2.0f) / shadowMapSize;
	// Now we transform from the world space into the light space
	Math::Vector3D lightSpaceCameraPos(shadowCameraTransform.m_pos.Rotate(shadowCameraTransform.m_rot.Conjugate()));

	// Now we need to snap the lightSpaceCameraPos to shadow map texel size increments
	lightSpaceCameraPos.SetX(worldSpaceShadowMapTexelSize * floor(lightSpaceCameraPos.GetX() / worldSpaceShadowMapTexelSize));
	lightSpaceCameraPos.SetY(worldSpaceShadowMapTexelSize * floor(lightSpaceCameraPos.GetY() / worldSpaceShadowMapTexelSize));

	// Now we transform back from the light space into the world space
	shadowCameraTransform.m_pos = lightSpaceCameraPos.Rotate(shadowCameraTransform.m_rot);
	/* ==================== Fixing the shimmering effect end ==================== */
	return shadowCameraTransform;
}

std::string DirectionalLight::ToString() const
{
	std::stringstream ss("");
	ss << "(Intensity=" << m_intensity << "; Color=" << m_color.ToString() << "; Direction=" << GetTransform().GetTransformedRot().GetForward().ToString() << ")";
	return ss.str();
}

#ifdef SIMULATE_SUN_BEHAVIOR
void DirectionalLight::Update(Math::Real delta)
{
	DELOCUST_LOG("Directional light update with delta time = %.5f", delta);

	//Core::CoreEngine* coreEngine = Core::CoreEngine::GetCoreEngine();
	//Math::Real timeOfDay = coreEngine->GetCurrentInGameTime();
	//Math::Angle sunElevation = coreEngine->GetSunElevation();
	//Math::Angle sunAzimuth = coreEngine->GetSunAzimuth();
	//Math::Real daytimeTransitionFactor;
	//Rendering::GameTime::Daytime daytime = coreEngine->GetCurrentDaytime(daytimeTransitionFactor);

	//m_isEnabled = (daytime != Rendering::GameTime::NIGHT);

	//switch (daytime)
	//{
	//case GameTime::NIGHT:
	//	m_color = m_sunlightNighttimeColor;
	//	break;
	//case GameTime::BEFORE_DAWN:
	//	m_color = m_sunlightNighttimeColor.Lerp(m_sunNearHorizonColor, daytimeTransitionFactor);
	//	m_intensity = daytimeTransitionFactor * m_maxIntensity;
	//	break;
	//case GameTime::SUNRISE:
	//	m_color = m_sunNearHorizonColor.Lerp(m_sunlightDaytimeColor, daytimeTransitionFactor);
	//	m_intensity = m_maxIntensity;
	//	break;
	//case GameTime::DAY:
	//	m_color = m_sunlightDaytimeColor;
	//	break;
	//case GameTime::SUNSET:
	//	m_color = m_sunNearHorizonColor.Lerp(m_sunlightDaytimeColor, daytimeTransitionFactor);
	//	m_intensity = m_maxIntensity;
	//	break;
	//case GameTime::AFTER_DUSK:
	//	m_color = m_sunlightNighttimeColor.Lerp(m_sunNearHorizonColor, daytimeTransitionFactor);
	//	m_intensity = daytimeTransitionFactor * m_maxIntensity;
	//	break;
	//}
	//Math::Matrix4D rotMatrix(-sunElevation, -sunAzimuth);
	//GetTransform().SetRot(Math::Quaternion(rotMatrix)); // TODO: Use quaternion interpolation to smoothly go from one rotation to another (see LookAtComponent.cpp)
}

#endif