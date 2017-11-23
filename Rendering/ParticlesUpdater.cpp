#include "stdafx.h"
#include "ParticlesUpdater.h"

/* ==================== class ParticlesUpdater begin ==================== */
rendering::particles::ParticlesUpdater::ParticlesUpdater()
{
}


rendering::particles::ParticlesUpdater::~ParticlesUpdater()
{
}
/* ==================== class ParticlesUpdater end ==================== */

/* ==================== class EulerParticlesUpdater begin ==================== */
rendering::particles::EulerParticlesUpdater::EulerParticlesUpdater(const math::Vector3D& acceleration) :
	ParticlesUpdater(),
	m_acceleration(acceleration)
{
}

rendering::particles::EulerParticlesUpdater::~EulerParticlesUpdater()
{
}

void rendering::particles::EulerParticlesUpdater::Update(math::Real deltaTime, ParticlesContainer* particlesContainer)
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
rendering::particles::LifeSpanParticlesUpdater::LifeSpanParticlesUpdater() :
	ParticlesUpdater()
{
}

rendering::particles::LifeSpanParticlesUpdater::~LifeSpanParticlesUpdater()
{
}

void rendering::particles::LifeSpanParticlesUpdater::Update(math::Real deltaTime, ParticlesContainer* particlesContainer)
{
	for (size_t i = 0; i < particlesContainer->GetAliveCount(); ++i)
	{
		particlesContainer->IncreaseLifeSpan(i, deltaTime);
		if (!particlesContainer->IsAlive(i))
		{
			particlesContainer->Kill(i);
		}
	}
}
/* ==================== class LifeSpanParticlesUpdater end ==================== */

/* ==================== class RotationParticlesUpdater begin ==================== */
rendering::particles::RotationParticlesUpdater::RotationParticlesUpdater(const math::Angle& rotationSpeedAngle) :
	ParticlesUpdater(),
	m_rotationSpeedAngle(rotationSpeedAngle)
{
}

rendering::particles::RotationParticlesUpdater::~RotationParticlesUpdater()
{
}

void rendering::particles::RotationParticlesUpdater::Update(math::Real deltaTime, ParticlesContainer* particlesContainer)
{
	for (size_t i = 0; i < particlesContainer->GetAliveCount(); ++i)
	{
		particlesContainer->IncreaseRotation(i, m_rotationSpeedAngle * deltaTime);
	}
}
/* ==================== class RotationParticlesUpdater end ==================== */

/* ==================== class ScaleEffectParticlesUpdater begin ==================== */
rendering::particles::ScaleEffectParticlesUpdater::ScaleEffectParticlesUpdater(math::effects::Effect<math::Real>* effect) :
	ParticlesUpdater(),
	m_effect(effect)
{
}

rendering::particles::ScaleEffectParticlesUpdater::~ScaleEffectParticlesUpdater()
{
}

void rendering::particles::ScaleEffectParticlesUpdater::Update(math::Real deltaTime, ParticlesContainer* particlesContainer)
{
	m_effect->Update(deltaTime);
	for (size_t i = 0; i < particlesContainer->GetAliveCount(); ++i)
	{
		m_effect->Apply(particlesContainer->GetScalePtr(i));
	}
}
/* ==================== class ScaleEffectParticlesUpdater end ==================== */