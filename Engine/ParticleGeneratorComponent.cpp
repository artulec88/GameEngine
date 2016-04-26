#include "stdafx.h"
#include "ParticleGeneratorComponent.h"


Engine::ParticleGeneratorComponent::ParticleGeneratorComponent(GameManager* gameManager, Rendering::ParticleTexture* particleTexture, Math::Real particlesPerSecondCount, Math::Real particleLifeSpanLimit,
	Math::Real particleSpeed, Math::Real particleGravityComplient, const Math::Angle& particleRotation, Math::Real particleScale) :
	m_particleGenerator(NULL)
{
	m_particleGenerator = new FreeFallParticleGenerator(particleTexture, particlesPerSecondCount, particleLifeSpanLimit, particleSpeed, particleGravityComplient, particleRotation, particleScale);
	gameManager->AddParticleGenerator(m_particleGenerator);
}


Engine::ParticleGeneratorComponent::~ParticleGeneratorComponent()
{
	SAFE_DELETE(m_particleGenerator);
}

void Engine::ParticleGeneratorComponent::Update(Math::Real deltaTime)
{
	m_particleGenerator->Update(deltaTime);
	m_particleGenerator->GenerateParticles(GetTransform().GetTransformedPos(), deltaTime);
}