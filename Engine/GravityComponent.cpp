#include "stdafx.h"
#include "GravityComponent.h"

#include "Math\FloatingPoint.h"

#include "Physics\PhysicsObject.h"

#include "Utility\IConfig.h"

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
	Math::Real terrainHeight = (Math::AlmostEqual(GetTransform().GetPos().x, m_lastX) && Math::AlmostEqual(GetTransform().GetPos().z, m_lastZ)) ? m_lastHeight :
		m_terrainMesh->GetHeightAt(GetTransform().GetPos().x, GetTransform().GetPos().z) + m_heightAdjustment;
	m_lastX = GetTransform().GetPos().x;
	m_lastZ = GetTransform().GetPos().z;
	m_lastHeight = terrainHeight;
	if (GetTransform().GetPos().y > terrainHeight)
	{
		GetPhysicsObject()->ApplyLinearAcceleration(GRAVITY_ACCELERATION);
	}
	else
	{
		// Applying an impulse to stop the physics object immediately
		GetPhysicsObject()->GetLinearVelocity().y = REAL_ZERO;
		GetTransform().SetPosY(terrainHeight);
	}

}
