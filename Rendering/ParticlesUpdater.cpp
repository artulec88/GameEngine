#include "stdafx.h"
#include "ParticlesUpdater.h"

/* ==================== class ParticlesUpdater begin ==================== */
Rendering::ParticlesUpdater::ParticlesUpdater()
{
}


Rendering::ParticlesUpdater::~ParticlesUpdater()
{
}
/* ==================== class ParticlesUpdater end ==================== */

/* ==================== class EulerParticlesUpdater begin ==================== */
Rendering::EulerParticlesUpdater::EulerParticlesUpdater(const Math::Vector3D& acceleration) :
	ParticlesUpdater(),
	m_acceleration(acceleration)
{
}

Rendering::EulerParticlesUpdater::~EulerParticlesUpdater()
{
}

void Rendering::EulerParticlesUpdater::Update(Math::Real deltaTime, ParticleContainer* particlesContainer)
{
	for (size_t i = 0; i < particlesContainer->GetAliveCount(); ++i)
	{
		particlesContainer->IncreaseAcceleration(i, m_acceleration);
	}

	for (size_t i = 0; i < particlesContainer->GetAliveCount(); ++i)
	{
		particlesContainer->IncreaseVelocity(i, particlesContainer->GetAcceleration(i) * deltaTime);
	}

	for (size_t i = 0; i < particlesContainer->GetAliveCount(); ++i)
	{
		particlesContainer->IncreasePosition(i, particlesContainer->GetVelocity(i) * deltaTime);
	}
}
/* ==================== class EulerParticlesUpdater end ==================== */

/* ==================== class LifeSpanParticlesUpdater begin ==================== */
Rendering::LifeSpanParticlesUpdater::LifeSpanParticlesUpdater() :
	ParticlesUpdater()
{
}

Rendering::LifeSpanParticlesUpdater::~LifeSpanParticlesUpdater()
{
}

void Rendering::LifeSpanParticlesUpdater::Update(Math::Real deltaTime, ParticleContainer* particlesContainer)
{
	if (particlesContainer->GetAliveCount() == 0)
	{
		return;
	}

	for (size_t i = 0; i < particlesContainer->GetAliveCount(); ++i)
	{
		particlesContainer->IncreaseLifeSpan(i, -deltaTime);
		if (!particlesContainer->IsAlive(i))
		{
			particlesContainer->Kill(i);
		}
	}
}
/* ==================== class LifeSpanParticlesUpdater end ==================== */