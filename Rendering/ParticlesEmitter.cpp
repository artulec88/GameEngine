#include "stdafx.h"
#include "ParticlesEmitter.h"

#include "Math\FloatingPoint.h"

#include "Utility\ILogger.h"

Rendering::Particles::ParticlesEmitter::ParticlesEmitter(Math::Real numberOfParticlesToGeneratePerSecond) :
	m_currentTimer(REAL_ZERO),
	m_timeToEmitOneParticle(REAL_ONE / numberOfParticlesToGeneratePerSecond)
{
	CHECK_CONDITION_EXIT_ALWAYS_RENDERING(numberOfParticlesToGeneratePerSecond > REAL_ZERO, Utility::Logging::ERR,
		"The number of particles to emit must be greater than 0 whereas it equals ", numberOfParticlesToGeneratePerSecond);
}


Rendering::Particles::ParticlesEmitter::~ParticlesEmitter()
{
}

void Rendering::Particles::ParticlesEmitter::Emit(Math::Real deltaTime, ParticlesContainer* particleContainer)
{
	m_currentTimer += deltaTime;
	if (m_currentTimer > m_timeToEmitOneParticle)
	{
		const size_t maxNewParticles = static_cast<size_t>(m_currentTimer / m_timeToEmitOneParticle);
		m_currentTimer = std::fmod(m_currentTimer, m_timeToEmitOneParticle); // see http://en.cppreference.com/w/cpp/numeric/math/fmod.
		const size_t startId = particleContainer->GetAliveCount();
		const size_t endId = std::min(startId + maxNewParticles, particleContainer->GetCount());

		//ERROR_LOG_RENDERING("Delta time = ", deltaTime, "[ms]. MaxNewParticles = ", maxNewParticles, ". StartId = ", startId, ". EndId = ",
		//	endId, ". Alive = ", particleContainer->GetAliveCount(), ". All = ", particleContainer->GetCount());

		for (auto &gen : m_generators)
		{
			gen->Generate(deltaTime, particleContainer, startId, endId);
		}

		for (size_t i = startId; i < endId; ++i)
		{
			particleContainer->Revive(i);
		}
	}
}