#include "stdafx.h"
#include "PhysicsObject.h"

using namespace Physics;

PhysicsObject::PhysicsObject(const Math::Vector3D& position, const Math::Vector3D& velocity) :
	m_position(position),
	m_velocity(velocity),
	m_collider(NULL)
{
}

PhysicsObject::~PhysicsObject()
{
}

void PhysicsObject::Integrate(Math::Real passedTime)
{
	m_position += m_velocity * passedTime;
}

void PhysicsObject::AddCollider(Collider* collider)
{
	// TODO: For now there is only one collider supported.
	m_collider = collider;
}

Math::IntersectInfo PhysicsObject::Intersect(const PhysicsObject& otherPhysicsObject) const
{
	if ((m_collider == NULL) || (otherPhysicsObject.GetCollider() == NULL))
	{
		return Math::IntersectInfo(false, 0.0f /* some arbitrary value */);
	}
	return m_collider->Intersect(*otherPhysicsObject.GetCollider());
}