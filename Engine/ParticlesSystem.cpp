#include "stdafx.h"
#include "ParticlesSystem.h"
#include <algorithm>

/* ==================== ParticleSystem class begin ==================== */
engine::ParticlesSystem::ParticlesSystem(rendering::particles::ParticleTexture* particleTexture, math::Real particlesPerSecondCount, math::Real particleLifeSpanLimit) :
	m_particleTexture(particleTexture),
	//m_particles(),
	m_particleLifeSpanLimit(particleLifeSpanLimit),
	m_currentTimer(0.0f),
	m_timeForGeneratingOneParticle(1.0f / particlesPerSecondCount),
	m_aliveParticlesCount(0),
	m_lastRevivedParticleIndex(0),
	m_randomGenerator(math::random::RandomGeneratorFactory::GetRandomGeneratorFactory().GetRandomGenerator(math::random::generator_ids::SIMPLE))
{
}


engine::ParticlesSystem::~ParticlesSystem()
{
	SAFE_DELETE(m_particleTexture);
}

void engine::ParticlesSystem::SortParticles(const math::Vector3D& originPosition /* cameraPosition */)
{
	/* ==================== Small test whether sorting particles work fine begin ==================== */
	//std::array<Rendering::Particle, 5> testParticles;
	//testParticles[1].Revive(math::Vector3D(1.0f, 1.0f, 1.0f), math::Vector3D(), 0.0f, 5.0f, math::Angle(3.0f), 1.0f);
	//testParticles[3].Revive(math::Vector3D(2.0f, 2.0f, -2.0f), math::Vector3D(), 0.0f, 5.0f, math::Angle(3.0f), 1.0f);
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

	std::sort(m_particles.begin(), m_particles.end(), rendering::particles::ParticleComparator(originPosition));
}

void engine::ParticlesSystem::Update(math::Real deltaTime)
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
/* ==================== ParticleSystem class end ==================== */

/* ==================== FireParticleSystem class begin ==================== */
engine::FireParticlesSystem::FireParticlesSystem(rendering::particles::ParticleTexture* particleTexture, math::Real particlesPerSecondCount, math::Real particleLifeSpanLimit,
	math::Real particleSpeed, math::Real particleGravityComplient, const math::Angle& particleRotation, math::Real particleScale) :
	ParticlesSystem(particleTexture, particlesPerSecondCount, particleLifeSpanLimit),
	m_particleSpeed(particleSpeed),
	m_particleGravityComplient(particleGravityComplient),
	m_particleRotation(particleRotation),
	m_particleScale(particleScale)
{
}


engine::FireParticlesSystem::~FireParticlesSystem()
{
}

void engine::FireParticlesSystem::GenerateParticles(const math::Vector3D& initialPosition, math::Real deltaTime)
{
	while (m_currentTimer > m_timeForGeneratingOneParticle)
	{
		EmitParticle(initialPosition);
		m_currentTimer -= m_timeForGeneratingOneParticle;
	}
}
/* ==================== FreeFallParticleSystem class end ==================== */

/* ==================== FreeFallParticleSystem class begin ==================== */
engine::FreeFallParticlesSystem::FreeFallParticlesSystem(rendering::particles::ParticleTexture* particleTexture, math::Real particlesPerSecondCount, math::Real particleLifeSpanLimit,
	math::Real particleSpeed, math::Real particleGravityComplient, const math::Angle& particleRotation, math::Real particleScale) :
	ParticlesSystem(particleTexture, particlesPerSecondCount, particleLifeSpanLimit),
	m_particleSpeed(particleSpeed),
	m_particleGravityComplient(particleGravityComplient),
	m_particleRotation(particleRotation),
	m_particleScale(particleScale)
{
}


engine::FreeFallParticlesSystem::~FreeFallParticlesSystem()
{
}

void engine::FreeFallParticlesSystem::GenerateParticles(const math::Vector3D& initialPosition, math::Real deltaTime)
{
	while (m_currentTimer > m_timeForGeneratingOneParticle)
	{
		math::Real x = initialPosition.x - m_randomGenerator.NextFloat(-3.0f, 3.0f);;
		math::Real y = initialPosition.y + 0.5f;
		math::Real z = initialPosition.z - m_randomGenerator.NextFloat(-3.0f, 3.0f);;
		EmitParticle(math::Vector3D(x, y, z));
		m_currentTimer -= m_timeForGeneratingOneParticle;
	}
}
/* ==================== FreeFallParticleSystem class end ==================== */