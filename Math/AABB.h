#ifndef __MATH_AABB_H__
#define __MATH_AABB_H__

#include "Math.h"
#include "Vector.h"

namespace Math
{

class IntersectInfo;

/// <summary>
/// The mathematical representation of the axis-aligned bounding box.
/// An axis-aligned bounding box (AABB) is a 3D cuboid whose six rectangular faces are aligned with a particular
/// coordinate frame's mutually orthogonal axes. As such, an AABB can be represented by a six-element vector
/// containing the minimum and maximum coordinates along each of the 3 principal axes,
/// <code>[x_min, x_max, y_min, y_max, z_min, z_max]</code>, or two points <code>P_min</code> and <code>P_max</code>.
/// </summary>
/// <remarks>
/// See section 4.6.4 "Axis-Aligned Bounding Boxes (AABB)" in "Game Engine Architecture" by Jason Gregory.
/// </remarks>
class AABB
{
/* ==================== Static variables begin ==================== */
/* ==================== Static variables end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	/// <summary>
	/// The constructor of the axis-aligned bounding box. It takes two parameters representing the bottom-left and top-right positions of the AABB object
	/// (<paramref name="bottomLeftPos"/> and <paramref name="topRightPos"/> respectively).
	/// </summary>
	/// <param name='bottomLeftPos'>The bottom-left vertex of the AABB.</param>
	/// <param name='topRightPos'>The top-right vertex of the AABB.</param>
	MATH_API AABB(const Vector3D& bottomLeftPos, const Vector3D& topRightPos);

	/// <summary>
	/// The destructor of the axis-aligned bounding box.
	/// </summary>
	MATH_API ~AABB(void);
private:
	//IntersectInfo(const IntersectInfo& intersectInfo) {} // don't implement
	//void operator=(const IntersectInfo& intersectInfo) {} // don't implement
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static, non-virtual member functions begin ==================== */
public:
	IntersectInfo DoesIntersectAABB(const AABB& aabb) const;
	inline const Vector3D& GetBottomLeftPos() const { return m_bottomLeftPos; }
	inline const Vector3D& GetTopRightPos() const { return m_topRightPos; }
/* ==================== Non-static, non-virtual member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
private:
	/// <summary>
	/// Represents the 3D position of the bottom-left vertex of the AABB.
	/// </summary>
	Vector3D m_bottomLeftPos;

	/// <summary>
	/// Represents the 3D position of the top-right vertex of the AABB.
	/// </summary>
	Vector3D m_topRightPos;
/* ==================== Non-static member variables end ==================== */
}; /* end class AABB */

} /* end namespace Math */

#endif /* __MATH_AABB_H__ */