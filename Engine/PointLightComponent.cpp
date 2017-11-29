#include "stdafx.h"
#include "PointLightComponent.h"


engine::PointLightComponent::PointLightComponent(rendering::lighting::PointLight* pointLight) :
	GameComponent(),
	m_pointLight(pointLight)
{
}


engine::PointLightComponent::~PointLightComponent()
{
	SAFE_DELETE(m_pointLight);
}

engine::PointLightComponent::PointLightComponent(PointLightComponent&& pointLightComponent) noexcept:
	GameComponent(std::move(pointLightComponent)),
	m_pointLight(std::move(pointLightComponent.m_pointLight))
{
	pointLightComponent.m_pointLight = nullptr;
}

engine::PointLightComponent& engine::PointLightComponent::operator=(PointLightComponent&& pointLightComponent) noexcept
{
	GameComponent::operator=(std::move(pointLightComponent));
	m_pointLight = std::move(pointLightComponent.m_pointLight);
	pointLightComponent.m_pointLight = nullptr;
	return *this;
}