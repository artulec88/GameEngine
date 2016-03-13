#ifndef __PHYSICS_AABB_COLLIDER_H__
#define __PHYSICS_AABB_COLLIDER_H__

#include "Physics.h"
#include "Collider.h"
#include "Math\AABB.h"
#include "Math\IntersectInfo.h"

namespace Physics
{
	/// <summary>
	/// The representation of the AABB collider object.
	/// </summary>
	class AABBCollider : public Collider
	{
	/* ==================== Static variables begin ==================== */
	/* ==================== Static variables end ==================== */

	/* ==================== Constructors and destructors begin ==================== */
	public:
		PHYSICS_API AABBCollider(const Math::Vector3D& bottomLeftPos, const Math::Vector3D& topRightPos);
		PHYSICS_API virtual ~AABBCollider(void);
	private:
		AABBCollider(const AABBCollider& aabbCollider);
		void operator=(const AABBCollider& aabbCollider);
	/* ==================== Constructors and destructors end ==================== */

	/* ==================== Non-static, non-virtual member functions begin ==================== */
	public:
		virtual Math::IntersectInfo Intersect(const Collider& collider) const;
	/* ==================== Non-static, non-virtual member functions end ==================== */

	/* ==================== Non-static member variables begin ==================== */
	private:
		Math::AABB m_aabb;
	/* ==================== Non-static member variables end ==================== */
	}; /* end class AABBCollider */

} /* end namespace Physics */

#endif // __PHYSICS_AABB_COLLIDER_H__