#include "stdafx.h"
#include "ParticleGenerator.h"
#include <algorithm>

/* ==================== ParticleGenerator class begin ==================== */
Engine::ParticleGenerator::ParticleGenerator(Rendering::ParticleTexture* particleTexture, Math::Real particlesPerSecondCount, Math::Real particleLifeSpanLimit) :
	m_particleTexture(particleTexture),
	m_particleLifeSpanLimit(particleLifeSpanLimit),
	m_currentTimer(0.0f),
	m_timeForGeneratingOneParticle(1.0f / particlesPerSecondCount),
	m_aliveParticlesCount(0),
	m_lastRevivedParticleIndex(0),
	m_randomGenerator(Math::Random::RandomGeneratorFactory::GetRandomGeneratorFactory().GetRandomGenerator(Math::Random::Generators::SIMPLE))
{
}


Engine::ParticleGenerator::~ParticleGenerator()
{
	SAFE_DELETE(m_particleTexture);
}

void Engine::ParticleGenerator::SortParticles(const Math::Vector3D& originPosition /* cameraPosition */)
{
	/* ==================== Small test whether sorting particles work fine begin ==================== */
	//std::array<Rendering::Particle, 5> testParticles;
	//testParticles[1].Revive(Math::Vector3D(1.0f, 1.0f, 1.0f), Math::Vector3D(), 0.0f, 5.0f, Math::Angle(3.0f), 1.0f);
	//testParticles[3].Revive(Math::Vector3D(2.0f, 2.0f, -2.0f), Math::Vector3D(), 0.0f, 5.0f, Math::Angle(3.0f), 1.0f);
	//CRITICAL_LOG_ENGINE("Origin point = ", originPosition.ToString());
	//for (size_t i = 0; i < testParticles.size(); ++i)
	//{
	//	CRITICAL_LOG_ENGINE("Test particle[",  i, "] = ", testParticles[i].GetPosition().ToString());
	//}
	//std::sort(testParticles.begin(), testParticles.end(), Rendering::ParticleComparator(originPosition));
	//for (size_t i = 0; i < testParticles.size(); ++i)
	//{
	//	CRITICAL_LOG_ENGINE("Test particle[", i, "] = ", testParticles[i].GetPosition().ToString());
	//}
	/* ==================== Small test whether sorting particles work fine end ==================== */

	std::sort(m_particles.begin(), m_particles.end(), Rendering::ParticleComparator(originPosition));
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
/* ==================== ParticleGenerator class end ==================== */

/* ==================== FireParticleGenerator class begin ==================== */
Engine::FireParticleGenerator::FireParticleGenerator(Rendering::ParticleTexture* particleTexture, Math::Real particlesPerSecondCount, Math::Real particleLifeSpanLimit,
	Math::Real particleSpeed, Math::Real particleGravityComplient, const Math::Angle& particleRotation, Math::Real particleScale) :
	ParticleGenerator(particleTexture, particlesPerSecondCount, particleLifeSpanLimit),
	m_particleSpeed(particleSpeed),
	m_particleGravityComplient(particleGravityComplient),
	m_particleRotation(particleRotation),
	m_particleScale(particleScale)
{
}


Engine::FireParticleGenerator::~FireParticleGenerator()
{
}

void Engine::FireParticleGenerator::GenerateParticles(const Math::Vector3D& initialPosition, Math::Real deltaTime)
{
	while (m_currentTimer > m_timeForGeneratingOneParticle)
	{
		EmitParticle(initialPosition);
		m_currentTimer -= m_timeForGeneratingOneParticle;
	}
}
/* ==================== FreeFallParticleGenerator class end ==================== */

/* ==================== FreeFallParticleGenerator class begin ==================== */
Engine::FreeFallParticleGenerator::FreeFallParticleGenerator(Rendering::ParticleTexture* particleTexture, Math::Real particlesPerSecondCount, Math::Real particleLifeSpanLimit,
	Math::Real particleSpeed, Math::Real particleGravityComplient, const Math::Angle& particleRotation, Math::Real particleScale) :
	ParticleGenerator(particleTexture, particlesPerSecondCount, particleLifeSpanLimit),
	m_particleSpeed(particleSpeed),
	m_particleGravityComplient(particleGravityComplient),
	m_particleRotation(particleRotation),
	m_particleScale(particleScale)
{
}


Engine::FreeFallParticleGenerator::~FreeFallParticleGenerator()
{
}

void Engine::FreeFallParticleGenerator::GenerateParticles(const Math::Vector3D& initialPosition, Math::Real deltaTime)
{
	while (m_currentTimer > m_timeForGeneratingOneParticle)
	{
		Math::Real x = initialPosition.GetX() - m_randomGenerator.NextFloat(-3.0f, 3.0f);;
		Math::Real y = initialPosition.GetY() + 0.5f;
		Math::Real z = initialPosition.GetZ() - m_randomGenerator.NextFloat(-3.0f, 3.0f);;
		EmitParticle(Math::Vector3D(x, y, z));
		m_currentTimer -= m_timeForGeneratingOneParticle;
	}
}
/* ==================== FreeFallParticleGenerator class end ==================== */