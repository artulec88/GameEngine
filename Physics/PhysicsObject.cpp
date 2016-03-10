#include "stdafx.h"
#include "PhysicsObject.h"

using namespace Physics;

PhysicsObject::PhysicsObject(Math::Transform& transform, Math::Real mass, const Math::Vector3D& velocity) :
	m_transform(transform),
	m_mass(mass),
	m_velocity(velocity),
	m_collider(NULL)
{
}

PhysicsObject::~PhysicsObject()
{
}

void PhysicsObject::Integrate(Math::Real passedTime)
{
	m_transform.SetPos(m_transform.GetPos() + m_velocity * passedTime);
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