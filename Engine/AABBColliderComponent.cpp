#include "stdafx.h"
#include "AABBColliderComponent.h"

engine::AABBColliderComponent::AABBColliderComponent(const Math::Vector3D& bottomLeftPos, const Math::Vector3D& topRightPos) :
	m_aabbCollider(bottomLeftPos, topRightPos)
{
	GetPhysicsObject()->AddCollider(&m_aabbCollider);
}


engine::AABBColliderComponent::~AABBColliderComponent()
{
}
