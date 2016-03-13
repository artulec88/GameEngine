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
	return Math::IntersectInfo(false, 0.0f);
}