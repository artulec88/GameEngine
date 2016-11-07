#include "stdafx.h"
#include "ParticlesContainer.h"

Rendering::Particles::ParticlesContainer::ParticlesContainer() :
	ParticlesContainer(0, Attributes::AttributesMask(0))
{
}

Rendering::Particles::ParticlesContainer::ParticlesContainer(size_t maxCount, const Attributes::AttributesMask& attributesMask) :
	m_attributesMask(attributesMask),
	m_positions(nullptr),
	m_velocities(nullptr),
	m_accelerations(nullptr),
	m_gravityEffectFactors(nullptr),
	m_lifeSpanLimits(nullptr),
	m_lifeSpans(nullptr),
	m_rotations(nullptr),
	m_scales(nullptr),
	m_textureOffsets(nullptr),
	m_colors(nullptr),
	m_masses(nullptr),
	m_aliveFlags(nullptr),
	m_IDs(nullptr),
	m_count(maxCount),
	m_countAlive(0)
{
	SetAttributesMask(m_attributesMask);
}

Rendering::Particles::ParticlesContainer::~ParticlesContainer()
{
}

void Rendering::Particles::ParticlesContainer::Kill(size_t id)
{
	if (m_countAlive > 0)
	{
		//DEBUG_LOG_RENDERING("Killing particle #", id);
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
		//DEBUG_LOG_RENDERING("Reviving particle #", id);
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
	//std::swap(m_positions[a], m_positions[b]);
	//std::swap(m_velocities[a], m_velocities[b]);
	//std::swap(m_accelerations[a], m_accelerations[b]);
	//std::swap(m_gravityEffectFactors[a], m_gravityEffectFactors[b]);
	//std::swap(m_lifeSpanLimits[a], m_lifeSpanLimits[b]);
	//std::swap(m_lifeSpans[a], m_lifeSpans[b]);
	//std::swap(m_rotations[a], m_rotations[b]);
	//std::swap(m_scales[a], m_scales[b]);
	//std::swap(m_textureOffsets[a], m_textureOffsets[b]);
	//std::swap(m_colors[a], m_colors[b]);
	//std::swap(m_masses[a], m_masses[b]);
	//std::swap(m_aliveFlags[a], m_aliveFlags[b]);
	//std::swap(m_IDs[a], m_IDs[b]);

	if (m_attributesMask.IsAttributeEnabled(Attributes::POSITION))
	{
		std::swap(m_positions[a], m_positions[b]);
	}
	if (m_attributesMask.IsAttributeEnabled(Attributes::VELOCITY))
	{
		std::swap(m_velocities[a], m_velocities[b]);
	}
	if (m_attributesMask.IsAttributeEnabled(Attributes::ACCELERATION))
	{
		std::swap(m_accelerations[a], m_accelerations[b]);
	}
	if (m_attributesMask.IsAttributeEnabled(Attributes::GRAVITY_EFFECT_FACTOR))
	{
		std::swap(m_gravityEffectFactors[a], m_gravityEffectFactors[b]);
	}
	if (m_attributesMask.IsAttributeEnabled(Attributes::LIFE_SPAN))
	{
		std::swap(m_lifeSpanLimits[a], m_lifeSpanLimits[b]);
		std::swap(m_lifeSpans[a], m_lifeSpans[b]);
	}
	if (m_attributesMask.IsAttributeEnabled(Attributes::ROTATION))
	{
		std::swap(m_rotations[a], m_rotations[b]);
	}
	if (m_attributesMask.IsAttributeEnabled(Attributes::SCALE))
	{
		std::swap(m_scales[a], m_scales[b]);
	}
	if (m_attributesMask.IsAttributeEnabled(Attributes::TEXTURE_OFFSET))
	{
		std::swap(m_textureOffsets[a], m_textureOffsets[b]);
	}
	if (m_attributesMask.IsAttributeEnabled(Attributes::COLOR))
	{
		std::swap(m_colors[a], m_colors[b]);
	}
	if (m_attributesMask.IsAttributeEnabled(Attributes::MASS))
	{
		std::swap(m_masses[a], m_masses[b]);
	}
	if (m_attributesMask.IsAttributeEnabled(Attributes::ALIVE))
	{
		std::swap(m_aliveFlags[a], m_aliveFlags[b]);
	}
	if (m_attributesMask.IsAttributeEnabled(Attributes::ID))
	{
		std::swap(m_IDs[a], m_IDs[b]);
	}
}

void Rendering::Particles::ParticlesContainer::SetAttributesMask(Attributes::AttributesMask attributesMask)
{
	m_attributesMask = attributesMask;

	//m_positions.reset(new Math::Vector3D[m_count]);
	//m_velocities.reset(new Math::Vector3D[m_count]);
	//m_accelerations.reset(new Math::Vector3D[m_count]);
	//m_gravityEffectFactors.reset(new Math::Real[m_count]);
	//m_lifeSpanLimits.reset(new Math::Real[m_count]);
	//m_lifeSpans.reset(new Math::Real[m_count]);
	//m_rotations.reset(new Math::Angle[m_count]);
	//m_scales.reset(new Math::Real[m_count]);
	//m_textureOffsets.reset(new Math::Vector2D[m_count]);
	//m_colors.reset(new Color[m_count]);
	//m_masses.reset(new Math::Real[m_count]);
	//m_aliveFlags.reset(new bool[m_count]);
	//m_IDs.reset(new int[m_count]);

	if (m_attributesMask.IsAttributeEnabled(Attributes::POSITION))
	{
		m_positions.reset(new Math::Vector3D[m_count]);
		for (size_t i = 0; i < m_count; ++i)
		{
			m_positions[i].Set(REAL_ZERO, REAL_ZERO, REAL_ZERO);
		}
	}
	if (m_attributesMask.IsAttributeEnabled(Attributes::VELOCITY))
	{
		m_velocities.reset(new Math::Vector3D[m_count]);
		for (size_t i = 0; i < m_count; ++i)
		{
			m_velocities[i].Set(REAL_ZERO, REAL_ZERO, REAL_ZERO);
		}
	}
	if (m_attributesMask.IsAttributeEnabled(Attributes::ACCELERATION))
	{
		m_accelerations.reset(new Math::Vector3D[m_count]);
		for (size_t i = 0; i < m_count; ++i)
		{
			m_accelerations[i].Set(REAL_ZERO, REAL_ZERO, REAL_ZERO);
		}
	}
	if (m_attributesMask.IsAttributeEnabled(Attributes::GRAVITY_EFFECT_FACTOR))
	{
		m_gravityEffectFactors.reset(new Math::Real[m_count]);
		for (size_t i = 0; i < m_count; ++i)
		{
			m_gravityEffectFactors[i] = REAL_ZERO;
		}
	}
	if (m_attributesMask.IsAttributeEnabled(Attributes::LIFE_SPAN))
	{
		m_lifeSpanLimits.reset(new Math::Real[m_count]);
		for (size_t i = 0; i < m_count; ++i)
		{
			m_lifeSpanLimits[i] = REAL_ZERO;
		}
		m_lifeSpans.reset(new Math::Real[m_count]);
		for (size_t i = 0; i < m_count; ++i)
		{
			m_lifeSpans[i] = REAL_ZERO;
		}
	}
	if (m_attributesMask.IsAttributeEnabled(Attributes::ROTATION))
	{
		m_rotations.reset(new Math::Angle[m_count]);
		for (size_t i = 0; i < m_count; ++i)
		{
			m_rotations[i].Set(REAL_ZERO, Math::Unit::RADIAN);
		}
	}
	if (m_attributesMask.IsAttributeEnabled(Attributes::SCALE))
	{
		m_scales.reset(new Math::Real[m_count]);
		for (size_t i = 0; i < m_count; ++i)
		{
			m_scales[i] = REAL_ONE;
		}
	}
	if (m_attributesMask.IsAttributeEnabled(Attributes::TEXTURE_OFFSET))
	{
		m_textureOffsets.reset(new Math::Vector2D[m_count]);
		for (size_t i = 0; i < m_count; ++i)
		{
			m_textureOffsets[i].Set(REAL_ZERO, REAL_ZERO);
		}
	}
	if (m_attributesMask.IsAttributeEnabled(Attributes::COLOR))
	{
		m_colors.reset(new Color[m_count]);
		for (size_t i = 0; i < m_count; ++i)
		{
			m_colors[i].Set(REAL_ONE, REAL_ONE, REAL_ONE, REAL_ONE);
		}
	}
	if (m_attributesMask.IsAttributeEnabled(Attributes::MASS))
	{
		m_masses.reset(new Math::Real[m_count]);
		for (size_t i = 0; i < m_count; ++i)
		{
			m_masses[i] = REAL_ZERO;
		}
	}
	if (m_attributesMask.IsAttributeEnabled(Attributes::ALIVE))
	{
		m_aliveFlags.reset(new bool[m_count]);
		for (size_t i = 0; i < m_count; ++i)
		{
			m_aliveFlags[i] = false;
		}
	}
	if (m_attributesMask.IsAttributeEnabled(Attributes::ID))
	{
		m_IDs.reset(new int[m_count]);
		for (size_t i = 0; i < m_count; ++i)
		{
			m_IDs[i] = -1; // some invalid ID
		}
	}
}
