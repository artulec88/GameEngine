#pragma once

#include "Physics.h"
#include "Collider.h"

#include "Math\Vector.h"
#include "Math\IntersectInfo.h"

namespace Physics
{

	/// <summary>
	/// The representation of the physics object.
	/// </summary>
	class PhysicsObject
	{
	/* ==================== Static variables begin ==================== */
	/* ==================== Static variables end ==================== */

	/* ==================== Constructors and destructors begin ==================== */
	public:
		PHYSICS_API PhysicsObject(Math::Real mass, Math::Vector3D& postion, const Math::Vector3D& velocity);
		PHYSICS_API ~PhysicsObject(void);
	private:
		//PhysicsObject(const PhysicsObject& physicObject) {} // don't implement
		//void operator=(const PhysicsObject& physicObject) {} // don't implement
	/* ==================== Constructors and destructors end ==================== */

	/* ==================== Non-static, non-virtual member functions begin ==================== */
	public:
		/// <summary>
		/// Based on the velocity of the physics object the function updates its position.
		/// </summary>
		/// <param name="passedTime">The representation of time for the physics object. It represent the amount of time the object should simulate forward in time.</param>
		PHYSICS_API void Integrate(Math::Real passedTime);
		PHYSICS_API Math::Real GetMass() const { return m_mass; }
		PHYSICS_API inline const Math::Vector3D& GetPosition() const { return m_position; }
		PHYSICS_API inline const Math::Vector3D& GetVelocity() const { return m_velocity; }
		PHYSICS_API inline Math::Vector3D& GetVelocity() { return m_velocity; }
		PHYSICS_API void SetVelocity(const Math::Vector3D& velocity) { m_velocity = velocity; }

		const Collider* GetCollider() const { return m_collider; }
		void AddCollider(Collider* collider);
		Math::IntersectInfo Intersect(const PhysicsObject& otherPhysicsObject) const;
	/* ==================== Non-static, non-virtual member functions end ==================== */

	/* ==================== Non-static member variables begin ==================== */
	private:
		Math::Real m_mass;
		Math::Vector3D m_position;
		Math::Vector3D m_velocity;
		
		/// <summary>
		/// The collider that the physics object may use to check if it collides with other objects.
		/// </summary>
		/// <remarks>
		/// In the future this should be turned into a tree hierarchy (or other type of hierarchy) of colliders.
		/// The root collider could be the AABB, next BoundingSphere, etc. It is important that for each pair of parent-child colliders the parent must fully contain the child.
		/// </remarks>
		Collider* m_collider;
	/* ==================== Non-static member variables end ==================== */
	}; /* end class PhysicsObject */

} /* end namespace Physics */