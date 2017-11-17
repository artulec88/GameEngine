#include "stdafx.h"
#include "ParticleAccelerationGenerator.h"

#include "Math/RandomGeneratorFactory.h"

/* ==================== class ConstantAccelerationGenerator begin ==================== */
Rendering::Particles::Generators::ConstantAccelerationGenerator::ConstantAccelerationGenerator(math::Real accelerationX, math::Real accelerationY, math::Real accelerationZ) :
	AccelerationGenerator(),
	m_acceleration(accelerationX, accelerationY, accelerationZ)
{
}

Rendering::Particles::Generators::ConstantAccelerationGenerator::ConstantAccelerationGenerator(const math::Vector3D& acceleration) :
	AccelerationGenerator(),
	m_acceleration(acceleration)
{
}

Rendering::Particles::Generators::ConstantAccelerationGenerator::~ConstantAccelerationGenerator()
{
}

void Rendering::Particles::Generators::ConstantAccelerationGenerator::Generate(math::Real deltaTime, ParticlesContainer* particleContainer, size_t startId, size_t endId)
{
	for (size_t i = startId; i < endId; ++i)
	{
		Set(particleContainer, i, m_acceleration);
	}
}
/* ==================== class ConstantAccelerationGenerator end ==================== */

/* ==================== class RangeAccelerationGenerator begin ==================== */
Rendering::Particles::Generators::RangeAccelerationGenerator::RangeAccelerationGenerator(math::Real minAccelerationX, math::Real maxAccelerationX, math::Real minAccelerationY, math::Real maxAccelerationY, math::Real minAccelerationZ, math::Real maxAccelerationZ) :
	AccelerationGenerator(),
	m_minAccelerationX(minAccelerationX),
	m_maxAccelerationX(maxAccelerationX),
	m_minAccelerationY(minAccelerationY),
	m_maxAccelerationY(maxAccelerationY),
	m_minAccelerationZ(minAccelerationZ),
	m_maxAccelerationZ(maxAccelerationZ),
	m_randomGenerator(math::random::RandomGeneratorFactory::GetRandomGeneratorFactory().GetRandomGenerator(math::random::generator_ids::SIMPLE))
{
}

Rendering::Particles::Generators::RangeAccelerationGenerator::~RangeAccelerationGenerator()
{
}

void Rendering::Particles::Generators::RangeAccelerationGenerator::Generate(math::Real deltaTime, ParticlesContainer* particleContainer, size_t startId, size_t endId)
{
	for (size_t i = startId; i < endId; ++i)
	{
		Set(particleContainer, i, math::Vector3D{ m_randomGenerator.NextFloat(m_minAccelerationX, m_maxAccelerationX),
			m_randomGenerator.NextFloat(m_minAccelerationY, m_maxAccelerationY), m_randomGenerator.NextFloat(m_minAccelerationZ, m_maxAccelerationZ) });
	}
}
/* ==================== class RangeAccelerationGenerator end ==================== */