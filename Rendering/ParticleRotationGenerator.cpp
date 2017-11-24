#include "stdafx.h"
#include "ParticleRotationGenerator.h"

#include "Math/RandomGeneratorFactory.h"

/* ==================== class ConstantRotationGenerator begin ==================== */
rendering::particles::generators::ConstantRotationGenerator::ConstantRotationGenerator(const math::Angle& angle) :
	RotationGenerator(),
	m_angle(angle)
{
}

rendering::particles::generators::ConstantRotationGenerator::~ConstantRotationGenerator()
{
}

void rendering::particles::generators::ConstantRotationGenerator::Generate(math::Real deltaTime, ParticlesContainer* particleContainer, size_t startId, size_t endId)
{
	for (auto i = startId; i < endId; ++i)
	{
		Set(particleContainer, i, m_angle);
	}
}
/* ==================== class ConstantRotationGenerator end ==================== */

/* ==================== class RangeRotationGenerator begin ==================== */
rendering::particles::generators::RangeRotationGenerator::RangeRotationGenerator(const math::Angle& minAngle, const math::Angle& maxAngle) :
	RotationGenerator(),
	m_minAngleInRadians(minAngle.Get()),
	m_maxAngleInRadians(maxAngle.Get()),
	m_randomGenerator(math::random::RandomGeneratorFactory::GetRandomGeneratorFactory().GetRandomGenerator(math::random::generator_ids::SIMPLE))
{
}

rendering::particles::generators::RangeRotationGenerator::~RangeRotationGenerator()
{
}

void rendering::particles::generators::RangeRotationGenerator::Generate(math::Real deltaTime, ParticlesContainer* particleContainer, size_t startId, size_t endId)
{
	for (auto i = startId; i < endId; ++i)
	{
		Set(particleContainer, i, math::Angle(m_randomGenerator.NextFloat(m_minAngleInRadians, m_maxAngleInRadians), math::units::RADIAN));
	}
}
/* ==================== class RangeRotationGenerator end ==================== */