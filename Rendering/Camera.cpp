#include "StdAfx.h"
#include "Camera.h"

#include "Utility\Log.h"

using namespace Rendering;
using namespace Math;

const Vector3D Camera::yAxis = Math::Vector3D(0.0, 1.0, 0.0);

Camera::Camera() :
	pos(0.0, 0.0, 0.0),
	forward(0.0, 0.0, 1.0),
	up(0.0, 1.0, 0.0)
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
}

void Camera::SetUp(const Vector3D& up)
{
	this->up = up;
}

void Camera::Move(const Vector3D& dir, Math::Real amount)
{
	this->pos = this->pos + (dir * amount);
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
		stdlog(Utility::Warning, LOGPLACE, "Some unknown key pressed");
		break;
	}
}