#ifndef __MATH_SPHERE_H__
#define __MATH_SPHERE_H__

#include "Math.h"
#include "Vector.h"

namespace Math
{

class IntersectInfo;

/// <summary>
/// A mathematical representation of a sphere.
/// </summary>
/// <remarks>
/// The sphere is based on the section 4.6.2 "Spheres" in "Game Engine Architecture" by Jason Gregory.
/// As 4 floating-point numbers are used to describe a sphere there are distinct benefits to being able
/// to pack data into a vector containing 32-bit floats (read more about SIMD vector processing).
/// </remarks>
class Sphere
{
/* ==================== Static variables begin ==================== */
/* ==================== Static variables end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	MATH_API Sphere(const Vector3D& center, Real radius);
	MATH_API ~Sphere(void);
private:
	//Sphere(const Sphere& sphere) {} // don't implement
	//void operator=(const Sphere& sphere) {} // don't implement
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static, non-virtual member functions begin ==================== */
public:
	MATH_API IntersectInfo DoesIntersectSphere(const Sphere& sphere) const;
	MATH_API inline const Math::Vector3D& GetCenter() const { return m_center; }
	MATH_API inline const Math::Real GetRadius() const { return m_radius; }
/* ==================== Non-static, non-virtual member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
private:
	const Vector3D m_center;
	const Real m_radius;
/* ==================== Non-static member variables end ==================== */
}; /* end class Sphere */

} /* end namespace Math */

#endif /* __MATH_SPHERE_H__ */