#include "stdafx.h"
#include "ParticleLifeSpanGenerator.h"

#include "Math\RandomGeneratorFactory.h"

/* ==================== class BasicLifeSpanLimitGenerator begin ==================== */
Rendering::Particles::BasicLifeSpanLimitGenerator::BasicLifeSpanLimitGenerator(Math::Real minLifeSpanLimit, Math::Real maxLifeSpanLimit) :
	LifeSpanLimitGenerator(),
	m_minLifeSpanLimit(minLifeSpanLimit),
	m_maxLifeSpanLimit(maxLifeSpanLimit),
	m_randomGenerator(Math::Random::RandomGeneratorFactory::GetRandomGeneratorFactory().GetRandomGenerator(Math::Random::Generators::SIMPLE))
{
}

Rendering::Particles::BasicLifeSpanLimitGenerator::~BasicLifeSpanLimitGenerator()
{
}

void Rendering::Particles::BasicLifeSpanLimitGenerator::Generate(Math::Real deltaTime, ParticlesContainer* particleContainer, size_t startId, size_t endId)
{
	for (size_t i = startId; i < endId; ++i)
	{
		Set(particleContainer, i, m_randomGenerator.NextFloat(m_minLifeSpanLimit, m_maxLifeSpanLimit));
	}
}
/* ==================== class BasicLifeSpanLimitGenerator end ==================== */