#include "stdafx.h"
#include "PhysicsObject.h"

using namespace Physics;

PhysicsObject::PhysicsObject(Math::Transform& transform, Math::Real mass, const Math::Vector3D& linearVelocity) :
	m_massCenter(/* TODO: must be calculated from given points and point masses */),
	m_mass(mass),
	m_intertia(/* TODO: Specify intertia */),
	m_frictionCoefficientStatic( /* TODO: Specify static friction coefficient */ ),
	m_frictionCoefficientKinetic( /* TODO: Specify static friction coefficient */ ),
	m_restitutionCoefficient( /* TODO: Specify restitution coefficient */ ),
	m_transform(transform),
	m_linearVelocity(linearVelocity),
	m_angularVelocity( /* TODO: Specify angular velocity maybe using quaternions */ ),
	m_totalForce(REAL_ZERO, REAL_ZERO, REAL_ZERO), // TODO: What about gravity?
	m_totalTorque( /* TODO: Specify total torque */ ),
	m_frictionForce( /* TODO: Specify friction force */ ),
	m_totalImpulse( /* TODO: Specify total impulse */ ),
	m_collider(NULL)
{
}

PhysicsObject::~PhysicsObject()
{
	SAFE_DELETE(m_collider);
}

void PhysicsObject::Simulate(Math::Real passedTime)
{
	DELOCUST_LOG("Simulating physics object. Passed time = %.3f", passedTime);
	m_linearVelocity += (m_totalForce / m_mass) * passedTime;
	if (m_totalForce.LengthSquared() > REAL_ZERO)
	{
		//CRITICAL_LOG("Total force = %s", m_totalForce.ToString().c_str());
	}
	//CRITICAL_LOG("Total force = %s; Linear velocity = %s", m_totalForce.ToString().c_str(), m_linearVelocity.ToString().c_str());
	m_transform.SetPos(m_transform.GetPos() + m_linearVelocity * passedTime);

	//m_linearVelocity.Set(REAL_ZERO, REAL_ZERO, REAL_ZERO);

	// Reset forces
	m_totalForce.Set(REAL_ZERO, REAL_ZERO, REAL_ZERO);
	//m_transform.SetPos(m_transform.GetPos() + m_linearVelocity * passedTime);
}

void PhysicsObject::ApplyLinearAcceleration(const Math::Vector3D& acceleration)
{
	//ApplyLinearForce(acceleration * m_mass);
	m_totalForce += acceleration * m_mass;
	//CRITICAL_LOG("Total force = %s", m_totalForce.ToString().c_str());
}

void PhysicsObject::ApplyLinearForce(const Math::Vector3D& force)
{
	////ApplyForce(force, m_massCenter);
	m_totalForce += force;
}

void PhysicsObject::ApplyTorque(Math::Real torque)
{
}

void PhysicsObject::ApplyForce(const Math::Vector3D& force, const Math::Vector3D& contactPoint)
{
}

void PhysicsObject::ApplyFriction(Math::Real gravity)
{
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

void PhysicsObject::HandleCollision(const PhysicsObject& otherPhysicsObject, const Math::Vector3D& collisionNormal)
{
}