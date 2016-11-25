#include "stdafx.h"
#include "Triangle.h"
#include "FloatingPoint.h"
#include "Utility\ILogger.h"

Math::Triangle::Triangle(const Vector3D& v1, const Vector3D& v2, const Vector3D& v3) noexcept :
	m_v1(v1),
	m_v2(v2),
	m_v3(v3)
{
}


Math::Triangle::~Triangle()
{
}

Math::Real Math::Triangle::CalculatePerimeter() const noexcept
{
	return CalculateEdgeLength1() + CalculateEdgeLength2() + CalculateEdgeLength3();
}

Math::Angle Math::Triangle::CalculateAngle1() const noexcept
{
	const Real edgeLength1 = CalculateEdgeLength1();
	const Real edgeLength2 = CalculateEdgeLength2();
	const Real edgeLength3 = CalculateEdgeLength3();
	const Real radians = acos(edgeLength2 * edgeLength2 + edgeLength3 * edgeLength3 - edgeLength1 * edgeLength1) / (2.0f * edgeLength2 * edgeLength3);
	return Angle(radians, Unit::RADIAN);
}

Math::Angle Math::Triangle::CalculateAngle2() const noexcept
{
	const Real edgeLength1 = CalculateEdgeLength1();
	const Real edgeLength2 = CalculateEdgeLength2();
	const Real edgeLength3 = CalculateEdgeLength3();
	const Real radians = acos(edgeLength1 * edgeLength1 + edgeLength3 * edgeLength3 - edgeLength2 * edgeLength2) / (2.0f * edgeLength1 * edgeLength3);
	return Angle(radians, Unit::RADIAN);
}

Math::Angle Math::Triangle::CalculateAngle3() const noexcept
{
	const Real edgeLength1 = CalculateEdgeLength1();
	const Real edgeLength2 = CalculateEdgeLength2();
	const Real edgeLength3 = CalculateEdgeLength3();
	const Real radians = acos(edgeLength1 * edgeLength1 + edgeLength2 * edgeLength2 - edgeLength3 * edgeLength3) / (2.0f * edgeLength1 * edgeLength2);
	return Angle(radians, Unit::RADIAN);
}

Math::Real Math::Triangle::CalculateArea() const noexcept
{
	// ==================== Heron's formula implementation begin ==================== */
	//const Real edgeLength1 = CalculateEdgeLength1();
	//const Real edgeLength2 = CalculateEdgeLength2();
	//const Real edgeLength3 = CalculateEdgeLength3();
	//const Real semiperimeter = (edgeLength1 + edgeLength2 + edgeLength3) / 2.0f;
	//return sqrt(semiperimeter * (semiperimeter - edgeLength1) * (semiperimeter - edgeLength2) * (semiperimeter - edgeLength3));
	// ==================== Heron's formula implementation end ==================== */

	// ==================== "3D math primer for graphics and game development" (page 273) implementation begin ==================== */
	const Math::Vector3D edge1 = m_v3 - m_v2;
	const Math::Vector3D edge2 = m_v1 - m_v3;
	return (edge1.Cross(edge2).Length()) / 2.0f;
	// ==================== "3D math primer for graphics and game development" (page 273) implementation end ==================== */
}

Math::Vector3D Math::Triangle::CalculateBarycentricCoordinates(const Vector3D& vec) const noexcept
{
	const Vector3D edge1 = m_v3 - m_v2;
	const Vector3D edge2 = m_v1 - m_v3;
	const Vector3D edge3 = m_v2 - m_v1;
	const Vector3D d1 = vec - m_v1; // the vector from m_v1 to vec
	const Vector3D d2 = vec - m_v2; // the vector from m_v2 to vec
	const Vector3D d3 = vec - m_v3; // the vector from m_v3 to vec
	const Vector3D crossResult = edge1.Cross(edge2);
	// According to the book "3D math primer for graphics and game development" it isn't necessary to normalize the triangleNormal.
	const Vector3D triangleNormal = crossResult / crossResult.Length();

	const Real denominator = crossResult.Dot(triangleNormal);

	const Real b1 = ((edge1.Cross(d3)).Dot(triangleNormal)) / denominator;
	const Real b2 = ((edge2.Cross(d1)).Dot(triangleNormal)) / denominator;
	const Real b3 = ((edge3.Cross(d2)).Dot(triangleNormal)) / denominator;

	return Vector3D(b1, b2, b3);
}

Math::Vector3D Math::Triangle::CalculatePositionFromBarycentricCoordinates(Real barycentricWeight1, Real barycentricWeight2) const noexcept
{
	return (m_v1 * barycentricWeight1) + (m_v2 * barycentricWeight2) + (m_v3 * (REAL_ONE- barycentricWeight1 - barycentricWeight2));
}

Math::Vector3D Math::Triangle::CalculateIncenter() const noexcept
{
	const Real edgeLength1 = CalculateEdgeLength1();
	const Real edgeLength2 = CalculateEdgeLength2();
	const Real edgeLength3 = CalculateEdgeLength3();

	return ((m_v1 * edgeLength1) + (m_v2 * edgeLength2) + (m_v3 * edgeLength3)) / (edgeLength1 + edgeLength2 + edgeLength3);
}

Math::Vector3D Math::Triangle::CalculateCircumcenter() const noexcept
{
	const Vector3D edge1 = m_v3 - m_v2;
	const Vector3D edge2 = m_v1 - m_v3;
	const Vector3D edge3 = m_v2 - m_v1;

	const Real d1 = -edge2.Dot(edge3);
	const Real d2 = -edge3.Dot(edge1);
	const Real d3 = -edge1.Dot(edge2);
	const Real c1 = d2 * d3;
	const Real c2 = d3 * d1;
	const Real c3 = d1 * d2;
	const Real c = c1 + c2 + c3;

	return ((m_v1 * (c2 + c3)) + (m_v2 * (c3 + c1)) + (m_v3 * (c1 + c2))) / (2.0f * c);
}

bool Math::Triangle::IsCorrect() const noexcept
{
	const Real edgeLength1 = CalculateEdgeLength1();
	const Real edgeLength2 = CalculateEdgeLength2();
	const Real edgeLength3 = CalculateEdgeLength3();
	return ((edgeLength1 + edgeLength2 >= edgeLength3) && (edgeLength1 + edgeLength3 >= edgeLength2) &&
		(edgeLength2 + edgeLength3 >= edgeLength1));
}