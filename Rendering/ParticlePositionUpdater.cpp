#include "stdafx.h"
#include "ParticlePositionUpdater.h"


rendering::particles::updaters::EulerParticlesUpdater::EulerParticlesUpdater(const math::Vector3D& acceleration) :
	m_acceleration(acceleration)
{
}


rendering::particles::updaters::EulerParticlesUpdater::~EulerParticlesUpdater()
{
}

void rendering::particles::updaters::EulerParticlesUpdater::Update(math::Real deltaTime, ParticlesContainer* particlesContainer) const
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