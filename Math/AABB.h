#ifndef __MATH_AABB_H__
#define __MATH_AABB_H__

#include "Math.h"
#include "Vector.h"

namespace math
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
		/// <param name="bottomLeftPos">The bottom-left vertex of the AABB.</param>
		/// <param name="topRightPos">The top-right vertex of the AABB.</param>
		MATH_API AABB(const Vector3D& bottomLeftPos, const Vector3D& topRightPos) noexcept;

		/// <summary>
		/// Axis-aligned bounding box copy constructor.
		/// </summary>
		/// <param name="aabb"> The axis-aligned bounding box to copy construct from. </param>
		MATH_API AABB(const AABB& aabb) noexcept = default;

		/// <summary>
		/// Axis-aligned bounding box move constructor.
		/// </summary>
		/// <param name="aabb"> The axis-aligned bounding box to move construct from. </param>
		MATH_API AABB(AABB&& aabb) noexcept = default;

		/// <summary>
		/// Axis-aligned bounding box copy assignment operator.
		/// </summary>
		/// <param name="aabb"> The axis-aligned bounding box to copy assign from. </param>
		/// <returns> The reference to the newly copy-assigned axis-aligned bounding box. </returns>
		MATH_API AABB& operator=(const AABB& aabb) noexcept = default;

		/// <summary>
		/// Axis-aligned bounding box move assignment operator.
		/// </summary>
		/// <param name="aabb"> The axis-aligned bounding box to move assign from. </param>
		/// <returns> The reference to the newly move-assigned axis-aligned bounding box. </returns>
		MATH_API AABB& operator=(AABB&& aabb) noexcept = default;

		/// <summary>
		/// The destructor of the axis-aligned bounding box.
		/// </summary>
		MATH_API ~AABB();
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static, non-virtual member functions begin ==================== */
	public:
		/// <summary>
		/// Gives information about the potential intersection between the current axis-aligned bounding box and given <paramref name="aabb"/>.
		/// </summary>
		/// <param name="aabb"> The axis-aligned bounding box we want to check the potential intersection against. </param>
		/// <returns> The intersection info. </returns>
		IntersectInfo DoesIntersectAABB(const AABB& aabb) const noexcept;

		/// <summary>
		/// Gives the bottom left position of the axis-aligned bounding box.
		/// </summary>
		/// <returns> The bottom left position of the AABB. </returns>
		inline const Vector3D& GetBottomLeftPos() const noexcept { return m_bottomLeftPos; }

		/// <summary>
		/// Gives the top right position of the axis-aligned bounding box.
		/// </summary>
		/// <returns> The top right position of the AABB. </returns>
		inline const Vector3D& GetTopRightPos() const noexcept { return m_topRightPos; }
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