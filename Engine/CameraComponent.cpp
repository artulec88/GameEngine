#include "stdafx.h"
#include "CameraComponent.h"
#include "GameNode.h"
#include "CoreEngine.h"
#include "GameNodeMoveCommand.h"

#include "Math\FloatingPoint.h"

Engine::CameraComponent::CameraComponent(Rendering::Camera* camera) :
	GameComponent(),
	IUpdateable(),
	m_camera(camera)
{
}


Engine::CameraComponent::~CameraComponent(void)
{
}

void Engine::CameraComponent::Update(Math::Real deltaTime)
{
	if (!m_camera->IsActive())
	{
		return;
	}
#ifdef ANT_TWEAK_BAR_ENABLED
	//if ((!Math::AlmostEqual(m_prevAspectRatio, m_aspectRatio)) || (!Math::AlmostEqual(m_prevNearPlane, m_nearPlane)) || (!Math::AlmostEqual(m_prevFarPlane, m_farPlane)) || (m_prevFov != m_fov))
	//{
	//	INFO_LOG_ENGINE("Recalculating the projection matrix for the selected camera");

	//	m_projection.SetPerspectiveProjection(m_fov, m_aspectRatio, m_nearPlane, m_farPlane);

	//	m_prevFov = m_fov;
	//	m_prevAspectRatio = m_aspectRatio;
	//	m_prevNearPlane = m_nearPlane;
	//	m_prevFarPlane = m_farPlane;
	//}
#endif
}

Engine::CameraMoveComponent::CameraMoveComponent(Rendering::Camera* camera) :
	CameraComponent(camera),
	m_forward(false),
	m_backward(false),
	m_left(false),
	m_right(false),
	m_up(false),
	m_down(false),
	m_velocity(REAL_ZERO, REAL_ZERO, REAL_ZERO),
	m_maxSpeed(REAL_ONE),
	m_isLocked(false)
{
}


Engine::CameraMoveComponent::~CameraMoveComponent(void)
{
}

void Engine::CameraMoveComponent::MouseButtonEvent(int button, int action, int mods)
{
	if (!m_camera->IsActive())
	{
		return;
	}
	DEBUG_LOG_ENGINE("Mouse button event for the moving camera (button = ", button, ", action = ", action, ", mods = ", mods, ")");
	switch (button)
	{
	case GLFW_MOUSE_BUTTON_LEFT:
		m_isLocked = (action == GLFW_PRESS); // on GLFW_RELEASE we stop modifying the angle around the entity
		break;
	case GLFW_MOUSE_BUTTON_RIGHT:
		//m_changingPitch = (action == GLFW_PRESS); // on GLFW_RELEASE we stop modifying the pitch
		break;
	case GLFW_MOUSE_BUTTON_MIDDLE:
		break;
	}
}

void Engine::CameraMoveComponent::MousePosEvent(double xPos, double yPos)
{
	if (!m_camera->IsActive() || !m_isLocked)
	{
		return;
	}
	DEBUG_LOG_ENGINE("Mouse position event for the camera following an entity (xPos = ", xPos, "; yPos = ", yPos, ")");

	int width = CoreEngine::GetCoreEngine()->GetWindowWidth();
	int height = CoreEngine::GetCoreEngine()->GetWindowHeight();
	Math::Vector2D centerPosition(static_cast<Math::Real>(width) / 2, static_cast<Math::Real>(height) / 2);
	Math::Vector2D deltaPosition(static_cast<Math::Real>(xPos), static_cast<Math::Real>(yPos));
	deltaPosition -= centerPosition;

	bool rotX = !Math::AlmostEqual(deltaPosition.GetX(), REAL_ZERO);
	bool rotY = !Math::AlmostEqual(deltaPosition.GetY(), REAL_ZERO);

	if (rotX || rotY)
	{
		if (rotX)
		{
			GetTransform().Rotate(Math::Vector3D(0, 1, 0), Math::Angle(deltaPosition.GetX() * m_camera->GetSensitivity()));
		}
		if (rotY)
		{
			GetTransform().Rotate(GetTransform().GetRot().GetRight(), Math::Angle(deltaPosition.GetY() * m_camera->GetSensitivity()));
		}
		CoreEngine::GetCoreEngine()->CentralizeCursor();
	}
}

