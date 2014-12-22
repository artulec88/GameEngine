#pragma once

#include "Physics.h"
#include "IntersectInfo.h"
#include "BoundingSphere.h"
#include "Math\Vector.h"

namespace Physics
{

class PHYSICS_API Plane
{
/* ==================== Static variables begin ==================== */
/* ==================== Static variables end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	Plane(const Math::Vector3D& normal, Math::Real distance);
	~Plane(void);
private:
	//Plane(const Plane& plane) {} // don't implement
	//void operator=(const Plane& plane) {} // don't implement
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static, non-virtual member functions begin ==================== */
public:
	IntersectInfo IntersectBoundingSphere(const BoundingSphere& boundingSphere) const;

	Plane Normalized() const;

	inline const Math::Vector3D& GetNormal() const { return m_normal; }
	inline Math::Real GetDistance() const { return m_distance; }
/* ==================== Non-static, non-virtual member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
private:
	const Math::Vector3D m_normal; // represents the normal vector of the infinite plane
	const Math::Real m_distance; // represents how far along the normal vector the plane lies
/* ==================== Non-static member variables end ==================== */
}; /* end class Plane */

} /* end namespace Physics */
