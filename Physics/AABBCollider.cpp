#include "stdafx.h"
#include "AABBCollider.h"


physics::AabbCollider::AabbCollider(const math::Vector3D& bottomLeftPos, const math::Vector3D& topRightPos) :
	Collider(collider_types::TYPE_AABB),
	m_aabb(bottomLeftPos, topRightPos)
{
}


physics::AabbCollider::~AabbCollider()
{
}

math::IntersectInfo physics::AabbCollider::Intersect(const Collider& collider) const
{
	// TODO: Implement this function
	return math::IntersectInfo(REAL_ONE /* some arbitrary (but positive) value */);
}