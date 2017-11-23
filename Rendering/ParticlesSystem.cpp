#include "stdafx.h"
#include "ParticlesSystem.h"
#include "Texture.h"
#include "Shader.h"
#include "ShaderIDs.h"
#include "TextureIDs.h"

#include "Utility/ILogger.h"

rendering::particles::ParticlesSystem::ParticlesSystem() :
	ParticlesSystem(0, attributes::AttributesMask(0), texture_ids::INVALID, shader_ids::INVALID)
{
}

rendering::particles::ParticlesSystem::ParticlesSystem(size_t maxCount, attributes::AttributesMask attributesMask, int particleTextureId, int particleShaderId) :
	m_maxCount(maxCount),
	m_particles(maxCount, attributesMask),
	m_emitters(),
	m_updaters(),
	m_textureId(particleTextureId),
	m_shaderId(particleShaderId)
{
	// TODO: Determine m_shaderID based on the given attributesMask
}


rendering::particles::ParticlesSystem::~ParticlesSystem()
{
}

void rendering::particles::ParticlesSystem::Update(math::Real deltaTime)
{
	DEBUG_LOG_RENDERING("Emitting new particles started");
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

	DEBUG_LOG_RENDERING("Updating particles started");
	//CRITICAL_LOG_RENDERING("Particles after emission, before update = ");
	//ERROR_LOG_RENDERING(m_particles);
	for (auto& updater : m_updaters)
	{
		updater->Update(deltaTime, &m_particles);
	}
	//CRITICAL_LOG_RENDERING("Particles after update = ");
	//ERROR_LOG_RENDERING(m_particles);
}

void rendering::particles::ParticlesSystem::Reset()
{
	m_particles.Reset();
}
