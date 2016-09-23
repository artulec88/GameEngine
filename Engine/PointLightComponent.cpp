#include "stdafx.h"
#include "PointLightComponent.h"


Engine::PointLightComponent::PointLightComponent(Rendering::Lighting::PointLight* pointLight) :
	GameComponent(),
	m_pointLight(pointLight)
{
}


Engine::PointLightComponent::~PointLightComponent()
{
	SAFE_DELETE(m_pointLight);
}

Engine::PointLightComponent::PointLightComponent(PointLightComponent&& pointLightComponent) :
	GameComponent(std::move(pointLightComponent)),
	m_pointLight(std::move(pointLightComponent.m_pointLight))
{
	pointLightComponent.m_pointLight = NULL;
}

Engine::PointLightComponent& Engine::PointLightComponent::operator=(PointLightComponent&& pointLightComponent)
{
	GameComponent::operator=(std::move(pointLightComponent));
	m_pointLight = std::move(pointLightComponent.m_pointLight);
	pointLightComponent.m_pointLight = NULL;
}