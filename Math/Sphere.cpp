#include "stdafx.h"
#include "Sphere.h"
#include "IntersectInfo.h"


math::Sphere::Sphere(const Vector3D& center, Real radius) :
	m_center(center),
	m_radius(radius)
{
}


math::Sphere::~Sphere()
{
}

math::IntersectInfo math::Sphere::DoesIntersectSphere(const Sphere& sphere) const
{
	const auto radiusDistance = m_radius + sphere.GetRadius();
	const auto centerDistance = (sphere.GetCenter() - m_center).Length();
	return IntersectInfo(centerDistance - radiusDistance);
}