void Engine::CameraMoveComponent::ScrollEvent(double xOffset, double yOffset)
{
}

void Engine::CameraMoveComponent::Update(Math::Real deltaTime)
{
	if (!m_camera->IsActive())
	{
		return;
	}
	CameraComponent::Update(deltaTime);
	//unsigned int currentCameraIndex = CoreEngine::GetCoreEngine()->GetCurrentCameraIndex();
	//Transform& transform = cameraNodes[currentCameraIndex]->GetTransform();
	//
	///* ==================== Adjusting camera vertical position begin ==================== */
	//timeToUpdateCameraHeight += elapsedTime;
	//if ((m_heightMapCalculationEnabled) && (timeToUpdateCameraHeight > CAMERA_HEIGHT_UPDATE_INTERVAL))
	//{
	//	Math::Real height = planeMesh->GetHeightAt(transform.GetPos().GetXZ());
	//	transform.GetPos().SetY(height);
	//	timeToUpdateCameraHeight = REAL_ZERO;
	//}
	///* ==================== Adjusting camera vertical position begin ==================== */

	Math::Vector3D acceleration;
	if (m_forward)
	{
		acceleration += GetTransform().GetRot().GetForward().Normalized();
	}
	if (m_backward)
	{
		acceleration -= GetTransform().GetRot().GetForward().Normalized();
	}
	if (m_left)
	{
		acceleration -= GetTransform().GetRot().GetRight().Normalized();
	}
	if (m_right)
	{
		acceleration += GetTransform().GetRot().GetRight().Normalized();
	}
	if (m_up)
	{
		acceleration += GetTransform().GetRot().GetUp().Normalized();
	}
	if (m_down)
	{
		acceleration -= GetTransform().GetRot().GetUp().Normalized();
	}
	m_velocity += acceleration * deltaTime * m_camera->GetSensitivity();
	const Math::Real step = 0.1f;
	const Math::Real approachedValue = 0.0f; // must be ZERO!
	if (Math::AlmostEqual(acceleration.GetX(), approachedValue))
	{
		m_velocity.ApproachX(step, approachedValue);
	}
	if (Math::AlmostEqual(acceleration.GetY(), approachedValue))
	{
		m_velocity.ApproachY(step, approachedValue);
	}
	if (Math::AlmostEqual(acceleration.GetZ(), approachedValue))
	{
		m_velocity.ApproachZ(step, approachedValue);
	}
	m_velocity.Threshold(m_maxSpeed);
	//velocity += acceleration * delta;
	//velocity -= slowDownVec;
	//DEBUG_LOG_ENGINE("Acceleration = ", acceleration.ToString(), "\tVelocity = ", velocity.ToString());
	GetTransform().SetPos(GetTransform().GetPos() + m_velocity);
}

//std::string CameraComponent::ToString() const
//{
//	std::stringstream ss("");
//
//	ss << "Camera = { pos = ";
//	ss << GetTransform().GetPos().ToString();
//	ss << "; forward = ";
//	ss << GetTransform().GetRot().GetForward().ToString();
//	ss << "; up = ";
//	ss << GetTransform().GetRot().GetUp().ToString();
//	ss << " } ";
//
//	return ss.str();
//}

/* static */ const Math::Real Engine::CameraFollowComponent::MINIMUM_DISTANCE_TO_ENTITY = 0.1f;
/* static */ const Math::Real Engine::CameraFollowComponent::MAXIMUM_DISTANCE_TO_ENTITY = 0.5f;
/* static */ const Math::Angle Engine::CameraFollowComponent::MINIMUM_PITCH_ANGLE(2.0f);
/* static */ const Math::Angle Engine::CameraFollowComponent::MAXIMUM_PITCH_ANGLE(70.0f);

