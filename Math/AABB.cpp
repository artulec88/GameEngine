#include "stdafx.h"
#include "AABB.h"
#include "IntersectInfo.h"

Math::AABB::AABB(const Math::Vector3D& bottomLeftPos, const Vector3D& topRightPos) :
	m_bottomLeftPos(bottomLeftPos),
	m_topRightPos(topRightPos)
{
}

Math::AABB::~AABB()
{
}

Math::IntersectInfo Math::AABB::DoesIntersectAABB(const Math::AABB& aabb) const
{
	// TODO: Remove intermediate local variables.
	Math::Vector3D distances1 = aabb.GetBottomLeftPos() - m_topRightPos;
	Math::Vector3D distances2 = m_bottomLeftPos - aabb.GetTopRightPos();
	Math::Vector3D distances = distances1.Max(distances2);

	Math::Real maxDistance = distances.Max();

	return Math::IntersectInfo(maxDistance < 0, maxDistance);
}