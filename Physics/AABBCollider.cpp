#include "stdafx.h"
#include "AABBCollider.h"


Physics::AABBCollider::AABBCollider(const math::Vector3D& bottomLeftPos, const math::Vector3D& topRightPos) :
	Collider(TYPE_AABB),
	m_aabb(bottomLeftPos, topRightPos)
{
}


Physics::AABBCollider::~AABBCollider()
{
}

math::IntersectInfo Physics::AABBCollider::Intersect(const Collider& collider) const
{
	// TODO: Implement this function
	return math::IntersectInfo(REAL_ONE /* some arbitrary (but positive) value */);
}