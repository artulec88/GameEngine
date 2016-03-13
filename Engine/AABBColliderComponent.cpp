#include "stdafx.h"
#include "AABBColliderComponent.h"

Engine::AABBColliderComponent::AABBColliderComponent(const Math::Vector3D& bottomLeftPos, const Math::Vector3D& topRightPos) :
	m_aabbCollider(bottomLeftPos, topRightPos)
{
	GetPhysicsObject()->AddCollider(&m_aabbCollider);
}


Engine::AABBColliderComponent::~AABBColliderComponent()
{
}
