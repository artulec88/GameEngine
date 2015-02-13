#include "StdAfx.h"
#include "DirectionalLight.h"
#include "CoreEngine.h"
#include "Shader.h"
#include "ShadowInfo.h"
#include "Utility\IConfig.h"

using namespace Rendering;

/* static */ bool DirectionalLight::directionalLightsEnabled = true;

DirectionalLight::DirectionalLight(const Color& color /* = Color(REAL_ZERO, REAL_ZERO, REAL_ZERO) */, Math::Real intensity /* = REAL_ZERO */,
		int shadowMapSizeAsPowerOf2 /* = 0 */, Math::Real shadowArea /* = 80.0f */, Math::Real shadowSoftness /* = REAL_ONE */,
		Math::Real lightBleedingReductionAmount /* = static_cast<Math::Real>(0.2f) */,
		Math::Real minVariance /* = static_cast<Math::Real>(0.00002f) */ ) :
	BaseLight(color, intensity),
	m_maxIntensity(intensity),
	m_halfShadowArea(shadowArea / 2.0f),
	m_sunlightDaytimeColor(color),
	m_sunNearHorizonColor(GET_CONFIG_VALUE("directionalLightNearHorizonColorRed", REAL_ONE), GET_CONFIG_VALUE("directionalLightNearHorizonColorGreen", 0.2f), GET_CONFIG_VALUE("directionalLightNearHorizonColorBlue", REAL_ZERO)),
	m_sunlightNighttimeColor(GET_CONFIG_VALUE("directionalLightNighttimeColorRed", REAL_ZERO), GET_CONFIG_VALUE("directionalLightNighttimeColorGreen", REAL_ZERO), GET_CONFIG_VALUE("directionalLightNighttimeColorBlue", REAL_ZERO)),
	m_firstElevationLevel(GET_CONFIG_VALUE("directionalLightFirstElevationLevel", -REAL_ONE)),
	m_secondElevationLevel(GET_CONFIG_VALUE("directionalLightSecondElevationLevel", REAL_ZERO)),
	m_thirdElevationLevel(GET_CONFIG_VALUE("directionalLightThirdElevationLevel", REAL_ONE))
{
	SetShader(new Shader(GET_CONFIG_VALUE_STR("directionalLightShader", "forward-directional")));
	ASSERT(shader != NULL);
	if (shader == NULL)
	{
		LOG(Utility::Critical, LOGPLACE, "Cannot initialize directional light. Shader is NULL");
		exit(EXIT_FAILURE);
	}

	if (shadowMapSizeAsPowerOf2 != 0) // shadowMapSizeAsPowerOf2 == 0 means the light doesn't cast shadows
	{
		 Math::Matrix4D ortoMatrix = Math::Matrix4D::OrtographicProjection(-m_halfShadowArea, m_halfShadowArea, -m_halfShadowArea, m_halfShadowArea, -m_halfShadowArea, m_halfShadowArea);
		SetShadowInfo(new ShadowInfo(ortoMatrix, true, shadowMapSizeAsPowerOf2, shadowSoftness, lightBleedingReductionAmount, minVariance));
		ASSERT(shadowInfo != NULL);
		if (shadowInfo == NULL)
		{
			LOG(Utility::Emergency, LOGPLACE, "Cannot initialize directional light. Shadow info is NULL");
		}
	}
}


DirectionalLight::~DirectionalLight(void)
{
}

bool DirectionalLight::IsEnabled() const
{
	if (!directionalLightsEnabled)
	{
		return false;
	}
	return BaseLight::IsEnabled();
}

ShadowCameraTransform DirectionalLight::CalcShadowCameraTransform(const Math::Vector3D& cameraPos, const Math::Quaternion& cameraRot)
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
int tempCounter = 0;

