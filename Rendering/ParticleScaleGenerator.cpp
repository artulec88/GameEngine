#include "stdafx.h"
#include "ParticleScaleGenerator.h"

#include "Math/RandomGeneratorFactory.h"

/* ==================== class ConstantScaleGenerator begin ==================== */
Rendering::Particles::Generators::ConstantScaleGenerator::ConstantScaleGenerator(math::Real scale) :
	ScaleGenerator(),
	m_scale(scale)
{
}

Rendering::Particles::Generators::ConstantScaleGenerator::~ConstantScaleGenerator()
{
}

void Rendering::Particles::Generators::ConstantScaleGenerator::Generate(math::Real deltaTime, ParticlesContainer* particleContainer, size_t startId, size_t endId)
{
	for (size_t i = startId; i < endId; ++i)
	{
		Set(particleContainer, i, m_scale);
	}
}
/* ==================== class ConstantScaleGenerator end ==================== */

/* ==================== class RangeScaleGenerator begin ==================== */
Rendering::Particles::Generators::RangeScaleGenerator::RangeScaleGenerator(math::Real minScale, math::Real maxScale) :
	ScaleGenerator(),
	m_minScale(minScale),
	m_maxScale(maxScale),
	m_randomGenerator(math::random::RandomGeneratorFactory::GetRandomGeneratorFactory().GetRandomGenerator(math::random::generator_ids::SIMPLE))
{
}

Rendering::Particles::Generators::RangeScaleGenerator::~RangeScaleGenerator()
{
}

void Rendering::Particles::Generators::RangeScaleGenerator::Generate(math::Real deltaTime, ParticlesContainer* particleContainer, size_t startId, size_t endId)
{
	for (size_t i = startId; i < endId; ++i)
	{
		Set(particleContainer, i, m_randomGenerator.NextFloat(m_minScale, m_maxScale));
	}
}
/* ==================== class RangeScaleGenerator end ==================== */