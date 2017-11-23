#include "stdafx.h"
#include "ParticleAttributeGenerator.h"

#include "Math/RandomGeneratorFactory.h"

/* ==================== class BasicIdGenerator begin ==================== */
rendering::particles::generators::BasicIdGenerator::BasicIdGenerator() :
	IdGenerator(),
	m_id(0)
{
}

rendering::particles::generators::BasicIdGenerator::~BasicIdGenerator()
{
}

void rendering::particles::generators::BasicIdGenerator::Generate(math::Real deltaTime, ParticlesContainer* particleContainer, size_t startId, size_t endId)
{
	for (auto i = startId; i < endId; ++i)
	{
		Set(particleContainer, i, ++m_id);
	}
}
/* ==================== class BasicIdGenerator end ==================== */