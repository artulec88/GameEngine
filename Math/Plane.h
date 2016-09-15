#ifndef __MATH_PLANE_H__
#define __MATH_PLANE_H__

#include "Math.h"
#include "Vector.h"
#include "FloatingPoint.h"

namespace Math
{

	class IntersectInfo;
	class Sphere;

	/// <summary>
	/// A plane representation. A plane is a 2D surface in 3D space. The plane equation is as follows:
	/// <code>Ax + By + Cz + D = 0</code>.
	/// Planes are represented by a point <code>P_0</code> and a unit vector <code>n</code> that is normal to the plane.
	/// This is sometimes called point-normal form.
	/// </summary>
	/// <remarks>
	/// In order to gain more detail read section 4.6.3 "Planes" in "Game Engine Architecture" by Jason Gregory.
	/// </remarks>
	class Plane
	{
		/* ==================== Static variables begin ==================== */
		/* ==================== Static variables end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		/// <summary>
		/// Plane constructor.
		/// </summary>
		/// <param name="normal"> The normal vector of the plane. </param>
		/// <param name="distance"> The distance from the origin. </param>
		MATH_API Plane(const Vector3D& normal, Real distance);
		
		/// <summary> Plane destructor. </summary>
		MATH_API ~Plane(void);

		/// <summary> Plane copy constructor. </summary>
		MATH_API Plane(const Plane& plane) = default;
		/// <summary> Plane move constructor. </summary>
		MATH_API Plane(Plane&& plane) = default;
		/// <summary> Plane copy assignment operator. </summary>
		MATH_API Plane& operator=(const Plane& plane) = default;
		/// <summary> Plane move assignment operator. </summary>
		MATH_API Plane& operator=(Plane&& plane) = default;
	/* ==================== Constructors and destructors end ==================== */

	/* ==================== Non-static, non-virtual member functions begin ==================== */
	public:
		MATH_API Plane Normalized() const;
		MATH_API IntersectInfo DoesIntersectSphere(const Sphere& sphere) const;
		MATH_API bool IsPerpendicularToAxisX() const { return Math::AlmostEqual(m_normal.GetX(), REAL_ZERO); }
		MATH_API bool IsPerpendicularToAxisY() const { return Math::AlmostEqual(m_normal.GetY(), REAL_ZERO); }
		MATH_API bool IsPerpendicularToAxisZ() const { return Math::AlmostEqual(m_normal.GetZ(), REAL_ZERO); }
		MATH_API inline const Vector3D& GetNormal() const { return m_normal; }
		MATH_API inline Real GetDistance() const { return m_distance; }
		/* ==================== Non-static, non-virtual member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	private:
		/// <summary>
		/// Representation of the normal vector of the infinite plane.
		/// </summary>
		Vector3D m_normal;

		/// <summary>
		/// The representation of how far along the normal vector the plane lies.
		/// </summary>
		Real m_distance;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class Plane */

} /* end namespace Math */

#endif /* __MATH_PLANE_H__ */