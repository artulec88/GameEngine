#ifndef __MATH_INTERSECT_INFO_H__
#define __MATH_INTERSECT_INFO_H__

#include "Math.h"

namespace Math
{

	class IntersectInfo
	{
		/* ==================== Static variables begin ==================== */
		/* ==================== Static variables end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		/// <summary> Intersection information object constructor. </summary>
		/// <param name="distance">
		/// The distance that remains between two objects (if they do not intersect) or the distance they need to move away
		/// from each other to stop intersecting. The positive value means no intersection and negative values mean that
		/// two objects intersect with one another.
		/// </param>
		MATH_API IntersectInfo(Real distance);

		/// <summary> Intersection information object destructor. </summary>
		MATH_API ~IntersectInfo();

		/// <summary> Intersection information copy constructor. </summary>
		/// <param name="intersectInfo"> The intersection information object to copy construct from. </param>
		MATH_API IntersectInfo(const IntersectInfo& intersectInfo) noexcept = default;

		/// <summary> Intersection information move constructor. </summary>
		/// <param name="intersectInfo"> The intersection information object to move construct from. </param>
		MATH_API IntersectInfo(IntersectInfo&& intersectInfo) noexcept = default;

		/// <summary> Intersection information copy assignment operator. </summary>
		/// <param name="intersectInfo"> The intersection information object to copy assign from. </param>
		/// <returns> The reference to the newly copy-assigned intersection information. </returns>
		MATH_API IntersectInfo& operator=(const IntersectInfo& intersectInfo) noexcept = default;

		/// <summary> Intersection information move assignment operator. </summary>
		/// <param name="intersectInfo"> The intersection information object to move assign from. </param>
		/// <returns> The reference to the newly move-assigned intersection information. </returns>
		MATH_API IntersectInfo& operator=(IntersectInfo&& intersectInfo) noexcept = default;
	/* ==================== Constructors and destructors end ==================== */

	/* ==================== Non-static, non-virtual member functions begin ==================== */
	public:
		MATH_API inline bool IsIntersecting() const { /* return m_distance < REAL_ZERO; */return m_isIntersecting; }
		MATH_API inline Real GetDistance() const { return m_distance; }
		/* ==================== Non-static, non-virtual member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	private:
		/// <summary>
		/// The flag indicating whether two objects intersect with each other (<code>true</code>) or not (<code>false</code>).
		/// </summary>
		/// <remarks>
		/// TODO: It may be reasonable to remove this member variable since the intersection can be tested with a simple check
		/// whether the m_distance member variable is positive or negative.
		/// </remarks>
		bool m_isIntersecting;
		Real m_distance;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class IntersectInfo */

} /* end namespace Math */

#endif /* __MATH_INTERSECT_INFO_H__ */