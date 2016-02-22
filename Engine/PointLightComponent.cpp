#include "stdafx.h"
#include "PointLightComponent.h"


Engine::PointLightComponent::PointLightComponent(Rendering::Lighting::PointLight* pointLight) :
	Rendering::GameComponent(),
	m_pointLight(pointLight)
{
}


Engine::PointLightComponent::~PointLightComponent()
{
	SAFE_DELETE(m_pointLight);
}
