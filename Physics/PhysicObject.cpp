#include "stdafx.h"
#include "PhysicsObject.h"

using namespace Physics;

PhysicsObject::PhysicsObject(const Math::Vector3D& position, const Math::Vector3D& velocity) :
	m_position(position),
	m_velocity(velocity)
{
}

PhysicsObject::~PhysicsObject()
{
}

void PhysicsObject::Integrate(Math::Real delta)
{
	m_position += m_velocity * delta;
}