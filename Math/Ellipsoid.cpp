#include "stdafx.h"
#include "Ellipsoid.h"


math::Ellipsoid::Ellipsoid(const Vector3D& center, Real a, Real b, Real c) :
	m_center(center),
	m_a(a),
	m_b(b),
	m_c(c)
{
}


math::Ellipsoid::~Ellipsoid()
{
}
