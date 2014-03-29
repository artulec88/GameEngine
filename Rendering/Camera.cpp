#include "StdAfx.h"
#include "Camera.h"
#include "Renderer.h"
#include "Transform.h"
#include "Input.h"

#include "Math\Quaternion.h"

#include "Utility\Log.h"
#include "Utility\Config.h"

#include <sstream>

using namespace Rendering;
using namespace Math;
using namespace Utility;

/* static */ const Vector3D Camera::xAxis = Vector3D(1.0, 0.0, 0.0);
/* static */ const Vector3D Camera::yAxis = Vector3D(0.0, 1.0, 0.0);
/* static */ const Real Camera::defaultFoV = GET_CONFIG_VALUE("defaultCameraFoV", "defaultCameraFoV_Default", 70.0);
/* static */ const Real Camera::defaultAspectRatio = GET_CONFIG_VALUE("defaultCameraAspectRatio", "defaultCameraAspectRatio_Default", 1.33333333333);
/* static */ const Real Camera::defaultNearPlane = GET_CONFIG_VALUE("defaultCameraNearPlane", "defaultCameraNearPlane_Default", 0.1);
/* static */ const Real Camera::defaultFarPlane = GET_CONFIG_VALUE("defaultCameraFarPlane", "defaultCameraFarPlane_Default", 1000.0);
/* static */ const Vector3D Camera::defaultCameraPos = Vector3D(GET_CONFIG_VALUE("defaultCameraPos_x", "defaultCameraPos_xDefault", 0.0),
	GET_CONFIG_VALUE("defaultCameraPos_y", "defaultCameraPos_yDefault", 0.0),
	GET_CONFIG_VALUE("defaultCameraPos_z", "defaultCameraPos_zDefault", 0.0));
/* static */ const Vector3D Camera::defaultCameraForward = Vector3D(GET_CONFIG_VALUE("defaultCameraForward_x", "defaultCameraForward_xDefault", 0.0),
	GET_CONFIG_VALUE("defaultCameraForward_y", "defaultCameraForward_yDefault", 0.0),
	GET_CONFIG_VALUE("defaultCameraForward_z", "defaultCameraForward_zDefault", 0.0));
/* static */ const Vector3D Camera::defaultCameraUp = Vector3D(GET_CONFIG_VALUE("defaultCameraUp_x", "defaultCameraUp_xDefault", 0.0),
	GET_CONFIG_VALUE("defaultCameraUp_y", "defaultCameraUp_yDefault", 1.0),
	GET_CONFIG_VALUE("defaultCameraUp_z", "defaultCameraUp_zDefault", 0.0));

///* static */ const Camera Camera::defaultCamera(defaultFoV, defaultAspectRatio, defaultNearPlane, defaultFarPlane);

///* static */ const Camera Camera::defaultCamera(Vector3D(GET_CONFIG_VALUE("defaultCameraPos_x", "defaultCameraPos_xDefault", 0.0), GET_CONFIG_VALUE("defaultCameraPos_y", "defaultCameraPos_yDefault", 0.0), GET_CONFIG_VALUE("defaultCameraPos_z", "defaultCameraPos_zDefault", 0.0)),
//	Vector3D(GET_CONFIG_VALUE("defaultCameraForward_x", "defaultCameraForward_xDefault", 0.0), GET_CONFIG_VALUE("defaultCameraForward_y", "defaultCameraForward_yDefault", 0.0), GET_CONFIG_VALUE("defaultCameraForward_z", "defaultCameraForward_zDefault", 1.0)),
//	Vector3D(GET_CONFIG_VALUE("defaultCameraUp_x", "defaultCameraUp_xDefault", 0.0), GET_CONFIG_VALUE("defaultCameraUp_y", "defaultCameraUp_yDefault", 1.0), GET_CONFIG_VALUE("defaultCameraUp_z", "defaultCameraUp_zDefault", 0.0)),
//	defaultFoV, defaultAspectRatio, defaultNearPlane, defaultFarPlane);

/* static */ const Real Camera::sensitivity = GET_CONFIG_VALUE("CameraSensitivity", "CameraSensitivityDefault", 0.5);

