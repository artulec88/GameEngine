#include "stdafx.h"
#include "ParticleRotationUpdater.h"

/* ==================== ConstantRotationParticlesUpdater class begin ==================== */
rendering::particles::updaters::ConstantRotationParticlesUpdater::ConstantRotationParticlesUpdater(const math::Angle& angle) :
	m_angle(angle)
{
}


rendering::particles::updaters::ConstantRotationParticlesUpdater::~ConstantRotationParticlesUpdater()
{
}

void rendering::particles::updaters::ConstantRotationParticlesUpdater::Update(math::Real deltaTime, ParticlesContainer* particlesContainer) const
{
	for (size_t i = 0; i < particlesContainer->GetAliveCount(); ++i)
	{
		Increase(particlesContainer, i, m_angle * deltaTime);
	}
}
/* ==================== ConstantRotationParticlesUpdater class end ==================== */
