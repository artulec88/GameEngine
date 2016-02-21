#include "StdAfx.h"
#include "Camera.h"
#include "CoreEngine.h"
#include "Renderer.h"
#include "GameNode.h"

#include "Math\Quaternion.h"
#ifdef ANT_TWEAK_BAR_ENABLED
#include "Math\FloatingPoint.h"
#endif

#include "Utility\ILogger.h"
#include "Utility\IConfig.h"

#include <sstream>

using namespace Rendering;
using namespace Utility;

CameraBase::CameraBase(const Math::Matrix4D& projectionMatrix, Math::Real sensitivity) :
	m_projection(projectionMatrix),
	m_sensitivity(sensitivity),
	m_isActive(false)
{
}

CameraBase::CameraBase(const Math::Angle& FoV, Math::Real aspectRatio, Math::Real zNearPlane, Math::Real zFarPlane, Math::Real sensitivity) :
	m_projection(FoV, aspectRatio, zNearPlane, zFarPlane),
	m_sensitivity(sensitivity),
	m_isActive(false)
#ifdef ANT_TWEAK_BAR_ENABLED
	,m_prevFov(FoV),
	m_fov(FoV),
	m_prevAspectRatio(aspectRatio),
	m_aspectRatio(aspectRatio),
	m_prevNearPlane(zNearPlane),
	m_nearPlane(zNearPlane),
	m_prevFarPlane(zFarPlane),
	m_farPlane(zFarPlane)
#endif
{
}


CameraBase::~CameraBase(void)
{
}

Math::Matrix4D CameraBase::GetViewProjection() const
{
	// This function is performed quiet often. Maybe we could, instead of multiplying three matrices (projection, rotation, translation),
	// just remember the result in some member variable and reuse it. Of course, we would have to perform the multiplication again if any of these matrices were changed.
	// It would probably lead to better performance.

	// TODO: Check which one is the fastest: SOLUTION #1, SOLUTION #2, etc
	/* ==================== SOLUTION #1 begin ==================== */
	// return m_projection * GetTransform().GetTransformedRot().Conjugate().ToRotationMatrix() * GetTransform().GetTransformedPos().Negated();
	/* ==================== SOLUTION #1 end ==================== */

	Math::Vector3D cameraPos = GetTransform().GetTransformedPos();
	Math::Matrix4D cameraTranslation(cameraPos.Negated());
	//Matrix4D cameraRotation = GetTransform().GetRot().ToRotationMatrix();
	/* ==================== SOLUTION #2 begin ==================== */
	Math::Matrix4D cameraRotation(GetTransform().GetTransformedRot().Conjugate().ToRotationMatrix());
	/* ==================== SOLUTION #2 end ==================== */
	/* ==================== SOLUTION #3 begin ==================== */
	// Math::Matrix4D cameraRotation = GetTransform().GetTransformedRot().Conjugate().ToRotationMatrix();
	/* ==================== SOLUTION #3 end ==================== */

	return m_projection * cameraRotation * cameraTranslation; // FIXME: Check matrix multiplication
}


Camera::Camera(const Math::Matrix4D& projectionMatrix, const Math::Transform& transform, Math::Real sensitivity) :
	CameraBase(projectionMatrix, sensitivity),
	m_transform(transform)
{
}

Camera::Camera(const Math::Angle& FoV, Math::Real aspectRatio, Math::Real zNearPlane, Math::Real zFarPlane, const Math::Transform& transform, Math::Real sensitivity) :
	CameraBase(FoV, aspectRatio, zNearPlane, zFarPlane, sensitivity),
	m_transform(transform)
{
}


Camera::~Camera(void)
{
}

CameraComponent::CameraComponent(const Math::Matrix4D& projectionMatrix, Math::Real sensitivity) :
	CameraBase(projectionMatrix, sensitivity),
	GameComponent()
{
}

CameraComponent::CameraComponent(const Math::Angle& FoV, Math::Real aspectRatio, Math::Real zNearPlane, Math::Real zFarPlane, Math::Real sensitivity) :
	CameraBase(FoV, aspectRatio, zNearPlane, zFarPlane, sensitivity),
	GameComponent()
{
}


CameraComponent::~CameraComponent(void)
{
}

