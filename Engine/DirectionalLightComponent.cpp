#include "stdafx.h"
#include "DirectionalLightComponent.h"
#include "CoreEngine.h"
#include "Utility\Time.h"

Engine::DirectionalLightComponent::DirectionalLightComponent(Rendering::Lighting::DirectionalLight* directionalLight,
	Math::Real maxIntensity,
	const Rendering::Color& sunDaytimeColor,
	const Rendering::Color& sunNearHorizonColor,
	const Rendering::Color& sunNighttimeColor) :
	GameComponent(),
	m_directionalLight(directionalLight),
	m_maxIntensity(maxIntensity),
	m_sunDaytimeColor(sunDaytimeColor),
	m_sunNearHorizonColor(sunNearHorizonColor),
	m_sunNighttimeColor(sunNighttimeColor)
{
}


Engine::DirectionalLightComponent::~DirectionalLightComponent()
{
	SAFE_DELETE(m_directionalLight);
}

#ifdef SIMULATE_SUN_BEHAVIOR
void Engine::DirectionalLightComponent::Update(Math::Real delta)
{
	DELOCUST_LOG_ENGINE("Directional light update with delta time = %.5f", delta);

	CoreEngine* coreEngine = CoreEngine::GetCoreEngine();
	Math::Real timeOfDay = coreEngine->GetCurrentInGameTime();
	Math::Angle sunElevation = coreEngine->GetSunElevation();
	Math::Angle sunAzimuth = coreEngine->GetSunAzimuth();
	Math::Real daytimeTransitionFactor;
	Utility::Timing::Daytime daytime = coreEngine->GetCurrentDaytime(daytimeTransitionFactor);

	m_directionalLight->SetIsEnabled(daytime != Utility::Timing::NIGHT);

	switch (daytime)
	{
	case Utility::Timing::NIGHT:
		m_directionalLight->SetColor(m_sunNighttimeColor);
		break;
	case Utility::Timing::BEFORE_DAWN:
		m_directionalLight->SetColor(m_sunNighttimeColor.Lerp(m_sunNearHorizonColor, daytimeTransitionFactor));
		m_directionalLight->SetIntensity(daytimeTransitionFactor * m_maxIntensity);
		break;
	case Utility::Timing::SUNRISE:
		m_directionalLight->SetColor(m_sunNearHorizonColor.Lerp(m_sunDaytimeColor, daytimeTransitionFactor));
		m_directionalLight->SetIntensity(m_maxIntensity);
		break;
	case Utility::Timing::DAY:
		m_directionalLight->SetColor(m_sunDaytimeColor);
		break;
	case Utility::Timing::SUNSET:
		m_directionalLight->SetColor(m_sunNearHorizonColor.Lerp(m_sunDaytimeColor, daytimeTransitionFactor));
		m_directionalLight->SetIntensity(m_maxIntensity);
		break;
	case Utility::Timing::AFTER_DUSK:
		m_directionalLight->SetColor(m_sunNighttimeColor.Lerp(m_sunNearHorizonColor, daytimeTransitionFactor));
		m_directionalLight->SetIntensity(daytimeTransitionFactor * m_maxIntensity);
		break;
	}
	Math::Matrix4D rotMatrix(-sunElevation, -sunAzimuth);
	GetTransform().SetRot(Math::Quaternion(rotMatrix)); // TODO: Use quaternion interpolation to smoothly go from one rotation to another (see LookAtComponent.cpp)
}
#endif