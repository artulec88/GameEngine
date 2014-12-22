#pragma once

#include "Physics.h"
#include "IntersectInfo.h"
#include "Math\Vector.h"

namespace Physics
{

/**
 * Axis-aligned bounding box
 */
class PHYSICS_API AABB
{
/* ==================== Static variables begin ==================== */
/* ==================== Static variables end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	AABB(const Math::Vector3D& minExtents, const Math::Vector3D& maxExtents);
	~AABB(void);
private:
	//AABB(const AABB& aabb) {} // don't implement
	//void operator=(const AABB& aabb) {} // don't implement
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static, non-virtual member functions begin ==================== */
public:
	IntersectInfo IntersectAABB(const AABB& aabb) const;
	inline const Math::Vector3D& GetMinExtents() const { return m_minExtents; }
	inline const Math::Vector3D& GetMaxExtents() const { return m_maxExtents; }
/* ==================== Non-static, non-virtual member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
private:
	const Math::Vector3D m_minExtents; // represents the bottom left point position
	const Math::Vector3D m_maxExtents; // represents the top right point position
/* ==================== Non-static member variables end ==================== */
}; /* end class AABB */

} /* end namespace Physics */
