#include "stdafx.h"
#include "CameraComponent.h"
#include "CoreEngine.h"

/* ==================== CameraComponent class implementation begin ==================== */
engine::CameraComponent::CameraComponent(rendering::Camera* camera, CameraBehavior* cameraBehavior /* = nullptr */) :
	GameComponent(),
	IActionHandler(),
	IStateHandler(),
	IRangeHandler(),
	m_camera(camera),
	m_cameraBehavior(cameraBehavior)
{
}

engine::CameraComponent::~CameraComponent()
{
	SAFE_DELETE(m_camera);
	SAFE_DELETE(m_cameraBehavior);
}

engine::CameraComponent::CameraComponent(CameraComponent&& cameraComponent) noexcept:
	GameComponent(std::move(cameraComponent)),
	IActionHandler(std::move(cameraComponent)),
	IStateHandler(std::move(cameraComponent)),
	IRangeHandler(std::move(cameraComponent)),
	m_camera(std::move(cameraComponent.m_camera)),
	m_cameraBehavior(std::move(cameraComponent.m_cameraBehavior))
{
	cameraComponent.m_cameraBehavior = nullptr;
}


engine::CameraComponent& engine::CameraComponent::operator=(CameraComponent&& cameraComponent) noexcept
{
	GameComponent::operator=(std::move(cameraComponent));
	IActionHandler::operator=(std::move(cameraComponent));
	IStateHandler::operator=(std::move(cameraComponent));
	IRangeHandler::operator=(std::move(cameraComponent));
	m_camera = std::move(cameraComponent.m_camera);
	m_cameraBehavior = std::move(cameraComponent.m_cameraBehavior);
	cameraComponent.m_camera = nullptr;
	cameraComponent.m_cameraBehavior = nullptr;
	return *this;
}
/* ==================== CameraComponent class implementation end ==================== */

