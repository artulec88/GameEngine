#include "stdafx.h"
#include "Triangle.h"


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

bool Math::Triangle::IsCorrect() const noexcept
{
	const Real edgeLength1 = CalculateEdgeLength1();
	const Real edgeLength2 = CalculateEdgeLength2();
	const Real edgeLength3 = CalculateEdgeLength3();
	return ((edgeLength1 + edgeLength2 >= edgeLength3) && (edgeLength1 + edgeLength3 >= edgeLength2) &&
		(edgeLength2 + edgeLength3 >= edgeLength1));
}