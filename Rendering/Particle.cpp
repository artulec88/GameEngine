#include "stdafx.h"
#include "Particle.h"
#include "Texture.h"

/* static */ const Math::Real Rendering::Particle::GRAVITY_ACCELERATION = -9.8f; // TODO: The same is defined in the GravityComponent class.

Rendering::Particle::Particle() :
	m_position(REAL_ZERO, REAL_ZERO, REAL_ZERO),
	m_velocity(REAL_ZERO, REAL_ZERO, REAL_ZERO),
	m_gravityEffectFactor(REAL_ZERO),
	m_lifeSpanLimit(REAL_ZERO),
	m_lifeSpan(REAL_ONE /* to make the particle dead we make the lifespan greater than lifespan limit */),
	m_rotation(REAL_ZERO),
	m_scale(REAL_ZERO)
{
}

Rendering::Particle::Particle(const Math::Vector3D& position, const Math::Vector3D& velocity,
	Math::Real gravityEffectFactor, Math::Real lifespanLimit, const Math::Angle& rotation, Math::Real scale) :
	m_position(position),
	m_velocity(velocity),
	m_gravityEffectFactor(gravityEffectFactor),
	m_lifeSpanLimit(lifespanLimit),
	m_lifeSpan(REAL_ZERO),
	m_rotation(rotation),
	m_scale(scale)
{
}


Rendering::Particle::~Particle()
{
}

void Rendering::Particle::Revive(const Math::Vector3D& position, const Math::Vector3D& velocity,
	Math::Real gravityEffectFactor, Math::Real lifespanLimit, const Math::Angle& rotation, Math::Real scale)
{
	m_position = position;
	m_velocity = velocity;
	m_gravityEffectFactor = gravityEffectFactor;
	m_lifeSpanLimit = lifespanLimit;
	m_lifeSpan = REAL_ZERO;
	m_rotation = rotation;
	m_scale = scale;
}

bool Rendering::Particle::Update(Math::Real deltaTime)
{
	m_velocity.SetY(m_velocity.GetY() + GRAVITY_ACCELERATION * m_gravityEffectFactor * deltaTime);
	m_position += m_velocity * deltaTime;
	m_lifeSpan += deltaTime;

	// TODO: Calculate CalculateTextureAtlasInfo here?

	return m_lifeSpan < m_lifeSpanLimit;
	//return IsAlive();
}

void Rendering::Particle::CalculateTextureAtlasInfo(int textureAtlasRowsCount, Math::Vector2D& textureOffset0, Math::Vector2D& textureOffset1, Math::Real& textureAtlasBlendFactor) const
{
	/* Updating the texture offsets and the blend factor begin */
	Math::Real lifeSpanFactor = m_lifeSpan / m_lifeSpanLimit;
	int stageCount = textureAtlasRowsCount * textureAtlasRowsCount;
	Math::Real textureAtlasProgression = lifeSpanFactor * stageCount;
	int index0 = Math::Floor(textureAtlasProgression);
	int index1 = (index0 == stageCount - 1) ? index0 : index0 + 1;
	textureAtlasBlendFactor = textureAtlasProgression - static_cast<int>(textureAtlasProgression); // fractional part of the variable
	
	SetTextureOffset(textureOffset0, index0, textureAtlasRowsCount);
	SetTextureOffset(textureOffset1, index1, textureAtlasRowsCount);
	/* Updating the texture offsets and the blend factor end */
}