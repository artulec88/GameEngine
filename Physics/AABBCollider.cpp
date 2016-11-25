#include "stdafx.h"
#include "AABBCollider.h"


Physics::AABBCollider::AABBCollider(const Math::Vector3D& bottomLeftPos, const Math::Vector3D& topRightPos) :
	Collider(TYPE_AABB),
	m_aabb(bottomLeftPos, topRightPos)
{
}


Physics::AABBCollider::~AABBCollider()
{
}

Math::IntersectInfo Physics::AABBCollider::Intersect(const Collider& collider) const
{
	// TODO: Implement this function
	return Math::IntersectInfo(REAL_ONE /* some arbitrary (but positive) value */);
}