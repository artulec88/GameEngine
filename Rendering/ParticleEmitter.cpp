#include "stdafx.h"
#include "ParticleEmitter.h"


Rendering::ParticleEmitter::ParticleEmitter(Math::Real emitRate) :
	m_emitRate(emitRate)
{
}


Rendering::ParticleEmitter::~ParticleEmitter()
{
}

void Rendering::ParticleEmitter::Emit(Math::Real deltaTime, ParticleContainer* particleContainer)
{
	const size_t maxNewParticles = static_cast<size_t>(deltaTime * m_emitRate);
	const size_t startId = particleContainer->GetAliveCount();
	const size_t endId = std::min(startId + maxNewParticles, particleContainer->GetCount() - 1);

	for (auto &gen : m_generators)
	{
		gen->Generate(deltaTime, particleContainer, startId, endId);
	}

	for (size_t i = startId; i < endId; ++i)
	{
		particleContainer->Revive(i);
	}
}