#include "stdafx.h"
#include "ParticleVelocityGenerator.h"

#include "Math\RandomGeneratorFactory.h"

/* ==================== class ConstantVelocityGenerator begin ==================== */
Rendering::Particles::Generators::ConstantVelocityGenerator::ConstantVelocityGenerator(const math::Vector3D& velocity) :
	ConstantVelocityGenerator(velocity.x, velocity.y, velocity.z)
{
}

Rendering::Particles::Generators::ConstantVelocityGenerator::ConstantVelocityGenerator(math::Real velocityX, math::Real velocityY, math::Real velocityZ) :
	VelocityGenerator(),
	m_velocity(velocityX, velocityY, velocityZ)
{
}

Rendering::Particles::Generators::ConstantVelocityGenerator::~ConstantVelocityGenerator()
{
}

void Rendering::Particles::Generators::ConstantVelocityGenerator::Generate(math::Real deltaTime, ParticlesContainer* particleContainer, size_t startId, size_t endId)
{
	for (size_t i = startId; i < endId; ++i)
	{
		Set(particleContainer, i, m_velocity);
	}
}
/* ==================== class ConstantVelocityGenerator end ==================== */

/* ==================== class RangeVelocityGenerator begin ==================== */
Rendering::Particles::Generators::RangeVelocityGenerator::RangeVelocityGenerator(math::Real minSpeedX, math::Real maxSpeedX, math::Real minSpeedY, math::Real maxSpeedY, math::Real minSpeedZ, math::Real maxSpeedZ) :
	VelocityGenerator(),
	m_minSpeedX(minSpeedX),
	m_maxSpeedX(maxSpeedX),
	m_minSpeedY(minSpeedY),
	m_maxSpeedY(maxSpeedY),
	m_minSpeedZ(minSpeedZ),
	m_maxSpeedZ(maxSpeedZ),
	m_randomGenerator(math::random::RandomGeneratorFactory::GetRandomGeneratorFactory().GetRandomGenerator(math::random::generator_ids::SIMPLE))
{
}

Rendering::Particles::Generators::RangeVelocityGenerator::~RangeVelocityGenerator()
{
}

void Rendering::Particles::Generators::RangeVelocityGenerator::Generate(math::Real deltaTime, ParticlesContainer* particleContainer, size_t startId, size_t endId)
{
	for (size_t i = startId; i < endId; ++i)
	{
		Set(particleContainer, i, math::Vector3D{ m_randomGenerator.NextFloat(m_minSpeedX, m_maxSpeedX), m_randomGenerator.NextFloat(m_minSpeedY, m_maxSpeedY), m_randomGenerator.NextFloat(m_minSpeedZ, m_maxSpeedZ) });
	}
}
/* ==================== class RangeVelocityGenerator end ==================== */