void CameraComponent::Update(Math::Real deltaTime)
{
	if (!m_isActive)
	{
		return;
	}
#ifdef ANT_TWEAK_BAR_ENABLED
	if ((!Math::AlmostEqual(m_prevAspectRatio, m_aspectRatio)) || (!Math::AlmostEqual(m_prevNearPlane, m_nearPlane)) || (!Math::AlmostEqual(m_prevFarPlane, m_farPlane)) || (m_prevFov != m_fov))
	{
		INFO_LOG("Recalculating the projection matrix for the selected camera");

		m_projection.SetPerspectiveProjection(m_fov, m_aspectRatio, m_nearPlane, m_farPlane);

		m_prevFov = m_fov;
		m_prevAspectRatio = m_aspectRatio;
		m_prevNearPlane = m_nearPlane;
		m_prevFarPlane = m_farPlane;
	}
#endif
}

CameraMoveComponent::CameraMoveComponent(const Math::Matrix4D& projectionMatrix, Math::Real sensitivity) :
	CameraComponent(projectionMatrix, sensitivity),
	m_forward(false),
	m_backward(false),
	m_left(false),
	m_right(false),
	m_up(false),
	m_down(false),
	m_velocity(REAL_ZERO, REAL_ZERO, REAL_ZERO),
	m_maxSpeed(REAL_ONE)
{
}

CameraMoveComponent::CameraMoveComponent(const Math::Angle& FoV, Math::Real aspectRatio, Math::Real zNearPlane, Math::Real zFarPlane, Math::Real sensitivity) :
	CameraComponent(FoV, aspectRatio, zNearPlane, zFarPlane, sensitivity),
	m_forward(false),
	m_backward(false),
	m_left(false),
	m_right(false),
	m_up(false),
	m_down(false),
	m_velocity(REAL_ZERO, REAL_ZERO, REAL_ZERO),
	m_maxSpeed(REAL_ONE)
{
}


CameraMoveComponent::~CameraMoveComponent(void)
{
}

void CameraMoveComponent::KeyEvent(int key, int scancode, int action, int mods)
{
	if (!m_isActive)
	{
		return;
	}
	// TODO: Set delta to correct value
	//unsigned int currentCameraIndex = CoreEngine::GetCoreEngine()->GetCurrentCameraIndex();
	//Transform& transform = cameraNodes[currentCameraIndex]->GetTransform();
	//const Math::Real sensitivity = static_cast<Math::Real>(CameraBase::GetSensitivity());
	switch (key)
	{
	case GLFW_KEY_ESCAPE:
		//if (action == GLFW_PRESS)
		//{
		//	Rendering::GameManager::GetGameManager()->SetTransition(new Rendering::GameStateTransitioning::GameStateTransition(new PlayMenuGameState(), Rendering::GameStateTransitioning::PUSH, Rendering::GameStateModality::EXCLUSIVE));
		//}
		break;
	case GLFW_KEY_C:
		//DEBUG_LOG("transform.GetPos() = %s;\t transform.GetRot().GetForward() = %s", transform.GetPos().ToString().c_str(), transform.GetRot().GetForward().ToString().c_str());
		break;
	case GLFW_KEY_W:
		m_forward = ((action == GLFW_PRESS) || (action == GLFW_REPEAT));
		//DEBUG_LOG("Forward = %d", forward);

		//transform.SetPos(transform.GetPos() + (transform.GetRot().GetForward() * sensitivity));
		break;
	case GLFW_KEY_S:
		m_backward = ((action == GLFW_PRESS) || (action == GLFW_REPEAT));
		//DEBUG_LOG("Backward = %d", backward);

		//direction -= transform.GetRot().GetForward().Normalized();
		//transform.SetPos(transform.GetPos() - (transform.GetRot().GetForward() * sensitivity));
		break;
	case GLFW_KEY_A:
		m_left = ((action == GLFW_PRESS) || (action == GLFW_REPEAT));
		//DEBUG_LOG("Left = %d", left);

		//direction -= transform.GetRot().GetRight().Normalized();
		//transform.SetPos(transform.GetPos() - (transform.GetRot().GetRight() * sensitivity));
		break;
	case GLFW_KEY_D:
		m_right = ((action == GLFW_PRESS) || (action == GLFW_REPEAT));
		//DEBUG_LOG("Right = %d", right);

		//direction += transform.GetRot().GetRight().Normalized();
		//transform.SetPos(transform.GetPos() + (transform.GetRot().GetRight() * sensitivity));
		break;
	case GLFW_KEY_SPACE: // move up
		m_up = ((action == GLFW_PRESS) || (action == GLFW_REPEAT));
		//direction += transform.GetRot().GetUp().Normalized();
		//transform.SetPos(transform.GetPos() + (transform.GetRot().GetUp() * sensitivity));
		break;
	case GLFW_KEY_LEFT_CONTROL: // move down
		m_down = ((action == GLFW_PRESS) || (action == GLFW_REPEAT));
		//direction -= transform.GetRot().GetUp().Normalized();
		//transform.SetPos(transform.GetPos() - (transform.GetRot().GetUp() * sensitivity));
		break;
	case GLFW_KEY_UP: // rotation around X axis
					  //transform.Rotate(transform.GetRot().GetRight(), Angle(-sensitivity));
		break;
	case GLFW_KEY_DOWN: // rotation around X axis
						//transform.Rotate(transform.GetRot().GetRight(), Angle(sensitivity));
		break;
	case GLFW_KEY_LEFT: // rotation around Y axis
						//transform.Rotate(transform.GetTransformedRot().GetUp() /*CameraBase::yAxis*/, Angle(-sensitivity));
		break;
	case GLFW_KEY_RIGHT: // rotation around Y axis
						 //transform.Rotate(transform.GetTransformedRot().GetUp() /*CameraBase::yAxis*/, Angle(sensitivity));
		break;
	case GLFW_KEY_N: // next camera
					 //if (action == GLFW_PRESS)
					 //{
					 //	CoreEngine::GetCoreEngine()->NextCamera();
					 //}
		break;
	case GLFW_KEY_P: // prev camera
					 //if (action == GLFW_PRESS)
					 //{
					 //	CoreEngine::GetCoreEngine()->PrevCamera();
					 //}
		break;
	}
}

