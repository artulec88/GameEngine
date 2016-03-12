#include "stdafx.h"
#include "GravityComponent.h"

// TODO: Don't hard-code any value! Ever!
// TODO: This variable should be moved into the Physics engine and be accessible from anywhere (similarily as stdlog is accessible).
/* static */ const Math::Vector3D Engine::GravityComponent::GRAVITY_ACCELERATION(0.0f, -9.8f, 0.0f);

Engine::GravityComponent::GravityComponent(Physics::PhysicsObject* physicsObject) :
	GameComponent(),
	m_physicsObject(physicsObject)
{
	m_physicsObject->ApplyLinearAcceleration(GRAVITY_ACCELERATION);
}


Engine::GravityComponent::~GravityComponent()
{
}

void Engine::GravityComponent::Update(Math::Real deltaTime)
{
	//m_physicsObject->ApplyLinearAcceleration(GRAVITY_ACCELERATION);

	//Math::Vector3D& position = GetTransform().GetPos();
	//Math::Real terrainHeight = m_terrainMesh->GetHeightAt(position.GetXZ());
	//position.SetY(terrainHeight);
	//m_currentMovementSpeed = REAL_ZERO;
	//m_currentRotationSpeed = Math::Angle(REAL_ZERO);
	//m_currentStrafeSpeed = REAL_ZERO;

}
