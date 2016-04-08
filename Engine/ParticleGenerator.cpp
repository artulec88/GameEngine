#include "stdafx.h"
#include "ParticleGenerator.h"


Engine::ParticleGenerator::ParticleGenerator(GameManager* gameManager, Math::Real particlesPerSecondCount, Math::Real speed, Math::Real gravityComplient, Math::Real lifeSpanLimit) :
	m_gameManager(gameManager),
	m_particlesPerSecondCount(particlesPerSecondCount),
	m_speed(speed),
	m_gravityComplient(gravityComplient),
	m_lifeSpanLimit(lifeSpanLimit)
{
}


Engine::ParticleGenerator::~ParticleGenerator()
{
}

void Engine::ParticleGenerator::GenerateParticles(const Math::Vector3D& initialPosition, Math::Real deltaTime) const
{
	// TODO: If m_particlesPerSecondCount is not big enough then we may always get 0 here and never create a single particle.
	// On the other hand if we make it too big we will end up having billions of particles.
	Math::Real particlesToGenerateCountReal = m_particlesPerSecondCount * deltaTime;
	int particlesToGenerateCount = static_cast<int>(particlesToGenerateCountReal);
	Math::Real particlesToGenerateFractionalPart = particlesToGenerateCountReal - particlesToGenerateCount;
	if ((static_cast<Math::Real>((rand() % 1001)) * 1000.0f) > particlesToGenerateFractionalPart)
	{
		EmitParticle(initialPosition);
	}

	for (int i = 0; i < particlesToGenerateCount; ++i)
	{
		EmitParticle(initialPosition);
	}
}