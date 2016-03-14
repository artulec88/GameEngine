#include "stdafx.h"
#include "GravityComponent.h"

#include "Math\FloatingPoint.h"

#include "Physics\PhysicsObject.h"

// TODO: Don't hard-code any value! Ever!
// TODO: This variable should be moved into the Physics engine and be accessible from anywhere (similarily as stdlog is accessible).
/* static */ const Math::Vector3D Engine::GravityComponent::GRAVITY_ACCELERATION(0.0f, -9.8f, 0.0f);

Engine::GravityComponent::GravityComponent(const Rendering::TerrainMesh* terrainMesh) :
	GameComponent(),
	m_terrainMesh(terrainMesh),
	m_lastX(REAL_ZERO),
	m_lastZ(REAL_ZERO),
	m_lastHeight(REAL_ZERO)
{
	//m_physicsObject->ApplyLinearAcceleration(GRAVITY_ACCELERATION);
}


Engine::GravityComponent::~GravityComponent()
{
}

void Engine::GravityComponent::Update(Math::Real deltaTime)
{
	Math::Vector3D& position = GetTransform().GetPos();
	Math::Real terrainHeight = (Math::AlmostEqual(position.GetX(), m_lastX) && Math::AlmostEqual(position.GetZ(), m_lastZ)) ? m_lastHeight :
		m_terrainMesh->GetHeightAt(position.GetX(), position.GetZ(), true);
	m_lastX = position.GetX();
	m_lastZ = position.GetZ();
	m_lastHeight = terrainHeight;
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
