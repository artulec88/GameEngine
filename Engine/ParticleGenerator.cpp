#include "stdafx.h"
#include "ParticleGenerator.h"
//#include "Math\ISort.h"

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

void Engine::ParticleGenerator::SortParticles(const Math::Vector3D& originPosition /* cameraPosition */)
{
	//Math::Sorting::ISort::GetSortingObject(Math::Sorting::INSERTION_SORT)->Sort()
	std::vector<Math::Real> originDistances(m_particles.size());
	for (size_t i = 0; i < m_particles.size(); ++i)
	{
		originDistances.push_back((originPosition - m_particles[i].GetPosition()).LengthSquared());
	}

	for (size_t i = 1; i < m_particles.size(); ++i)
	{
		int j = i - 1;
		while ((j >= 0) && (originDistances[i] < originDistances[j]))
		{
			m_particles[j + 1] = m_particles[j];
			m_particles[j] = m_particles[i];
			originDistances[j + 1] = originDistances[j];
			originDistances[j] = originDistances[i];
			--j;
		}
	}
}