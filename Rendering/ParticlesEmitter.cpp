#include "stdafx.h"
#include "ParticlesEmitter.h"


Rendering::Particles::ParticlesEmitter::ParticlesEmitter(Math::Real emitRate) :
	m_emitRate(emitRate)
{
}


Rendering::Particles::ParticlesEmitter::~ParticlesEmitter()
{
}

void Rendering::Particles::ParticlesEmitter::Emit(Math::Real deltaTime, ParticlesContainer* particleContainer)
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