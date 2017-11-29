#include "stdafx.h"
#include "ParticlesSystemComponent.h"


engine::ParticlesSystemComponent::ParticlesSystemComponent(GameManager* gameManager, rendering::particles::ParticlesSystem* particlesSystem) :
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

engine::ParticlesSystemComponent::ParticlesSystemComponent(ParticlesSystemComponent&& particlesSystemComponent) noexcept:
	GameComponent(std::move(particlesSystemComponent)),
	IUpdateable(std::move(particlesSystemComponent)),
	m_particlesSystem(std::move(particlesSystemComponent.m_particlesSystem))
{
	particlesSystemComponent.m_particlesSystem = nullptr;
}

engine::ParticlesSystemComponent& engine::ParticlesSystemComponent::operator=(ParticlesSystemComponent&& particlesSystemComponent) noexcept
{
	GameComponent::operator=(std::move(particlesSystemComponent));
	IUpdateable::operator=(std::move(particlesSystemComponent));
	m_particlesSystem = std::move(particlesSystemComponent.m_particlesSystem);
	particlesSystemComponent.m_particlesSystem = nullptr;
	return *this;
}

void engine::ParticlesSystemComponent::Update(math::Real deltaTime)
{
	m_particlesSystem->Update(deltaTime);
}