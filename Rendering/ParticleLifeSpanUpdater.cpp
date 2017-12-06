#include "stdafx.h"
#include "ParticleLifeSpanUpdater.h"


rendering::particles::updaters::ConstantLifeSpanUpdater::ConstantLifeSpanUpdater(math::Real lifeSpanIncrement) :
	LifeSpanUpdater(),
	m_lifeSpanIncrement(lifeSpanIncrement)
{
}


rendering::particles::updaters::ConstantLifeSpanUpdater::~ConstantLifeSpanUpdater()
{
}

void rendering::particles::updaters::ConstantLifeSpanUpdater::Update(math::Real deltaTime, ParticlesContainer* particlesContainer) const
{
	for (size_t i = 0; i < particlesContainer->GetAliveCount(); ++i)
	{
		Set(particlesContainer, i, m_lifeSpanIncrement);
	}

	//for (size_t i = 0; i < particlesContainer->GetAliveCount(); ++i)
	//{
	//	particlesContainer->IncreaseLifeSpan(i, deltaTime);
	//	if (!particlesContainer->IsAlive(i))
	//	{
	//		particlesContainer->Kill(i);
	//	}
	//}
}
