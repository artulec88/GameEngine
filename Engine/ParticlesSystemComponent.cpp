#include "stdafx.h"
#include "ParticlesSystemComponent.h"


engine::ParticlesSystemComponent::ParticlesSystemComponent(GameManager* gameManager, Rendering::Particles::ParticlesSystem* particlesSystem) :
	GameComponent(),
	IUpdateable(),
	m_particlesSystem(particlesSystem)
{
	gameManager->AddParticlesSystem(m_particlesSystem);
}


engine::ParticlesSystemComponent::~ParticlesSystemComponent()
{
	SAFE_DELETE(m_particlesSystem);
}

engine::ParticlesSystemComponent::ParticlesSystemComponent(ParticlesSystemComponent&& particlesSystemComponent) :
	GameComponent(std::move(particlesSystemComponent)),
	IUpdateable(std::move(particlesSystemComponent)),
	m_particlesSystem(std::move(particlesSystemComponent.m_particlesSystem))
{
	particlesSystemComponent.m_particlesSystem = NULL;
}

engine::ParticlesSystemComponent& engine::ParticlesSystemComponent::operator=(ParticlesSystemComponent&& particlesSystemComponent)
{
	GameComponent::operator=(std::move(particlesSystemComponent));
	IUpdateable::operator=(std::move(particlesSystemComponent));
	m_particlesSystem = std::move(particlesSystemComponent.m_particlesSystem);
	particlesSystemComponent.m_particlesSystem = NULL;
}

void engine::ParticlesSystemComponent::Update(Math::Real deltaTime)
{
	m_particlesSystem->Update(deltaTime);
}