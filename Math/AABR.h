#ifndef __MATH_AABR_H__
#define __MATH_AABR_H__

#include "Math.h"
#include "Vector.h"

namespace Math
{

	class IntersectInfo;

	/// <summary>
	/// The mathematical representation of the axis-aligned bounding rectangle for 2D objects.
	/// The AABR's are very similar to the AABB's. They are simply the 2D representation of the AABB.
	/// An axis-aligned bounding rectangle (AABR) is a 2D rectangle whose four faces are aligned with a particular
	/// coordinate frame's mutually orthogonal axes. As such, an AABR can be represented by a four values
	/// containing the minimum and maximum coordinates along each of the 2 principal axes,
	/// <code>[x_min, x_max, y_min, y_max]</code>, or two 2D points <code>P_min</code> and <code>P_max</code>.
	/// </summary>
	/// <remarks>
	/// See section 4.6.4 "Axis-Aligned Bounding Boxes (AABB)" in "Game Engine Architecture" by Jason Gregory.
	/// </remarks>
	class AABR
	{
	/* ==================== Static variables begin ==================== */
	/* ==================== Static variables end ==================== */

	/* ==================== Constructors and destructors begin ==================== */
	public:
		/// <summary>
		/// The constructor of the axis-aligned bounding rectangle. It takes two parameters representing the bottom-left and top-right positions of the AABR object
		/// (<paramref name="bottomLeftPos"/> and <paramref name="topRightPos"/> respectively).
		/// </summary>
		/// <param name='bottomLeftPos'>The bottom-left vertex of the AABR.</param>
		/// <param name='topRightPos'>The top-right vertex of the AABR.</param>
		MATH_API AABR(const Vector2D& bottomLeftPos, const Vector2D& topRightPos);

		/// <summary>
		/// The destructor of the axis-aligned bounding rectangle.
		/// </summary>
		MATH_API ~AABR(void);
	private:
		//IntersectInfo(const IntersectInfo& intersectInfo) {} // don't implement
		//void operator=(const IntersectInfo& intersectInfo) {} // don't implement
	/* ==================== Constructors and destructors end ==================== */

	/* ==================== Non-static, non-virtual member functions begin ==================== */
	public:
		MATH_API IntersectInfo DoesContainPoint(Math::Real x, Math::Real y) const;
		MATH_API IntersectInfo DoesContainPoint(const Math::Vector2D& point) const;
		MATH_API IntersectInfo DoesIntersectAABR(const AABR& aabr) const;
		MATH_API inline const Vector2D& GetBottomLeftPos() const { return m_bottomLeftPos; }
		MATH_API inline const Vector2D& GetTopRightPos() const { return m_topRightPos; }
	private:
		Math::Real CalcDistanceToNearestFaceX(Math::Real x) const;
		Math::Real CalcDistanceToNearestFaceY(Math::Real y) const;
	/* ==================== Non-static, non-virtual member functions end ==================== */

	/* ==================== Non-static member variables begin ==================== */
	private:
		/// <summary>
		/// Represents the 2D position of the bottom-left vertex of the AABR.
		/// </summary>
		Vector2D m_bottomLeftPos;

		/// <summary>
		/// Represents the 2D position of the top-right vertex of the AABR.
		/// </summary>
		Vector2D m_topRightPos;
	/* ==================== Non-static member variables end ==================== */
	}; /* end class AABR */

} /* end namespace Math */

#endif /* __MATH_AABR_H__ */