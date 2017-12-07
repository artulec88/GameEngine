#include "stdafx.h"
#include "PhysicsObject.h"
#include "Collider.h"

#include "Math/Transform.h"
#include "Math/IntersectInfo.h"

#include "Utility/ILogger.h"

physics::PhysicsObject::PhysicsObject(math::Transform& transform, math::Real mass, const math::Vector3D& linearVelocity) :
	m_massCenter(/* TODO: must be calculated from given points and point masses */),
	m_mass(mass),
	m_intertia(/* TODO: Specify intertia */),
	m_frictionCoefficientStatic( /* TODO: Specify static friction coefficient */),
	m_frictionCoefficientKinetic( /* TODO: Specify static friction coefficient */),
	m_restitutionCoefficient( /* TODO: Specify restitution coefficient */),
	m_transform(transform),
	m_linearVelocity(linearVelocity),
	m_angularVelocity( /* TODO: Specify angular velocity maybe using quaternions */),
	m_totalForce(REAL_ZERO, REAL_ZERO, REAL_ZERO), // TODO: What about gravity?
	m_totalTorque( /* TODO: Specify total torque */),
	m_frictionForce( /* TODO: Specify friction force */),
	m_totalImpulse( /* TODO: Specify total impulse */),
	m_aabbCollider(nullptr)
{
}

physics::PhysicsObject::~PhysicsObject()
{
	SAFE_DELETE(m_aabbCollider);
}

void physics::PhysicsObject::Simulate(math::Real passedTime)
{
	DELOCUST_LOG_PHYSICS("Simulating physics object. Passed time = ", passedTime);
	m_linearVelocity += m_totalForce / m_mass * passedTime;
	if (m_totalForce.LengthSquared() > REAL_ZERO)
	{
		//CRITICAL_LOG_PHYSICS("Total force = \"", m_totalForce.ToString(), "\".");
	}
	//CRITICAL_LOG_PHYSICS("Total force = \"", m_totalForce.ToString(), "\"; Linear velocity = \"", m_linearVelocity.ToString(), "\".");
	m_transform.SetPos(m_transform.GetPos() + m_linearVelocity * passedTime);

	//m_linearVelocity.Set(REAL_ZERO, REAL_ZERO, REAL_ZERO);

	// Reset forces
	m_totalForce.Zero();
	//m_transform.SetPos(m_transform.GetPos() + m_linearVelocity * passedTime);
}

void physics::PhysicsObject::ApplyLinearAcceleration(const math::Vector3D& acceleration)
{
	//ApplyLinearForce(acceleration * m_mass);
	m_totalForce += acceleration * m_mass;
	//CRITICAL_LOG_PHYSICS("Total force = \"", m_totalForce.ToString(), "\".");
}

void physics::PhysicsObject::ApplyLinearForce(const math::Vector3D& force)
{
	////ApplyForce(force, m_massCenter);
	m_totalForce += force;
}

void physics::PhysicsObject::ApplyTorque(math::Real torque)
{
}

void physics::PhysicsObject::ApplyForce(const math::Vector3D& force, const math::Vector3D& contactPoint)
{
}

void physics::PhysicsObject::ApplyFriction(math::Real gravity)
{
}

void physics::PhysicsObject::AddCollider(Collider* collider)
{
	// TODO: For now there is only one collider supported.
	m_aabbCollider = collider;
}

math::IntersectInfo physics::PhysicsObject::Intersect(const PhysicsObject& otherPhysicsObject) const
{
	if (m_aabbCollider == nullptr || otherPhysicsObject.GetAabbCollider() == nullptr)
	{
		return math::IntersectInfo(REAL_ONE /* some arbitrary (but positive) value */);
	}
	return m_aabbCollider->Intersect(*otherPhysicsObject.GetAabbCollider());
}

void physics::PhysicsObject::HandleCollision(const PhysicsObject& otherPhysicsObject, const math::Vector3D& collisionNormal)
{
}