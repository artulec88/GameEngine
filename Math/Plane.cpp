#include "stdafx.h"
#include "Plane.h"
#include "Sphere.h"
#include "IntersectInfo.h"

Math::Plane::Plane(const Vector3D& normal, Real distance) :
	m_normal(normal),
	m_distance(distance)
{
}

Math::Plane::~Plane(void)
{
}

Math::Plane Math::Plane::Normalized() const
{
	Math::Real length = m_normal.Length();
	return Plane(m_normal.Normalized(), m_distance / length);
}

Math::IntersectInfo Math::Plane::DoesIntersectSphere(const Sphere& sphere) const
{
	// how far the sphere center is along the plane normal vector
	Math::Real distanceFromSphereCenter = m_normal.Dot(sphere.GetCenter());
	distanceFromSphereCenter += m_distance; // now it represents the distance between the plane and the sphere
	distanceFromSphereCenter = Math::Absolute(distanceFromSphereCenter);

	Math::Real distanceFromSphere = distanceFromSphereCenter - sphere.GetRadius();

	return Math::IntersectInfo(distanceFromSphere < REAL_ZERO, distanceFromSphere);
}