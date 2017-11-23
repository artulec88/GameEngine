#include "stdafx.h"
#include "ParticlesContainer.h"

rendering::particles::ParticlesContainer::ParticlesContainer() :
	ParticlesContainer(0, attributes::AttributesMask(0))
{
}

rendering::particles::ParticlesContainer::ParticlesContainer(size_t maxCount, const attributes::AttributesMask& attributesMask) :
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
	m_maxCount(maxCount),
	m_countAlive(0)
{
	SetAttributesMask(m_attributesMask);
}

rendering::particles::ParticlesContainer::~ParticlesContainer()
{
}

void rendering::particles::ParticlesContainer::Kill(size_t id)
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

void rendering::particles::ParticlesContainer::Revive(size_t id)
{
	if (m_countAlive < m_maxCount)
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

void rendering::particles::ParticlesContainer::SwapData(size_t a, size_t b)
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

	if (m_attributesMask.IsAttributeEnabled(attributes::POSITION))
	{
		std::swap(m_positions[a], m_positions[b]);
	}
	if (m_attributesMask.IsAttributeEnabled(attributes::VELOCITY))
	{
		std::swap(m_velocities[a], m_velocities[b]);
	}
	if (m_attributesMask.IsAttributeEnabled(attributes::ACCELERATION))
	{
		std::swap(m_accelerations[a], m_accelerations[b]);
	}
	if (m_attributesMask.IsAttributeEnabled(attributes::GRAVITY_EFFECT_FACTOR))
	{
		std::swap(m_gravityEffectFactors[a], m_gravityEffectFactors[b]);
	}
	if (m_attributesMask.IsAttributeEnabled(attributes::LIFE_SPAN))
	{
		std::swap(m_lifeSpanLimits[a], m_lifeSpanLimits[b]);
		std::swap(m_lifeSpans[a], m_lifeSpans[b]);
	}
	if (m_attributesMask.IsAttributeEnabled(attributes::ROTATION))
	{
		std::swap(m_rotations[a], m_rotations[b]);
	}
	if (m_attributesMask.IsAttributeEnabled(attributes::SCALE))
	{
		std::swap(m_scales[a], m_scales[b]);
	}
	if (m_attributesMask.IsAttributeEnabled(attributes::TEXTURE_OFFSET))
	{
		std::swap(m_textureOffsets[a], m_textureOffsets[b]);
	}
	if (m_attributesMask.IsAttributeEnabled(attributes::COLOR))
	{
		std::swap(m_colors[a], m_colors[b]);
	}
	if (m_attributesMask.IsAttributeEnabled(attributes::MASS))
	{
		std::swap(m_masses[a], m_masses[b]);
	}
	if (m_attributesMask.IsAttributeEnabled(attributes::ALIVE))
	{
		std::swap(m_aliveFlags[a], m_aliveFlags[b]);
	}
	if (m_attributesMask.IsAttributeEnabled(attributes::ID))
	{
		std::swap(m_IDs[a], m_IDs[b]);
	}
}

