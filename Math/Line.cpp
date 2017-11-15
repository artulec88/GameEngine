#include "stdafx.h"
#include "Line.h"

math::Line::Line(const Vector3D& point, const Vector3D& direction) :
	m_point(point),
	m_direction(direction)
{
}


math::Line::~Line()
{
}
