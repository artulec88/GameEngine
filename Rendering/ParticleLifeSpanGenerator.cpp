#include "stdafx.h"
#include "ParticleLifeSpanGenerator.h"

#include "Math/RandomGeneratorFactory.h"

/* ==================== class ConstantLifeSpanLimitGenerator begin ==================== */
rendering::particles::generators::ConstantLifeSpanLimitGenerator::ConstantLifeSpanLimitGenerator(math::Real lifeSpanLimit) :
	LifeSpanLimitGenerator(),
	m_lifeSpanLimit(lifeSpanLimit)
{
}

rendering::particles::generators::ConstantLifeSpanLimitGenerator::~ConstantLifeSpanLimitGenerator()
{
}

void rendering::particles::generators::ConstantLifeSpanLimitGenerator::Generate(math::Real deltaTime, ParticlesContainer* particleContainer, size_t startId, size_t endId)
{
	for (size_t i = startId; i < endId; ++i)
	{
		Set(particleContainer, i, m_lifeSpanLimit);
	}
}
/* ==================== class ConstantLifeSpanLimitGenerator end ==================== */

/* ==================== class RangeLifeSpanLimitGenerator begin ==================== */
rendering::particles::generators::RangeLifeSpanLimitGenerator::RangeLifeSpanLimitGenerator(math::Real minLifeSpanLimit, math::Real maxLifeSpanLimit) :
	LifeSpanLimitGenerator(),
	m_minLifeSpanLimit(minLifeSpanLimit),
	m_maxLifeSpanLimit(maxLifeSpanLimit),
	m_randomGenerator(math::random::RandomGeneratorFactory::GetRandomGeneratorFactory().GetRandomGenerator(math::random::generator_ids::SIMPLE))
{
}

rendering::particles::generators::RangeLifeSpanLimitGenerator::~RangeLifeSpanLimitGenerator()
{
}

void rendering::particles::generators::RangeLifeSpanLimitGenerator::Generate(math::Real deltaTime, ParticlesContainer* particleContainer, size_t startId, size_t endId)
{
	for (size_t i = startId; i < endId; ++i)
	{
		Set(particleContainer, i, m_randomGenerator.NextFloat(m_minLifeSpanLimit, m_maxLifeSpanLimit));
	}
}
/* ==================== class RangeLifeSpanLimitGenerator end ==================== */