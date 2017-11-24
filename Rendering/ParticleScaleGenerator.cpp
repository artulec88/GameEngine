#include "stdafx.h"
#include "ParticleScaleGenerator.h"

#include "Math/RandomGeneratorFactory.h"

/* ==================== class ConstantScaleGenerator begin ==================== */
rendering::particles::generators::ConstantScaleGenerator::ConstantScaleGenerator(math::Real scale) :
	ScaleGenerator(),
	m_scale(scale)
{
}

rendering::particles::generators::ConstantScaleGenerator::~ConstantScaleGenerator()
{
}

void rendering::particles::generators::ConstantScaleGenerator::Generate(math::Real deltaTime, ParticlesContainer* particleContainer, size_t startId, size_t endId)
{
	for (auto i = startId; i < endId; ++i)
	{
		Set(particleContainer, i, m_scale);
	}
}
/* ==================== class ConstantScaleGenerator end ==================== */

/* ==================== class RangeScaleGenerator begin ==================== */
rendering::particles::generators::RangeScaleGenerator::RangeScaleGenerator(math::Real minScale, math::Real maxScale) :
	ScaleGenerator(),
	m_minScale(minScale),
	m_maxScale(maxScale),
	m_randomGenerator(math::random::RandomGeneratorFactory::GetRandomGeneratorFactory().GetRandomGenerator(math::random::generator_ids::SIMPLE))
{
}

rendering::particles::generators::RangeScaleGenerator::~RangeScaleGenerator()
{
}

void rendering::particles::generators::RangeScaleGenerator::Generate(math::Real deltaTime, ParticlesContainer* particleContainer, size_t startId, size_t endId)
{
	for (auto i = startId; i < endId; ++i)
	{
		Set(particleContainer, i, m_randomGenerator.NextFloat(m_minScale, m_maxScale));
	}
}
/* ==================== class RangeScaleGenerator end ==================== */