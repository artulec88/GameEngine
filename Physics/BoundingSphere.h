#pragma once

#include "Physics.h"
#include "IntersectInfo.h"
#include "Math\Vector.h"

namespace Physics
{

class PHYSICS_API BoundingSphere
{
/* ==================== Static variables begin ==================== */
/* ==================== Static variables end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	BoundingSphere(const Math::Vector3D& center, Math::Real radius);
	~BoundingSphere(void);
private:
	//BoundingSphere(const BoundingSphere& boundingSphere) {} // don't implement
	//void operator=(const BoundingSphere& boundingSphere) {} // don't implement
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static, non-virtual member functions begin ==================== */
public:
	IntersectInfo IntersectBoundingSphere(const BoundingSphere& boundingSphere) const;
	inline const Math::Vector3D& GetCenter() const { return m_center; }
	inline const Math::Real GetRadius() const { return m_radius; }
/* ==================== Non-static, non-virtual member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
private:
	const Math::Vector3D m_center;
	const Math::Real m_radius;
/* ==================== Non-static member variables end ==================== */
}; /* end class BoundingSphere */

} /* end namespace Physics */
