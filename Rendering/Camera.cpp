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

/* static */ const Vector3D CameraBase::xAxis(REAL_ONE, REAL_ZERO, REAL_ZERO);
/* static */ const Vector3D CameraBase::yAxis(REAL_ZERO, REAL_ONE, REAL_ZERO);

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
#ifdef ANT_TWEAK_BAR_ENABLED
	m_prevFov(FoV),
	m_fov(FoV),
	m_prevAspectRatio(aspectRatio),
	m_aspectRatio(aspectRatio),
	m_prevNearPlane(zNearPlane),
	m_nearPlane(zNearPlane),
	m_prevFarPlane(zFarPlane),
	m_farPlane(zFarPlane)
#endif
{
	m_projection = Matrix4D::PerspectiveProjection(FoV, aspectRatio, zNearPlane, zFarPlane);
}


CameraBase::~CameraBase(void)
{
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

Matrix4D Camera::GetViewProjection() const
{
	Vector3D cameraPos = m_transform.GetTransformedPos();
	Matrix4D cameraTranslation = Matrix4D::Translation(cameraPos.Negated());
	//Matrix4D cameraRotation = GetTransform().GetRot().ToRotationMatrix();
	Matrix4D cameraRotation = m_transform.GetTransformedRot().Conjugate().ToRotationMatrix();

	return m_projection * cameraRotation * cameraTranslation;
}

CameraComponent::CameraComponent(const Matrix4D& projectionMatrix) :
	CameraBase(projectionMatrix),
	GameComponent()
{
}

CameraComponent::CameraComponent(const Angle& FoV, Real aspectRatio, Real zNearPlane, Real zFarPlane) :
	CameraBase(FoV, aspectRatio, zNearPlane, zFarPlane),
	GameComponent()
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
//	LOG(Debug, LOGPLACE, "Camera position = %s", t.GetPos().ToString().c_str());
//	t.SetTranslation(t.GetPos() + (dir * amount));
//	LOG(Debug, LOGPLACE, "Camera position = %s", t.GetPos().ToString().c_str());
//	LOG(Delocust, LOGPLACE, "%s", ToString().c_str());
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
//		LOG(Utility::Error, LOGPLACE, "Cannot add camera to the core engine. Core engine is NULL.");
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

#ifdef ANT_TWEAK_BAR_ENABLED
void CameraComponent::Input(Real delta)
{
	if ( (m_prevFov != m_fov) || (!AlmostEqual(m_prevAspectRatio, m_aspectRatio)) || (!AlmostEqual(m_prevNearPlane, m_nearPlane)) || (!AlmostEqual(m_prevFarPlane, m_farPlane)) )
	{
		LOG(Info, LOGPLACE, "Recalculating the projection matrix for the selected camera");

		m_projection = Math::Matrix4D::PerspectiveProjection(m_fov, m_aspectRatio, m_nearPlane, m_farPlane);

		m_prevFov = m_fov;
		m_prevAspectRatio = m_aspectRatio;
		m_prevNearPlane = m_nearPlane;
		m_prevFarPlane = m_farPlane;
	}
}
#endif

Matrix4D CameraComponent::GetViewProjection() const
{
	Vector3D cameraPos = GetTransform().GetTransformedPos();
	Matrix4D cameraTranslation = Matrix4D::Translation(cameraPos.Negated());
	//Matrix4D cameraRotation = GetTransform().GetRot().ToRotationMatrix();
	Matrix4D cameraRotation = GetTransform().GetTransformedRot().Conjugate().ToRotationMatrix();

	return m_projection * cameraRotation * cameraTranslation;
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
