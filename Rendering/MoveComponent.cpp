#include "stdafx.h"
#include "MoveComponent.h"
#include "Transform.h"
#include "Math\Quaternion.h"

/* static */ const Math::Real Rendering::MoveComponent::GRAVITY = 0.001f;

Rendering::MoveComponent::MoveComponent(Math::Real movementSpeed, const Math::Angle& rotationSpeed, Math::Real strafeSpeed, Math::Real jumpSpeed) :
	GameComponent(),
	m_movementSpeed(movementSpeed),
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


Rendering::MoveComponent::~MoveComponent()
{
}

void Rendering::MoveComponent::Update(Math::Real deltaTime)
{
	Transform& transform = GetTransform();
	transform.SetRot(transform.GetRot() * Math::Quaternion(Math::Vector3D(REAL_ZERO, REAL_ONE, REAL_ZERO), m_currentRotationSpeed * deltaTime));
	Math::Vector3D distanceToMove = transform.GetRot().GetForward() * m_currentMovementSpeed * deltaTime;
	//if (m_currentMovementSpeed > 0.0000001f)
	//{
		//CRITICAL_LOG("m_currentMovementSpeed = %.2f, deltaTime = %.6f, distanceToMove = %s", m_currentMovementSpeed, deltaTime, distanceToMove.ToString().c_str());
	//}
	m_currentJumpSpeed -= GRAVITY * deltaTime;
	transform.SetPos(transform.GetPos() + distanceToMove);
	Math::Real y = transform.GetPos().GetY() + m_currentJumpSpeed;
	if (y < REAL_ZERO /* terrain height */) // TODO: Terrain height calculation
	{
		y = REAL_ZERO;
		m_currentJumpSpeed = REAL_ZERO;
		m_isJumping = false;
	}
	transform.GetPos().SetY(y);
	//m_currentMovementSpeed = REAL_ZERO;
	//m_currentRotationSpeed = Math::Angle(REAL_ZERO);
	//m_currentStrafeSpeed = REAL_ZERO;
}

void Rendering::MoveComponent::KeyEvent(int key, int scancode, int action, int mods)
{
	DEBUG_LOG("key = %d, scancode = %d, action = %d, mods = %d", key, scancode, action, mods);
	switch (key)
	{
	case GLFW_KEY_W:
		m_currentMovementSpeed = ((action == GLFW_PRESS) || (action == GLFW_REPEAT)) ? m_movementSpeed : REAL_ZERO;
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

void Rendering::MoveComponent::MouseButtonEvent(int button, int action, int mods)
{
	// TODO: Primary attack action
}

void Rendering::MoveComponent::MousePosEvent(double xPos, double yPos)
{
}

void Rendering::MoveComponent::ScrollEvent(double xOffset, double yOffset)
{
}