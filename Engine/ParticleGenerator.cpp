#include "stdafx.h"
#include "ParticleGenerator.h"
#include <algorithm>

///* static */ const int Engine::ParticleGenerator::MAX_PARTICLES_COUNT = 10000;

Engine::ParticleGenerator::ParticleGenerator(Rendering::ParticleTexture* particleTexture, Math::Real particlesPerSecondCount,
	Math::Real particleSpeed, Math::Real particleGravityComplient, Math::Real particleLifeSpanLimit) :
	m_particleTexture(particleTexture),
	m_particleSpeed(particleSpeed),
	m_particleGravityComplient(particleGravityComplient),
	m_particleLifeSpanLimit(particleLifeSpanLimit),
	m_currentTimer(0.0f),
	m_timeForGeneratingOneParticle(1.0f / particlesPerSecondCount),
	m_aliveParticlesCount(0),
	m_lastRevivedParticleIndex(0)
{
}


Engine::ParticleGenerator::~ParticleGenerator()
{
	SAFE_DELETE(m_particleTexture);
}

void Engine::ParticleGenerator::Update(Math::Real deltaTime)
{
	m_currentTimer += deltaTime;
	m_aliveParticlesCount = 0;
	for (int i = 0; i < MAX_PARTICLES_COUNT; ++i)
	{
		if (m_particles[i].IsAlive() && m_particles[i].Update(deltaTime))
		{
			++m_aliveParticlesCount;
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
	originDistances.reserve(MAX_PARTICLES_COUNT);
	for (int i = 0; i < MAX_PARTICLES_COUNT; ++i)
	{
		if (m_particles[i].IsAlive())
		{
			originDistances.push_back((originPosition - m_particles[i].GetPosition()).LengthSquared());
		}
		else
		{
			originDistances.push_back(-999999999999.9f); // for particles that are dead we want to store them at the end of the particles array.
		}
	}

	for (size_t i = 1; i < MAX_PARTICLES_COUNT; ++i)
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

	//for (int i = 0; i < m_aliveParticlesCount; ++i)
	//{
	//	ERROR_LOG("particle[%d] = %.3f", i, originDistances[i]);
	//}
}