#include "stdafx.h"
#include "ParticleRotationGenerator.h"

#include "Math\RandomGeneratorFactory.h"

/* ==================== class RandomRotationGenerator begin ==================== */
Rendering::Particles::RandomRotationGenerator::RandomRotationGenerator(const Math::Angle& minAngle, const Math::Angle& maxAngle) :
	RotationGenerator(),
	m_minAngleInRadians(minAngle.Get()),
	m_maxAngleInRadians(maxAngle.Get()),
	m_randomGenerator(Math::Random::RandomGeneratorFactory::GetRandomGeneratorFactory().GetRandomGenerator(Math::Random::Generators::SIMPLE))
{
}

Rendering::Particles::RandomRotationGenerator::~RandomRotationGenerator()
{
}

void Rendering::Particles::RandomRotationGenerator::Generate(Math::Real deltaTime, ParticlesContainer* particleContainer, size_t startId, size_t endId)
{
	for (size_t i = startId; i < endId; ++i)
	{
		Set(particleContainer, i, Math::Angle(m_randomGenerator.NextFloat(m_minAngleInRadians, m_maxAngleInRadians), Math::Unit::RADIAN));
	}
}
/* ==================== class RandomRotationGenerator end ==================== */