#include "stdafx.h"
#include "ParticlesKiller.h"
#include "ParticlesContainer.h"

#include "Math/RandomGeneratorFactory.h"

#include "Utility/ILogger.h"

#include <numeric>
#include <algorithm>

/* ==================== ParticlesKiller class begin ==================== */
rendering::particles::ParticlesKiller::ParticlesKiller()
{
}


rendering::particles::ParticlesKiller::~ParticlesKiller()
{
}
/* ==================== ParticlesKiller class end ==================== */

/* ==================== LifeSpanParticlesKiller class begin ==================== */
rendering::particles::LifeSpanParticlesKiller::LifeSpanParticlesKiller() :
	ParticlesKiller()
{
}

rendering::particles::LifeSpanParticlesKiller::~LifeSpanParticlesKiller()
{
}

void rendering::particles::LifeSpanParticlesKiller::Kill(math::Real deltaTime, ParticlesContainer* particleContainer)
{
	// TODO: Instead of removing the dead particles one by one we could collect their indices and
	/// then pass those indices of dead particles to particles container to remove them in one go.
	std::vector<size_t> indicesToKill;
	for (size_t i = 0; i < particleContainer->GetAliveCount(); ++i)
	{
		if (particleContainer->GetLifeSpan(i) >= particleContainer->GetLifeSpanLimit(i))
		{
			indicesToKill.push_back(i);
		}
	}
	particleContainer->Kill(indicesToKill.data(), indicesToKill.size());
}

/* ==================== LifeSpanParticlesKiller class end ==================== */

/* ==================== TimerParticlesKiller class begin ==================== */
/// <summary>
/// A simple sequence generator. The structure is used to simplify the process of creating a sequence of integers starting from given <code>m_current</code> variable value.
/// This values is then incremented each time the generator is called, but when it reaches the constant value <code>m_limit</code> it is reset to <value>0</value>.
/// </summary>
struct SeqGenerator
{
	SeqGenerator(int start, int limit) : m_current(start), m_limit(limit) {}
	int operator() ()
	{
		if (m_current == m_limit)
		{
			m_current = 0;
		}
		return m_current++;
	}
private:
	int m_current;
	const int m_limit;
};

rendering::particles::TimerParticlesKiller::TimerParticlesKiller(math::Real particlesToKillPerSecond) :
	ParticlesKiller(),
	m_timeToKillOneParticle(REAL_ONE / particlesToKillPerSecond),
	m_currentTimer(REAL_ZERO),
	m_randomGenerator(math::random::RandomGeneratorFactory::GetRandomGeneratorFactory().GetRandomGenerator(math::random::generator_ids::SIMPLE))
{
}

rendering::particles::TimerParticlesKiller::~TimerParticlesKiller()
{
}

void rendering::particles::TimerParticlesKiller::Kill(math::Real deltaTime, ParticlesContainer* particleContainer)
{
	// TODO: Instead of removing the dead particles one by one we could collect their indices and
	/// then pass those indices of dead particles to particles container to remove them in one go.
	m_currentTimer += deltaTime;
	if (m_currentTimer > m_timeToKillOneParticle)
	{
		const auto maxParticlesToBeKilledCount = static_cast<size_t>(m_currentTimer / m_timeToKillOneParticle);
		m_currentTimer = fmod(m_currentTimer, m_timeToKillOneParticle); // see http://en.cppreference.com/w/cpp/numeric/math/fmod.
		const auto particlesToBeKilledCount = std::min(maxParticlesToBeKilledCount, particleContainer->GetAliveCount());

		CHECK_CONDITION_RETURN_VOID_ALWAYS_RENDERING(particlesToBeKilledCount > 0, utility::logging::WARNING,
			"Calculated number of particles to be killed equals ", particlesToBeKilledCount);
		//ERROR_LOG_RENDERING("Delta time = ", deltaTime, "[ms]. MaxNewParticles = ", maxNewParticles, ". StartId = ", startId, ". EndId = ",
		//	endId, ". Alive = ", particleContainer->GetAliveCount(), ". All = ", particleContainer->GetCount());

		const auto startIndex = particleContainer->GetAliveCount() > 1 ? m_randomGenerator.NextInt(0, particleContainer->GetAliveCount() - 1) : 0;

		DELOCUST_LOG_RENDERING("Killing starts: particlesToBeKilledCount: ", particlesToBeKilledCount, " startIndex: ",
			startIndex, " aliveParticles: ", particleContainer->GetAliveCount());

		std::vector<size_t> indicesToKill(particlesToBeKilledCount);
		if (startIndex + particlesToBeKilledCount > particleContainer->GetAliveCount())
		{
			SeqGenerator seqGenerator(startIndex, particleContainer->GetAliveCount());
			std::generate(indicesToKill.begin(), indicesToKill.end(), seqGenerator);
		}
		else
		{
			std::iota(indicesToKill.begin(), indicesToKill.end(), startIndex);
		}
		particleContainer->Kill(indicesToKill.data(), indicesToKill.size());
	}
}

/* ==================== TimerParticlesKiller class end ==================== */