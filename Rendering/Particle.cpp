#include "stdafx.h"
#include "Particle.h"

/* static */ const Math::Real Rendering::Particle::GRAVITY_ACCELERATION = -9.8f; // TODO: The same is defined in the GravityComponent class.

Rendering::Particle::Particle(const Math::Vector3D& position, const Math::Vector3D& velocity, Math::Real gravityEffectFactor, Math::Real lifespanLimit, Math::Real rotation, Math::Real scale) :
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

bool Rendering::Particle::Update(Math::Real deltaTime)
{
	m_velocity.SetY(m_velocity.GetY() + GRAVITY_ACCELERATION * m_gravityEffectFactor * deltaTime);
	m_position += m_velocity * deltaTime;
	m_lifeSpan += deltaTime;
	return m_lifeSpan < m_lifeSpanLimit;
	//return IsAlive();
}