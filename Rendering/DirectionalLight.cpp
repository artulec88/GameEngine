#include "StdAfx.h"
#include "DirectionalLight.h"
#include "CoreEngine.h"
#include "Shader.h"
#include "ShadowInfo.h"
#include "Utility\IConfig.h"

using namespace Rendering::Lighting;

/* static */ bool DirectionalLight::directionalLightsEnabled = true;

DirectionalLight::DirectionalLight(const Rendering::Color& color /* = Color(REAL_ZERO, REAL_ZERO, REAL_ZERO, REAL_ONE) */, Math::Real intensity /* = REAL_ZERO */) :
	BaseLight(color, intensity)
{
}


DirectionalLight::~DirectionalLight(void)
{
}

//void DirectionalLight::InitializeShaders()
//{
//	SetShader(new Shader(GET_CONFIG_VALUE_STR("directionalLightShader", "forward-directional")));
//	ASSERT(m_shader != NULL);
//	if (m_shader == NULL)
//	{
//		LOG(Utility::Critical, LOGPLACE, "Cannot initialize directional light. Shader is NULL.");
//		exit(EXIT_FAILURE);
//	}
//
//	SetTerrainShader(new Shader(GET_CONFIG_VALUE_STR("directionalLightShaderTerrain", "forward-directional-terrain")));
//	ASSERT(m_terrainShader != NULL);
//	if (m_terrainShader == NULL)
//	{
//		LOG(Utility::Critical, LOGPLACE, "Cannot initialize directional light. Terrain shader is NULL.");
//		exit(EXIT_FAILURE);
//	}
//}

bool DirectionalLight::IsEnabled() const
{
	if (!directionalLightsEnabled)
	{
		return false;
	}
	return BaseLight::IsEnabled();
}

Rendering::ShadowCameraTransform DirectionalLight::CalcShadowCameraTransform(const Math::Vector3D& cameraPos, const Math::Quaternion& cameraRot)
{
	// This function in directional light allows the directional light to be casting shadows only in the area around the camera current position
	ShadowCameraTransform shadowCameraTransform;
	shadowCameraTransform.pos = cameraPos + cameraRot.GetForward() * m_halfShadowArea;
	shadowCameraTransform.rot = GetTransform().GetTransformedRot();

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
	Math::Vector3D lightSpaceCameraPos = shadowCameraTransform.pos.Rotate(shadowCameraTransform.rot.Conjugate());

	// Now we need to snap the lightSpaceCameraPos to shadow map texel size increments
	lightSpaceCameraPos.SetX(worldSpaceShadowMapTexelSize * floor(lightSpaceCameraPos.GetX() / worldSpaceShadowMapTexelSize));
	lightSpaceCameraPos.SetY(worldSpaceShadowMapTexelSize * floor(lightSpaceCameraPos.GetY() / worldSpaceShadowMapTexelSize));

	// Now we transform back from the light space into the world space
	shadowCameraTransform.pos = lightSpaceCameraPos.Rotate(shadowCameraTransform.rot);
	/* ==================== Fixing the shimmering effect end ==================== */
	return shadowCameraTransform;
}

#ifdef SIMULATE_SUN_BEHAVIOR

void DirectionalLight::Update(Math::Real delta)
{
	CoreEngine* coreEngine = CoreEngine::GetCoreEngine();
	Math::Real timeOfDay = coreEngine->GetCurrentInGameTime();
	Math::Angle sunElevation = coreEngine->GetSunElevation();
	Math::Angle sunAzimuth = coreEngine->GetSunAzimuth();
	Math::Real daytimeTransitionFactor;
	Rendering::GameTime::Daytime daytime = coreEngine->GetCurrentDaytime(daytimeTransitionFactor);

	m_isEnabled = (daytime != Rendering::GameTime::NIGHT);

	switch (daytime)
	{
	case GameTime::NIGHT:
		m_color = m_sunlightNighttimeColor;
		break;
	case GameTime::BEFORE_DAWN:
		m_color = m_sunlightNighttimeColor.Lerp(m_sunNearHorizonColor, daytimeTransitionFactor);
		m_intensity = daytimeTransitionFactor * m_maxIntensity;
		break;
	case GameTime::SUNRISE:
		m_color = m_sunNearHorizonColor.Lerp(m_sunlightDaytimeColor, daytimeTransitionFactor);
		m_intensity = m_maxIntensity;
		break;
	case GameTime::DAY:
		m_color = m_sunlightDaytimeColor;
		break;
	case GameTime::SUNSET:
		m_color = m_sunNearHorizonColor.Lerp(m_sunlightDaytimeColor, daytimeTransitionFactor);
		m_intensity = m_maxIntensity;
		break;
	case GameTime::AFTER_DUSK:
		m_color = m_sunlightNighttimeColor.Lerp(m_sunNearHorizonColor, daytimeTransitionFactor);
		m_intensity = daytimeTransitionFactor * m_maxIntensity;
		break;
	}
	Math::Matrix4D rotMatrix = Math::Matrix4D::RotationEuler(-sunElevation, -sunAzimuth);
	GetTransform().SetRot(Math::Quaternion(rotMatrix)); // TODO: Use quaternion interpolation to smoothly go from one rotation to another (see LookAtComponent.cpp)
	// e.g. GetTransform().SetRot(GetTransform().GetRot().Slerp(Math::Quaternion(rotMatrix), 0.5f, true));

	//GetTransform().Rotate(GetTransform().GetRot().Slerp(Math::Quaternion(rotMatrix), delta * 5, true));
	//GetTransform().Rotate(Math::Quaternion(rotMatrix));
}

#endif