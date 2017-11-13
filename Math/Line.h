#ifndef __MATH_LINE_H__
#define __MATH_LINE_H__

#include "Math.h"
#include "Vector.h"

namespace Math
{

	/// <summary>
	/// The mathematical representation of a line, which in turn can also represent a ray or a segment.
	/// An infinite line can be represented by a point <code>P_0</code> plus a unit vector <code>u</code>
	/// in the direction of the line. A parametric equation of a line traces out every possible point <code>P</code>
	/// along the line by starting at the initial point <code>P_0</code> and moving an arbitrary distance <code>t</code>
	/// along the direction of the unit vector <code>u</code>. The infinitely large set of points becomes a
	/// vector function of the scalar parameter <code>t</code>:
	/// <code>P(t) = P_0 + tu</code>, where <code>-inf < t < +inf</code>.
	/// A ray is a line that extends to infinity in only one direction. This is easily expressed as <code>P(t)</code>
	/// with the constraint <code>t>=0</code>.
	/// A line segment is bounded at both ends by <code>P_0</code> and <code>P_1</code>. It can be represented by
	/// <code>P(t)</code> in either one of the following ways (where <code>L = P_1 - P_0</code>):
	/// <code>P(t) = P_0 + tu</code>, where <code>0 <= t <= |L|</code>
	/// <code>P(t) = P_0 + tL</code>, where <code>0 <= t <= 1</code>.
	/// The latter format is particularly convenient because the parameter <code>t</code> is normalized.
	/// This means we do not have to store the constraint |L| in a separate floating-point parameter.
	/// </summary>
	/// <remarks>
	/// The summary is based on the section 4.6.1 "Lines, Rays, and Line Segments" in
	/// "Game Engine Architecture" by Jason Gregory.
	/// </remarks>
	class Line
	{
		/* ==================== Static variables begin ==================== */
		/* ==================== Static variables end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		/// <summary>
		/// The constructor of the line. It takes two parameters representing the point (<paramref name="point"/>) and the direction of the line (<paramref name="direction"/>).
		/// </summary>
		/// <param name="point">The point on the line.</param>
		/// <param name="direction">The direction of the line.</param>
		MATH_API Line(const Vector3D& point, const Vector3D& direction);

		/// <summary>
		/// The destructor of the line object.
		/// </summary>
		MATH_API ~Line();
		
		//Line(const Line& line) {} // don't implement
		//void operator=(const Line& line) {} // don't implement
	/* ==================== Constructors and destructors end ==================== */

	/* ==================== Non-static, non-virtual member functions begin ==================== */
	public:
		inline const Vector3D& GetPoint() const { return m_point; }
		inline const Vector3D& GetDirection() const { return m_direction; }
		/* ==================== Non-static, non-virtual member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	private:
		/// <summary>
		/// Represents the 3D point on the line.
		/// </summary>
		Vector3D m_point;

		/// <summary>
		/// Represents the direction of the line.
		/// </summary>
		Vector3D m_direction;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class Line */

} /* end namespace Math */

#endif /* __MATH_LINE_H__ */