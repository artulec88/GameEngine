#ifndef __PHYSICS_PHYSICS_OBJECT_H__
#define __PHYSICS_PHYSICS_OBJECT_H__

#include "Physics.h"
#include "Collider.h"

#include "Math\Vector.h"
#include "Math\Transform.h"
#include "Math\IntersectInfo.h"
#include "Math\Quaternion.h"

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
		PHYSICS_API PhysicsObject(Math::Transform& transform, Math::Real mass, const Math::Vector3D& linearVelocity);
		PHYSICS_API ~PhysicsObject(void);
	private:
		PhysicsObject(const PhysicsObject& physicsObject);
		void operator=(const PhysicsObject& physicsObject);
	/* ==================== Constructors and destructors end ==================== */

	/* ==================== Non-static, non-virtual member functions begin ==================== */
	public:
		/// <summary>
		/// Based on the velocity of the physics object the function updates its physical state.
		/// </summary>
		/// <param name="passedTime">The representation of time for the physics object. It represent the amount of time the object should simulate forward in time.</param>
		PHYSICS_API void Simulate(Math::Real passedTime);

		PHYSICS_API void ApplyLinearAcceleration(const Math::Vector3D& acceleration);
		PHYSICS_API void ApplyLinearForce(const Math::Vector3D& force);
		PHYSICS_API void ApplyTorque(Math::Real torque);
		PHYSICS_API void ApplyForce(const Math::Vector3D& force, const Math::Vector3D& contactPoint);
		PHYSICS_API void ApplyFriction(Math::Real gravity);

		PHYSICS_API inline const Math::Transform& GetTransform() const { return m_transform; }
		PHYSICS_API inline const Math::Vector3D& GetLinearVelocity() const { return m_linearVelocity; }
		PHYSICS_API inline Math::Vector3D& GetLinearVelocity() { return m_linearVelocity; }
		PHYSICS_API inline Math::Real GetAngularVelocity() const { return m_angularVelocity; }
		PHYSICS_API inline Math::Real GetMass() const { return m_mass; }
		PHYSICS_API inline const Math::Vector3D& GetMassCenter() const { return m_massCenter; }
		PHYSICS_API inline Math::Real GetIntertia() const { return m_intertia; }
		PHYSICS_API inline Math::Real GetFrictionCoefficientStatic() const { return m_frictionCoefficientStatic; }
		PHYSICS_API inline Math::Real GetFrictionCoefficientKinetic() const { return m_frictionCoefficientKinetic; }
		PHYSICS_API inline Math::Real GetRestitutionCoefficient() const { return m_restitutionCoefficient; }

		PHYSICS_API void Stop() { m_linearVelocity.Set(REAL_ZERO, REAL_ZERO, REAL_ZERO); }
		PHYSICS_API void SetLinearVelocity(const Math::Vector3D& linearVelocity) { m_linearVelocity = linearVelocity; }

		const Collider* GetCollider() const { return m_collider; }
		void AddCollider(Collider* collider);
		Math::IntersectInfo Intersect(const PhysicsObject& otherPhysicsObject) const;
		void HandleCollision(const PhysicsObject& otherPhysicsObject, const Math::Vector3D& collisionNormal);
	/* ==================== Non-static, non-virtual member functions end ==================== */

	/* ==================== Non-static member variables begin ==================== */
	private:
		// Physical attributes
		/// <summary>
		/// The single point of an object from where all mass distribution is equal, frequently called the center of gravity.
		/// </summary>
		/// <remarks>
		/// The center of mass can be calculated using the following equation:
		/// <code>
		/// CenterOfMass = sum_{i} (Position_{i} * mass_{i}) / sum_{i} (mass_{i}).
		/// </code>
		/// </remarks>
		Math::Vector3D m_massCenter;
		/// <summary>
		/// The mass of an entity. The measurement of how much matter an object has.
		/// Its value does not correlate with the gravitational field.
		/// The mass is always the same regardless if an object is on Earth (greater gravity) or Moon (less gravity).
		/// </summary>
		Math::Real m_mass;
		Math::Real m_intertia;
		Math::Real m_frictionCoefficientStatic;
		Math::Real m_frictionCoefficientKinetic;
		Math::Real m_restitutionCoefficient;

		// Physics variables
		/// <summary>
		/// The transformation of the physical object. It gives information about object's position, rotation and scale.
		/// </summary>
		Math::Transform& m_transform;
		/// <summary>
		/// The change of position of an object during period of time.
		/// Its values tells how much the object is supposed to change its position per unit of time.
		/// </summary>
		Math::Vector3D m_linearVelocity;
		/// <summary>
		/// The change of orientation of an object in a given period of time.
		/// Its values tells how much the object is supposed to change its orientation per unit of time.
		/// </summary>
		Math::Real m_angularVelocity; // TODO: Maybe use quaternion?
		Math::Vector3D m_totalForce;
		Math::Real m_totalTorque;
		Math::Vector3D m_frictionForce;
		Math::Real m_totalImpulse;

		/// <summary>
		/// The collider that the physics object may use to check if it collides with other objects.
		/// If physics object does not have any colliders then it cannot collide with anything (e.g. hence, it may go through walls).
		/// </summary>
		/// <remarks>
		/// In the future this should be turned into a tree hierarchy (or other type of hierarchy) of colliders.
		/// The root collider could be the AABB, next BoundingSphere, etc. It is important that for each pair of parent-child colliders the parent must fully contain the child.
		/// </remarks>
		Collider* m_collider;
	/* ==================== Non-static member variables end ==================== */
	}; /* end class PhysicsObject */

} /* end namespace Physics */

#endif // __PHYSICS_PHYSICS_OBJECT_H__