#include "stdafx.h"
#include "ParticleSystem.h"


Rendering::ParticleSystem::ParticleSystem(size_t maxCount) :
	m_count(maxCount),
	m_particles(maxCount),
	m_emitters(),
	m_updaters()
{
}


Rendering::ParticleSystem::~ParticleSystem()
{
}

void Rendering::ParticleSystem::Update(Math::Real deltaTime)
{
	for (auto& emitter : m_emitters)
	{
		emitter.Emit(deltaTime, &m_particles);
	}

	constexpr Math::Vector3D zeroVector{ REAL_ZERO, REAL_ZERO, REAL_ZERO };
	for (size_t i = 0; i < m_count; ++i)
	{
		m_particles.SetAcceleration(i, zeroVector);
	}

	for (auto& updater : m_updaters)
	{
		updater->Update(deltaTime, &m_particles);
	}
}

void Rendering::ParticleSystem::Reset()
{
	m_particles.Reset();
}
