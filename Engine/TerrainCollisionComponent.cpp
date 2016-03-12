#include "stdafx.h"
#include "TerrainCollisionComponent.h"

// TODO: Don't hard-code any value! Ever!
// TODO: This variable should be moved into the Physics engine and be accessible from anywhere (similarily as stdlog is accessible).
/* static */ const Math::Vector3D Engine::TerrainCollisionComponent::GRAVITY_ACCELERATION(0.0f, -9.8f, 0.0f);

Engine::TerrainCollisionComponent::TerrainCollisionComponent(Physics::PhysicsObject* physicsObject, Rendering::TerrainMesh* terrainMesh) :
	GameComponent(),
	m_physicsObject(physicsObject),
	m_terrainMesh(terrainMesh),
	m_isTouchingTerrain(false)
{
}


Engine::TerrainCollisionComponent::~TerrainCollisionComponent()
{
}

void Engine::TerrainCollisionComponent::Update(Math::Real deltaTime)
{
	Math::Vector3D& position = GetTransform().GetPos();
	Math::Real terrainHeight = m_terrainMesh->GetHeightAt(position.GetXZ(), true);
	if (position.GetY() < terrainHeight)
	{
		position.SetY(terrainHeight);
		if (!m_isTouchingTerrain)
		{
			m_physicsObject->ApplyLinearAcceleration(-GRAVITY_ACCELERATION);
			m_isTouchingTerrain = true;
		}
	}
}
