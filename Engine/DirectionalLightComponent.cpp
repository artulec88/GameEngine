#include "stdafx.h"
#include "DirectionalLightComponent.h"


Engine::DirectionalLightComponent::DirectionalLightComponent(Rendering::Lighting::DirectionalLight* directionalLight) :
	GameComponent(),
	m_directionalLight(directionalLight)
{
}


Engine::DirectionalLightComponent::~DirectionalLightComponent()
{
	SAFE_DELETE(m_directionalLight);
}