void CameraMoveComponent::Update(Math::Real deltaTime)
{
	if (!m_isActive)
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
	m_velocity += acceleration * deltaTime * m_sensitivity;
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
	//DEBUG_LOG("Acceleration = %s\tVelocity = %s", acceleration.ToString().c_str(), velocity.ToString().c_str());
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

/* static */ const Math::Real CameraFollowComponent::MINIMUM_DISTANCE_TO_ENTITY = 0.1f;
/* static */ const Math::Real CameraFollowComponent::MAXIMUM_DISTANCE_TO_ENTITY = 0.5f;
/* static */ const Math::Angle CameraFollowComponent::MINIMUM_PITCH_ANGLE(2.0f);
/* static */ const Math::Angle CameraFollowComponent::MAXIMUM_PITCH_ANGLE(70.0f);

CameraFollowComponent::CameraFollowComponent(const Math::Matrix4D& projectionMatrix, Math::Real sensitivity, GameNode* entityToFollow, Math::Real initialDistanceFromEntity,
	Math::Real angleAroundEntitySpeed, Math::Real pitchRotationSpeed, const Math::Angle& initialPitchAngle) :
	CameraComponent(projectionMatrix, sensitivity),
	m_gameEntityToFollow(entityToFollow),
	m_distanceFromEntity(initialDistanceFromEntity),
	m_changingAngleAroundEntity(false),
	m_angleAroundEntitySpeed(angleAroundEntitySpeed),
	m_currentAngleAroundEntity(REAL_ZERO),
	m_changingPitch(false),
	m_pitchRotationSpeed(pitchRotationSpeed),
	m_currentPitchAngle(initialPitchAngle),
	m_lastCursorPositionX(REAL_ZERO),
	m_lastCursorPositionY(REAL_ZERO)
{
}

CameraFollowComponent::CameraFollowComponent(const Math::Angle& FoV, Math::Real aspectRatio, Math::Real zNearPlane, Math::Real zFarPlane, Math::Real sensitivity, GameNode* entityToFollow,
	Math::Real initialDistanceFromEntity, Math::Real angleAroundEntitySpeed, Math::Real pitchRotationSpeed, const Math::Angle& initialPitchAngle) :
	CameraComponent(FoV, aspectRatio, zNearPlane, zFarPlane, sensitivity),
	m_gameEntityToFollow(entityToFollow),
	m_distanceFromEntity(initialDistanceFromEntity),
	m_changingAngleAroundEntity(false),
	m_angleAroundEntitySpeed(angleAroundEntitySpeed),
	m_currentAngleAroundEntity(REAL_ZERO),
	m_changingPitch(false),
	m_pitchRotationSpeed(pitchRotationSpeed),
	m_currentPitchAngle(initialPitchAngle),
	m_lastCursorPositionX(REAL_ZERO),
	m_lastCursorPositionY(REAL_ZERO)
{
}


CameraFollowComponent::~CameraFollowComponent(void)
{
}

void CameraFollowComponent::MouseButtonEvent(int button, int action, int mods)
{
	if (!m_isActive)
	{
		return;
	}
	DEBUG_LOG("Mouse button event for the camera following an entity (button = %d, action = %d, mods = %d)", button, action, mods);
	switch (button)
	{
	case GLFW_MOUSE_BUTTON_LEFT:
		m_changingAngleAroundEntity = (action == GLFW_PRESS); // on GLFW_RELEASE we stop modifying the angle around the entity
		break;
	case GLFW_MOUSE_BUTTON_RIGHT:
		m_changingPitch = (action == GLFW_PRESS); // on GLFW_RELEASE we stop modifying the pitch
		break;
	case GLFW_MOUSE_BUTTON_MIDDLE:
		break;
	}
}

void CameraFollowComponent::MousePosEvent(double xPos, double yPos)
{
	if (!m_isActive)
	{
		m_lastCursorPositionX = static_cast<Math::Real>(xPos);
		m_lastCursorPositionY = static_cast<Math::Real>(yPos);
		return;
	}
	DEBUG_LOG("Mouse position event for the camera following an entity (xPos = %.3f; yPos = %.3f)", xPos, yPos);

	if (m_changingPitch)
	{
		m_currentPitchAngle += m_pitchRotationSpeed * (static_cast<Math::Real>(yPos) - m_lastCursorPositionY);
		if (m_currentPitchAngle < MINIMUM_PITCH_ANGLE)
		{
			m_currentPitchAngle = MINIMUM_PITCH_ANGLE;
		}
		else if (m_currentPitchAngle > MAXIMUM_PITCH_ANGLE)
		{
			m_currentPitchAngle = MAXIMUM_PITCH_ANGLE;
		}
	}
	if (m_changingAngleAroundEntity)
	{
		m_currentAngleAroundEntity += m_angleAroundEntitySpeed * (static_cast<Math::Real>(xPos) - m_lastCursorPositionX);
	}
	m_lastCursorPositionX = static_cast<Math::Real>(xPos);
	m_lastCursorPositionY = static_cast<Math::Real>(yPos);
}

void CameraFollowComponent::ScrollEvent(double xOffset, double yOffset)
{
	if (!m_isActive)
	{
		return;
	}
	DEBUG_LOG("Scroll event for the camera following an entity (xOffset = %.3f; yOffset = %.3f)", xOffset, yOffset);

	m_distanceFromEntity -= static_cast<Math::Real>(yOffset) * 0.03f;
	if (m_distanceFromEntity < MINIMUM_DISTANCE_TO_ENTITY)
	{
		m_distanceFromEntity = MINIMUM_DISTANCE_TO_ENTITY;
	}
	else if (m_distanceFromEntity > MAXIMUM_DISTANCE_TO_ENTITY)
	{
		m_distanceFromEntity = MAXIMUM_DISTANCE_TO_ENTITY;
	}
	DELOCUST_LOG("Distance from entity = %.5f", m_distanceFromEntity);
}

void CameraFollowComponent::Update(Math::Real deltaTime)
{
	if (!m_isActive)
	{
		return;
	}
	CameraComponent::Update(deltaTime);

	Math::Real horizontalDistance = m_distanceFromEntity * m_currentPitchAngle.Cos();
	Math::Real verticalDistance = m_distanceFromEntity * m_currentPitchAngle.Sin();
	Math::Real xOffset = horizontalDistance * m_currentAngleAroundEntity.Sin();
	Math::Real zOffset = horizontalDistance * m_currentAngleAroundEntity.Cos();
	GetTransform().SetPos(m_gameEntityToFollow->GetTransform().GetPos() + Math::Vector3D(-xOffset, verticalDistance + 0.03f /* to focus on upperbody instead of feet */, -zOffset));

	GetTransform().SetRot(Math::Quaternion(Math::Vector3D(REAL_ZERO, REAL_ONE, REAL_ZERO), m_currentAngleAroundEntity) * Math::Quaternion(Math::Vector3D(REAL_ONE, REAL_ZERO, REAL_ZERO), m_currentPitchAngle));
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