void rendering::particles::ParticlesContainer::SetAttributesMask(attributes::AttributesMask attributesMask)
{
	m_attributesMask = attributesMask;

	//m_positions.reset(new math::Vector3D[m_count]);
	//m_velocities.reset(new math::Vector3D[m_count]);
	//m_accelerations.reset(new math::Vector3D[m_count]);
	//m_gravityEffectFactors.reset(new math::Real[m_count]);
	//m_lifeSpanLimits.reset(new math::Real[m_count]);
	//m_lifeSpans.reset(new math::Real[m_count]);
	//m_rotations.reset(new math::Angle[m_count]);
	//m_scales.reset(new math::Real[m_count]);
	//m_textureOffsets.reset(new math::Vector2D[m_count]);
	//m_colors.reset(new Color[m_count]);
	//m_masses.reset(new math::Real[m_count]);
	//m_aliveFlags.reset(new bool[m_count]);
	//m_IDs.reset(new int[m_count]);

	if (m_attributesMask.IsAttributeEnabled(attributes::POSITION))
	{
		m_positions.reset(new math::Vector3D[m_maxCount]);
		for (size_t i = 0; i < m_maxCount; ++i)
		{
			m_positions[i].Zero();
		}
	}
	if (m_attributesMask.IsAttributeEnabled(attributes::VELOCITY))
	{
		m_velocities.reset(new math::Vector3D[m_maxCount]);
		for (size_t i = 0; i < m_maxCount; ++i)
		{
			m_velocities[i].Zero();
		}
	}
	if (m_attributesMask.IsAttributeEnabled(attributes::ACCELERATION))
	{
		m_accelerations.reset(new math::Vector3D[m_maxCount]);
		for (size_t i = 0; i < m_maxCount; ++i)
		{
			m_accelerations[i].Zero();
		}
	}
	if (m_attributesMask.IsAttributeEnabled(attributes::GRAVITY_EFFECT_FACTOR))
	{
		m_gravityEffectFactors.reset(new math::Real[m_maxCount]);
		for (size_t i = 0; i < m_maxCount; ++i)
		{
			m_gravityEffectFactors[i] = REAL_ZERO;
		}
	}
	if (m_attributesMask.IsAttributeEnabled(attributes::LIFE_SPAN))
	{
		m_lifeSpanLimits.reset(new math::Real[m_maxCount]);
		for (size_t i = 0; i < m_maxCount; ++i)
		{
			m_lifeSpanLimits[i] = REAL_ZERO;
		}
		m_lifeSpans.reset(new math::Real[m_maxCount]);
		for (size_t i = 0; i < m_maxCount; ++i)
		{
			m_lifeSpans[i] = REAL_ZERO;
		}
	}
	if (m_attributesMask.IsAttributeEnabled(attributes::ROTATION))
	{
		m_rotations.reset(new math::Angle[m_maxCount]);
		for (size_t i = 0; i < m_maxCount; ++i)
		{
			m_rotations[i].Set(REAL_ZERO, math::units::RADIAN);
		}
	}
	if (m_attributesMask.IsAttributeEnabled(attributes::SCALE))
	{
		m_scales.reset(new math::Real[m_maxCount]);
		for (size_t i = 0; i < m_maxCount; ++i)
		{
			m_scales[i] = REAL_ONE;
		}
	}
	if (m_attributesMask.IsAttributeEnabled(attributes::TEXTURE_OFFSET))
	{
		m_textureOffsets.reset(new math::Vector2D[m_maxCount]);
		for (size_t i = 0; i < m_maxCount; ++i)
		{
			m_textureOffsets[i].x = REAL_ZERO;
			m_textureOffsets[i].y = REAL_ZERO;
		}
	}
	if (m_attributesMask.IsAttributeEnabled(attributes::COLOR))
	{
		m_colors.reset(new Color[m_maxCount]);
		for (size_t i = 0; i < m_maxCount; ++i)
		{
			m_colors[i].Set(REAL_ONE, REAL_ONE, REAL_ONE, REAL_ONE);
		}
	}
	if (m_attributesMask.IsAttributeEnabled(attributes::MASS))
	{
		m_masses.reset(new math::Real[m_maxCount]);
		for (size_t i = 0; i < m_maxCount; ++i)
		{
			m_masses[i] = REAL_ZERO;
		}
	}
	if (m_attributesMask.IsAttributeEnabled(attributes::ALIVE))
	{
		m_aliveFlags.reset(new bool[m_maxCount]);
		for (size_t i = 0; i < m_maxCount; ++i)
		{
			m_aliveFlags[i] = false;
		}
	}
	if (m_attributesMask.IsAttributeEnabled(attributes::ID))
	{
		m_IDs.reset(new int[m_maxCount]);
		for (size_t i = 0; i < m_maxCount; ++i)
		{
			m_IDs[i] = -1; // some invalid ID
		}
	}
}
