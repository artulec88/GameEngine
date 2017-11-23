#include "stdafx.h"
#include "DirectionalLightComponent.h"
#include "CoreEngine.h"

#include "Utility/IConfig.h"
#include "Utility/Time.h"

engine::DirectionalLightComponent::DirectionalLightComponent(rendering::lighting::DirectionalLight* directionalLight, math::Real maxIntensity, const rendering::Color& sunDaytimeColor,
	const rendering::Color& sunNearHorizonColor, const rendering::Color& sunNighttimeColor, const math::Angle& latitude, const math::Angle& longitude,
	const math::Angle& firstElevationLevel, const math::Angle& secondElevationLevel, const math::Angle& thirdElevationLevel) :
	GameComponent(),
#ifdef SIMULATE_SUN_BEHAVIOR
	IUpdateable(),
#endif
	m_directionalLight(directionalLight),
	m_maxIntensity(maxIntensity),
	m_sunDaytimeColor(sunDaytimeColor),
	m_sunNearHorizonColor(sunNearHorizonColor),
	m_sunNighttimeColor(sunNighttimeColor),
	m_latitude(latitude),
	m_longitude(longitude),
	m_sunElevation(REAL_ZERO),
	m_sunAzimuth(REAL_ZERO),
	m_firstElevationLevel(firstElevationLevel),
	m_secondElevationLevel(secondElevationLevel),
	m_thirdElevationLevel(thirdElevationLevel)
{
}


engine::DirectionalLightComponent::~DirectionalLightComponent()
{
	SAFE_DELETE(m_directionalLight);
}

engine::DirectionalLightComponent::DirectionalLightComponent(DirectionalLightComponent&& directionalLightComponent) :
	GameComponent(std::move(directionalLightComponent)),
#ifdef SIMULATE_SUN_BEHAVIOR
	IUpdateable(std::move(directionalLightComponent)),
#endif
	m_directionalLight(std::move(directionalLightComponent.m_directionalLight)),
	m_maxIntensity(std::move(directionalLightComponent.m_maxIntensity)),
	m_sunDaytimeColor(std::move(directionalLightComponent.m_sunDaytimeColor)),
	m_sunNearHorizonColor(std::move(directionalLightComponent.m_sunNearHorizonColor)),
	m_sunNighttimeColor(std::move(directionalLightComponent.m_sunNighttimeColor)),
	m_latitude(std::move(directionalLightComponent.m_latitude)),
	m_longitude(std::move(directionalLightComponent.m_longitude)),
	m_sunElevation(std::move(directionalLightComponent.m_sunElevation)),
	m_sunAzimuth(std::move(directionalLightComponent.m_sunAzimuth)),
	m_firstElevationLevel(std::move(directionalLightComponent.m_firstElevationLevel)),
	m_secondElevationLevel(std::move(directionalLightComponent.m_secondElevationLevel)),
	m_thirdElevationLevel(std::move(directionalLightComponent.m_thirdElevationLevel))
{
}

engine::DirectionalLightComponent& engine::DirectionalLightComponent::operator=(DirectionalLightComponent&& directionalLightComponent)
{
	GameComponent::operator=(std::move(directionalLightComponent));
#ifdef SIMULATE_SUN_BEHAVIOR
	IUpdateable::operator=(std::move(directionalLightComponent));
#endif
	m_directionalLight = std::move(directionalLightComponent.m_directionalLight);
	m_maxIntensity = std::move(directionalLightComponent.m_maxIntensity);
	m_sunDaytimeColor = std::move(directionalLightComponent.m_sunDaytimeColor);
	m_sunNearHorizonColor = std::move(directionalLightComponent.m_sunNearHorizonColor);
	m_sunNighttimeColor = std::move(directionalLightComponent.m_sunNighttimeColor);
	m_latitude = std::move(directionalLightComponent.m_latitude);
	m_longitude = std::move(directionalLightComponent.m_longitude);
	m_sunElevation = std::move(directionalLightComponent.m_sunElevation);
	m_sunAzimuth = std::move(directionalLightComponent.m_sunAzimuth);
	m_firstElevationLevel = std::move(directionalLightComponent.m_firstElevationLevel);
	m_secondElevationLevel = std::move(directionalLightComponent.m_secondElevationLevel);
	m_thirdElevationLevel = std::move(directionalLightComponent.m_thirdElevationLevel);
	return *this;
}

