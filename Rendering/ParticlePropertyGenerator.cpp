#include "stdafx.h"
#include "ParticlePropertyGenerator.h"

#include "Math\RandomGeneratorFactory.h"

/* ==================== class ParticlePropertyGenerator begin ==================== */
Rendering::ParticlePropertyGenerator::ParticlePropertyGenerator()
{
}


Rendering::ParticlePropertyGenerator::~ParticlePropertyGenerator()
{
}
/* ==================== class ParticlePropertyGenerator end ==================== */

/* ==================== class BoxPositionGenerator begin ==================== */
Rendering::BoxPositionGenerator::BoxPositionGenerator(Math::Real minX, Math::Real maxX, Math::Real minY, Math::Real maxY, Math::Real minZ, Math::Real maxZ) :
	ParticlePropertyGenerator(),
	m_minX(minX),
	m_maxX(maxX),
	m_minY(minY),
	m_maxY(maxY),
	m_minZ(minZ),
	m_maxZ(maxZ),
	m_randomGenerator(Math::Random::RandomGeneratorFactory::GetRandomGeneratorFactory().GetRandomGenerator(Math::Random::Generators::SIMPLE))
{
}

Rendering::BoxPositionGenerator::~BoxPositionGenerator()
{
}

void Rendering::BoxPositionGenerator::Generate(Math::Real deltaTime, ParticleContainer* particleContainer, size_t startId, size_t endId)
{
	for (size_t i = startId; i < endId; ++i)
	{
		particleContainer->SetPosition(i, Math::Vector3D{ m_randomGenerator.NextFloat(m_minX, m_maxX), m_randomGenerator.NextFloat(m_minY, m_maxY), m_randomGenerator.NextFloat(m_minZ, m_maxZ) });
	}
}
/* ==================== class BoxPositionGenerator end ==================== */