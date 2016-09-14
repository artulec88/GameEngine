#include "stdafx.h"
#include "ParticlesContainer.h"

Rendering::Particles::ParticlesContainer::ParticlesContainer(size_t maxCount, const Attributes::AttributesMask& attributesMask /* = Attributes::POSITION | Attributes::VELOCITY | Attributes::LIFE_SPAN */) :
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
	m_IDs.reset(new int[m_count]);
	// TODO: Temporary loop. Remove it in the future.
	for (size_t i = 0; i < m_count; ++i)
	{
		m_rotations[i] = Math::Angle(0.0f);
		m_scales[i] = 1.0f;
		m_IDs[i] = -1; // invalid value
	}
}

Rendering::Particles::ParticlesContainer::~ParticlesContainer()
{
}

void Rendering::Particles::ParticlesContainer::Kill(size_t id)
{
	if (m_countAlive > 0)
	{
		//m_alive[id] = false;
		if (id != m_countAlive - 1)
		{
			SwapData(id, m_countAlive - 1);
		}
		--m_countAlive;
	}
}

void Rendering::Particles::ParticlesContainer::Revive(size_t id)
{
	if (m_countAlive < m_count)
	{
		//m_alive[id] = true;
		if (id != m_countAlive)
		{
			SwapData(id, m_countAlive);
		}
		++m_countAlive;
	}
}

void Rendering::Particles::ParticlesContainer::SwapData(size_t a, size_t b)
{
	DELOCUST_LOG_RENDERING("Swapping data between particle #", a, " and particle #", b);
	std::swap(m_positions[a], m_positions[b]);
	std::swap(m_velocities[a], m_velocities[b]);
	std::swap(m_accelerations[a], m_accelerations[b]);
	std::swap(m_gravityEffectFactors[a], m_gravityEffectFactors[b]);
	std::swap(m_lifeSpans[a], m_lifeSpans[b]);
	std::swap(m_rotations[a], m_rotations[b]);
	std::swap(m_scales[a], m_scales[b]);
	std::swap(m_textureOffsets[a], m_textureOffsets[b]);
	std::swap(m_IDs[a], m_IDs[b]);
}

std::string Rendering::Particles::ParticlesContainer::ToString() const
{
	std::stringstream ss("");
	for (size_t i = 0; i < m_count; ++i)
	{
		ss << std::endl << "p[" << i << "] = { id = " << m_IDs[i] << "; pos = " << m_positions[i].ToString() << "; vel = " <<
			m_velocities[i].ToString() << "; lifeSpan = " << m_lifeSpans[i] << " }";
	}
	return ss.str();
}