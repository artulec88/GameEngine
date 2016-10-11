#include "stdafx.h"
#include "GravityComponent.h"

#include "Math\FloatingPoint.h"

#include "Physics\PhysicsObject.h"

#include "Utility\IConfig.h"

// TODO: Don't hard-code any value! Ever!
// TODO: This variable should be moved into the Physics engine and be accessible from anywhere (similarily as stdlog is accessible).
/* static */ const Math::Vector3D Engine::GravityComponent::GRAVITY_ACCELERATION(0.0f, -9.8f, 0.0f);

Engine::GravityComponent::GravityComponent(const Rendering::TerrainMesh* terrainMesh) :
	GameComponent(),
	IUpdateable(),
	m_terrainMesh(terrainMesh),
	m_lastX(REAL_ZERO),
	m_lastZ(REAL_ZERO),
	m_lastHeight(REAL_ZERO),
	m_heightAdjustment(GET_CONFIG_VALUE_ENGINE("headPositionHeightAdjustment", 2.5f))
{
	//m_physicsObject->ApplyLinearAcceleration(GRAVITY_ACCELERATION);
}


Engine::GravityComponent::~GravityComponent()
{
}

Engine::GravityComponent::GravityComponent(GravityComponent&& gravityComponent) :
	GameComponent(std::move(gravityComponent)),
	IUpdateable(std::move(gravityComponent)),
	m_terrainMesh(std::move(gravityComponent.m_terrainMesh)),
	m_lastX(std::move(gravityComponent.m_lastX)),
	m_lastZ(std::move(gravityComponent.m_lastZ)),
	m_lastHeight(std::move(gravityComponent.m_lastHeight)),
	m_heightAdjustment(std::move(gravityComponent.m_heightAdjustment))
{
}

Engine::GravityComponent& Engine::GravityComponent::operator=(GravityComponent&& gravityComponent)
{
	GameComponent::operator=(std::move(gravityComponent));
	IUpdateable::operator=(std::move(gravityComponent));
	m_terrainMesh = std::move(gravityComponent.m_terrainMesh);
	m_lastX = std::move(gravityComponent.m_lastX);
	m_lastZ = std::move(gravityComponent.m_lastZ);
	m_lastHeight = std::move(gravityComponent.m_lastHeight);
	m_heightAdjustment = std::move(gravityComponent.m_heightAdjustment);
	return *this;
}

void Engine::GravityComponent::Update(Math::Real deltaTime)
{
	Math::Real terrainHeight = (Math::AlmostEqual(GetTransform().GetPos().GetX(), m_lastX) && Math::AlmostEqual(GetTransform().GetPos().GetZ(), m_lastZ)) ? m_lastHeight :
		m_terrainMesh->GetHeightAt(GetTransform().GetPos().GetX(), GetTransform().GetPos().GetZ()) + m_heightAdjustment;
	m_lastX = GetTransform().GetPos().GetX();
	m_lastZ = GetTransform().GetPos().GetZ();
	m_lastHeight = terrainHeight;
	if (GetTransform().GetPos().GetY() > terrainHeight)
	{
		GetPhysicsObject()->ApplyLinearAcceleration(GRAVITY_ACCELERATION);
	}
	else
	{
		// Applying an impulse to stop the physics object immediately
		GetPhysicsObject()->GetLinearVelocity().SetY(REAL_ZERO);
		GetTransform().SetPosY(terrainHeight);
	}

}
