#include "stdafx.h"
#include "AABBColliderComponent.h"

engine::AabbColliderComponent::AabbColliderComponent(const math::Vector3D& bottomLeftPos, const math::Vector3D& topRightPos) :
	m_aabbCollider(bottomLeftPos, topRightPos)
{
	GetPhysicsObject()->AddCollider(&m_aabbCollider);
}


engine::AabbColliderComponent::~AabbColliderComponent()
{
}
