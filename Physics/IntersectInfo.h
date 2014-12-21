#pragma once

#include "Physics.h"
#include "Math\Math.h"

namespace Physics
{

class PHYSICS_API IntersectInfo
{
/* ==================== Static variables begin ==================== */
/* ==================== Static variables end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	IntersectInfo(const bool isIntersecting, const Math::Real distance);
	~IntersectInfo(void);
private:
	//IntersectInfo(const IntersectInfo& intersectInfo) {} // don't implement
	//void operator=(const IntersectInfo& intersectInfo) {} // don't implement
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static, non-virtual member functions begin ==================== */
public:
	inline bool IsIntersecting() const { return m_isIntersecting; }
	inline Math::Real GetDistance() const { return m_distance; }
/* ==================== Non-static, non-virtual member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
private:
	const bool m_isIntersecting;
	const Math::Real m_distance;
/* ==================== Non-static member variables end ==================== */
}; /* end class IntersectInfo */

} /* end namespace Physics */