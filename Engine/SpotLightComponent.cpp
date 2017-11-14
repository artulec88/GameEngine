#include "stdafx.h"
#include "SpotLightComponent.h"


engine::SpotLightComponent::SpotLightComponent(Rendering::Lighting::SpotLight* spotLight) :
	GameComponent(),
	m_spotLight(spotLight)
{
}


engine::SpotLightComponent::~SpotLightComponent()
{
	SAFE_DELETE(m_spotLight);
}

engine::SpotLightComponent::SpotLightComponent(SpotLightComponent&& spotLightComponent) :
	GameComponent(std::move(spotLightComponent)),
	m_spotLight(std::move(spotLightComponent.m_spotLight))
{
	spotLightComponent.m_spotLight = NULL;
}

engine::SpotLightComponent& engine::SpotLightComponent::operator=(SpotLightComponent&& spotLightComponent)
{
	GameComponent::operator=(std::move(spotLightComponent));
	m_spotLight = std::move(spotLightComponent.m_spotLight);
	spotLightComponent.m_spotLight = NULL;
}