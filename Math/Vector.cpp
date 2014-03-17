#include "stdafx.h"
#include "Vector.h"
#include <math.h>

using namespace Math;


Vector2D::Vector2D() :
	m_x(0),
	m_y(0)
{
}

Vector2D::Vector2D(Real x, Real y) :
	m_x(x),
	m_y(y)
{
}

Vector2D::Vector2D(const Vector2D& v)
{
	this->m_x = v.GetX();
	this->m_y = v.GetY();
}

Vector2D::~Vector2D()
{
}

std::string Vector2D::ToString() const
{
	std::stringstream ss("");
	ss << "(x=" << m_x << "; y=" << m_y << ")";
	return ss.str();
}

Real Vector2D::Length() const
{
	return static_cast<Real>(sqrt(static_cast<Real>(LengthSquared())));
}

Real Vector2D::LengthSquared() const
{
	return static_cast<Real>(m_x * m_x + m_y * m_y);
}

Vector2D Vector2D::Normalized() const
{
	return (*this) / Length();
}

void Vector2D::Normalize()
{
	(*this) = (*this) / Length();
}

Real Vector2D::Dot(const Vector2D& v) const
{
	return (m_x * v.GetX() + m_y * v.GetY());
}

Vector2D Vector2D::Rotate(Real angleInDegrees)
{
	Real rad = Math::ToRad(angleInDegrees);
	Real cosine = cos(rad);
	Real sine = sin(rad);

	return Vector2D(m_x * cosine - m_y * sine, m_x * sine + m_y * cosine);
}

Vector2D& Vector2D::operator+=(const Vector2D& v)
{
	this->m_x += v.GetX();
	this->m_y += v.GetY();

	return *this;
}

Vector2D& Vector2D::operator-=(const Vector2D& v)
{
	this->m_x -= v.GetX();
	this->m_y -= v.GetY();

	return *this;
}

Vector2D& Vector2D::operator*=(const Vector2D& v)
{
	this->m_x *= v.GetX();
	this->m_y *= v.GetY();

	return *this;
}

Vector2D& Vector2D::operator/=(const Vector2D& v)
{
	this->m_x /= v.GetX();
	this->m_y /= v.GetY();

	return *this;
}

Vector2D& Vector2D::operator=(const Vector2D& v)
{
	this->m_x = v.GetX();
	this->m_y = v.GetY();

	return *this;
}


/* ==================== Vector3D ==================== */
Vector3D::Vector3D() :
	m_x(0),
	m_y(0),
	m_z(0)
{
}

Vector3D::Vector3D(Real x, Real y, Real z) :
	m_x(x),
	m_y(y),
	m_z(z)
{
}

Vector3D::Vector3D(const Vector3D& v)
{
	this->m_x = v.GetX();
	this->m_y = v.GetY();
	this->m_z = v.GetZ();
}

Vector3D::~Vector3D()
{
}

std::string Vector3D::ToString() const
{
	std::stringstream ss("");
	ss << "(x=" << m_x << "; y=" << m_y << "; z=" << m_z << ")";
	return ss.str();
}

Real Vector3D::Length() const
{
	return static_cast<Real>(sqrt(static_cast<Real>(LengthSquared())));
}

Real Vector3D::LengthSquared() const
{
	return static_cast<Real>(m_x * m_x + m_y * m_y + m_z * m_z);
}

Vector3D Vector3D::Normalized() const
{
	return (*this) / Length();
}

void Vector3D::Normalize()
{
	(*this) = (*this) / Length();
}

Real Vector3D::Dot(const Vector3D& v) const
{
	return (m_x * v.GetX() + m_y * v.GetY() + m_z * v.GetZ());
}

Vector3D Vector3D::Cross(const Vector3D& v) const
{
	Real x = m_y * v.GetZ() - m_z * v.GetY();
	Real y = m_z * v.GetX() - m_x * v.GetZ();
	Real z = m_x * v.GetY() - m_y * v.GetX();

	return Vector3D(x, y, z);
}

Vector3D Vector3D::Rotate(Real angleInDegrees)
{
	// TODO: Implement this using Quaternions

	return *this; // just for now
}

Vector3D& Vector3D::operator+=(const Vector3D& v)
{
	this->m_x += v.GetX();
	this->m_y += v.GetY();
	this->m_z += v.GetZ();

	return *this;
}

Vector3D& Vector3D::operator-=(const Vector3D& v)
{
	this->m_x -= v.GetX();
	this->m_y -= v.GetY();
	this->m_z -= v.GetZ();

	return *this;
}

Vector3D& Vector3D::operator*=(const Vector3D& v)
{
	this->m_x *= v.GetX();
	this->m_y *= v.GetY();
	this->m_z *= v.GetZ();

	return *this;
}

Vector3D& Vector3D::operator/=(const Vector3D& v)
{
	this->m_x /= v.GetX();
	this->m_y /= v.GetY();
	this->m_z /= v.GetZ();

	return *this;
}

Vector3D& Vector3D::operator=(const Vector3D& v)
{
	this->m_x = v.GetX();
	this->m_y = v.GetY();
	this->m_z = v.GetZ();

	return *this;
}