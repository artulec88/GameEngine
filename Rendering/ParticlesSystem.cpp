#include "stdafx.h"
#include "ParticlesSystem.h"
#include "Texture.h"
#include "Shader.h"
#include "ShaderIDs.h"
#include "TextureIDs.h"

#include "Utility\ILogger.h"

Rendering::Particles::ParticlesSystem::ParticlesSystem() :
	ParticlesSystem(0, Attributes::AttributesMask(0), TextureIDs::INVALID, ShaderIDs::INVALID)
{
}

Rendering::Particles::ParticlesSystem::ParticlesSystem(size_t maxCount, Attributes::AttributesMask attributesMask, int particleTextureID, int particleShaderID) :
	m_count(maxCount),
	m_particles(maxCount, attributesMask),
	m_emitters(),
	m_updaters(),
	m_textureID(particleTextureID),
	m_shaderID(particleShaderID)
{
	// TODO: Determine m_shaderID based on the given attributesMask
}


Rendering::Particles::ParticlesSystem::~ParticlesSystem()
{
}

void Rendering::Particles::ParticlesSystem::Update(Math::Real deltaTime)
{
	DEBUG_LOG_RENDERING("Emitting new particles started");
	//CRITICAL_LOG_RENDERING("Particles before emission = ");
	//ERROR_LOG_RENDERING(m_particles);
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
	//ERROR_LOG_RENDERING(m_particles);
	for (auto& updater : m_updaters)
	{
		updater->Update(deltaTime, &m_particles);
	}
	//CRITICAL_LOG_RENDERING("Particles after update = ");
	//ERROR_LOG_RENDERING(m_particles);
}

void Rendering::Particles::ParticlesSystem::Reset()
{
	m_particles.Reset();
}
