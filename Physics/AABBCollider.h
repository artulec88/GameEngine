#ifndef __PHYSICS_AABB_COLLIDER_H__
#define __PHYSICS_AABB_COLLIDER_H__

#include "Physics.h"
#include "Collider.h"
#include "Math/AABB.h"
#include "Math/IntersectInfo.h"

namespace physics
{
	/// <summary>
	/// The representation of the AABB collider object.
	/// </summary>
	class AabbCollider : public Collider
	{
	/* ==================== Static variables begin ==================== */
	/* ==================== Static variables end ==================== */

	/* ==================== Constructors and destructors begin ==================== */
	public:
		PHYSICS_API AabbCollider(const math::Vector3D& bottomLeftPos, const math::Vector3D& topRightPos);
		PHYSICS_API virtual ~AabbCollider();
	
		/// <summary> The copy constructor of the AABB collider. </summary>
		/// <param name="aabbCollider"> The AABB collider to copy construct from. </param>
		AabbCollider(const AabbCollider& aabbCollider) = delete;

		/// <summary> The move constructor of the AABB collider. </summary>
		/// <param name="aabbCollider"> The r-value reference of the AABB collider to move construct from. </param>
		AabbCollider(AabbCollider&& aabbCollider) = delete;

		/// <summary> The copy assignment operator of the AABB collider. </summary>
		/// <param name="aabbCollider"> The AABB collider to copy assign from. </param>
		/// <returns> The reference to the newly copy-assigned <code>AABBCollider</code> object. </returns>
		AabbCollider& operator=(const AabbCollider& aabbCollider) = delete;

		/// <summary> The move assignment operator of the AABB collider. </summary>
		/// <param name="aabbCollider"> The r-value reference of the AABB collider to move assign from. </param>
		/// <returns> The reference to the newly move-assigned <code>AABBCollider</code> object. </returns>
		AabbCollider& operator=(AabbCollider&& aabbCollider) = delete;
	/* ==================== Constructors and destructors end ==================== */

	/* ==================== Non-static, non-virtual member functions begin ==================== */
		math::IntersectInfo Intersect(const Collider& collider) const override;
	/* ==================== Non-static, non-virtual member functions end ==================== */

	/* ==================== Non-static member variables begin ==================== */
	private:
		math::AABB m_aabb;
	/* ==================== Non-static member variables end ==================== */
	}; /* end class AABBCollider */

} /* end namespace physics */

#endif // __PHYSICS_AABB_COLLIDER_H__