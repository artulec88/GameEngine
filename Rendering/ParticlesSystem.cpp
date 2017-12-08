#include "stdafx.h"
#include "ParticlesSystem.h"
#include "Texture.h"
#include "Shader.h"

#include "Utility/ILogger.h"

rendering::particles::ParticlesSystem::ParticlesSystem(const ParticlesSystemBuilder& particlesSystemBuilder) :
	m_maxCount(particlesSystemBuilder.GetMaxCount()),
	m_particles(particlesSystemBuilder.GetMaxCount(), particlesSystemBuilder.GetAttributesMask()),
	m_emitters(particlesSystemBuilder.GetEmitters()),
	m_updaters(particlesSystemBuilder.GetUpdaters()),
	m_killer(particlesSystemBuilder.GetKiller()),
	m_textureId(particlesSystemBuilder.GetTextureId()),
	m_textureAtlasRowsCount(particlesSystemBuilder.GetTextureAtlasRowsCount()),
	m_isAdditive(particlesSystemBuilder.IsAdditive()),
	m_shaderId(particlesSystemBuilder.GetShaderId())
{
	// TODO: Determine m_shaderID based on the given attributesMask
}

rendering::particles::ParticlesSystem::~ParticlesSystem()
{
}

void rendering::particles::ParticlesSystem::Update(math::Real deltaTime)
{
	DEBUG_LOG_RENDERING("Killing particles started. Delta time = ", deltaTime);
	if (m_killer != nullptr)
	{
		m_killer->Kill(deltaTime, &m_particles);
	}

	DEBUG_LOG_RENDERING("Updating particles started. Delta time = ", deltaTime);
	//CRITICAL_LOG_RENDERING("Particles after emission, before update = ");
	//ERROR_LOG_RENDERING(m_particles);
	for (auto& updater : m_updaters)
	{
		updater->Update(deltaTime, &m_particles);
	}
	//CRITICAL_LOG_RENDERING("Particles after update = ");
	//ERROR_LOG_RENDERING(m_particles);

	DEBUG_LOG_RENDERING("Emitting new particles started. Delta time = ", deltaTime);
	//CRITICAL_LOG_RENDERING("Particles before emission = ");
	//ERROR_LOG_RENDERING(m_particles);
	for (auto& emitter : m_emitters)
	{
		emitter.Emit(deltaTime, &m_particles);
	}

	//constexpr math::Vector3D zeroVector{ REAL_ZERO, REAL_ZERO, REAL_ZERO };
	//for (size_t i = 0; i < m_count; ++i)
	//{
	//	m_particles.SetAcceleration(i, zeroVector);
	//}
}

void rendering::particles::ParticlesSystem::Reset()
{
	m_particles.Reset();
}
