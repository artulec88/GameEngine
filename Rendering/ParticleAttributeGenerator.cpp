#include "stdafx.h"
#include "ParticleAttributeGenerator.h"

#include "Math/RandomGeneratorFactory.h"

/* ==================== class BasicIdGenerator begin ==================== */
Rendering::Particles::Generators::BasicIdGenerator::BasicIdGenerator() :
	IdGenerator(),
	m_id(0)
{
}

Rendering::Particles::Generators::BasicIdGenerator::~BasicIdGenerator()
{
}

void Rendering::Particles::Generators::BasicIdGenerator::Generate(math::Real deltaTime, ParticlesContainer* particleContainer, size_t startId, size_t endId)
{
	for (size_t i = startId; i < endId; ++i)
	{
		Set(particleContainer, i, ++m_id);
	}
}
/* ==================== class BasicIdGenerator end ==================== */