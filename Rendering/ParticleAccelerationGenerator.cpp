#include "stdafx.h"
#include "ParticleAccelerationGenerator.h"

#include "Math\RandomGeneratorFactory.h"

/* ==================== class ConstantAccelerationGenerator begin ==================== */
Rendering::Particles::Generators::ConstantAccelerationGenerator::ConstantAccelerationGenerator(Math::Real accelerationX, Math::Real accelerationY, Math::Real accelerationZ) :
	AccelerationGenerator(),
	m_acceleration(accelerationX, accelerationY, accelerationZ)
{
}

Rendering::Particles::Generators::ConstantAccelerationGenerator::ConstantAccelerationGenerator(const Math::Vector3D& acceleration) :
	AccelerationGenerator(),
	m_acceleration(acceleration)
{
}

Rendering::Particles::Generators::ConstantAccelerationGenerator::~ConstantAccelerationGenerator()
{
}

void Rendering::Particles::Generators::ConstantAccelerationGenerator::Generate(Math::Real deltaTime, ParticlesContainer* particleContainer, size_t startId, size_t endId)
{
	for (size_t i = startId; i < endId; ++i)
	{
		Set(particleContainer, i, m_acceleration);
	}
}
/* ==================== class ConstantAccelerationGenerator end ==================== */

/* ==================== class RangeAccelerationGenerator begin ==================== */
Rendering::Particles::Generators::RangeAccelerationGenerator::RangeAccelerationGenerator(Math::Real minAccelerationX, Math::Real maxAccelerationX, Math::Real minAccelerationY, Math::Real maxAccelerationY, Math::Real minAccelerationZ, Math::Real maxAccelerationZ) :
	AccelerationGenerator(),
	m_minAccelerationX(minAccelerationX),
	m_maxAccelerationX(maxAccelerationX),
	m_minAccelerationY(minAccelerationY),
	m_maxAccelerationY(maxAccelerationY),
	m_minAccelerationZ(minAccelerationZ),
	m_maxAccelerationZ(maxAccelerationZ),
	m_randomGenerator(Math::Random::RandomGeneratorFactory::GetRandomGeneratorFactory().GetRandomGenerator(Math::Random::Generators::SIMPLE))
{
}

Rendering::Particles::Generators::RangeAccelerationGenerator::~RangeAccelerationGenerator()
{
}

void Rendering::Particles::Generators::RangeAccelerationGenerator::Generate(Math::Real deltaTime, ParticlesContainer* particleContainer, size_t startId, size_t endId)
{
	for (size_t i = startId; i < endId; ++i)
	{
		Set(particleContainer, i, Math::Vector3D{ m_randomGenerator.NextFloat(m_minAccelerationX, m_maxAccelerationX),
			m_randomGenerator.NextFloat(m_minAccelerationY, m_maxAccelerationY), m_randomGenerator.NextFloat(m_minAccelerationZ, m_maxAccelerationZ) });
	}
}
/* ==================== class RangeAccelerationGenerator end ==================== */