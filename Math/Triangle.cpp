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