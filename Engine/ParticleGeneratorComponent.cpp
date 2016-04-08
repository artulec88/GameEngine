#include "stdafx.h"
#include "ParticleGeneratorComponent.h"


Engine::ParticleGeneratorComponent::ParticleGeneratorComponent(GameManager* gameManager, Math::Real particlesPerSecondCount, Math::Real speed, Math::Real gravityComplient, Math::Real lifeSpanLimit) :
	m_particleGenerator(gameManager,particlesPerSecondCount, speed, gravityComplient, lifeSpanLimit)
{
}


Engine::ParticleGeneratorComponent::~ParticleGeneratorComponent()
{
}

void Engine::ParticleGeneratorComponent::Update(Math::Real deltaTime)
{
	m_particleGenerator.GenerateParticles(GetTransform().GetTransformedPos(), deltaTime);
}