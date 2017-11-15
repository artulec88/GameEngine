#include "stdafx.h"
#include "ParticleRotationGenerator.h"

#include "Math\RandomGeneratorFactory.h"

/* ==================== class ConstantRotationGenerator begin ==================== */
Rendering::Particles::Generators::ConstantRotationGenerator::ConstantRotationGenerator(const math::Angle& angle) :
	RotationGenerator(),
	m_angle(angle)
{
}

Rendering::Particles::Generators::ConstantRotationGenerator::~ConstantRotationGenerator()
{
}

void Rendering::Particles::Generators::ConstantRotationGenerator::Generate(math::Real deltaTime, ParticlesContainer* particleContainer, size_t startId, size_t endId)
{
	for (size_t i = startId; i < endId; ++i)
	{
		Set(particleContainer, i, m_angle);
	}
}
/* ==================== class ConstantRotationGenerator end ==================== */

/* ==================== class RangeRotationGenerator begin ==================== */
Rendering::Particles::Generators::RangeRotationGenerator::RangeRotationGenerator(const math::Angle& minAngle, const math::Angle& maxAngle) :
	RotationGenerator(),
	m_minAngleInRadians(minAngle.Get()),
	m_maxAngleInRadians(maxAngle.Get()),
	m_randomGenerator(math::random::RandomGeneratorFactory::GetRandomGeneratorFactory().GetRandomGenerator(math::random::generator_ids::SIMPLE))
{
}

Rendering::Particles::Generators::RangeRotationGenerator::~RangeRotationGenerator()
{
}

void Rendering::Particles::Generators::RangeRotationGenerator::Generate(math::Real deltaTime, ParticlesContainer* particleContainer, size_t startId, size_t endId)
{
	for (size_t i = startId; i < endId; ++i)
	{
		Set(particleContainer, i, math::Angle(m_randomGenerator.NextFloat(m_minAngleInRadians, m_maxAngleInRadians), math::units::RADIAN));
	}
}
/* ==================== class RangeRotationGenerator end ==================== */