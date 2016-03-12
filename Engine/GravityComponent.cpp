#include "stdafx.h"
#include "GravityComponent.h"

#include "Physics\PhysicsObject.h"

// TODO: Don't hard-code any value! Ever!
// TODO: This variable should be moved into the Physics engine and be accessible from anywhere (similarily as stdlog is accessible).
/* static */ const Math::Vector3D Engine::GravityComponent::GRAVITY_ACCELERATION(0.0f, -9.8f, 0.0f);

Engine::GravityComponent::GravityComponent(Rendering::TerrainMesh const* terrainMesh) :
	GameComponent(),
	m_terrainMesh(terrainMesh)
{
	//m_physicsObject->ApplyLinearAcceleration(GRAVITY_ACCELERATION);
}


Engine::GravityComponent::~GravityComponent()
{
}

void Engine::GravityComponent::Update(Math::Real deltaTime)
{
	Math::Vector3D& position = GetTransform().GetPos();
	Math::Real terrainHeight = m_terrainMesh->GetHeightAt(position.GetXZ(), true);
	if (position.GetY() > terrainHeight)
	{
		GetPhysicsObject()->ApplyLinearAcceleration(GRAVITY_ACCELERATION);
	}
	else
	{
		// Applying an impulse to stop the physics object immediately
		GetPhysicsObject()->GetLinearVelocity().SetY(REAL_ZERO);
		position.SetY(terrainHeight);
	}

}
