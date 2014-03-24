#include "StdAfx.h"
#include "Camera.h"

#include "Utility\Log.h"
#include "Utility\Config.h"

#include <sstream>

using namespace Rendering;
using namespace Math;
using namespace Utility;

const Vector3D Camera::yAxis = Math::Vector3D(0.0, 1.0, 0.0);

Camera::Camera() :
	pos(GET_CONFIG_VALUE("defaultCameraPos_x", "defaultCameraPos_xDefault", 0.0), GET_CONFIG_VALUE("defaultCameraPos_y", "defaultCameraPos_yDefault", 0.0), GET_CONFIG_VALUE("defaultCameraPos_z", "defaultCameraPos_zDefault", 0.0)),
	forward(GET_CONFIG_VALUE("defaultCameraForward_x", "defaultCameraForward_xDefault", 0.0), GET_CONFIG_VALUE("defaultCameraForward_y", "defaultCameraForward_yDefault", 0.0), GET_CONFIG_VALUE("defaultCameraForward_z", "defaultCameraForward_zDefault", 1.0)),
	up(GET_CONFIG_VALUE("defaultCameraUp_x", "defaultCameraUp_xDefault", 0.0), GET_CONFIG_VALUE("defaultCameraUp_y", "defaultCameraUp_yDefault", 1.0), GET_CONFIG_VALUE("defaultCameraUp_z", "defaultCameraUp_zDefault", 0.0))
{
}

Camera::Camera(const Vector3D& pos, const Vector3D& forward, const Vector3D& up) :
	pos(pos),
	forward(forward),
	up(up)
{
	this->forward.Normalize();
	this->up.Normalize();
}


Camera::~Camera(void)
{
}

void Camera::SetPos(const Vector3D& pos)
{
	this->pos = pos;
}

void Camera::SetForward(const Vector3D& forward)
{
	this->forward = forward;
	this->forward.Normalize();
}

void Camera::SetUp(const Vector3D& up)
{
	this->up = up;
	this->up.Normalize();
}

void Camera::Move(const Vector3D& dir, Math::Real amount)
{
	this->pos = this->pos + (dir * amount);
	//stdlog(Debug, LOGPLACE, "Camera position = %s", this->pos.ToString().c_str());
	stdlog(Debug, LOGPLACE, "%s", ToString().c_str());
}

Vector3D Camera::GetLeft() const
{
	Vector3D left = forward.Cross(up);
	left.Normalize(); // TODO: Check whether Normalize() is necessary
	return left;
}

Math::Vector3D Camera::GetRight() const
{
	//Vector3D right = GetLeft();
	//right.Negate();

	Vector3D right = up.Cross(forward);
	right.Normalize(); // TODO: Check whether Normalize() is necessary
	return right;
}

void Camera::RotateX(Real angleInDegrees)
{
	Vector3D horizontalAxis = Camera::yAxis.Cross(forward);
	horizontalAxis.Normalize(); // TODO: Check whether Normalize() is necessary

	forward.Rotate(angleInDegrees, horizontalAxis);
	forward.Normalize(); // TODO: Check whether Normalize() is necessary

	up = forward.Cross(horizontalAxis);
	up.Normalize(); // TODO: Check whether Normalize() is necessary
}

void Camera::RotateY(Real angleInDegrees)
{
	Vector3D horizontalAxis = Camera::yAxis.Cross(forward);
	horizontalAxis.Normalize(); // TODO: Check whether Normalize() is necessary

	forward.Rotate(angleInDegrees, Camera::yAxis);
	forward.Normalize(); // TODO: Check whether Normalize() is necessary

	up = forward.Cross(horizontalAxis);
	up.Normalize(); // TODO: Check whether Normalize() is necessary
}

void Camera::Input(int key, Real delta)
{
	Real moveAmount = static_cast<Real>(10 * delta);
	Real rotationAmount = static_cast<Real>(100 * delta);

	switch (key)
	{
	case GLFW_KEY_W: // move forward
		Move(GetForward(), moveAmount);
		break;
	case GLFW_KEY_S: // move backward
		Move(GetForward(), -moveAmount);
		break;
	case GLFW_KEY_A: // move left
		Move(GetLeft(), moveAmount);
		break;
	case GLFW_KEY_D: // move right
		Move(GetRight(), moveAmount);
		break;
	case GLFW_KEY_UP: // rotation around X axis
		RotateX(-rotationAmount);
		break;
	case GLFW_KEY_DOWN: // rotation around X axis
		RotateX(rotationAmount);
		break;
	case GLFW_KEY_LEFT: // rotation around Y axis
		RotateY(-rotationAmount);
		break;
	case GLFW_KEY_RIGHT: // rotation around Y axis
		RotateY(rotationAmount);
		break;
	default:
		stdlog(Utility::Info, LOGPLACE, "Some unknown key pressed");
		break;
	}
}

std::string Camera::ToString() const
{
	std::stringstream ss("");

	ss << "Camera = { pos = ";
	ss << pos.ToString();
	ss << "; forward = ";
	ss << forward.ToString();
	ss << "; up = ";
	ss << up.ToString();
	ss << " } ";

	return ss.str();
}