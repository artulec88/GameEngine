#include "stdafx.h"
#include "MoveComponent.h"
#include "Math/Transform.h"
#include "Math/Quaternion.h"

engine::MoveComponent::MoveComponent(physics::PhysicsObject* physicsObject, math::Real movementSpeed, math::Real runSpeedFactor, math::Angle& rotationSpeed, math::Real strafeSpeed, math::Real jumpSpeed) :
	GameComponent(),
	m_physicsObject(physicsObject),
	m_movementSpeed(movementSpeed),
	m_runSpeedFactor(runSpeedFactor),
	m_rotationSpeed(rotationSpeed),
	m_strafeSpeed(strafeSpeed),
	m_jumpSpeed(jumpSpeed),
	m_currentMovementSpeed(REAL_ZERO),
	m_currentRotationSpeed(REAL_ZERO),
	m_currentStrafeSpeed(REAL_ZERO),
	m_currentJumpSpeed(REAL_ZERO),
	m_isJumping(false)
{
}


engine::MoveComponent::~MoveComponent()
{
}

void engine::MoveComponent::Update(math::Real deltaTime)
{
	math::Transform& transform = GetTransform();
	transform.SetRot(transform.GetRot() * math::Quaternion(math::Vector3D(REAL_ZERO, REAL_ONE, REAL_ZERO), m_currentRotationSpeed * deltaTime));
	const auto distanceToMove = transform.GetRot().GetForward() * m_currentMovementSpeed * deltaTime;
	transform.SetPos(transform.GetPos() + distanceToMove);

	if (m_isJumping)
	{
		// todo: jump physical entity
		m_isJumping = false;
	}
	//m_currentJumpSpeed -= GRAVITY * deltaTime;
	//math::Real y = transform.GetPos().GetY() + m_currentJumpSpeed;
	//if (y < REAL_ZERO /* terrain height */) // TODO: Terrain height calculation
	//{
	//	y = REAL_ZERO;
	//	m_currentJumpSpeed = REAL_ZERO;
	//	m_isJumping = false;
	//}
	//transform.GetPos().SetY(y);

	//m_currentMovementSpeed = REAL_ZERO;
	//m_currentRotationSpeed = math::Angle(REAL_ZERO);
	//m_currentStrafeSpeed = REAL_ZERO;
}

void engine::MoveComponent::MouseButtonEvent(int button, int action, int mods)
{
	// TODO: Primary attack action
}

void engine::MoveComponent::MousePosEvent(double xPos, double yPos)
{
}

void engine::MoveComponent::ScrollEvent(double xOffset, double yOffset)
{
}