Engine::CameraFollowComponent::CameraFollowComponent(Rendering::Camera* camera, GameNode* entityToFollow, Math::Real initialDistanceFromEntity,
	Math::Real angleAroundEntitySpeed, Math::Real pitchRotationSpeed, const Math::Angle& initialPitchAngle) :
	CameraComponent(camera),
	m_gameEntityToFollow(entityToFollow),
	m_distanceFromEntity(initialDistanceFromEntity),
	m_changingAngleAroundEntity(false),
	m_angleAroundEntitySpeed(angleAroundEntitySpeed),
	m_currentAngleAroundEntity(REAL_ZERO),
	m_changingPitch(false),
	m_pitchRotationSpeed(pitchRotationSpeed),
	m_currentPitchAngle(initialPitchAngle),
	m_mousePos(REAL_ZERO, REAL_ZERO),
	m_prevMousePos(REAL_ZERO, REAL_ZERO),
	m_mousePosChanged(false)
{
}


Engine::CameraFollowComponent::~CameraFollowComponent(void)
{
}

//void Engine::CameraFollowComponent::MouseButtonEvent(int button, int action, int mods)
//{
//	if (!m_camera->IsActive())
//	{
//		return;
//	}
//	DEBUG_LOG_ENGINE("Mouse button event for the camera following an entity (button = ", button, ", action = ", action, ", mods = ", mods, ")");
//	switch (button)
//	{
//	case GLFW_MOUSE_BUTTON_LEFT:
//		m_changingAngleAroundEntity = (action == GLFW_PRESS); // on GLFW_RELEASE we stop modifying the angle around the entity
//		break;
//	case GLFW_MOUSE_BUTTON_RIGHT:
//		m_changingPitch = (action == GLFW_PRESS); // on GLFW_RELEASE we stop modifying the pitch
//		break;
//	case GLFW_MOUSE_BUTTON_MIDDLE:
//		break;
//	}
//}

//void Engine::CameraFollowComponent::MousePosEvent(double xPos, double yPos)
//{
//	if (!m_camera->IsActive())
//	{
//		m_lastCursorPositionX = static_cast<Math::Real>(xPos);
//		m_lastCursorPositionY = static_cast<Math::Real>(yPos);
//		return;
//	}
//	DEBUG_LOG_ENGINE("Mouse position event for the camera following an entity (xPos = ", xPos, "; yPos = ", yPos, ")");
//
//	if (m_changingPitch)
//	{
//		m_currentPitchAngle += m_pitchRotationSpeed * (static_cast<Math::Real>(yPos) - m_lastCursorPositionY);
//		if (m_currentPitchAngle < MINIMUM_PITCH_ANGLE)
//		{
//			m_currentPitchAngle = MINIMUM_PITCH_ANGLE;
//		}
//		else if (m_currentPitchAngle > MAXIMUM_PITCH_ANGLE)
//		{
//			m_currentPitchAngle = MAXIMUM_PITCH_ANGLE;
//		}
//	}
//	if (m_changingAngleAroundEntity)
//	{
//		m_currentAngleAroundEntity += m_angleAroundEntitySpeed * (static_cast<Math::Real>(xPos) - m_lastCursorPositionX);
//	}
//	m_lastCursorPositionX = static_cast<Math::Real>(xPos);
//	m_lastCursorPositionY = static_cast<Math::Real>(yPos);
//}

