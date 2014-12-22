#include "stdafx.h"
#include "AABB.h"

using namespace Physics;

AABB::AABB(const Math::Vector3D& minExtents, const Math::Vector3D& maxExtents) :
	m_minExtents(minExtents),
	m_maxExtents(maxExtents)
{
}

AABB::~AABB()
{
}

IntersectInfo AABB::IntersectAABB(const AABB& aabb) const
{
	Math::Vector3D distances1 = aabb.GetMinExtents() - m_maxExtents;
	Math::Vector3D distances2 = m_minExtents - aabb.GetMaxExtents();
	Math::Vector3D distances = distances1.Max(distances2);

	Math::Real maxDistance = distances.Max();

	return IntersectInfo(maxDistance < 0, maxDistance);
}