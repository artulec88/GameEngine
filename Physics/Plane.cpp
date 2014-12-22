#include "stdafx.h"
#include "Plane.h"

using namespace Physics;

Plane::Plane(const Math::Vector3D& normal, Math::Real distance) :
	m_normal(normal),
	m_distance(distance)
{
}

Plane::~Plane()
{
}

Plane Plane::Normalized() const
{
	Math::Real length = m_normal.Length();

	return Plane(m_normal.Normalized(), m_distance / length);
}

IntersectInfo Plane::IntersectBoundingSphere(const BoundingSphere& boundingSphere) const
{
	// how far the sphere center is along the plane normal vector
	Math::Real distanceFromSphereCenter = m_normal.Dot(boundingSphere.GetCenter());
	distanceFromSphereCenter += m_distance; // now it represents the distance between the plane and the sphere
	distanceFromSphereCenter = Math::Absolute(distanceFromSphereCenter);

	Math::Real distanceFromSphere = distanceFromSphereCenter - boundingSphere.GetRadius();

	return IntersectInfo(distanceFromSphere < REAL_ZERO, distanceFromSphere);
}