#include "stdafx.h"
#include "ParticleGeneratorComponent.h"


Engine::ParticleGeneratorComponent::ParticleGeneratorComponent(GameManager* gameManager, Rendering::ParticleTexture* particleTexture, Math::Real particlesPerSecondCount,
	Math::Real speed, Math::Real gravityComplient, Math::Real lifeSpanLimit) :
	m_particleGenerator(particleTexture, particlesPerSecondCount, speed, gravityComplient, lifeSpanLimit)
{
	gameManager->AddParticleGenerator(&m_particleGenerator);
}


Engine::ParticleGeneratorComponent::~ParticleGeneratorComponent()
{
}

void Engine::ParticleGeneratorComponent::Update(Math::Real deltaTime)
{
	m_particleGenerator.Update(deltaTime);
	m_particleGenerator.GenerateParticles(GetTransform().GetTransformedPos(), deltaTime);
}