#include "stdafx.h"
#include "ParticleScaleGenerator.h"

/* ==================== class ConstantScaleGenerator begin ==================== */
Rendering::Particles::ConstantScaleGenerator::ConstantScaleGenerator(Math::Real scale) :
	ScaleGenerator(),
	m_scale(scale)
{
}

Rendering::Particles::ConstantScaleGenerator::~ConstantScaleGenerator()
{
}

void Rendering::Particles::ConstantScaleGenerator::Generate(Math::Real deltaTime, ParticlesContainer* particleContainer, size_t startId, size_t endId)
{
	for (size_t i = startId; i < endId; ++i)
	{
		Set(particleContainer, i, m_scale);
	}
}
/* ==================== class ConstantScaleGenerator end ==================== */