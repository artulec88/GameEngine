#include "stdafx.h"
#include "PhysicsComponent.h"
#include "Math\Transform.h"
#include "Math\Quaternion.h"

///* static */ const Math::Real Engine::PhysicsComponent::GRAVITY = 0.001f; // TODO: Don't hard-code any value! Ever!

Engine::PhysicsComponent::PhysicsComponent(Math::Real jumpForce) :
	GameComponent(),
	m_jumpAcceleration(REAL_ZERO, jumpForce, REAL_ZERO),
	m_isJumping(false)
{
}


Engine::PhysicsComponent::~PhysicsComponent()
{
	//SAFE_DELETE(m_physicsObject);
}

void Engine::PhysicsComponent::Update(Math::Real deltaTime)
{
	if (m_isJumping)
	{
		GetPhysicsObject()->ApplyLinearAcceleration(m_jumpAcceleration);
		m_isJumping = false;
	}
	//m_physicsObject->Integrate(deltaTime);
}

void Engine::PhysicsComponent::KeyEvent(int key, int scancode, int action, int mods)
{
	DEBUG_LOG("key = %d, scancode = %d, action = %d, mods = %d", key, scancode, action, mods);
	switch (key)
	{
	case GLFW_KEY_LEFT_SHIFT:
		break;
	case GLFW_KEY_W:
		break;
	case GLFW_KEY_S:
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
			//m_physicsObject->ApplyLinearAcceleration(m_jumpAcceleration);
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
		//((action == GLFW_PRESS) || (action == GLFW_REPEAT)) ? m_currentRotationSpeed = -m_rotationSpeed : m_currentRotationSpeed.SetAngleInRadians(REAL_ZERO);
		break;
	case GLFW_KEY_RIGHT: // rotation around Y axis
		//((action == GLFW_PRESS) || (action == GLFW_REPEAT)) ? m_currentRotationSpeed = m_rotationSpeed : m_currentRotationSpeed.SetAngleInRadians(REAL_ZERO);
		break;
	}
}
