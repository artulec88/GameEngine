#include "stdafx.h"
#include "DirectionalLightComponent.h"
#include "CoreEngine.h"

#include "Utility\IConfig.h"
#include "Utility\Time.h"

Engine::DirectionalLightComponent::DirectionalLightComponent(Rendering::Lighting::DirectionalLight* directionalLight, Math::Real maxIntensity, const Rendering::Color& sunDaytimeColor,
	const Rendering::Color& sunNearHorizonColor, const Rendering::Color& sunNighttimeColor, const Math::Angle& latitude, const Math::Angle& longitude,
	const Math::Angle& firstElevationLevel, const Math::Angle& secondElevationLevel, const Math::Angle& thirdElevationLevel) :
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


Engine::DirectionalLightComponent::~DirectionalLightComponent()
{
	SAFE_DELETE(m_directionalLight);
}

Engine::DirectionalLightComponent::DirectionalLightComponent(DirectionalLightComponent&& directionalLightComponent) :
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

Engine::DirectionalLightComponent& Engine::DirectionalLightComponent::operator=(DirectionalLightComponent&& directionalLightComponent)
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
void Engine::DirectionalLightComponent::Update(Math::Real delta)
{
	DELOCUST_LOG_ENGINE("Directional light update with delta time = ", delta);

	//CRITICAL_LOG_ENGINE("In-game time: ", m_inGameDateTime->ToDateString());

	//CoreEngine* coreEngine = CoreEngine::GetCoreEngine();
	//Math::Real timeOfDay = coreEngine->GetCurrentInGameTime();
	//Math::Real daytimeTransitionFactor;
	//Utility::Timing::Daytime daytime = coreEngine->GetCurrentDaytime(daytimeTransitionFactor);

	//m_directionalLight->SetIsEnabled(daytime != Utility::Timing::NIGHT);

	//switch (daytime)
	//{
	//case Utility::Timing::NIGHT:
	//	m_directionalLight->SetColor(m_sunNighttimeColor);
	//	break;
	//case Utility::Timing::BEFORE_DAWN:
	//	m_directionalLight->SetColor(m_sunNighttimeColor.Lerp(m_sunNearHorizonColor, daytimeTransitionFactor));
	//	m_directionalLight->SetIntensity(daytimeTransitionFactor * m_maxIntensity);
	//	break;
	//case Utility::Timing::SUNRISE:
	//	m_directionalLight->SetColor(m_sunNearHorizonColor.Lerp(m_sunDaytimeColor, daytimeTransitionFactor));
	//	m_directionalLight->SetIntensity(m_maxIntensity);
	//	break;
	//case Utility::Timing::DAY:
	//	m_directionalLight->SetColor(m_sunDaytimeColor);
	//	break;
	//case Utility::Timing::SUNSET:
	//	m_directionalLight->SetColor(m_sunNearHorizonColor.Lerp(m_sunDaytimeColor, daytimeTransitionFactor));
	//	m_directionalLight->SetIntensity(m_maxIntensity);
	//	break;
	//case Utility::Timing::AFTER_DUSK:
	//	m_directionalLight->SetColor(m_sunNighttimeColor.Lerp(m_sunNearHorizonColor, daytimeTransitionFactor));
	//	m_directionalLight->SetIntensity(daytimeTransitionFactor * m_maxIntensity);
	//	break;
	//}
	//Math::Matrix4D rotMatrix(-m_sunElevation, -m_sunAzimuth);
	//GetTransform().SetRot(Math::Quaternion(rotMatrix)); // TODO: Use quaternion interpolation to smoothly go from one rotation to another (see LookAtComponent.cpp)
}
#endif

void Engine::DirectionalLightComponent::CalculateSunElevationAndAzimuth()
{
	//const int timeGMTdifference = 1;

	//const Math::Angle b(0.9863014f * (m_inGameDateTime->GetDayInYear() - 81)); // 0,98630136986301369863013698630137 = 360 / 365
	//const Math::Real bSin = b.Sin();
	//const Math::Real bCos = b.Cos();

	//const Math::Real equationOfTime = 19.74f * bSin * bCos - 7.53f * bCos - 1.5f * bSin; // EoT
	//const Math::Real declinationSin = TROPIC_OF_CANCER_SINUS * bSin;
	//const Math::Angle declinationAngle(asin(declinationSin), Math::Unit::RADIAN);
	////DEBUG_LOG_ENGINE("Declination in degrees = ", declinationAngle.GetAngleInDegrees());

	//const Math::Real timeCorrectionInSeconds = 60.0f * (4.0f * (m_longitude.GetAngleInDegrees() - 15.0f * timeGMTdifference) + equationOfTime);
	//const Math::Real localSolarTime = m_inGameDateTime->GetDayTime() + timeCorrectionInSeconds;
	////DEBUG_LOG_ENGINE("Time correction in seconds = ", timeCorrectionInSeconds);
	////DEBUG_LOG_ENGINE("Local time = ", m_timeOfDay, "\tLocal solar time = ", localSolarTime);

	//const Math::Angle hourAngle(15.0f * (localSolarTime - 12 * Utility::Timing::DateTime::SECONDS_PER_HOUR) / Utility::Timing::DateTime::SECONDS_PER_HOUR);
	////DEBUG_LOG_ENGINE("Hour angle = ", hourAngle.GetAngleInDegrees());

	//const Math::Real sunElevationSin = declinationSin * m_latitude.Sin() + declinationAngle.Cos() * m_latitude.Cos() * hourAngle.Cos();
	//m_sunElevation.SetAngleInRadians(asin(sunElevationSin));
	////DEBUG_LOG_ENGINE("Sun elevation = ", m_sunElevation.GetAngleInDegrees());

	//const Math::Real sunAzimuthCos = ((declinationSin * m_latitude.Cos()) - (declinationAngle.Cos() * m_latitude.Sin() * hourAngle.Cos())) / m_sunElevation.Cos();
	//m_sunAzimuth.SetAngleInRadians(acos(sunAzimuthCos));
	//bool isAfternoon = (localSolarTime > 12.0f * Utility::Timing::DateTime::SECONDS_PER_HOUR) || (hourAngle.GetAngleInDegrees() > REAL_ZERO);
	//if (isAfternoon)
	//{
	//	m_sunAzimuth.SetAngleInDegrees(360.0f - m_sunAzimuth.GetAngleInDegrees());
	//}
	//DEBUG_LOG_ENGINE("Sun azimuth = ", m_sunAzimuth.GetAngleInDegrees());
}