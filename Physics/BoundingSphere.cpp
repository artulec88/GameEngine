#include "StdAfx.h"
#include "BoundingSphere.h"

using namespace Physics;

BoundingSphere::BoundingSphere(const Math::Vector3D& center, Math::Real radius) :
	m_center(center),
	m_radius(radius)
{
}


BoundingSphere::~BoundingSphere(void)
{
}

IntersectInfo BoundingSphere::IntersectBoundingSphere(const BoundingSphere& boundingSphere) const
{
	Math::Real radiusDistance = m_radius + boundingSphere.GetRadius();
	Math::Real centerDistance = (boundingSphere.GetCenter() - m_center).Length();
	return IntersectInfo(centerDistance < radiusDistance, centerDistance - radiusDistance);
}