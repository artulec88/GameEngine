#include "stdafx.h"
#include "GravityComponent.h"

#include "Math\FloatingPoint.h"

#include "Physics\PhysicsObject.h"

#include "Utility\IConfig.h"

engine::GravityComponent::GravityComponent(const Rendering::Terrain* terrain) :
	GameComponent(),
	IUpdateable(),
	m_terrain(terrain),
	m_lastX(REAL_ZERO),
	m_lastZ(REAL_ZERO),
	m_lastHeight(REAL_ZERO),
	m_heightAdjustment(GET_CONFIG_VALUE_ENGINE("headPositionHeightAdjustment", 2.5f))
{
	//m_physicsObject->ApplyLinearAcceleration(GRAVITY_ACCELERATION);
}


engine::GravityComponent::~GravityComponent()
{
}

engine::GravityComponent::GravityComponent(GravityComponent&& gravityComponent) :
	GameComponent(std::move(gravityComponent)),
	IUpdateable(std::move(gravityComponent)),
	m_terrain(std::move(gravityComponent.m_terrain)),
	m_lastX(std::move(gravityComponent.m_lastX)),
	m_lastZ(std::move(gravityComponent.m_lastZ)),
	m_lastHeight(std::move(gravityComponent.m_lastHeight)),
	m_heightAdjustment(std::move(gravityComponent.m_heightAdjustment))
{
}

engine::GravityComponent& engine::GravityComponent::operator=(GravityComponent&& gravityComponent)
{
	GameComponent::operator=(std::move(gravityComponent));
	IUpdateable::operator=(std::move(gravityComponent));
	m_terrain = std::move(gravityComponent.m_terrain);
	m_lastX = std::move(gravityComponent.m_lastX);
	m_lastZ = std::move(gravityComponent.m_lastZ);
	m_lastHeight = std::move(gravityComponent.m_lastHeight);
	m_heightAdjustment = std::move(gravityComponent.m_heightAdjustment);
	return *this;
}

void engine::GravityComponent::Update(math::Real deltaTime)
{
	math::Real terrainHeight = (math::AlmostEqual(GetTransform().GetPos().x, m_lastX) && math::AlmostEqual(GetTransform().GetPos().z, m_lastZ)) ? m_lastHeight :
		m_terrain->GetHeightAt(GetTransform().GetPos().x, GetTransform().GetPos().z) + m_heightAdjustment;
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
