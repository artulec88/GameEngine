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
		MATH_API IntersectInfo(bool isIntersecting, Real distance);
		MATH_API ~IntersectInfo(void);
	private:
		//IntersectInfo(const IntersectInfo& intersectInfo) {} // don't implement
		//void operator=(const IntersectInfo& intersectInfo) {} // don't implement
	/* ==================== Constructors and destructors end ==================== */

	/* ==================== Non-static, non-virtual member functions begin ==================== */
	public:
		MATH_API inline bool IsIntersecting() const { return m_isIntersecting; }
		MATH_API inline Real GetDistance() const { return m_distance; }
		/* ==================== Non-static, non-virtual member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	private:
		const bool m_isIntersecting;
		const Real m_distance;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class IntersectInfo */

} /* end namespace Math */

#endif /* __MATH_INTERSECT_INFO_H__ */