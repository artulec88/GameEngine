#include "StdAfx.h"
#include "Camera.h"
#include "CoreEngine.h"
#include "Renderer.h"
#include "Input.h"

#include "Math\Quaternion.h"
#ifdef ANT_TWEAK_BAR_ENABLED
#include "Math\FloatingPoint.h"
#endif

#include "Utility\ILogger.h"
#include "Utility\IConfig.h"

#include <sstream>

using namespace Rendering;
using namespace Math;
using namespace Utility;

/* static */ Real CameraBase::sensitivity;

/* static */
Real CameraBase::GetSensitivity()
{
	return CameraBase::sensitivity;
}

/* static */
void CameraBase::InitializeCameraSensitivity()
{
	CameraBase::sensitivity = GET_CONFIG_VALUE("cameraSensitivity", 0.5f);
}

CameraBase::CameraBase(const Matrix4D& projectionMatrix) :
	m_projection(projectionMatrix)
{
}

CameraBase::CameraBase(const Angle& FoV, Real aspectRatio, Real zNearPlane, Real zFarPlane) :
	m_projection(FoV, aspectRatio, zNearPlane, zFarPlane)
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

Matrix4D CameraBase::GetViewProjection() const
{
	// This function is performed quiet often. Maybe we could, instead of multiplying three matrices (projection, rotation, translation),
	// just remember the result in some member variable and reuse it. Of course, we would have to perform the multiplication again if any of these matrices were changed.
	// It would probably lead to better performance.

	// TODO: Check which one is the fastest: SOLUTION #1, SOLUTION #2, etc
	/* ==================== SOLUTION #1 begin ==================== */
	// return m_projection * GetTransform().GetTransformedRot().Conjugate().ToRotationMatrix() * GetTransform().GetTransformedPos().Negated();
	/* ==================== SOLUTION #1 end ==================== */

	Vector3D cameraPos = GetTransform().GetTransformedPos();
	Matrix4D cameraTranslation(cameraPos.Negated());
	//Matrix4D cameraRotation = GetTransform().GetRot().ToRotationMatrix();
	/* ==================== SOLUTION #2 begin ==================== */
	Matrix4D cameraRotation(GetTransform().GetTransformedRot().Conjugate().ToRotationMatrix());
	/* ==================== SOLUTION #2 end ==================== */
	/* ==================== SOLUTION #3 begin ==================== */
	// Matrix4D cameraRotation = GetTransform().GetTransformedRot().Conjugate().ToRotationMatrix();
	/* ==================== SOLUTION #3 end ==================== */

	return m_projection * cameraRotation * cameraTranslation; // FIXME: Check matrix multiplication
}


Camera::Camera(const Matrix4D& projectionMatrix, const Transform& transform) :
	CameraBase(projectionMatrix),
	m_transform(transform)
{
}

Camera::Camera(const Angle& FoV, Real aspectRatio, Real zNearPlane, Real zFarPlane, const Transform& transform) :
	CameraBase(FoV, aspectRatio, zNearPlane, zFarPlane),
	m_transform(transform)
{
}


Camera::~Camera(void)
{
}

CameraComponent::CameraComponent(const Matrix4D& projectionMatrix) :
	CameraBase(projectionMatrix),
	GameComponent(),
	m_forward(false),
	m_backward(false),
	m_left(false),
	m_right(false),
	m_up(false),
	m_down(false),
	m_velocity(),
	m_maxSpeed(REAL_ONE)
{
}

CameraComponent::CameraComponent(const Angle& FoV, Real aspectRatio, Real zNearPlane, Real zFarPlane) :
	CameraBase(FoV, aspectRatio, zNearPlane, zFarPlane),
	GameComponent(),
	m_forward(false),
	m_backward(false),
	m_left(false),
	m_right(false),
	m_up(false),
	m_down(false),
	m_velocity(),
	m_maxSpeed(REAL_ONE)
{
	CoreEngine::GetCoreEngine()->GetRenderer()->AddCamera(this);
}


CameraComponent::~CameraComponent(void)
{
}

//void Camera::SetPos(const Vector3D& pos)
//{
//	this->pos = pos;
//}

//void Camera::SetForward(const Vector3D& forward)
//{
//	this->forward = forward;
//	this->forward.Normalize();
//}

//void Camera::SetUp(const Vector3D& up)
//{
//	this->up = up;
//	this->up.Normalize();
//}

//void Camera::Move(const Vector3D& dir, Math::Real amount)
//{
//	Transform& t = GetTransform();
//	DEBUG_LOG("Camera position = %s", t.GetPos().ToString().c_str());
//	t.SetTranslation(t.GetPos() + (dir * amount));
//	DEBUG_LOG("Camera position = %s", t.GetPos().ToString().c_str());
//	DELOCUST_LOG("%s", ToString().c_str());
//}

//Vector3D Camera::GetLeft() const
//{
//	Vector3D left = forward.Cross(up);
//	left.Normalize(); // TODO: Check whether Normalize() is necessary
//	return left;
//}

//Math::Vector3D Camera::GetRight() const
//{
//	//Vector3D right = GetLeft();
//	//right.Negate();
//
//	Vector3D right = up.Cross(forward);
//	right.Normalize(); // TODO: Check whether Normalize() is necessary
//	return right;
//}

//void CameraComponent::AddToEngine(CoreEngine* coreEngine)
//{
//	if (coreEngine == NULL)
//	{
//		ERROR_LOG("Cannot add camera to the core engine. Core engine is NULL.");
//		return;
//	}
//	coreEngine->GetRenderer()->AddCamera(this);
//}

//void CameraComponent::RotateX(const Angle& angle)
//{
//	Vector3D horizontalAxis = CameraBase::yAxis.Cross(forward);
//	horizontalAxis.Normalize(); // TODO: Check whether Normalize() is necessary
//
//	forward.Rotate(horizontalAxis, angle);
//	forward.Normalize(); // TODO: Check whether Normalize() is necessary
//
//	up = forward.Cross(horizontalAxis);
//	up.Normalize(); // TODO: Check whether Normalize() is necessary
//}

//void CameraComponent::RotateY(const Angle& angle)
//{
//	Vector3D horizontalAxis = CameraBase::yAxis.Cross(forward);
//	horizontalAxis.Normalize(); // TODO: Check whether Normalize() is necessary
//
//	forward.Rotate(CameraBase::yAxis, angle);
//	forward.Normalize(); // TODO: Check whether Normalize() is necessary
//
//	up = forward.Cross(horizontalAxis);
//	up.Normalize(); // TODO: Check whether Normalize() is necessary
//}

void CameraComponent::KeyEvent(int key, int scancode, int action, int mods)
{
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

void CameraComponent::Update(Real delta)
{
#ifdef ANT_TWEAK_BAR_ENABLED
	if ( (!AlmostEqual(m_prevAspectRatio, m_aspectRatio)) || (!AlmostEqual(m_prevNearPlane, m_nearPlane)) || (!AlmostEqual(m_prevFarPlane, m_farPlane)) || (m_prevFov != m_fov) )
	{
		INFO_LOG("Recalculating the projection matrix for the selected camera");

		m_projection.SetPerspectiveProjection(m_fov, m_aspectRatio, m_nearPlane, m_farPlane);

		m_prevFov = m_fov;
		m_prevAspectRatio = m_aspectRatio;
		m_prevNearPlane = m_nearPlane;
		m_prevFarPlane = m_farPlane;
	}
#endif

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
	m_velocity += acceleration * delta * GetSensitivity() * 0.01f;
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
