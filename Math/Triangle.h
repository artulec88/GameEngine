#ifndef __MATH_TRIANGLE_H__
#define __MATH_TRIANGLE_H__

#include "Math.h"
#include "Vector.h"

namespace Math
{
	class Angle;

	/// <summary>
	/// The mathematical representation of the triangle. Read "3D math primer for graphics and game development" page 257.
	/// </summary>
	class Triangle
	{
		/* ==================== Static variables begin ==================== */
		/* ==================== Static variables end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		/// <summary>
		/// The constructor of the triangle.
		/// </summary>
		MATH_API Triangle(const Vector3D& v1, const Vector3D& v2, const Vector3D& v3) noexcept;

		/// <summary>
		/// Triangle copy constructor.
		/// </summary>
		MATH_API Triangle(const Triangle& triangle) noexcept = default;

		/// <summary>
		/// Triangle move constructor.
		/// </summary>
		MATH_API Triangle(Triangle&& triangle) noexcept = default;

		/// <summary>
		/// Triangle copy assignment operator.
		/// </summary>
		MATH_API Triangle& operator=(const Triangle& triangle) noexcept = default;

		/// <summary>
		/// Triangle move assignment operator.
		/// </summary>
		MATH_API Triangle& operator=(Triangle&& triangle) noexcept = default;

		/// <summary>
		/// The destructor of the triangle.
		/// </summary>
		MATH_API ~Triangle(void);
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static, non-virtual member functions begin ==================== */
	public:
		/// <summary> Calculates and returns the perimeter (circumference) of the triangle. </summary>
		/// <returns> The perimeter (circumference) of the triangle. </returns>
		MATH_API Real CalculatePerimeter() const noexcept;

		/// <summary>
		/// Calculates and returns the length of the edge opposite the first vertex of the triangle (<code>m_v1</code>).
		/// In other words, it returns the difference between <code>m_v3</code> and <code>m_v2</code> vertices.
		/// </summary>
		/// <returns> The length of the edge opposite the first vertex of the triangle. </returns>
		MATH_API Real CalculateEdgeLength1() const noexcept { return (m_v3 - m_v2).Length(); }

		/// <summary>
		/// Calculates and returns the length of the edge opposite the second vertex of the triangle (<code>m_v2</code>).
		/// In other words, it returns the difference between <code>m_v1</code> and <code>m_v3</code> vertices.
		/// </summary>
		/// <returns> The length of the edge opposite the second vertex of the triangle. </returns>
		MATH_API Real CalculateEdgeLength2() const noexcept { return (m_v1 - m_v3).Length(); }

		/// <summary>
		/// Calculates and returns the length of the edge opposite the third vertex of the triangle (<code>m_v3</code>).
		/// In other words, it returns the difference between <code>m_v2</code> and <code>m_v1</code> vertices.
		/// </summary>
		/// <returns> The length of the edge opposite the third vertex of the triangle. </returns>
		MATH_API Real CalculateEdgeLength3() const noexcept { return (m_v2 - m_v1).Length(); }

		//MATH_API Angle Calculate
		/* ==================== Non-static, non-virtual member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	private:
		Vector3D m_v1;
		Vector3D m_v2;
		Vector3D m_v3;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class Triangle */

} /* end namespace Math */

#endif /* __MATH_TRIANGLE_H__ */