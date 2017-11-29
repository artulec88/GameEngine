#include "stdafx.h"
#include "SpotLightComponent.h"


engine::SpotLightComponent::SpotLightComponent(rendering::lighting::SpotLight* spotLight) :
	GameComponent(),
	m_spotLight(spotLight)
{
}


engine::SpotLightComponent::~SpotLightComponent()
{
	SAFE_DELETE(m_spotLight);
}

engine::SpotLightComponent::SpotLightComponent(SpotLightComponent&& spotLightComponent) noexcept:
	GameComponent(std::move(spotLightComponent)),
	m_spotLight(std::move(spotLightComponent.m_spotLight))
{
	spotLightComponent.m_spotLight = nullptr;
}

engine::SpotLightComponent& engine::SpotLightComponent::operator=(SpotLightComponent&& spotLightComponent) noexcept
{
	GameComponent::operator=(std::move(spotLightComponent));
	m_spotLight = std::move(spotLightComponent.m_spotLight);
	spotLightComponent.m_spotLight = nullptr;
	return *this;
}