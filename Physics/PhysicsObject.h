#pragma once

#include "Physics.h"
#include "Math\Vector.h"

namespace Physics
{

/**
 * The representation of the physics object
 */
class PhysicsObject
{
/* ==================== Static variables begin ==================== */
/* ==================== Static variables end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	PHYSICS_API PhysicsObject(const Math::Vector3D& postion, const Math::Vector3D& velocity);
	PHYSICS_API ~PhysicsObject(void);
private:
	//PhysicsObject(const PhysicsObject& physicObject) {} // don't implement
	//void operator=(const PhysicsObject& physicObject) {} // don't implement
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static, non-virtual member functions begin ==================== */
public:
	PHYSICS_API void Integrate(Math::Real delta);
	PHYSICS_API inline const Math::Vector3D& GetPosition() const { return m_position; }
	PHYSICS_API inline const Math::Vector3D& GetVelocity() const { return m_velocity; }
/* ==================== Non-static, non-virtual member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
private:
	Math::Vector3D m_position;
	Math::Vector3D m_velocity;
	Math::Real radius;
/* ==================== Non-static member variables end ==================== */
}; /* end class PhysicsObject */

} /* end namespace Physics */