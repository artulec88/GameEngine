#include "stdafx.h"
#include "Point.h"

using namespace Math;

std::string Point::ToString() const
{
	std::stringstream ss("");
	ss << m_x << " " << m_y << " ";
	return ss.str();
}

Real Point::Length() const
{
	return static_cast<Real>(sqrt(static_cast<Real>(LengthSquared())));
}

Real Point::LengthSquared() const
{
	return static_cast<Real>(m_x * m_x + m_y * m_y);
}

Point Point::Normalized() const
{
	return (*this) / Length();
}

void Point::Normalize()
{
	(*this) = (*this) / Length();
}

Real Point::Dot(const Point& v) const
{
	return (m_x * v.GetX() + m_y * v.GetY());
}
