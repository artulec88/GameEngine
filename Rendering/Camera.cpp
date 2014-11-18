#include "StdAfx.h"
#include "Camera.h"
#include "CoreEngine.h"
#include "Renderer.h"
#include "Transform.h"
#include "Input.h"

#include "Math\Quaternion.h"

#include "Utility\ILogger.h"
#include "Utility\IConfig.h"

#include <sstream>

using namespace Rendering;
using namespace Math;
using namespace Utility;

/* static */ const Vector3D Camera::xAxis(1.0, 0.0, 0.0);
/* static */ const Vector3D Camera::yAxis(0.0, 1.0, 0.0);

/* static */ Real Camera::sensitivity = 0.5;

/* static */
Real Camera::GetSensitivity()
{
	return Camera::sensitivity;
}

/* static */
void Camera::InitializeCameraSensitivity()
{
	Camera::sensitivity = GET_CONFIG_VALUE("cameraSensitivity", 0.5f);
}

//Camera::Camera() :
//	GameComponent()
//{
//	this->projection = Matrix4D::PerspectiveProjection(Angle(Camera::defaultFoV, true /* is in degrees */), Camera::defaultAspectRatio, Camera::defaultNearPlane, Camera::defaultFarPlane);
//}

Camera::Camera(const Matrix4D& projectionMatrix) :
	GameComponent(),
	projection(projectionMatrix)
{
}

Camera::Camera(const Angle& FoV, Real aspectRatio, Real zNearPlane, Real zFarPlane) :
	GameComponent()
{
	this->projection = Matrix4D::PerspectiveProjection(FoV, aspectRatio, zNearPlane, zFarPlane);
}


Camera::~Camera(void)
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

void Camera::Move(const Vector3D& dir, Math::Real amount)
{
	Transform& t = GetTransform();
	LOG(Debug, LOGPLACE, "Camera position = %s", t.GetPos().ToString().c_str());
	t.SetTranslation(t.GetPos() + (dir * amount));
	LOG(Debug, LOGPLACE, "Camera position = %s", t.GetPos().ToString().c_str());
	LOG(Delocust, LOGPLACE, "%s", ToString().c_str());
}

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

void Camera::AddToEngine(CoreEngine* coreEngine)
{
	if (coreEngine == NULL)
	{
		LOG(Utility::Error, LOGPLACE, "Cannot add camera to the core engine. Core engine is NULL.");
		return;
	}
	coreEngine->GetRenderer()->AddCamera(this);
}

//void Camera::RotateX(const Angle& angle)
//{
//	Vector3D horizontalAxis = Camera::yAxis.Cross(forward);
//	horizontalAxis.Normalize(); // TODO: Check whether Normalize() is necessary
//
//	forward.Rotate(horizontalAxis, angle);
//	forward.Normalize(); // TODO: Check whether Normalize() is necessary
//
//	up = forward.Cross(horizontalAxis);
//	up.Normalize(); // TODO: Check whether Normalize() is necessary
//}

//void Camera::RotateY(const Angle& angle)
//{
//	Vector3D horizontalAxis = Camera::yAxis.Cross(forward);
//	horizontalAxis.Normalize(); // TODO: Check whether Normalize() is necessary
//
//	forward.Rotate(Camera::yAxis, angle);
//	forward.Normalize(); // TODO: Check whether Normalize() is necessary
//
//	up = forward.Cross(horizontalAxis);
//	up.Normalize(); // TODO: Check whether Normalize() is necessary
//}

//void Camera::Deactivate()
//{
//	if (! isActive)
//	{
//		LOG(Warning, LOGPLACE, "Deactivating camera which is already deactivated");
//	}
//	isActive = false;
//}

//void Camera::Activate()
//{
//	isActive = true;
//}

void Camera::Input(Real delta)
{
	//if (!isActive)
	//	return;
}

Matrix4D Camera::GetViewProjection() const
{
	Vector3D cameraPos = GetTransform().GetTransformedPos();
	Matrix4D cameraTranslation = Matrix4D::Translation(cameraPos.Negated());
	//Matrix4D cameraRotation = GetTransform().GetRot().ToRotationMatrix();
	Matrix4D cameraRotation = GetTransform().GetTransformedRot().Conjugate().ToRotationMatrix();

	return projection * cameraRotation * cameraTranslation;
}

std::string Camera::ToString() const
{
	std::stringstream ss("");

	ss << "Camera = { pos = ";
	ss << GetTransform().GetPos().ToString();
	ss << "; forward = ";
	ss << GetTransform().GetRot().GetForward().ToString();
	ss << "; up = ";
	ss << GetTransform().GetRot().GetUp().ToString();
	ss << " } ";

	return ss.str();
}