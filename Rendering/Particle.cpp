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
#ifdef PROFILING_RENDERING_MODULE_ENABLED
	, m_classStats(STATS_STORAGE.GetClassStats("Particle"))
#endif
{
	START_PROFILING_RENDERING(false, "1");
	STOP_PROFILING_RENDERING("1");
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
#ifdef PROFILING_RENDERING_MODULE_ENABLED
	, m_classStats(STATS_STORAGE.GetClassStats("Particle"))
#endif
{
	START_PROFILING_RENDERING(false, "2");
	STOP_PROFILING_RENDERING("2");
}


Rendering::Particle::~Particle()
{
	START_PROFILING_RENDERING(false, "");
	STOP_PROFILING_RENDERING("");
}

Rendering::Particle::Particle(const Particle& particle) :
	m_position(particle.m_position),
	m_velocity(particle.m_velocity),
	m_gravityEffectFactor(particle.m_gravityEffectFactor),
	m_lifeSpanLimit(particle.m_lifeSpanLimit),
	m_lifeSpan(particle.m_lifeSpan),
	m_rotation(particle.m_rotation),
	m_scale(particle.m_scale)
#ifdef PROFILING_RENDERING_MODULE_ENABLED
	, m_classStats(STATS_STORAGE.GetClassStats("Particle"))
#endif
{
	START_PROFILING_RENDERING(false, "3");
	STOP_PROFILING_RENDERING("3");
}

Rendering::Particle::Particle(Particle&& particle) :
	m_position(std::move(particle.m_position)),
	m_velocity(std::move(particle.m_velocity)),
	m_gravityEffectFactor(std::move(particle.m_gravityEffectFactor)),
	m_lifeSpanLimit(std::move(particle.m_lifeSpanLimit)),
	m_lifeSpan(std::move(particle.m_lifeSpan)),
	m_rotation(std::move(particle.m_rotation)),
	m_scale(std::move(particle.m_scale))
#ifdef PROFILING_RENDERING_MODULE_ENABLED
	, m_classStats(STATS_STORAGE.GetClassStats("Particle"))
#endif
{
	START_PROFILING_RENDERING(false, "4");
	STOP_PROFILING_RENDERING("4");
}

Rendering::Particle& Rendering::Particle::operator=(const Particle& particle)
{
	START_PROFILING_RENDERING(false, "1");
	m_position = particle.m_position;
	m_velocity = particle.m_velocity;
	m_gravityEffectFactor = particle.m_gravityEffectFactor;
	m_lifeSpanLimit = particle.m_lifeSpanLimit;
	m_lifeSpan = particle.m_lifeSpan;
	m_rotation = particle.m_rotation;
	m_scale = particle.m_scale;
	STOP_PROFILING_RENDERING("1");
	return *this;
}

Rendering::Particle& Rendering::Particle::operator=(Particle&& particle)
{
	START_PROFILING_RENDERING(false, "2");
	m_position = std::move(particle.m_position);
	m_velocity = std::move(particle.m_velocity);
	m_gravityEffectFactor = std::move(particle.m_gravityEffectFactor);
	m_lifeSpanLimit = std::move(particle.m_lifeSpanLimit);
	m_lifeSpan = std::move(particle.m_lifeSpan);
	m_rotation = std::move(particle.m_rotation);
	m_scale = std::move(particle.m_scale);
	STOP_PROFILING_RENDERING("2");
	return *this;
}

void Rendering::Particle::Revive(const Math::Vector3D& position, const Math::Vector3D& velocity,
	Math::Real gravityEffectFactor, Math::Real lifespanLimit, const Math::Angle& rotation, Math::Real scale)
{
	START_PROFILING_RENDERING(false, "");
	m_position = position;
	m_velocity = velocity;
	m_gravityEffectFactor = gravityEffectFactor;
	m_lifeSpanLimit = lifespanLimit;
	m_lifeSpan = REAL_ZERO;
	m_rotation = rotation;
	m_scale = scale;
	STOP_PROFILING_RENDERING("");
}

bool Rendering::Particle::Update(Math::Real deltaTime)
{
	START_PROFILING_RENDERING(false, "");
	m_velocity.SetY(m_velocity.GetY() + GRAVITY_ACCELERATION * m_gravityEffectFactor * deltaTime);
	m_position += m_velocity * deltaTime;
	m_lifeSpan += deltaTime;

	// TODO: Calculate CalculateTextureAtlasInfo here?

	STOP_PROFILING_RENDERING("");
	return m_lifeSpan < m_lifeSpanLimit;
	//return IsAlive();
}

void Rendering::Particle::CalculateTextureAtlasInfo(int textureAtlasRowsCount, Math::Vector2D& textureOffset0, Math::Vector2D& textureOffset1, Math::Real& textureAtlasBlendFactor) const
{
	START_PROFILING_RENDERING(false, "");
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

	STOP_PROFILING_RENDERING("");
}