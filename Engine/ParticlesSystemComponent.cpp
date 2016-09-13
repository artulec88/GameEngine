#include "stdafx.h"
#include "ParticlesSystemComponent.h"


Engine::ParticlesSystemComponent::ParticlesSystemComponent(GameManager* gameManager, Rendering::Particles::ParticlesSystem* particlesSystem) :
	m_particlesSystem(particlesSystem)
{
	gameManager->AddParticlesSystem(m_particlesSystem);
}


Engine::ParticlesSystemComponent::~ParticlesSystemComponent()
{
	SAFE_DELETE(m_particlesSystem);
}

void Engine::ParticlesSystemComponent::Update(Math::Real deltaTime)
{
	m_particlesSystem->Update(deltaTime);
}