#include "stdafx.h"
#include "ParticlesSystemComponent.h"


Engine::ParticlesSystemComponent::ParticlesSystemComponent(GameManager* gameManager, Rendering::Particles::ParticlesSystem* particlesSystem) :
	GameComponent(),
	IUpdateable(),
	m_particlesSystem(particlesSystem)
{
	gameManager->AddParticlesSystem(m_particlesSystem);
}


Engine::ParticlesSystemComponent::~ParticlesSystemComponent()
{
	SAFE_DELETE(m_particlesSystem);
}

Engine::ParticlesSystemComponent::ParticlesSystemComponent(ParticlesSystemComponent&& particlesSystemComponent) :
	GameComponent(std::move(particlesSystemComponent)),
	IUpdateable(std::move(particlesSystemComponent)),
	m_particlesSystem(std::move(particlesSystemComponent.m_particlesSystem))
{
	particlesSystemComponent.m_particlesSystem = NULL;
}

Engine::ParticlesSystemComponent& Engine::ParticlesSystemComponent::operator=(ParticlesSystemComponent&& particlesSystemComponent)
{
	GameComponent::operator=(std::move(particlesSystemComponent));
	IUpdateable::operator=(std::move(particlesSystemComponent));
	m_particlesSystem = std::move(particlesSystemComponent.m_particlesSystem);
	particlesSystemComponent.m_particlesSystem = NULL;
}

void Engine::ParticlesSystemComponent::Update(Math::Real deltaTime)
{
	m_particlesSystem->Update(deltaTime);
}