#ifdef SIMULATE_SUN_BEHAVIOR
void engine::DirectionalLightComponent::Update(math::Real delta)
{
	DELOCUST_LOG_ENGINE("Directional light update with delta time = ", delta);

	//CRITICAL_LOG_ENGINE("In-game time: ", m_inGameDateTime->ToDateString());

	//CoreEngine* coreEngine = CoreEngine::GetCoreEngine();
	//math::Real timeOfDay = coreEngine->GetCurrentInGameTime();
	//math::Real daytimeTransitionFactor;
	//Utility::timing::Daytime daytime = coreEngine->GetCurrentDaytime(daytimeTransitionFactor);

	//m_directionalLight->SetIsEnabled(daytime != Utility::timing::NIGHT);

	//switch (daytime)
	//{
	//case Utility::timing::NIGHT:
	//	m_directionalLight->SetColor(m_sunNighttimeColor);
	//	break;
	//case Utility::timing::BEFORE_DAWN:
	//	m_directionalLight->SetColor(m_sunNighttimeColor.Lerp(m_sunNearHorizonColor, daytimeTransitionFactor));
	//	m_directionalLight->SetIntensity(daytimeTransitionFactor * m_maxIntensity);
	//	break;
	//case Utility::timing::SUNRISE:
	//	m_directionalLight->SetColor(m_sunNearHorizonColor.Lerp(m_sunDaytimeColor, daytimeTransitionFactor));
	//	m_directionalLight->SetIntensity(m_maxIntensity);
	//	break;
	//case Utility::timing::DAY:
	//	m_directionalLight->SetColor(m_sunDaytimeColor);
	//	break;
	//case Utility::timing::SUNSET:
	//	m_directionalLight->SetColor(m_sunNearHorizonColor.Lerp(m_sunDaytimeColor, daytimeTransitionFactor));
	//	m_directionalLight->SetIntensity(m_maxIntensity);
	//	break;
	//case Utility::timing::AFTER_DUSK:
	//	m_directionalLight->SetColor(m_sunNighttimeColor.Lerp(m_sunNearHorizonColor, daytimeTransitionFactor));
	//	m_directionalLight->SetIntensity(daytimeTransitionFactor * m_maxIntensity);
	//	break;
	//}
	//math::Matrix4D rotMatrix(-m_sunElevation, -m_sunAzimuth);
	//GetTransform().SetRot(math::Quaternion(rotMatrix)); // TODO: Use quaternion interpolation to smoothly go from one rotation to another (see LookAtComponent.cpp)
}
#endif

void engine::DirectionalLightComponent::CalculateSunElevationAndAzimuth()
{
	//const int timeGMTdifference = 1;

	//const math::Angle b(0.9863014f * (m_inGameDateTime->GetDayInYear() - 81)); // 0,98630136986301369863013698630137 = 360 / 365
	//const math::Real bSin = b.Sin();
	//const math::Real bCos = b.Cos();

	//const math::Real equationOfTime = 19.74f * bSin * bCos - 7.53f * bCos - 1.5f * bSin; // EoT
	//const math::Real declinationSin = TROPIC_OF_CANCER_SINUS * bSin;
	//const math::Angle declinationAngle(asin(declinationSin), math::units::RADIAN);
	////DEBUG_LOG_ENGINE("Declination in degrees = ", declinationAngle.GetAngleInDegrees());

	//const math::Real timeCorrectionInSeconds = 60.0f * (4.0f * (m_longitude.GetAngleInDegrees() - 15.0f * timeGMTdifference) + equationOfTime);
	//const math::Real localSolarTime = m_inGameDateTime->GetDayTime() + timeCorrectionInSeconds;
	////DEBUG_LOG_ENGINE("Time correction in seconds = ", timeCorrectionInSeconds);
	////DEBUG_LOG_ENGINE("Local time = ", m_timeOfDay, "\tLocal solar time = ", localSolarTime);

	//const math::Angle hourAngle(15.0f * (localSolarTime - 12 * Utility::timing::DateTime::SECONDS_PER_HOUR) / Utility::timing::DateTime::SECONDS_PER_HOUR);
	////DEBUG_LOG_ENGINE("Hour angle = ", hourAngle.GetAngleInDegrees());

	//const math::Real sunElevationSin = declinationSin * m_latitude.Sin() + declinationAngle.Cos() * m_latitude.Cos() * hourAngle.Cos();
	//m_sunElevation.SetAngleInRadians(asin(sunElevationSin));
	////DEBUG_LOG_ENGINE("Sun elevation = ", m_sunElevation.GetAngleInDegrees());

	//const math::Real sunAzimuthCos = ((declinationSin * m_latitude.Cos()) - (declinationAngle.Cos() * m_latitude.Sin() * hourAngle.Cos())) / m_sunElevation.Cos();
	//m_sunAzimuth.SetAngleInRadians(acos(sunAzimuthCos));
	//bool isAfternoon = (localSolarTime > 12.0f * Utility::timing::DateTime::SECONDS_PER_HOUR) || (hourAngle.GetAngleInDegrees() > REAL_ZERO);
	//if (isAfternoon)
	//{
	//	m_sunAzimuth.SetAngleInDegrees(360.0f - m_sunAzimuth.GetAngleInDegrees());
	//}
	//DEBUG_LOG_ENGINE("Sun azimuth = ", m_sunAzimuth.GetAngleInDegrees());
}