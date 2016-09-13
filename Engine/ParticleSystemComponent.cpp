#include "stdafx.h"
#include "ParticleSystemComponent.h"


Engine::ParticleSystemComponent::ParticleSystemComponent(GameManager* gameManager, Rendering::ParticleTexture* particleTexture, Math::Real particlesPerSecondCount, Math::Real particleLifeSpanLimit,
	Math::Real particleSpeed, Math::Real particleGravityComplient, const Math::Angle& particleRotation, Math::Real particleScale) :
	m_particleSystem(NULL)
{
	m_particleSystem = new FreeFallParticleSystem(particleTexture, particlesPerSecondCount, particleLifeSpanLimit, particleSpeed, particleGravityComplient, particleRotation, particleScale);
	gameManager->AddParticleSystem(m_particleSystem);
}


Engine::ParticleSystemComponent::~ParticleSystemComponent()
{
	SAFE_DELETE(m_particleSystem);
}

void Engine::ParticleSystemComponent::Update(Math::Real deltaTime)
{
	m_particleSystem->Update(deltaTime);
	m_particleSystem->GenerateParticles(GetTransform().GetTransformedPos(), deltaTime);
}