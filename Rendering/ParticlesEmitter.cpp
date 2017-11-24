#include "stdafx.h"
#include "ParticlesEmitter.h"

#include "Utility/ILogger.h"

rendering::particles::ParticlesEmitter::ParticlesEmitter(math::Real numberOfParticlesToGeneratePerSecond) :
	m_currentTimer(REAL_ZERO),
	m_timeToEmitOneParticle(REAL_ONE / numberOfParticlesToGeneratePerSecond)
{
	CHECK_CONDITION_EXIT_ALWAYS_RENDERING(numberOfParticlesToGeneratePerSecond > REAL_ZERO, utility::logging::ERR,
		"The number of particles to emit must be greater than 0 whereas it equals ", numberOfParticlesToGeneratePerSecond);
}


rendering::particles::ParticlesEmitter::~ParticlesEmitter()
{
}

void rendering::particles::ParticlesEmitter::Emit(math::Real deltaTime, ParticlesContainer* particleContainer)
{
	m_currentTimer += deltaTime;
	if (m_currentTimer > m_timeToEmitOneParticle)
	{
		const auto maxNewParticles = static_cast<size_t>(m_currentTimer / m_timeToEmitOneParticle);
		m_currentTimer = fmod(m_currentTimer, m_timeToEmitOneParticle); // see http://en.cppreference.com/w/cpp/numeric/math/fmod.
		const auto startId = particleContainer->GetAliveCount();
		const auto endId = std::min(startId + maxNewParticles, particleContainer->GetMaxCount());

		//ERROR_LOG_RENDERING("Delta time = ", deltaTime, "[ms]. MaxNewParticles = ", maxNewParticles, ". StartId = ", startId, ". EndId = ",
		//	endId, ". Alive = ", particleContainer->GetAliveCount(), ". All = ", particleContainer->GetCount());

		for (auto& gen : m_generators)
		{
			gen->Generate(deltaTime, particleContainer, startId, endId);
		}

		for (auto i = startId; i < endId; ++i)
		{
			particleContainer->Revive(i);
		}
	}
}