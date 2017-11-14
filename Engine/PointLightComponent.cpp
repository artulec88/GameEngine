#include "stdafx.h"
#include "PointLightComponent.h"


engine::PointLightComponent::PointLightComponent(Rendering::Lighting::PointLight* pointLight) :
	GameComponent(),
	m_pointLight(pointLight)
{
}


engine::PointLightComponent::~PointLightComponent()
{
	SAFE_DELETE(m_pointLight);
}

engine::PointLightComponent::PointLightComponent(PointLightComponent&& pointLightComponent) :
	GameComponent(std::move(pointLightComponent)),
	m_pointLight(std::move(pointLightComponent.m_pointLight))
{
	pointLightComponent.m_pointLight = NULL;
}

engine::PointLightComponent& engine::PointLightComponent::operator=(PointLightComponent&& pointLightComponent)
{
	GameComponent::operator=(std::move(pointLightComponent));
	m_pointLight = std::move(pointLightComponent.m_pointLight);
	pointLightComponent.m_pointLight = NULL;
}