void DirectionalLight::Update(Math::Real delta)
{
	Math::Real timeOfDay = CoreEngine::GetCoreEngine()->GetCurrentInGameTime();

	Transform& t = GetTransform();
	Math::Angle sunElevation, sunAzimuth;
	CalculateSunElevationAndAzimuth(timeOfDay, sunElevation, sunAzimuth);

	if (isEnabled && (sunElevation < m_firstElevationLevel))
	{
		int inGameHours, inGameMinutes, inGameSeconds;
		CoreEngine::GetCoreEngine()->ConvertTimeOfDay(inGameHours, inGameMinutes, inGameSeconds);
		LOG(Utility::Info, LOGPLACE, "Disabling the sunlight at %d:%d:%d. Sun elevation = %.5f", inGameHours, inGameMinutes, inGameSeconds, sunElevation.GetAngleInDegrees());
		isEnabled = false;
		return;
	}
	else if (!isEnabled && (sunElevation > m_firstElevationLevel))
	{
		int inGameHours, inGameMinutes, inGameSeconds;
		CoreEngine::GetCoreEngine()->ConvertTimeOfDay(inGameHours, inGameMinutes, inGameSeconds);
		LOG(Utility::Info, LOGPLACE, "Enabling the sunlight at %d:%d:%d. Sun elevation = %.5f", inGameHours, inGameMinutes, inGameSeconds, sunElevation.GetAngleInDegrees());
		isEnabled = true;
	}

	if (isEnabled)
	{
		if (sunElevation > m_thirdElevationLevel)
		{
			color = m_sunlightDaytimeColor;
		
			if (tempCounter != 1)
			{
				int inGameHours, inGameMinutes, inGameSeconds;
				CoreEngine::GetCoreEngine()->ConvertTimeOfDay(inGameHours, inGameMinutes, inGameSeconds);
				LOG(Utility::Info, LOGPLACE, "Daytime sunlight setup at %d:%d:%d. Sun elevation = %.4f\tColor = %s\tIntensity = %.4f", inGameHours, inGameMinutes, inGameSeconds, sunElevation.GetAngleInDegrees(), color.ToString().c_str(), intensity);
			}
			tempCounter = 1;
		}
		else if (sunElevation > m_secondElevationLevel)
		{
			Math::Real lerpFactor = (sunElevation.GetAngleInDegrees() - m_secondElevationLevel.GetAngleInDegrees()) /
				(m_thirdElevationLevel.GetAngleInDegrees() - m_secondElevationLevel.GetAngleInDegrees());
			color = m_sunNearHorizonColor.Lerp(m_sunlightDaytimeColor, lerpFactor);
			intensity = m_maxIntensity;

			if (tempCounter != 2)
			{
				int inGameHours, inGameMinutes, inGameSeconds;
				CoreEngine::GetCoreEngine()->ConvertTimeOfDay(inGameHours, inGameMinutes, inGameSeconds);
				LOG(Utility::Info, LOGPLACE, "Sunrise sunlight setup at %d:%d:%d. Sun elevation = %.4f\tColor = %s\tIntensity = %.4f", inGameHours, inGameMinutes, inGameSeconds, sunElevation.GetAngleInDegrees(), color.ToString().c_str(), intensity);
			}
			tempCounter = 2;
		}
		else //if (sunElevation > m_firstElevationLevel)
		{
			Math::Real lerpFactor = (sunElevation.GetAngleInDegrees() - m_firstElevationLevel.GetAngleInDegrees()) /
				(m_secondElevationLevel.GetAngleInDegrees() - m_firstElevationLevel.GetAngleInDegrees());
			color = m_sunlightNighttimeColor.Lerp(m_sunNearHorizonColor, lerpFactor);
			intensity = lerpFactor * m_maxIntensity;

			if (tempCounter != 3)
			{
				int inGameHours, inGameMinutes, inGameSeconds;
				CoreEngine::GetCoreEngine()->ConvertTimeOfDay(inGameHours, inGameMinutes, inGameSeconds);
				LOG(Utility::Info, LOGPLACE, "Before dawn sunlight setup at %d:%d:%d. Sun elevation = %.4f\tColor = %s\tIntensity = %.4f", inGameHours, inGameMinutes, inGameSeconds, sunElevation.GetAngleInDegrees(), color.ToString().c_str(), intensity);
			}
			tempCounter = 3;
		}
	}
	else
	{
		color = m_sunlightNighttimeColor;

		if (tempCounter != 4)
		{
			int inGameHours, inGameMinutes, inGameSeconds;
			CoreEngine::GetCoreEngine()->ConvertTimeOfDay(inGameHours, inGameMinutes, inGameSeconds);
			LOG(Utility::Info, LOGPLACE, "Nighttime sunlight setup at %d:%d:%d. Sun elevation = %.4f\tColor = %s\tIntensity = %.4f", inGameHours, inGameMinutes, inGameSeconds, sunElevation.GetAngleInDegrees(), color.ToString().c_str(), intensity);
		}
		tempCounter = 4;
	}

	//LOG(Utility::Info, LOGPLACE, "Sun elevation = %.5f", sunElevation.GetAngleInDegrees());
	Math::Matrix4D rotMatrix = Math::Matrix4D::RotationEuler(-sunElevation, -sunAzimuth, Math::Angle(REAL_ZERO));
	t.SetRot(Math::Quaternion(rotMatrix));
}

