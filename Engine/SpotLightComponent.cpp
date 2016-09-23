#include "stdafx.h"
#include "SpotLightComponent.h"


Engine::SpotLightComponent::SpotLightComponent(Rendering::Lighting::SpotLight* spotLight) :
	GameComponent(),
	m_spotLight(spotLight)
{
}


Engine::SpotLightComponent::~SpotLightComponent()
{
	SAFE_DELETE(m_spotLight);
}

Engine::SpotLightComponent::SpotLightComponent(SpotLightComponent&& spotLightComponent) :
	GameComponent(std::move(spotLightComponent)),
	m_spotLight(std::move(spotLightComponent.m_spotLight))
{
	spotLightComponent.m_spotLight = NULL;
}

Engine::SpotLightComponent& Engine::SpotLightComponent::operator=(SpotLightComponent&& spotLightComponent)
{
	GameComponent::operator=(std::move(spotLightComponent));
	m_spotLight = std::move(spotLightComponent.m_spotLight);
	spotLightComponent.m_spotLight = NULL;
}