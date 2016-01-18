#include "stdafx.h"
#include "Sphere.h"
#include "IntersectInfo.h"


Math::Sphere::Sphere(const Vector3D& center, Real radius) :
	m_center(center),
	m_radius(radius)
{
}


Math::Sphere::~Sphere(void)
{
}

Math::IntersectInfo Math::Sphere::DoesIntersectSphere(const Sphere& sphere) const
{
	Math::Real radiusDistance = m_radius + sphere.GetRadius();
	Math::Real centerDistance = (sphere.GetCenter() - m_center).Length();
	return Math::IntersectInfo(centerDistance < radiusDistance, centerDistance - radiusDistance);
}