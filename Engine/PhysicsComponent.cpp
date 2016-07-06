#include "stdafx.h"
#include "PhysicsComponent.h"
#include "Math\Transform.h"
#include "Math\Quaternion.h"
#include "CoreEngine.h"

///* static */ const Math::Real Engine::PhysicsComponent::GRAVITY = 0.001f; // TODO: Don't hard-code any value! Ever!

Engine::PhysicsComponent::PhysicsComponent(Math::Real movementSpeed, Math::Real jumpForce) :
	GameComponent(),
	m_moveSpeed(movementSpeed),
	m_jumpAcceleration(REAL_ZERO, jumpForce, REAL_ZERO),
	m_isJumping(false)
{
	CoreEngine::GetCoreEngine()->GetAudioEngine().LoadSoundEffect(CoreEngine::GetCoreEngine()->GetAudioDirectory() + "\\Mario Jump.wav");
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
		m_isJumping = false; // TODO: make it false when landing on the surface.
	}
	//m_physicsObject->Integrate(deltaTime);
}

//void Engine::PhysicsComponent::KeyEvent(int key, int scancode, int action, int mods)
//{
//	DEBUG_LOG_ENGINE("key = %d, scancode = %d, action = %d, mods = %d", key, scancode, action, mods);
//	switch (key)
//	{
//	case GLFW_KEY_LEFT_SHIFT:
//		break;
//	case GLFW_KEY_W:
//		if (action == GLFW_PRESS)
//		{
//			//sendEvent(MOVE_FORWARD);
//			GetPhysicsObject()->ApplyLinearAcceleration(GetTransform().GetTransformedRot().GetForward() * m_moveSpeed);
//		}
//		else if (action == GLFW_RELEASE)
//		{
//			GetPhysicsObject()->ApplyLinearAcceleration(-GetTransform().GetTransformedRot().GetForward() * m_moveSpeed);
//		}
//		break;
//	case GLFW_KEY_S:
//		if (action == GLFW_PRESS)
//		{
//			GetPhysicsObject()->ApplyLinearAcceleration(-GetTransform().GetTransformedRot().GetForward() * m_moveSpeed);
//		}
//		else if (action == GLFW_RELEASE)
//		{
//			GetPhysicsObject()->ApplyLinearAcceleration(GetTransform().GetTransformedRot().GetForward() * m_moveSpeed);
//		}
//		break;
//	case GLFW_KEY_A:
//		if (action == GLFW_PRESS)
//		{
//			GetPhysicsObject()->ApplyLinearAcceleration(GetTransform().GetTransformedRot().GetLeft() * m_moveSpeed);
//		}
//		else if (action == GLFW_RELEASE)
//		{
//			GetPhysicsObject()->ApplyLinearAcceleration(-GetTransform().GetTransformedRot().GetLeft() * m_moveSpeed);
//		}
//		//DEBUG_LOG_ENGINE("Left = %d", left);
//		break;
//	case GLFW_KEY_D:
//		if (action == GLFW_PRESS)
//		{
//			GetPhysicsObject()->ApplyLinearAcceleration(GetTransform().GetTransformedRot().GetRight() * m_moveSpeed);
//		}
//		else if (action == GLFW_RELEASE)
//		{
//			GetPhysicsObject()->ApplyLinearAcceleration(-GetTransform().GetTransformedRot().GetRight() * m_moveSpeed);
//		}
//		break;
//	case GLFW_KEY_SPACE: // move up
//		if (!m_isJumping && (action == GLFW_PRESS || action == GLFW_REPEAT))
//		{
//			//m_physicsObject->ApplyLinearAcceleration(m_jumpAcceleration);
//			// TODO: Think of a more sophisticated way of triggerings sound effects.
//			// SendEvent(JUMP);
//			CoreEngine::GetCoreEngine()->GetAudioEngine().PlaySoundEffect(CoreEngine::GetCoreEngine()->GetAudioDirectory() + "\\Mario Jump.wav", 0.1f, 1.0f);
//			m_isJumping = true;
//		}
//		break;
//	case GLFW_KEY_LEFT_CONTROL: // move down
//								// m_down = ((action == GLFW_PRESS) || (action == GLFW_REPEAT));
//								// TODO: Crouch action
//		break;
//	case GLFW_KEY_UP: // rotation around X axis
//					  //transform.Rotate(transform.GetRot().GetRight(), Angle(-sensitivity));
//		break;
//	case GLFW_KEY_DOWN: // rotation around X axis
//						//transform.Rotate(transform.GetRot().GetRight(), Angle(sensitivity));
//		break;
//	case GLFW_KEY_LEFT: // rotation around Y axis
//						//((action == GLFW_PRESS) || (action == GLFW_REPEAT)) ? m_currentRotationSpeed = -m_rotationSpeed : m_currentRotationSpeed.SetAngleInRadians(REAL_ZERO);
//		break;
//	case GLFW_KEY_RIGHT: // rotation around Y axis
//						 //((action == GLFW_PRESS) || (action == GLFW_REPEAT)) ? m_currentRotationSpeed = m_rotationSpeed : m_currentRotationSpeed.SetAngleInRadians(REAL_ZERO);
//		break;
//	}
//}