//Engine::CameraMoveComponent::CameraMoveComponent(Rendering::Camera* camera) :
//	CameraComponent(camera),
//	IInputableMouse(),
//	m_forward(false),
//	m_backward(false),
//	m_left(false),
//	m_right(false),
//	m_up(false),
//	m_down(false),
//	m_velocity(REAL_ZERO, REAL_ZERO, REAL_ZERO),
//	m_maxSpeed(REAL_ONE),
//	m_isLocked(false)
//{
//}
//
//
//Engine::CameraMoveComponent::~CameraMoveComponent(void)
//{
//}
//
//Engine::CameraMoveComponent::CameraMoveComponent(CameraMoveComponent&& cameraMoveComponent) :
//	CameraComponent(std::move(cameraMoveComponent)),
//	IInputableMouse(std::move(cameraMoveComponent)),
//	m_forward(std::move(cameraMoveComponent.m_forward)),
//	m_backward(std::move(cameraMoveComponent.m_backward)),
//	m_left(std::move(cameraMoveComponent.m_left)),
//	m_right(std::move(cameraMoveComponent.m_right)),
//	m_up(std::move(cameraMoveComponent.m_up)),
//	m_down(std::move(cameraMoveComponent.m_down)),
//	m_velocity(std::move(cameraMoveComponent.m_velocity)),
//	m_maxSpeed(std::move(cameraMoveComponent.m_maxSpeed)),
//	m_isLocked(std::move(cameraMoveComponent.m_isLocked))
//{
//}
//
//Engine::CameraMoveComponent& Engine::CameraMoveComponent::operator=(CameraMoveComponent&& cameraMoveComponent)
//{
//	CameraComponent::operator=(std::move(cameraMoveComponent));
//	IInputableMouse::operator=(std::move(cameraMoveComponent));
//	m_forward = std::move(cameraMoveComponent.m_forward);
//	m_backward = std::move(cameraMoveComponent.m_backward);
//	m_left = std::move(cameraMoveComponent.m_left);
//	m_right = std::move(cameraMoveComponent.m_right);
//	m_up = std::move(cameraMoveComponent.m_up);
//	m_down = std::move(cameraMoveComponent.m_down);
//	m_velocity = std::move(cameraMoveComponent.m_velocity);
//	m_maxSpeed = std::move(cameraMoveComponent.m_maxSpeed);
//	m_isLocked = std::move(cameraMoveComponent.m_isLocked);
//	return *this;
//}
//
//void Engine::CameraMoveComponent::MouseButtonEvent(int button, int action, int mods)
//{
//	if (!m_camera->IsActive())
//	{
//		return;
//	}
//	DEBUG_LOG_ENGINE("Mouse button event for the moving camera (button = ", button, ", action = ", action, ", mods = ", mods, ")");
//	switch (button)
//	{
//	case GLFW_MOUSE_BUTTON_LEFT:
//		m_isLocked = (action == GLFW_PRESS); // on GLFW_RELEASE we stop modifying the angle around the entity
//		break;
//	case GLFW_MOUSE_BUTTON_RIGHT:
//		//m_changingPitch = (action == GLFW_PRESS); // on GLFW_RELEASE we stop modifying the pitch
//		break;
//	case GLFW_MOUSE_BUTTON_MIDDLE:
//		break;
//	}
//}
//
//void Engine::CameraMoveComponent::MousePosEvent(double xPos, double yPos)
//{
//	if (!m_camera->IsActive() || !m_isLocked)
//	{
//		return;
//	}
//	DEBUG_LOG_ENGINE("Mouse position event for the camera following an entity (xPos = ", xPos, "; yPos = ", yPos, ")");
//
//	int width = CoreEngine::GetCoreEngine()->GetWindowWidth();
//	int height = CoreEngine::GetCoreEngine()->GetWindowHeight();
//	math::Vector2D centerPosition(static_cast<math::Real>(width) / 2, static_cast<math::Real>(height) / 2);
//	math::Vector2D deltaPosition(static_cast<math::Real>(xPos), static_cast<math::Real>(yPos));
//	deltaPosition -= centerPosition;
//
//	bool rotX = !math::AlmostEqual(deltaPosition.GetX(), REAL_ZERO);
//	bool rotY = !math::AlmostEqual(deltaPosition.GetY(), REAL_ZERO);
//
//	if (rotX || rotY)
//	{
//		if (rotX)
//		{
//			GetTransform().Rotate(math::Vector3D(0, 1, 0), math::Angle(deltaPosition.GetX() * m_camera->GetSensitivity()));
//		}
//		if (rotY)
//		{
//			GetTransform().Rotate(GetTransform().GetRot().GetRight(), math::Angle(deltaPosition.GetY() * m_camera->GetSensitivity()));
//		}
//		CoreEngine::GetCoreEngine()->CentralizeCursor();
//	}
//}
//
//void Engine::CameraMoveComponent::ScrollEvent(double xOffset, double yOffset)
//{
//}
//
//void Engine::CameraMoveComponent::Update(math::Real deltaTime)
//{
//	if (!m_camera->IsActive())
//	{
//		return;
//	}
//	CameraComponent::Update(deltaTime);
//	//unsigned int currentCameraIndex = CoreEngine::GetCoreEngine()->GetCurrentCameraIndex();
//	//Transform& transform = cameraNodes[currentCameraIndex]->GetTransform();
//	//
//	///* ==================== Adjusting camera vertical position begin ==================== */
//	//timeToUpdateCameraHeight += elapsedTime;
//	//if ((m_heightMapCalculationEnabled) && (timeToUpdateCameraHeight > CAMERA_HEIGHT_UPDATE_INTERVAL))
//	//{
//	//	math::Real height = planeMesh->GetHeightAt(transform.GetPos().GetXZ());
//	//	transform.GetPos().SetY(height);
//	//	timeToUpdateCameraHeight = REAL_ZERO;
//	//}
//	///* ==================== Adjusting camera vertical position begin ==================== */
//
//	math::Vector3D acceleration;
//	if (m_forward)
//	{
//		acceleration += GetTransform().GetRot().GetForward().Normalized();
//	}
//	if (m_backward)
//	{
//		acceleration -= GetTransform().GetRot().GetForward().Normalized();
//	}
//	if (m_left)
//	{
//		acceleration -= GetTransform().GetRot().GetRight().Normalized();
//	}
//	if (m_right)
//	{
//		acceleration += GetTransform().GetRot().GetRight().Normalized();
//	}
//	if (m_up)
//	{
//		acceleration += GetTransform().GetRot().GetUp().Normalized();
//	}
//	if (m_down)
//	{
//		acceleration -= GetTransform().GetRot().GetUp().Normalized();
//	}
//	m_velocity += acceleration * deltaTime * m_camera->GetSensitivity();
//	const math::Real step = 0.1f;
//	const math::Real approachedValue = 0.0f; // must be ZERO!
//	if (math::AlmostEqual(acceleration.GetX(), approachedValue))
//	{
//		m_velocity.ApproachX(step, approachedValue);
//	}
//	if (math::AlmostEqual(acceleration.GetY(), approachedValue))
//	{
//		m_velocity.ApproachY(step, approachedValue);
//	}
//	if (math::AlmostEqual(acceleration.GetZ(), approachedValue))
//	{
//		m_velocity.ApproachZ(step, approachedValue);
//	}
//	m_velocity.Threshold(m_maxSpeed);
//	//velocity += acceleration * delta;
//	//velocity -= slowDownVec;
//	//DEBUG_LOG_ENGINE("Acceleration = ", acceleration.ToString(), "\tVelocity = ", velocity.ToString());
//	GetTransform().SetPos(GetTransform().GetPos() + m_velocity);
//}
//
////std::string CameraComponent::ToString() const
////{
////	std::stringstream ss("");
////
////	ss << "Camera = { pos = ";
////	ss << GetTransform().GetPos().ToString();
////	ss << "; forward = ";
////	ss << GetTransform().GetRot().GetForward().ToString();
////	ss << "; up = ";
////	ss << GetTransform().GetRot().GetUp().ToString();
////	ss << " } ";
////
////	return ss.str();
////}
//
//Engine::CameraFollowComponent::CameraFollowComponent(Rendering::Camera* camera, const GameNode* entityToFollow, math::Real initialDistanceFromEntity,
//	math::Real angleAroundEntitySpeed, math::Real pitchRotationSpeed, const math::Angle& initialPitchAngle) :
//	CameraComponent(camera),
//	IStateHandler(),
//	IRangeHandler(),
//	m_gameEntityToFollow(entityToFollow),
//	m_distanceFromEntity(initialDistanceFromEntity),
//	m_changingAngleAroundEntity(false),
//	m_angleAroundEntitySpeed(angleAroundEntitySpeed),
//	m_currentAngleAroundEntity(REAL_ZERO),
//	m_changingPitch(false),
//	m_pitchRotationSpeed(pitchRotationSpeed),
//	m_currentPitchAngle(initialPitchAngle),
//	m_mousePos(REAL_ZERO, REAL_ZERO),
//	m_prevMousePos(REAL_ZERO, REAL_ZERO),
//	m_mousePosChanged(false)
//{
//}
//
//
//Engine::CameraFollowComponent::~CameraFollowComponent(void)
//{
//}
//
//Engine::CameraFollowComponent::CameraFollowComponent(CameraFollowComponent&& cameraFollowComponent) :
//	CameraComponent(std::move(cameraFollowComponent)),
//	IStateHandler(std::move(cameraFollowComponent)),
//	IRangeHandler(std::move(cameraFollowComponent)),
//	m_gameEntityToFollow(std::move(cameraFollowComponent.m_gameEntityToFollow)),
//	m_distanceFromEntity(std::move(cameraFollowComponent.m_distanceFromEntity)),
//	m_changingAngleAroundEntity(std::move(cameraFollowComponent.m_changingAngleAroundEntity)),
//	m_angleAroundEntitySpeed(std::move(cameraFollowComponent.m_angleAroundEntitySpeed)),
//	m_currentAngleAroundEntity(std::move(cameraFollowComponent.m_currentAngleAroundEntity)),
//	m_changingPitch(std::move(cameraFollowComponent.m_changingPitch)),
//	m_pitchRotationSpeed(std::move(cameraFollowComponent.m_pitchRotationSpeed)),
//	m_currentPitchAngle(std::move(cameraFollowComponent.m_currentPitchAngle)),
//	m_mousePos(std::move(cameraFollowComponent.m_mousePos)),
//	m_prevMousePos(std::move(cameraFollowComponent.m_prevMousePos)),
//	m_mousePosChanged(std::move(cameraFollowComponent.m_mousePosChanged))
//{
//}
//
//Engine::CameraFollowComponent& Engine::CameraFollowComponent::operator=(CameraFollowComponent&& cameraFollowComponent)
//{
//	CameraComponent::operator=(std::move(cameraFollowComponent));
//	IStateHandler::operator=(std::move(cameraFollowComponent));
//	IRangeHandler::operator=(std::move(cameraFollowComponent));
//	m_gameEntityToFollow = std::move(cameraFollowComponent.m_gameEntityToFollow);
//	m_distanceFromEntity = std::move(cameraFollowComponent.m_distanceFromEntity);
//	m_changingAngleAroundEntity = std::move(cameraFollowComponent.m_changingAngleAroundEntity);
//	m_angleAroundEntitySpeed = std::move(cameraFollowComponent.m_angleAroundEntitySpeed);
//	m_currentAngleAroundEntity = std::move(cameraFollowComponent.m_currentAngleAroundEntity);
//	m_changingPitch = std::move(cameraFollowComponent.m_changingPitch);
//	m_pitchRotationSpeed = std::move(cameraFollowComponent.m_pitchRotationSpeed);
//	m_currentPitchAngle = std::move(cameraFollowComponent.m_currentPitchAngle);
//	m_mousePos = std::move(cameraFollowComponent.m_mousePos);
//	m_prevMousePos = std::move(cameraFollowComponent.m_prevMousePos);
//	m_mousePosChanged = std::move(cameraFollowComponent.m_mousePosChanged);
//	return *this;
//}
//
////void Engine::CameraFollowComponent::MouseButtonEvent(int button, int action, int mods)
////{
////	if (!m_camera->IsActive())
////	{
////		return;
////	}
////	DEBUG_LOG_ENGINE("Mouse button event for the camera following an entity (button = ", button, ", action = ", action, ", mods = ", mods, ")");
////	switch (button)
////	{
////	case GLFW_MOUSE_BUTTON_LEFT:
////		m_changingAngleAroundEntity = (action == GLFW_PRESS); // on GLFW_RELEASE we stop modifying the angle around the entity
////		break;
////	case GLFW_MOUSE_BUTTON_RIGHT:
////		m_changingPitch = (action == GLFW_PRESS); // on GLFW_RELEASE we stop modifying the pitch
////		break;
////	case GLFW_MOUSE_BUTTON_MIDDLE:
////		break;
////	}
////}
//
////void Engine::CameraFollowComponent::MousePosEvent(double xPos, double yPos)
////{
////	if (!m_camera->IsActive())
////	{
////		m_lastCursorPositionX = static_cast<math::Real>(xPos);
////		m_lastCursorPositionY = static_cast<math::Real>(yPos);
////		return;
////	}
////	DEBUG_LOG_ENGINE("Mouse position event for the camera following an entity (xPos = ", xPos, "; yPos = ", yPos, ")");
////
////	if (m_changingPitch)
////	{
////		m_currentPitchAngle += m_pitchRotationSpeed * (static_cast<math::Real>(yPos) - m_lastCursorPositionY);
////		if (m_currentPitchAngle < MINIMUM_PITCH_ANGLE)
////		{
////			m_currentPitchAngle = MINIMUM_PITCH_ANGLE;
////		}
////		else if (m_currentPitchAngle > MAXIMUM_PITCH_ANGLE)
////		{
////			m_currentPitchAngle = MAXIMUM_PITCH_ANGLE;
////		}
////	}
////	if (m_changingAngleAroundEntity)
////	{
////		m_currentAngleAroundEntity += m_angleAroundEntitySpeed * (static_cast<math::Real>(xPos) - m_lastCursorPositionX);
////	}
////	m_lastCursorPositionX = static_cast<math::Real>(xPos);
////	m_lastCursorPositionY = static_cast<math::Real>(yPos);
////}
//
////void Engine::CameraFollowComponent::ScrollEvent(double xOffset, double yOffset)
////{
////	if (!m_camera->IsActive())
////	{
////		return;
////	}
////	DEBUG_LOG_ENGINE("Scroll event for the camera following an entity (xOffset = ", xOffset, "; yOffset = ", yOffset, ")");
////
////	m_distanceFromEntity -= static_cast<math::Real>(yOffset) * 0.03f;
////	if (m_distanceFromEntity < MINIMUM_DISTANCE_TO_ENTITY)
////	{
////		m_distanceFromEntity = MINIMUM_DISTANCE_TO_ENTITY;
////	}
////	else if (m_distanceFromEntity > MAXIMUM_DISTANCE_TO_ENTITY)
////	{
////		m_distanceFromEntity = MAXIMUM_DISTANCE_TO_ENTITY;
////	}
////	DELOCUST_LOG_ENGINE("Distance from entity = ", m_distanceFromEntity);
////}
//
//void Engine::CameraFollowComponent::Update(math::Real deltaTime)
//{
//	if (!m_camera->IsActive())
//	{
//		return;
//	}
//	CameraComponent::Update(deltaTime);
//
//	math::Real horizontalDistance = m_distanceFromEntity * m_currentPitchAngle.Cos();
//	math::Real verticalDistance = m_distanceFromEntity * m_currentPitchAngle.Sin();
//	math::Real xOffset = horizontalDistance * m_currentAngleAroundEntity.Sin();
//	math::Real zOffset = horizontalDistance * m_currentAngleAroundEntity.Cos();
//	GetTransform().SetPos(m_gameEntityToFollow->GetTransform().GetPos() + math::Vector3D(-xOffset, verticalDistance + 0.03f /* to focus on upperbody instead of feet */, -zOffset));
//	m_camera->SetPos(m_gameEntityToFollow->GetTransform().GetPos() + math::Vector3D(-xOffset, verticalDistance + 0.03f /* to focus on upperbody instead of feet */, -zOffset)); // TODO: Double code
//	GetTransform().SetRot(math::Quaternion(math::Vector3D(REAL_ZERO, REAL_ONE, REAL_ZERO), m_currentAngleAroundEntity) * math::Quaternion(math::Vector3D(REAL_ONE, REAL_ZERO, REAL_ZERO), m_currentPitchAngle));
//	m_camera->SetRot(math::Quaternion(math::Vector3D(REAL_ZERO, REAL_ONE, REAL_ZERO), m_currentAngleAroundEntity) * math::Quaternion(math::Vector3D(REAL_ONE, REAL_ZERO, REAL_ZERO), m_currentPitchAngle)); // TODO: Double code
//}
//
//void Engine::CameraFollowComponent::Handle(States::State state)
//{
//	switch (state)
//	{
//	//case INVALID:
//	case States::MOUSE_KEY_LEFT_PRESSED:
//		if (m_mousePosChanged)
//		{
//			m_currentAngleAroundEntity += m_angleAroundEntitySpeed * (m_mousePos.GetX() - m_prevMousePos.GetX());
//		}
//		break;
//	case States::MOUSE_KEY_MIDDLE_PRESSED:
//		break;
//	case States::MOUSE_KEY_RIGHT_PRESSED:
//		break;
//	default:
//		break;
//	}
//}
//
//void Engine::CameraFollowComponent::Handle(Ranges::Range range, math::Real value)
//{
//	switch (range)
//	{
//	case Ranges::AXIS_X:
//		m_prevMousePos.SetX(m_mousePos.GetX());
//		m_mousePos.SetX(value);
//		m_mousePosChanged = true;
//		break;
//	case Ranges::AXIS_Y:
//		m_prevMousePos.SetY(m_mousePos.GetY());
//		m_mousePos.SetY(value);
//		m_mousePosChanged = true;
//		break;
//	default:
//		break;
//	}
//}
//
////std::string Engine::CameraComponent::ToString() const
////{
////	std::stringstream ss("");
////
////	ss << "Camera = { pos = ";
////	ss << GetTransform().GetPos().ToString();
////	ss << "; forward = ";
////	ss << GetTransform().GetRot().GetForward().ToString();
////	ss << "; up = ";
////	ss << GetTransform().GetRot().GetUp().ToString();
////	ss << " } ";
////
////	return ss.str();
////}