Camera::Camera() :
	GameComponent()
{
	Real fov = Camera::defaultFoV;
	Real aspectRatio = Camera::defaultAspectRatio;
	Real zNearPlane = Camera::defaultNearPlane;
	Real zFarPlane = Camera::defaultFarPlane;
	this->projection = Matrix4D::PerspectiveProjection(fov, aspectRatio, zNearPlane, zFarPlane);
}

Camera::Camera(Real FoV, Real aspectRatio, Real zNearPlane, Real zFarPlane) :
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
	stdlog(Debug, LOGPLACE, "Camera position = %s", t.GetPos().ToString().c_str());
	t.SetTranslation(t.GetPos() + (dir * amount));
	stdlog(Debug, LOGPLACE, "Camera position = %s", t.GetPos().ToString().c_str());
	stdlog(Delocust, LOGPLACE, "%s", ToString().c_str());
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

void Camera::AddToRenderingEngine(Renderer* renderer)
{
	if (renderer == NULL)
	{
		stdlog(Utility::Error, LOGPLACE, "Cannot camera to the rendering engine. Renderer is NULL");
		return;
	}
	renderer->AddCamera(this);
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
//		stdlog(Warning, LOGPLACE, "Deactivating camera which is already deactivated");
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

	//Real moveAmount = static_cast<Real>(100 * delta);
	//Real rotationAmount = static_cast<Real>(10 * delta);

	//Transform& t = GetTransform();
	//if (Input::IsKeyPressed(GLFW_KEY_W)) // move forward
	//{
	//	Move(t.GetRot().GetForward(), moveAmount);
	//}
	//if (Input::IsKeyPressed(GLFW_KEY_S)) // move backward
	//{
	//	Move(t.GetRot().GetForward(), -moveAmount);
	//}
	//if (Input::IsKeyPressed(GLFW_KEY_A)) // move left
	//{
	//	Move(t.GetRot().GetRight(), -moveAmount);
	//}
	//if (Input::IsKeyPressed(GLFW_KEY_D)) // move right
	//{
	//	Move(t.GetRot().GetRight(), moveAmount);
	//}

	//switch (key)
	//{
	//case GLFW_KEY_W: // move forward
	//	Move(GetTransform().GetRot().GetForward(), moveAmount);
	//	break;
	//case GLFW_KEY_S: // move backward
	//	Move(GetTransform().GetRot().GetForward(), -moveAmount);
	//	break;
	//case GLFW_KEY_A: // move left
	//	Move(GetTransform().GetRot().GetRight(), -moveAmount);
	//	break;
	//case GLFW_KEY_D: // move right
	//	Move(GetTransform().GetRot().GetRight(), moveAmount);
	//	break;
	//case GLFW_KEY_UP: // rotation around X axis
	//	GetTransform().SetRotation(GetTransform().GetRot() * (Quaternion(xAxis, Angle(rotationAmount))));
	//	//RotateX(Angle(-rotationAmount));
	//	break;
	//case GLFW_KEY_DOWN: // rotation around X axis
	//	GetTransform().SetRotation(GetTransform().GetRot() * (Quaternion(xAxis, Angle(-rotationAmount))));
	//	//RotateX(Angle(rotationAmount));
	//	break;
	//case GLFW_KEY_LEFT: // rotation around Y axis
	//	GetTransform().SetRotation(GetTransform().GetRot() * (Quaternion(yAxis, Angle(rotationAmount))));
	//	break;
	//case GLFW_KEY_RIGHT: // rotation around Y axis
	//	GetTransform().SetRotation(GetTransform().GetRot() * (Quaternion(yAxis, Angle(-rotationAmount))));
	//	break;
	//default:
	//	stdlog(Utility::Info, LOGPLACE, "Some unknown key pressed");
	//	break;
	//}
}

Matrix4D Camera::GetViewProjection() const
{
	Matrix4D cameraRotation = GetTransform().GetRot().ToRotationMatrix();
	Matrix4D cameraTranslation = Matrix4D::Translation(-GetTransform().GetPos());

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