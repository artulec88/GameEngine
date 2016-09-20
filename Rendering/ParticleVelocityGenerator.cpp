#include "stdafx.h"
#include "ParticleVelocityGenerator.h"

#include "Math\RandomGeneratorFactory.h"

/* ==================== class ConstantVelocityGenerator begin ==================== */
Rendering::Particles::Generators::ConstantVelocityGenerator::ConstantVelocityGenerator(const Math::Vector3D& velocity) :
	ConstantVelocityGenerator(velocity.GetX(), velocity.GetY(), velocity.GetZ())
{
}

Rendering::Particles::Generators::ConstantVelocityGenerator::ConstantVelocityGenerator(Math::Real velocityX, Math::Real velocityY, Math::Real velocityZ) :
	VelocityGenerator(),
	m_velocity(velocityX, velocityY, velocityZ)
{
}

Rendering::Particles::Generators::ConstantVelocityGenerator::~ConstantVelocityGenerator()
{
}

void Rendering::Particles::Generators::ConstantVelocityGenerator::Generate(Math::Real deltaTime, ParticlesContainer* particleContainer, size_t startId, size_t endId)
{
	for (size_t i = startId; i < endId; ++i)
	{
		Set(particleContainer, i, m_velocity);
	}
}
/* ==================== class ConstantVelocityGenerator end ==================== */

/* ==================== class RangeVelocityGenerator begin ==================== */
Rendering::Particles::Generators::RangeVelocityGenerator::RangeVelocityGenerator(Math::Real minSpeedX, Math::Real maxSpeedX, Math::Real minSpeedY, Math::Real maxSpeedY, Math::Real minSpeedZ, Math::Real maxSpeedZ) :
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

Rendering::Particles::Generators::RangeVelocityGenerator::~RangeVelocityGenerator()
{
}

void Rendering::Particles::Generators::RangeVelocityGenerator::Generate(Math::Real deltaTime, ParticlesContainer* particleContainer, size_t startId, size_t endId)
{
	for (size_t i = startId; i < endId; ++i)
	{
		Set(particleContainer, i, Math::Vector3D{ m_randomGenerator.NextFloat(m_minSpeedX, m_maxSpeedX), m_randomGenerator.NextFloat(m_minSpeedY, m_maxSpeedY), m_randomGenerator.NextFloat(m_minSpeedZ, m_maxSpeedZ) });
	}
}
/* ==================== class RangeVelocityGenerator end ==================== */