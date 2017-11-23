#include "stdafx.h"
#include "Triangle.h"
#include "FloatingPoint.h"

math::Triangle::Triangle(const Vector3D& v1, const Vector3D& v2, const Vector3D& v3) noexcept :
	m_v1(v1),
	m_v2(v2),
	m_v3(v3)
{
}


math::Triangle::~Triangle()
{
}

math::Real math::Triangle::CalculatePerimeter() const noexcept
{
	return CalculateEdgeLength1() + CalculateEdgeLength2() + CalculateEdgeLength3();
}

math::Angle math::Triangle::CalculateAngle1() const noexcept
{
	const auto edgeLength1 = CalculateEdgeLength1();
	const auto edgeLength2 = CalculateEdgeLength2();
	const auto edgeLength3 = CalculateEdgeLength3();
	const auto radians = acos(edgeLength2 * edgeLength2 + edgeLength3 * edgeLength3 - edgeLength1 * edgeLength1) / (2.0f * edgeLength2 * edgeLength3);
	return Angle(radians, units::RADIAN);
}

math::Angle math::Triangle::CalculateAngle2() const noexcept
{
	const auto edgeLength1 = CalculateEdgeLength1();
	const auto edgeLength2 = CalculateEdgeLength2();
	const auto edgeLength3 = CalculateEdgeLength3();
	const auto radians = acos(edgeLength1 * edgeLength1 + edgeLength3 * edgeLength3 - edgeLength2 * edgeLength2) / (2.0f * edgeLength1 * edgeLength3);
	return Angle(radians, units::RADIAN);
}

math::Angle math::Triangle::CalculateAngle3() const noexcept
{
	const auto edgeLength1 = CalculateEdgeLength1();
	const auto edgeLength2 = CalculateEdgeLength2();
	const auto edgeLength3 = CalculateEdgeLength3();
	const auto radians = acos(edgeLength1 * edgeLength1 + edgeLength2 * edgeLength2 - edgeLength3 * edgeLength3) / (2.0f * edgeLength1 * edgeLength2);
	return Angle(radians, units::RADIAN);
}

math::Real math::Triangle::CalculateArea() const noexcept
{
	// ==================== Heron's formula implementation begin ==================== */
	//const auto edgeLength1 = CalculateEdgeLength1();
	//const auto edgeLength2 = CalculateEdgeLength2();
	//const auto edgeLength3 = CalculateEdgeLength3();
	//const auto semiperimeter = (edgeLength1 + edgeLength2 + edgeLength3) / 2.0f;
	//return sqrt(semiperimeter * (semiperimeter - edgeLength1) * (semiperimeter - edgeLength2) * (semiperimeter - edgeLength3));
	// ==================== Heron's formula implementation end ==================== */

	// ==================== "3D math primer for graphics and game development" (page 273) implementation begin ==================== */
	const Vector3D edge1 = m_v3 - m_v2;
	const Vector3D edge2 = m_v1 - m_v3;
	return (edge1.Cross(edge2).Length()) / 2.0f;
	// ==================== "3D math primer for graphics and game development" (page 273) implementation end ==================== */
}

math::Vector3D math::Triangle::CalculateBarycentricCoordinates(const Vector3D& vec) const noexcept
{
	const auto edge1 = m_v3 - m_v2;
	const auto edge2 = m_v1 - m_v3;
	const auto edge3 = m_v2 - m_v1;
	const auto d1 = vec - m_v1; // the vector from m_v1 to vec
	const auto d2 = vec - m_v2; // the vector from m_v2 to vec
	const auto d3 = vec - m_v3; // the vector from m_v3 to vec
	const auto crossResult = edge1.Cross(edge2);
	// According to the book "3D math primer for graphics and game development" it isn't necessary to normalize the triangleNormal.
	const auto triangleNormal = crossResult / crossResult.Length();

	const auto denominator = crossResult.Dot(triangleNormal);

	const auto b1 = ((edge1.Cross(d3)).Dot(triangleNormal)) / denominator;
	const auto b2 = ((edge2.Cross(d1)).Dot(triangleNormal)) / denominator;
	const auto b3 = ((edge3.Cross(d2)).Dot(triangleNormal)) / denominator;

	return Vector3D(b1, b2, b3);
}

math::Vector3D math::Triangle::CalculatePositionFromBarycentricCoordinates(Real barycentricWeight1, Real barycentricWeight2) const noexcept
{
	return (m_v1 * barycentricWeight1) + (m_v2 * barycentricWeight2) + (m_v3 * (REAL_ONE- barycentricWeight1 - barycentricWeight2));
}

math::Vector3D math::Triangle::CalculateIncenter() const noexcept
{
	const auto edgeLength1 = CalculateEdgeLength1();
	const auto edgeLength2 = CalculateEdgeLength2();
	const auto edgeLength3 = CalculateEdgeLength3();

	return ((m_v1 * edgeLength1) + (m_v2 * edgeLength2) + (m_v3 * edgeLength3)) / (edgeLength1 + edgeLength2 + edgeLength3);
}

math::Vector3D math::Triangle::CalculateCircumcenter() const noexcept
{
	const auto edge1 = m_v3 - m_v2;
	const auto edge2 = m_v1 - m_v3;
	const auto edge3 = m_v2 - m_v1;

	const auto d1 = -edge2.Dot(edge3);
	const auto d2 = -edge3.Dot(edge1);
	const auto d3 = -edge1.Dot(edge2);
	const auto c1 = d2 * d3;
	const auto c2 = d3 * d1;
	const auto c3 = d1 * d2;
	const auto c = c1 + c2 + c3;

	return ((m_v1 * (c2 + c3)) + (m_v2 * (c3 + c1)) + (m_v3 * (c1 + c2))) / (2.0f * c);
}

bool math::Triangle::IsCorrect() const noexcept
{
	const auto edgeLength1 = CalculateEdgeLength1();
	const auto edgeLength2 = CalculateEdgeLength2();
	const auto edgeLength3 = CalculateEdgeLength3();
	return ((edgeLength1 + edgeLength2 >= edgeLength3) && (edgeLength1 + edgeLength3 >= edgeLength2) &&
		(edgeLength2 + edgeLength3 >= edgeLength1));
}