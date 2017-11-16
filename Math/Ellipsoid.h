#ifndef __MATH_ELLIPSOID_H__
#define __MATH_ELLIPSOID_H__

#include "Math.h"
#include "Vector.h"

namespace math
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
		/// <summary> Ellipsoid constructor. </summary>
		/// <param name="center"> The center of the ellipsoid. </param>
		/// <param name="a">
		/// The length of the semi-principal axis X. The point [a; 0; 0] lies on the surface of an ellipsoid.
		/// In other words it is the distance from the center of the ellipsoid to its surface along the X axis.
		/// </param>
		/// <param name="b">
		/// The length of the semi-principal axis Y. The point [0; b; 0] lies on the surface of an ellipsoid.
		/// In other words it is the distance from the center of the ellipsoid to its surface along the Y axis.
		/// </param>
		/// <param name="c">
		/// The length of the semi-principal axis Z. The point [0; 0; c] lies on the surface of an ellipsoid.
		/// In other words it is the distance from the center of the ellipsoid to its surface along the Z axis.
		/// </param>
		MATH_API Ellipsoid(const Vector3D& center, Real a, Real b, Real c);

		/// <summary> Ellipsoid destructor. </summary>
		MATH_API ~Ellipsoid();

		/// <summary> Ellipsoid copy constructor. </summary>
		/// <param name="ellipsoid"> The ellipsoid to copy construct from. </param>
		MATH_API Ellipsoid(const Ellipsoid& ellipsoid) noexcept = default;

		/// <summary> Ellipsoid move constructor. </summary>
		/// <param name="ellipsoid"> The ellipsoid to move construct from. </param>
		MATH_API Ellipsoid(Ellipsoid&& ellipsoid) noexcept = default;

		/// <summary> Ellipsoid copy assignment operator. </summary>
		/// <param name="ellipsoid"> The ellipsoid to copy assign from. </param>
		/// <returns> The reference to the newly copy-assigned ellipsoid. </returns>
		MATH_API Ellipsoid& operator=(const Ellipsoid& ellipsoid) noexcept = default;

		/// <summary> Ellipsoid move assignment operator. </summary>
		/// <param name="ellipsoid"> The ellipsoid to move assign from. </param>
		/// <returns> The reference to the newly move-assigned ellipsoid. </returns>
		MATH_API Ellipsoid& operator=(Ellipsoid&& ellipsoid) noexcept = default;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static, non-virtual member functions begin ==================== */
		/// <summary> Returns the center of the elllipsoid. </summary>
		/// <returns> The 3D Cartesian position that is the center of the ellipsoid. </returns>
		MATH_API const Vector3D& GetCenter() const { return m_center; }

		/// <summary>
		/// Returns the length of the semi-principal axis X. It is the distance from the center of the ellipsoid to its surface along the X axis.
		/// </summary>
		/// <returns> The distance from the center of the ellipsoid to its surface along the X axis. </returns>
		MATH_API Real GetA() const { return m_a; }

		/// <summary>
		/// Returns the length of the semi-principal axis Y. It is the distance from the center of the ellipsoid to its surface along the Y axis.
		/// </summary>
		/// <returns> The distance from the center of the ellipsoid to its surface along the Y axis. </returns>
		MATH_API Real GetB() const { return m_b; }

		/// <summary>
		/// Returns the length of the semi-principal axis Z. It is the distance from the center of the ellipsoid to its surface along the Z axis.
		/// </summary>
		/// <returns> The distance from the center of the ellipsoid to its surface along the Z axis. </returns>
		MATH_API Real GetC() const { return m_c; }
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

} /* end namespace math */

#endif /* __MATH_ELLIPSOID_H__ */