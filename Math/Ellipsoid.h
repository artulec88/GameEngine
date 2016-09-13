#ifndef __MATH_ELLIPSOID_H__
#define __MATH_ELLIPSOID_H__

#include "Math.h"
#include "Vector.h"

namespace Math
{

	class IntersectInfo;

	/// <summary>
	/// A mathematical representation of an ellipsoid. The ellipsoid is a closed quadric surface, that is a three-dimensional analogue of an ellipse.
	/// The standard equation of an ellipsoid centered at the origin of Cartesian coordinate system and aligned with the axes is:
	/// <code> [(x^2) / (a^2)] + [(y^2) / (b^2)] + [(z^2) / (c^2)] = 1. </code>
	/// </summary>
	/// <remarks>
	/// See https://en.wikipedia.org/wiki/Ellipsoid for details.
	/// </remarks>
	class Ellipsoid
	{
		/* ==================== Static variables begin ==================== */
		/* ==================== Static variables end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		MATH_API Ellipsoid(const Vector3D& center, Real a, Real b, Real c);
		MATH_API ~Ellipsoid(void);
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static, non-virtual member functions begin ==================== */
	public:
		MATH_API inline const Math::Vector3D& GetCenter() const { return m_center; }
		MATH_API inline const Math::Real GetA() const { return m_a; }
		MATH_API inline const Math::Real GetB() const { return m_b; }
		MATH_API inline const Math::Real GetC() const { return m_c; }
		/* ==================== Non-static, non-virtual member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	private:
		/// <summary>
		/// The origin position of the ellipsoid.
		/// </summary>
		Vector3D m_center;

		/// <summary>
		/// The length of the line segment from the center of the ellipsoid to the surface of the ellipsoid along the X axis.
		/// The point <code>(m_a; 0; 0)</code> lie on the surface of the ellipsoid.
		/// </summary>
		Real m_a;

		/// <summary>
		/// The length of the line segment from the center of the ellipsoid to the surface of the ellipsoid along the Y axis.
		/// The point <code>(0; m_b; 0)</code> lie on the surface of the ellipsoid.
		/// </summary>
		Real m_b;

		/// <summary>
		/// The length of the line segment from the center of the ellipsoid to the surface of the ellipsoid along the Z axis.
		/// The point <code>(0; 0; m_c)</code> lie on the surface of the ellipsoid.
		/// </summary>
		Real m_c;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class Ellipsoid */

} /* end namespace Math */

#endif /* __MATH_ELLIPSOID_H__ */