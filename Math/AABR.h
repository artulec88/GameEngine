#ifndef __MATH_AABR_H__
#define __MATH_AABR_H__

#include "Math.h"
#include "Vector.h"
#include "IntersectInfo.h"

namespace Math
{

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
		MATH_API AABR(const Vector2D& bottomLeftPos, const Vector2D& topRightPos) noexcept;

		/// <summary>
		/// Axis-aligned bounding rectangle copy constructor.
		/// </summary>
		/// <param name="aabr"> The axis-aligned bounding rectangle to copy construct from. </param>
		MATH_API AABR(const AABR& aabr) noexcept = default;

		/// <summary>
		/// Axis-aligned bounding rectangle move constructor.
		/// </summary>
		/// <param name="aabr"> The r-value reference to the axis-aligned bounding rectangle to move construct from. </param>
		MATH_API AABR(AABR&& aabr) noexcept = default;

		/// <summary>
		/// Axis-aligned bounding rectangle copy assignment operator.
		/// </summary>
		/// <param name="aabr"> The reference to the axis-aligned bounding rectangle to copy assign from. </param>
		/// <returns> The reference to the newly copy-assigned axis-aligned bounding rectangle. </returns>
		MATH_API AABR& operator=(const AABR& aabr) noexcept = default;

		/// <summary>
		/// Axis-aligned bounding rectangle move assignment operator.
		/// </summary>
		/// <param name="aabr"> The r-value reference to the axis-aligned bounding rectangle to move assign from. </param>
		/// <returns> The reference to the newly move-assigned axis-aligned bounding rectangle. </returns>
		MATH_API AABR& operator=(AABR&& aabr) noexcept = default;

		/// <summary>
		/// The destructor of the axis-aligned bounding rectangle.
		/// </summary>
		MATH_API ~AABR(void);
	/* ==================== Constructors and destructors end ==================== */

	/* ==================== Non-static, non-virtual member functions begin ==================== */
	public:
		/// <summary>
		/// Gives information about the mutual location between the current axis-aligned bounding rectangle and
		/// the 2D position given by the parameters <paramref name="x"/> and <paramref name="y"/>.
		/// </summary>
		/// <param name="x">
		/// The X component of the 2D point for which we want to know whether it is inside or outside the AABR and what distance it is from the nearest edge of the AABR.
		/// </param>
		/// <param name="y">
		/// The Y component of the 2D point for which we want to know whether it is inside or outside the AABR and what distance it is from the nearest edge of the AABR.
		/// </param>
		/// <returns> The intersection info. </returns>
		MATH_API IntersectInfo DoesContainPoint(Math::Real x, Math::Real y) const noexcept;

		/// <summary>
		/// Gives information about the mutual location between the current axis-aligned bounding rectangle and given 2D point <paramref name="point"/>.
		/// </summary>
		/// <param name="point">
		/// The 2D point for which we want to know whether it is inside or outside the AABR and what distance it is from the nearest edge of the AABR.
		/// </param>
		/// <returns> The intersection info. </returns>
		MATH_API IntersectInfo DoesContainPoint(const Math::Vector2D& point) const noexcept { return DoesContainPoint(point.x, point.y); }

		/// <summary>
		/// Gives information about the potential intersection between the current axis-aligned bounding rectangle and given <paramref name="aabr"/>.
		/// </summary>
		/// <param name="aabr"> The axis-aligned bounding rectangle we want to check the potential intersection against. </param>
		/// <returns> The intersection info. </returns>
		MATH_API IntersectInfo DoesIntersectAABR(const AABR& aabr) const noexcept;

		/// <summary>
		/// Gives the bottom left position of the axis-aligned bounding rectangle.
		/// </summary>
		/// <returns> The bottom left position of the AABR. </returns>
		MATH_API inline const Vector2D& GetBottomLeftPos() const noexcept { return m_bottomLeftPos; }

		/// <summary>
		/// Gives the top right position of the axis-aligned bounding rectangle.
		/// </summary>
		/// <returns> The top right position of the AABR. </returns>
		MATH_API inline const Vector2D& GetTopRightPos() const noexcept { return m_topRightPos; }

		/// <summary>
		/// Sets the bottom left position of the axis-aligned bounding rectangle.
		/// </summary>
		/// <param name="bottomLeftPos"> The 3D position that is to be the bottom left vertex of the AABR. </param>
		MATH_API void SetBottomLeftPos(const Math::Vector2D& bottomLeftPos) noexcept { m_bottomLeftPos = bottomLeftPos; }

		/// <summary>
		/// Sets the top right position of the axis-aligned bounding rectangle.
		/// </summary>
		/// <param name="topRightPos"> The 3D position that is to be the top right vertex of the AABR. </param>
		MATH_API void SetTopRightPos(const Math::Vector2D& topRightPos) noexcept { m_topRightPos = topRightPos; }
	private:
		Math::Real CalcDistanceToNearestFaceX(Math::Real x) const noexcept;
		Math::Real CalcDistanceToNearestFaceY(Math::Real y) const noexcept;
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