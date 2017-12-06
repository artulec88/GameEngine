#include "stdafx.h"
#include "ParticlesKiller.h"
#include "ParticlesContainer.h"

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
	for (size_t i = 0; i < particleContainer->GetAliveCount(); ++i)
	{
		if (particleContainer->GetLifeSpan(i) >= particleContainer->GetLifeSpanLimit(i))
		{
			particleContainer->Kill(i);
		}
	}
}

/* ==================== LifeSpanParticlesKiller class end ==================== */

/* ==================== TimerParticlesKiller class begin ==================== */
rendering::particles::TimerParticlesKiller::TimerParticlesKiller(math::Real particlesToKillPerSecond) :
	ParticlesKiller(),
	m_timeToKillOneParticle(REAL_ONE / particlesToKillPerSecond),
	m_currentTimer(REAL_ZERO)
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
		const auto endId = std::min(maxParticlesToBeKilledCount, particleContainer->GetAliveCount());

		//ERROR_LOG_RENDERING("Delta time = ", deltaTime, "[ms]. MaxNewParticles = ", maxNewParticles, ". StartId = ", startId, ". EndId = ",
		//	endId, ". Alive = ", particleContainer->GetAliveCount(), ". All = ", particleContainer->GetCount());

		for (auto i = 0; i < endId; ++i)
		{
			particleContainer->Kill(i);
		}
	}
}
/* ==================== TimerParticlesKiller class end ==================== */