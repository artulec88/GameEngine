#include "stdafx.h"
#include "ParticleColorUpdater.h"


rendering::particles::updaters::ConstantColorUpdater::ConstantColorUpdater(const Color& color) :
	ColorUpdater(),
	m_color(color)
{
}


rendering::particles::updaters::ConstantColorUpdater::~ConstantColorUpdater()
{
}

void rendering::particles::updaters::ConstantColorUpdater::Update(math::Real deltaTime, ParticlesContainer* particlesContainer) const
{
	for (size_t i = 0; i < particlesContainer->GetAliveCount(); ++i)
	{
		particlesContainer->SetColor(i, m_color);
	}
}
