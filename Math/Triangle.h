#ifndef __MATH_TRIANGLE_H__
#define __MATH_TRIANGLE_H__

#include "Math.h"
#include "Vector.h"
#include "Angle.h"

namespace Math
{

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
		MATH_API ~Triangle();
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

		/// <summary>
		/// Calculates and returns the angle in opposition to the first vertex of the triangle (<code>m_v1</code>).
		/// </summary>
		/// <returns> The angle located in opposition to the first vertex of the triangle. </returns>
		MATH_API Angle CalculateAngle1() const noexcept;

		/// <summary>
		/// Calculates and returns the angle in opposition to the second vertex of the triangle (<code>m_v2</code>).
		/// </summary>
		/// <returns> The angle located in opposition to the second vertex of the triangle. </returns>
		MATH_API Angle CalculateAngle2() const noexcept;

		/// <summary>
		/// Calculates and returns the angle in opposition to the third vertex of the triangle (<code>m_v3</code>).
		/// </summary>
		/// <returns> The angle located in opposition to the third vertex of the triangle. </returns>
		MATH_API Angle CalculateAngle3() const noexcept;

		/// <summary>
		/// Calculates and returns the area of the triangle.
		/// Read "3D math primer for graphics and game development" page 273 on how to calculate the area of a triangle from its vertices.
		/// </summary>
		/// <returns> The area of the triangle. </returns>
		MATH_API Real CalculateArea() const noexcept;

		/// <summary>
		/// Calculates and returns the 3D vector whose X, Y and Z components are the barycentric space coordinates values.
		/// In other words, it performs the conversion from 3D Cartesian space to barycentric space.
		/// Read "3D math primer for graphics and game development" page 273 on how to calculate barycentric coordinates for the triangle in 3D space.
		/// </summary>
		/// <param name="vec">
		/// The position for which we want to calculate the barycentric coordinates.
		/// The given 3D position must lie on the same plane as the triangle so in fact the <paramref name="vec"/> is first projected onto this plane and
		/// the result is then used to compute the barycentric coordinates.
		/// </param>
		/// <returns> The barycentric weights for the specified position <paramref name="vec"/> in 3D Cartesian space. </returns>
		MATH_API Vector3D CalculateBarycentricCoordinates(const Vector3D& vec) const noexcept;

		/// <summary>
		/// Calculates and returns the position in the 3D space based on the barycentric coordinates specified in the vector <paramref name="barycentricWeights"/>.
		/// In other words, it performs the conversion from barycentric space to 3D Cartesian space. What is worth noticing is that the returned position will always lie
		/// inside the triangle if and only if all barycentric coordinates are in range [0; 1]. If one or more coordinates are outside this range then the returned
		/// position will lie outside the triangle (but still on the plane the triangle is on).
		/// Read "3D math primer for graphics and game development" page 273 to find out in detail how this conversion is performed.
		/// </summary>
		/// <param name="barycentricWeights">
		/// The 2D vector in barycentric space. Barycentric space is 2D, but it has three coordinates. However, since the sum of the barycentric coordinates is always
		/// <code>1.0</code> it is sufficient to specify only two barycentric components and compute the third one from them.
		/// </param>
		/// <returns> The calculated 3D position from the specified <paramref name="barycentricWeights"/>. </returns>
		MATH_API Vector3D CalculatePositionFromBarycentricCoordinates(const Vector2D& barycentricWeights) const noexcept
		{
			return CalculatePositionFromBarycentricCoordinates(barycentricWeights.x, barycentricWeights.y);
		}

		/// <summary>
		/// Calculates and returns the position in the 3D space based on the barycentric coordinates specified in the vector <paramref name="barycentricWeights"/>.
		/// In other words, it performs the conversion from barycentric space to 3D Cartesian space. What is worth noticing is that the returned position will always lie
		/// inside the triangle if and only if all barycentric coordinates are in range [0; 1]. If one or more coordinates are outside this range then the returned
		/// position will lie outside the triangle (but still on the plane the triangle is on). Barycentric space is 2D, but it has three coordinates. However,
		/// since the sum of the barycentric coordinates is always <code>1.0</code> it is sufficient to specify only two barycentric components and compute the third one from them.
		/// Read "3D math primer for graphics and game development" page 273 to find out in detail how this conversion is performed.
		/// </summary>
		/// <param name="barycentricWeight1"> The first component of the 2D vector in barycentric space. </param>
		/// <param name="barycentricWeight2"> The second component of the 2D vector in barycentric space. </param>
		/// <returns>
		/// The calculated 3D position from the specified barycentric weights <paramref name="barycentricWeight1"/>, 
		/// <paramref name="barycentricWeight2"/>.
		/// </returns>
		MATH_API Vector3D CalculatePositionFromBarycentricCoordinates(Real barycentricWeight1, Real barycentricWeight2) const noexcept;

		/// <summary>
		/// Calculates and returns the center of gravity (also known as the centroid) of the triangle. The center of gravity of the triangle is the point
		/// where the triangle would balance perfectly. It is the intersection of the medians (a median is a line from one vertex to the midpoint
		/// of the opposite side). The center of gravity is the geometric average of the three vertices composing the triangle.
		/// The barycentic coordinates of the center of gravity of the triangle is always [1/3; 1/3; 1/3].
		/// </summary>
		/// <returns> The center of gravity of the triangle represented in the 3D Cartesian space. </returns>
		MATH_API Vector3D CalculateCenterOfGravity() const noexcept { return (m_v1 + m_v2 + m_v3) / 3.0f; }

		/// <summary>
		/// Calculates and returns the incenter of the triangle as a 3D position in the Cartesian space. The incenter of the triangle is the point
		/// equidistant from the sides. It is called the incenter because it is the center of the circle inscribed in the triangle.
		/// The incenter is constructed as the intersection of the angle bisectors. The barycentric coordinates of the incenter of a triangle are
		/// [len_1 / p; len_2 / p; len_3 / p], where <code>len_i</code> is the length of the edge opposite the <code>m_vi</code> triangle vertex and
		/// <code>p</code> is the perimeter of the triangle.
		/// where the triangle would balance perfectly. It is the intersection of the medians (a median is a line from one vertex to the midpoint
		/// of the opposite side). The center of gravity is the geometric average of the three vertices composing the triangle.
		/// The barycentic coordinates of the center of gravity of the triangle is always (1/3; 1/3; 1/3).
		/// </summary>
		/// <returns> The incenter of the triangle represented in the 3D Cartesian space. </returns>
		MATH_API Vector3D CalculateIncenter() const noexcept;

		/// <summary>
		/// Calculates and returns the circumcenter of the triangle as a 3D position in the Cartesian space. The circumcenter of the triangle is
		/// the point that is equidistant from the vertices. It is the center of the circle that circumscribes the triangle.
		/// The circumcenter is constructed as the intersection of the perpendicular bisectors of the sides. The barycentric coordinates of the
		/// circumcenter of a triangle can be found by solving the equation given in https://en.wikipedia.org/wiki/Circumscribed_circle#Barycentric_coordinates.
		/// </summary>
		/// <returns> The circumcenter of the triangle represented in the 3D Cartesian space. </returns>
		MATH_API Vector3D CalculateCircumcenter() const noexcept;
	private:
		/// <summary>
		/// Checks triangle inequality. See https://en.wikipedia.org/wiki/Triangle_inequality for details.
		/// </summary>
		bool IsCorrect() const noexcept;
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