void DirectionalLight::CalculateSunElevationAndAzimuth(Math::Real timeOfDay, Math::Angle& sunElevation, Math::Angle& sunAzimuth) const
{
	const int SECONDS_PER_HOUR = 3600;
	const int dayNumber = 172;
	const Math::Angle LATITUDE(52.0f);
	const Math::Angle LONGITUDE(-16.0f);
	const int timeGMTdifference = -1;
	
	const Math::Angle b((360.0f / 365.0f) * (dayNumber -81));
	const Math::Angle doubleB(b.GetAngleInRadians() * 2.0f, Math::Angle::RADIAN);
	const Math::Angle tropicOfCancer(23.45f);

	const Math::Real equationOfTime = 9.87f * doubleB.Sin() - 7.53f * b.Cos() - 1.5f * b.Sin(); // EoT
	LOG(Utility::Debug, LOGPLACE, "EoT = %.5f", equationOfTime);
	const Math::Real declinationSin = tropicOfCancer.Sin() * b.Sin();
	LOG(Utility::Debug, LOGPLACE, "Sin(declination) = %.5f", declinationSin);
	const Math::Angle declinationAngle(asin(declinationSin), Math::Angle::RADIAN);
	LOG(Utility::Debug, LOGPLACE, "Declination in degrees = %.5f", declinationAngle.GetAngleInDegrees());

	const Math::Real timeCorrectionInSeconds = 60.0f * (4.0f * (LONGITUDE.GetAngleInDegrees() - 15.0f * timeGMTdifference) + equationOfTime);
	const Math::Real localSolarTime = timeOfDay + timeCorrectionInSeconds;
	LOG(Utility::Debug, LOGPLACE, "Time correction in seconds = %.5f", timeCorrectionInSeconds);
	LOG(Utility::Debug, LOGPLACE, "Local time = %.5f\tLocal solar time = %.5f", timeOfDay, localSolarTime);
	
	const Math::Angle hourAngle(15.0f * (localSolarTime - 12 * SECONDS_PER_HOUR) / 3600.0f);
	LOG(Utility::Debug, LOGPLACE, "Hour angle = %.5f", hourAngle.GetAngleInDegrees());

	const Math::Real sunElevationSin = declinationSin * LATITUDE.Sin() + declinationAngle.Cos() * LATITUDE.Cos() * hourAngle.Cos();
	sunElevation.SetAngleInRadians(asin(sunElevationSin));
	LOG(Utility::Debug, LOGPLACE, "Sun elevation = %.5f", sunElevation.GetAngleInDegrees());

	const Math::Real sunAzimuthCos = ((declinationSin * LATITUDE.Cos()) - (declinationAngle.Cos() * LATITUDE.Sin() * hourAngle.Cos())) / sunElevation.Cos();
	sunAzimuth.SetAngleInRadians(acos(sunAzimuthCos));
	bool isAfternoon = (localSolarTime > 12.0f * SECONDS_PER_HOUR) || (hourAngle.GetAngleInDegrees() > REAL_ZERO);
	if (isAfternoon)
	{
		sunAzimuth.SetAngleInDegrees(360.0f - sunAzimuth.GetAngleInDegrees());
	}
	LOG(Utility::Debug, LOGPLACE, "Sun azimuth = %.5f", sunAzimuth.GetAngleInDegrees());
}
#endif