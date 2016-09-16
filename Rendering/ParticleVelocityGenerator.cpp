#include "stdafx.h"
#include "ParticleVelocityGenerator.h"

#include "Math\RandomGeneratorFactory.h"

/* ==================== class BasicVelocityGenerator begin ==================== */
Rendering::Particles::BasicVelocityGenerator::BasicVelocityGenerator(Math::Real minSpeedX, Math::Real maxSpeedX, Math::Real minSpeedY, Math::Real maxSpeedY, Math::Real minSpeedZ, Math::Real maxSpeedZ) :
	VelocityGenerator(),
	m_minSpeedX(minSpeedX),
	m_maxSpeedX(maxSpeedX),
	m_minSpeedY(minSpeedY),
	m_maxSpeedY(maxSpeedY),
	m_minSpeedZ(minSpeedZ),
	m_maxSpeedZ(maxSpeedZ),
	m_randomGenerator(Math::Random::RandomGeneratorFactory::GetRandomGeneratorFactory().GetRandomGenerator(Math::Random::Generators::SIMPLE))
{
}

Rendering::Particles::BasicVelocityGenerator::~BasicVelocityGenerator()
{
}

void Rendering::Particles::BasicVelocityGenerator::Generate(Math::Real deltaTime, ParticlesContainer* particleContainer, size_t startId, size_t endId)
{
	for (size_t i = startId; i < endId; ++i)
	{
		Set(particleContainer, i, Math::Vector3D{ m_randomGenerator.NextFloat(m_minSpeedX, m_maxSpeedX), m_randomGenerator.NextFloat(m_minSpeedY, m_maxSpeedY), m_randomGenerator.NextFloat(m_minSpeedZ, m_maxSpeedZ) });
	}
}
/* ==================== class BasicVelocityGenerator end ==================== */