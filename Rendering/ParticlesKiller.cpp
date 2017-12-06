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

void rendering::particles::ParticlesKiller::Kill(math::Real deltaTime, ParticlesContainer* particleContainer) const
{
	for (size_t i = 0; i < particleContainer->GetAliveCount(); ++i)
	{
		// TODO: Instead of removing the dead particles one by one we could collect their indices and
		// then pass those indices of dead particles to particles container to remove them in one go.
		if (!IsAlive(particleContainer, i))
		{
			particleContainer->Kill(i);
		}
	}
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

bool rendering::particles::LifeSpanParticlesKiller::IsAlive(ParticlesContainer* particlesContainer, size_t i) const
{
	return particlesContainer->GetLifeSpan(i) < particlesContainer->GetLifeSpanLimit(i);
}

/* ==================== LifeSpanParticlesKiller class end ==================== */

/* ==================== TimerParticlesKiller class begin ==================== */
//rendering::particles::TimerParticlesKiller::TimerParticlesKiller() :
//	ParticlesKiller()
//{
//}
//
//rendering::particles::TimerParticlesKiller::~TimerParticlesKiller()
//{
//}
/* ==================== TimerParticlesKiller class end ==================== */