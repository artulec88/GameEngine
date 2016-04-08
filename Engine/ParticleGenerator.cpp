#include "stdafx.h"
#include "ParticleGenerator.h"


Engine::ParticleGenerator::ParticleGenerator(Rendering::ParticleTexture* particleTexture, Math::Real particlesPerSecondCount,
	Math::Real particleSpeed, Math::Real particleGravityComplient, Math::Real particleLifeSpanLimit) :
	m_particleTexture(particleTexture),
	m_particleSpeed(particleSpeed),
	m_particleGravityComplient(particleGravityComplient),
	m_particleLifeSpanLimit(particleLifeSpanLimit),
	m_currentTimer(0.0f),
	m_timeForGeneratingOneParticle(1.0f / particlesPerSecondCount)
{
}


Engine::ParticleGenerator::~ParticleGenerator()
{
	SAFE_DELETE(m_particleTexture);
}

void Engine::ParticleGenerator::Update(Math::Real deltaTime)
{
	m_currentTimer += deltaTime;
	std::vector<Rendering::Particle>::iterator particleItr = m_particles.begin();
	while (particleItr != m_particles.end())
	{
		if (particleItr->Update(deltaTime))
		{
			++particleItr;
		}
		else
		{
			particleItr = m_particles.erase(particleItr);
		}
	}
}

void Engine::ParticleGenerator::GenerateParticles(const Math::Vector3D& initialPosition, Math::Real deltaTime)
{
	while (m_currentTimer > m_timeForGeneratingOneParticle)
	{
		EmitParticle(initialPosition);
		m_currentTimer -= m_timeForGeneratingOneParticle;
	}
}