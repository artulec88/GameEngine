#include "stdafx.h"
#include "MoveComponent.h"
#include "Math\Transform.h"
#include "Math\Quaternion.h"

Engine::MoveComponent::MoveComponent(Physics::PhysicsObject* physicsObject, Math::Real movementSpeed, Math::Real runSpeedFactor, Math::Angle& rotationSpeed, Math::Real strafeSpeed, Math::Real jumpSpeed) :
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


Engine::MoveComponent::~MoveComponent()
{
}

void Engine::MoveComponent::Update(Math::Real deltaTime)
{
	Math::Transform& transform = GetTransform();
	transform.SetRot(transform.GetRot() * Math::Quaternion(Math::Vector3D(REAL_ZERO, REAL_ONE, REAL_ZERO), m_currentRotationSpeed * deltaTime));
	Math::Vector3D distanceToMove = transform.GetRot().GetForward() * m_currentMovementSpeed * deltaTime;
	transform.SetPos(transform.GetPos() + distanceToMove);

	if (m_isJumping)
	{
		// todo: jump physical entity
		m_isJumping = false;
	}
	//m_currentJumpSpeed -= GRAVITY * deltaTime;
	//Math::Real y = transform.GetPos().GetY() + m_currentJumpSpeed;
	//if (y < REAL_ZERO /* terrain height */) // TODO: Terrain height calculation
	//{
	//	y = REAL_ZERO;
	//	m_currentJumpSpeed = REAL_ZERO;
	//	m_isJumping = false;
	//}
	//transform.GetPos().SetY(y);

	//m_currentMovementSpeed = REAL_ZERO;
	//m_currentRotationSpeed = Math::Angle(REAL_ZERO);
	//m_currentStrafeSpeed = REAL_ZERO;
}

void Engine::MoveComponent::KeyEvent(int key, int scancode, int action, int mods)
{
	DEBUG_LOG("key = %d, scancode = %d, action = %d, mods = %d", key, scancode, action, mods);
	switch (key)
	{
	case GLFW_KEY_LEFT_SHIFT:
		if (action == GLFW_PRESS)
		{
			m_currentMovementSpeed *= m_runSpeedFactor;
		}
		else if (action == GLFW_RELEASE)
		{
			m_currentMovementSpeed /= m_runSpeedFactor;
		}
		break;
	case GLFW_KEY_W:
		m_currentMovementSpeed = ((action == GLFW_PRESS) || (action == GLFW_REPEAT)) ? m_movementSpeed : REAL_ZERO;
		if ((GLFW_MOD_SHIFT & mods) == GLFW_MOD_SHIFT)
		{
			m_currentMovementSpeed *= m_runSpeedFactor;
		}
		break;
	case GLFW_KEY_S:
		m_currentMovementSpeed = ((action == GLFW_PRESS) || (action == GLFW_REPEAT)) ? -m_movementSpeed : REAL_ZERO;
		break;
	case GLFW_KEY_A:
		if ((action == GLFW_PRESS) || (action == GLFW_REPEAT))
		{
			//m_currentStrafeSpeed -= m_strafeSpeed;
		}
		//DEBUG_LOG("Left = %d", left);
		break;
	case GLFW_KEY_D:
		if ((action == GLFW_PRESS) || (action == GLFW_REPEAT))
		{
			//m_currentStrafeSpeed += m_strafeSpeed;
		}
		//DEBUG_LOG("Right = %d", right);
		break;
	case GLFW_KEY_SPACE: // move up
		if (!m_isJumping && (action == GLFW_PRESS || action == GLFW_REPEAT))
		{
			m_currentJumpSpeed = m_jumpSpeed;
			m_isJumping = true;
		}
		break;
	case GLFW_KEY_LEFT_CONTROL: // move down
								// m_down = ((action == GLFW_PRESS) || (action == GLFW_REPEAT));
								// TODO: Crouch action
		break;
	case GLFW_KEY_UP: // rotation around X axis
					  //transform.Rotate(transform.GetRot().GetRight(), Angle(-sensitivity));
		break;
	case GLFW_KEY_DOWN: // rotation around X axis
						//transform.Rotate(transform.GetRot().GetRight(), Angle(sensitivity));
		break;
	case GLFW_KEY_LEFT: // rotation around Y axis
		((action == GLFW_PRESS) || (action == GLFW_REPEAT)) ? m_currentRotationSpeed = -m_rotationSpeed : m_currentRotationSpeed.SetAngleInRadians(REAL_ZERO);
		break;
	case GLFW_KEY_RIGHT: // rotation around Y axis
		((action == GLFW_PRESS) || (action == GLFW_REPEAT)) ? m_currentRotationSpeed = m_rotationSpeed : m_currentRotationSpeed.SetAngleInRadians(REAL_ZERO);
		break;
	}
}

void Engine::MoveComponent::MouseButtonEvent(int button, int action, int mods)
{
	// TODO: Primary attack action
}

void Engine::MoveComponent::MousePosEvent(double xPos, double yPos)
{
}

void Engine::MoveComponent::ScrollEvent(double xOffset, double yOffset)
{
}