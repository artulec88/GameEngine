#include "StdAfx.h"
#include "Quaternion.h"
#include <sstream>

using namespace Math;

Quaternion::Quaternion(const Vector3D& axis, const Angle& angle)
{
	Real sinHalfAngle = static_cast<Real>(sin(angle.GetAngleInRadians() / 2));
	Real cosHalfAngle = static_cast<Real>(cos(angle.GetAngleInRadians() / 2));

	m_x = axis.GetX() * sinHalfAngle;
	m_y = axis.GetY() * sinHalfAngle;
	m_z = axis.GetZ() * sinHalfAngle;
	m_w = cosHalfAngle;
}

std::string Quaternion::ToString() const
{
	std::stringstream ss("");
	ss << m_x << " " << m_y << " " << m_z << " " << m_w << " ";
	return ss.str();
}

Real Quaternion::Length() const
{
	return static_cast<Real>(sqrt(static_cast<Real>(LengthSquared())));
}

Real Quaternion::LengthSquared() const
{
	return static_cast<Real>(m_x * m_x + m_y * m_y + m_z * m_z + m_w * m_w);
}

Quaternion Quaternion::Normalized() const
{
	Real x = m_x / Length();
	Real y = m_y / Length();
	Real z = m_z / Length();
	Real w = m_w / Length();

	return Quaternion(x, y, z, w);
}

void Quaternion::Normalize()
{
	Real length = Length();
	m_x /= length;
	m_y /= length;
	m_z /= length;
	m_w /= length;
}

Quaternion Quaternion::operator*(const Quaternion& q) const
{
	Real w = m_w * q.GetW() - m_x * q.GetX() - m_y * q.GetY() - m_z * q.GetZ();
	Real x = m_x * q.GetW() + m_w * q.GetX() + m_y * q.GetZ() - m_z * q.GetY();
	Real y = m_y * q.GetW() + m_w * q.GetY() + m_z * q.GetX() - m_x * q.GetZ();
	Real z = m_z * q.GetW() + m_w * q.GetZ() + m_x * q.GetY() - m_y * q.GetX();

	return Quaternion(x, y, z, w);
}

Quaternion Quaternion::operator*(const Vector3D& vec) const
{
	Real w = -m_x * vec.GetX() - m_y * vec.GetY() - m_z * vec.GetZ();
	Real x = m_w * vec.GetX() + m_y * vec.GetZ() - m_z * vec.GetY();
	Real y = m_w * vec.GetY() + m_z * vec.GetX() - m_x * vec.GetZ();
	Real z = m_w * vec.GetZ() + m_x * vec.GetY() - m_y * vec.GetX();

	return Quaternion(x, y, z, w);
}

//inline Vector3D Quaternion::GetForward() const
//{
//	Real x = 2.0f * (GetX() * GetZ() - GetW() * GetY());
//	Real y = 2.0f * (GetY() * GetZ() + GetW() * GetX());
//	Real z = 1.0f - 2.0f * (GetX() * GetX() + GetY() * GetY());
//	return Vector3D(x, y, z);
//
//	//return Vector3D(0.0, 0.0, 1.0).Rotate(*this);
//}
//
//inline Vector3D Quaternion::GetUp() const
//{
//	Real x = 2.0f * (GetX() * GetY() + GetW() * GetZ());
//	Real y = 1.0f - 2.0f * (GetX() * GetX() + GetZ() * GetZ());
//	Real z = 2.0f * (GetY() * GetZ() - GetW() * GetX());
//	return Vector3D(x, y, z);
//
//	//return Vector3D(0.0, 1.0, 0.0).Rotate(*this);
//}
//
//inline Vector3D Quaternion::GetRight() const
//{
//	Real x = 1.0f - 2.0f * (GetY() * GetY() + GetZ() * GetZ());
//	Real y = 2.0f * (GetX() * GetY() - GetW() * GetZ());
//	Real z = 2.0f * (GetX() * GetZ() + GetW() * GetY());
//	return Vector3D(x, y, z);
//
//	//return Vector3D(1.0, 0.0, 0.0).Rotate(*this);
//}

inline Matrix4D Quaternion::ToRotationMatrix() const
{
	Vector3D forward(2.0f * (GetX() * GetZ() - GetW() * GetY()), 2.0f * (GetY() * GetZ() + GetW() * GetX()), 1.0f - 2.0f * (GetX() * GetX() + GetY() * GetY()));
	Vector3D up(2.0f * (GetX() * GetY() + GetW() * GetZ()), 1.0f - 2.0f * (GetX() * GetX() + GetZ() * GetZ()), 2.0f * (GetY() * GetZ() - GetW() * GetX()));
	Vector3D right(1.0f - 2.0f * (GetY() * GetY() + GetZ() * GetZ()), 2.0f * (GetX() * GetY() - GetW() * GetZ()), 2.0f * (GetX() * GetZ() + GetW() * GetY()));

	return Matrix4D::Rotation(forward, up, right);
}