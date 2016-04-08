#include "stdafx.h"
#include "ParticleGenerator.h"


Engine::ParticleGenerator::ParticleGenerator(GameManager* gameManager, Math::Real particlesPerSecondCount, Math::Real speed, Math::Real gravityComplient, Math::Real lifeSpanLimit) :
	m_gameManager(gameManager),
	m_speed(speed),
	m_gravityComplient(gravityComplient),
	m_lifeSpanLimit(lifeSpanLimit),
	m_currentTimer(0.0f),
	m_timeForGeneratingOneParticle(1.0f / particlesPerSecondCount)
{
}


Engine::ParticleGenerator::~ParticleGenerator()
{
}

void Engine::ParticleGenerator::GenerateParticles(const Math::Vector3D& initialPosition, Math::Real deltaTime) const
{
	m_currentTimer += deltaTime;
	while (m_currentTimer > m_timeForGeneratingOneParticle)
	{
		EmitParticle(initialPosition);
		m_currentTimer -= m_timeForGeneratingOneParticle;
	}
}