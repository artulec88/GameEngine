#include "stdafx.h"
#include "ParticleRotationGenerator.h"

#include "Math\RandomGeneratorFactory.h"

/* ==================== class ConstantRotationGenerator begin ==================== */
Rendering::Particles::Generators::ConstantRotationGenerator::ConstantRotationGenerator(const Math::Angle& angle) :
	RotationGenerator(),
	m_angle(angle)
{
}

Rendering::Particles::Generators::ConstantRotationGenerator::~ConstantRotationGenerator()
{
}

void Rendering::Particles::Generators::ConstantRotationGenerator::Generate(Math::Real deltaTime, ParticlesContainer* particleContainer, size_t startId, size_t endId)
{
	for (size_t i = startId; i < endId; ++i)
	{
		Set(particleContainer, i, m_angle);
	}
}
/* ==================== class ConstantRotationGenerator end ==================== */

/* ==================== class RangeRotationGenerator begin ==================== */
Rendering::Particles::Generators::RangeRotationGenerator::RangeRotationGenerator(const Math::Angle& minAngle, const Math::Angle& maxAngle) :
	RotationGenerator(),
	m_minAngleInRadians(minAngle.Get()),
	m_maxAngleInRadians(maxAngle.Get()),
	m_randomGenerator(Math::Random::RandomGeneratorFactory::GetRandomGeneratorFactory().GetRandomGenerator(Math::Random::Generators::SIMPLE))
{
}

Rendering::Particles::Generators::RangeRotationGenerator::~RangeRotationGenerator()
{
}

void Rendering::Particles::Generators::RangeRotationGenerator::Generate(Math::Real deltaTime, ParticlesContainer* particleContainer, size_t startId, size_t endId)
{
	for (size_t i = startId; i < endId; ++i)
	{
		Set(particleContainer, i, Math::Angle(m_randomGenerator.NextFloat(m_minAngleInRadians, m_maxAngleInRadians), Math::Unit::RADIAN));
	}
}
/* ==================== class RangeRotationGenerator end ==================== */