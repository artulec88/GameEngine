#include "stdafx.h"
#include "ParticlesSystem.h"


#include "Utility\ILogger.h"

Rendering::Particles::ParticlesSystem::ParticlesSystem(size_t maxCount, const ParticleTexture& particleTexture) :
	m_count(maxCount),
	m_particles(maxCount),
	m_emitters(),
	m_updaters(),
	m_texture(particleTexture)
{
}


Rendering::Particles::ParticlesSystem::~ParticlesSystem()
{
}

void Rendering::Particles::ParticlesSystem::Update(Math::Real deltaTime)
{
	DEBUG_LOG_RENDERING("Emitting new particles started");
	//CRITICAL_LOG_RENDERING("Particles before emission = ");
	//ERROR_LOG_RENDERING(m_particles.ToString());
	for (auto& emitter : m_emitters)
	{
		emitter.Emit(deltaTime, &m_particles);
	}

	//constexpr Math::Vector3D zeroVector{ REAL_ZERO, REAL_ZERO, REAL_ZERO };
	//for (size_t i = 0; i < m_count; ++i)
	//{
	//	m_particles.SetAcceleration(i, zeroVector);
	//}

	DEBUG_LOG_RENDERING("Updating particles started");
	//CRITICAL_LOG_RENDERING("Particles after emission, before update = ");
	//ERROR_LOG_RENDERING(m_particles.ToString());
	for (auto& updater : m_updaters)
	{
		updater->Update(deltaTime, &m_particles);
	}
	//CRITICAL_LOG_RENDERING("Particles after update = ");
	//ERROR_LOG_RENDERING(m_particles.ToString());
}

void Rendering::Particles::ParticlesSystem::Reset()
{
	m_particles.Reset();
}
