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
