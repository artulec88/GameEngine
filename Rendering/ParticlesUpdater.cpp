#include "stdafx.h"
#include "ParticlesUpdater.h"

/* ==================== class ParticlesUpdater begin ==================== */
Rendering::Particles::ParticlesUpdater::ParticlesUpdater()
{
}


Rendering::Particles::ParticlesUpdater::~ParticlesUpdater()
{
}
/* ==================== class ParticlesUpdater end ==================== */

/* ==================== class EulerParticlesUpdater begin ==================== */
Rendering::Particles::EulerParticlesUpdater::EulerParticlesUpdater(const Math::Vector3D& acceleration) :
	ParticlesUpdater(),
	m_acceleration(acceleration)
{
}

Rendering::Particles::EulerParticlesUpdater::~EulerParticlesUpdater()
{
}

void Rendering::Particles::EulerParticlesUpdater::Update(Math::Real deltaTime, ParticlesContainer* particlesContainer)
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
Rendering::Particles::LifeSpanParticlesUpdater::LifeSpanParticlesUpdater() :
	ParticlesUpdater()
{
}

Rendering::Particles::LifeSpanParticlesUpdater::~LifeSpanParticlesUpdater()
{
}

void Rendering::Particles::LifeSpanParticlesUpdater::Update(Math::Real deltaTime, ParticlesContainer* particlesContainer)
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
Rendering::Particles::RotationParticlesUpdater::RotationParticlesUpdater(const Math::Angle& rotationSpeedAngle) :
	ParticlesUpdater(),
	m_rotationSpeedAngle(rotationSpeedAngle)
{
}

Rendering::Particles::RotationParticlesUpdater::~RotationParticlesUpdater()
{
}

void Rendering::Particles::RotationParticlesUpdater::Update(Math::Real deltaTime, ParticlesContainer* particlesContainer)
{
	for (size_t i = 0; i < particlesContainer->GetAliveCount(); ++i)
	{
		particlesContainer->IncreaseRotation(i, m_rotationSpeedAngle * deltaTime);
	}
}
/* ==================== class RotationParticlesUpdater end ==================== */

/* ==================== class ScaleEffectParticlesUpdater begin ==================== */
Rendering::Particles::ScaleEffectParticlesUpdater::ScaleEffectParticlesUpdater(Math::Effects::Effect<Math::Real>* effect) :
	ParticlesUpdater(),
	m_effect(effect)
{
}

Rendering::Particles::ScaleEffectParticlesUpdater::~ScaleEffectParticlesUpdater()
{
}

void Rendering::Particles::ScaleEffectParticlesUpdater::Update(Math::Real deltaTime, ParticlesContainer* particlesContainer)
{
	m_effect->Update(deltaTime);
	for (size_t i = 0; i < particlesContainer->GetAliveCount(); ++i)
	{
		m_effect->Apply(particlesContainer->GetScalePtr(i));
	}
}
/* ==================== class ScaleEffectParticlesUpdater end ==================== */