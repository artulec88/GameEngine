#include "stdafx.h"
#include "ParticlesContainer.h"

Rendering::Particles::ParticlesContainer::ParticlesContainer(size_t maxCount, const Attributes::AttributesMask& attributesMask /* = Attributes::POSITION | Attributes::VELOCITY */) :
	m_count(maxCount),
	m_countAlive(0)
{
	m_positions.reset(new Math::Vector3D[m_count]);
	m_velocities.reset(new Math::Vector3D[m_count]);
	m_accelerations.reset(new Math::Vector3D[m_count]);
	m_gravityEffectFactors.reset(new Math::Real[m_count]);
	m_lifeSpanLimits.reset(new Math::Real[m_count]);
	m_lifeSpans.reset(new Math::Real[m_count]);
	m_rotations.reset(new Math::Angle[m_count]);
	m_scales.reset(new Math::Real[m_count]);
	m_textureOffsets.reset(new Math::Vector2D[m_count]);
}

Rendering::Particles::ParticlesContainer::~ParticlesContainer()
{
}

void Rendering::Particles::ParticlesContainer::Kill(size_t id)
{
	if (m_countAlive > 0)
	{
		//m_alive[id] = false;
		SwapData(id, m_countAlive - 1);
		--m_countAlive;
	}
}

void Rendering::Particles::ParticlesContainer::Revive(size_t id)
{
	if (m_countAlive < m_count)
	{
		//m_alive[id] = true;
		SwapData(id, m_countAlive);
		++m_countAlive;
	}
}

void Rendering::Particles::ParticlesContainer::SwapData(size_t a, size_t b)
{
	std::swap(m_positions[a], m_positions[b]);
	std::swap(m_velocities[a], m_velocities[b]);
	std::swap(m_accelerations[a], m_accelerations[b]);
	std::swap(m_gravityEffectFactors[a], m_gravityEffectFactors[b]);
	std::swap(m_lifeSpanLimits[a], m_lifeSpanLimits[b]);
	std::swap(m_lifeSpans[a], m_lifeSpans[b]);
	std::swap(m_rotations[a], m_rotations[b]);
	std::swap(m_scales[a], m_scales[b]);
	std::swap(m_textureOffsets[a], m_textureOffsets[b]);
}