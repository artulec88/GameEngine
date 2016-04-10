#include "stdafx.h"
#include "ParticleGenerator.h"
#include <algorithm>

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
	ParticleContainer::iterator particleItr = m_particles.begin();
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

void Engine::ParticleGenerator::SortParticles(const Math::Vector3D& originPosition /* cameraPosition */)
{
	std::vector<Math::Real> originDistances;
	originDistances.reserve(m_particles.size());
	for (ParticleContainer::const_iterator particleItr = m_particles.begin(); particleItr != m_particles.end(); ++particleItr)
	{
		originDistances.push_back((originPosition - particleItr->GetPosition()).LengthSquared());
	}

	for (size_t i = 1; i < originDistances.size(); ++i)
	{
		Math::Real key = originDistances[i];
		int j = i - 1;
		while ((j >= 0) && (key > originDistances[j]))
		{
			std::swap(originDistances[j + 1], originDistances[j]);
			std::swap(m_particles[j + 1], m_particles[j]);
			--j;
		}
	}
}