//void Engine::CameraFollowComponent::ScrollEvent(double xOffset, double yOffset)
//{
//	if (!m_camera->IsActive())
//	{
//		return;
//	}
//	DEBUG_LOG_ENGINE("Scroll event for the camera following an entity (xOffset = ", xOffset, "; yOffset = ", yOffset, ")");
//
//	m_distanceFromEntity -= static_cast<Math::Real>(yOffset) * 0.03f;
//	if (m_distanceFromEntity < MINIMUM_DISTANCE_TO_ENTITY)
//	{
//		m_distanceFromEntity = MINIMUM_DISTANCE_TO_ENTITY;
//	}
//	else if (m_distanceFromEntity > MAXIMUM_DISTANCE_TO_ENTITY)
//	{
//		m_distanceFromEntity = MAXIMUM_DISTANCE_TO_ENTITY;
//	}
//	DELOCUST_LOG_ENGINE("Distance from entity = ", m_distanceFromEntity);
//}

void Engine::CameraFollowComponent::Update(Math::Real deltaTime)
{
	if (!m_camera->IsActive())
	{
		return;
	}
	CameraComponent::Update(deltaTime);

	Math::Real horizontalDistance = m_distanceFromEntity * m_currentPitchAngle.Cos();
	Math::Real verticalDistance = m_distanceFromEntity * m_currentPitchAngle.Sin();
	Math::Real xOffset = horizontalDistance * m_currentAngleAroundEntity.Sin();
	Math::Real zOffset = horizontalDistance * m_currentAngleAroundEntity.Cos();
	GetTransform().SetPos(m_gameEntityToFollow->GetTransform().GetPos() + Math::Vector3D(-xOffset, verticalDistance + 0.03f /* to focus on upperbody instead of feet */, -zOffset));
	m_camera->SetPos(m_gameEntityToFollow->GetTransform().GetPos() + Math::Vector3D(-xOffset, verticalDistance + 0.03f /* to focus on upperbody instead of feet */, -zOffset)); // TODO: Double code
	GetTransform().SetRot(Math::Quaternion(Math::Vector3D(REAL_ZERO, REAL_ONE, REAL_ZERO), m_currentAngleAroundEntity) * Math::Quaternion(Math::Vector3D(REAL_ONE, REAL_ZERO, REAL_ZERO), m_currentPitchAngle));
	m_camera->SetRot(Math::Quaternion(Math::Vector3D(REAL_ZERO, REAL_ONE, REAL_ZERO), m_currentAngleAroundEntity) * Math::Quaternion(Math::Vector3D(REAL_ONE, REAL_ZERO, REAL_ZERO), m_currentPitchAngle)); // TODO: Double code
}

void Engine::CameraFollowComponent::Handle(States::State state)
{
	switch (state)
	{
	//case INVALID:
	case States::MOUSE_KEY_LEFT_PRESSED:
		if (m_mousePosChanged)
		{
			m_currentAngleAroundEntity += m_angleAroundEntitySpeed * (m_mousePos.GetX() - m_prevMousePos.GetX());
		}
		break;
	case States::MOUSE_KEY_MIDDLE_PRESSED:
		break;
	case States::MOUSE_KEY_RIGHT_PRESSED:
		break;
	default:
		break;
	}
}

void Engine::CameraFollowComponent::Handle(Ranges::Range range, Math::Real value)
{
	switch (range)
	{
	case Ranges::AXIS_X:
		m_prevMousePos.SetX(m_mousePos.GetX());
		m_mousePos.SetX(value);
		m_mousePosChanged = true;
		break;
	case Ranges::AXIS_Y:
		m_prevMousePos.SetY(m_mousePos.GetY());
		m_mousePos.SetY(value);
		m_mousePosChanged = true;
		break;
	default:
		break;
	}
}

//std::string Engine::CameraComponent::ToString() const
//{
//	std::stringstream ss("");
//
//	ss << "Camera = { pos = ";
//	ss << GetTransform().GetPos().ToString();
//	ss << "; forward = ";
//	ss << GetTransform().GetRot().GetForward().ToString();
//	ss << "; up = ";
//	ss << GetTransform().GetRot().GetUp().ToString();
//	ss << " } ";
//
//	return ss.str();
//}