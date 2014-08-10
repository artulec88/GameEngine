#include "stdafx.h"
#include "Vector.h"
#include "Quaternion.h"
#include "FloatingPoint.h"
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
	//Math::Real Vector2D::*px = &Vector2D::m_x;
	//Math::Real Vector2D::*py = &Vector2D::m_y;
	//printf("data members: %p, %p, %p\n", px, py);
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
	Real length = LengthSquared();
	if (AlmostEqual(length, 0.0f))
	{
		return (*this);
	}
	return (*this) / static_cast<Real>(sqrt(length));
}

void Vector2D::Normalize()
{
	Real length = LengthSquared();
	if (AlmostEqual(length, 0.0f))
	{
		return;
	}
	*this = (*this) / static_cast<Real>(sqrt(length));
}

Real Vector2D::Cross(const Vector2D& v) const
{
	return m_x * v.GetY() - m_y * v.GetX();
}

Real Vector2D::Dot(const Vector2D& v) const
{
	return (m_x * v.GetX() + m_y * v.GetY());
}

Vector2D Vector2D::Rotate(const Angle& angle)
{
	Real rad = angle.GetAngleInRadians();
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

inline bool Vector2D::operator==(const Vector2D& v) const
{
	// TODO: Create a floating-point comparer and use it here
	return ( (this->GetX() == v.GetX()) && (this->GetY() == v.GetY()) );
}

Vector2D Vector2D::Lerp(const Vector2D& vec, Real lerpFactor) const
{
	return ((vec - (*this)) * lerpFactor) + (*this);
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
	//Math::Real Vector3D::*px = &Vector3D::m_x;
	//Math::Real Vector3D::*py = &Vector3D::m_y;
	//Math::Real Vector3D::*pz = &Vector3D::m_z;
	//printf("data members: %p, %p, %p\n", px, py, pz);
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
	Real length = LengthSquared();
	if (AlmostEqual(length, 0.0f))
	{
		return (*this);
	}
	return (*this) / static_cast<Real>(sqrt(length));
}

void Vector3D::Normalize()
{
	Real length = LengthSquared();
	if (AlmostEqual(length, 0.0f))
	{
		return;
	}
	(*this) = (*this) / static_cast<Real>(sqrt(length));
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

Vector3D Vector3D::Rotate(const Vector3D& axis, const Angle& angle)
{
	// TODO: Test which method is faster and use it instead of the other

	/* ==================== METHOD #1 ==================== */
	//Quaternion rotation(axis, angle);
	//Quaternion conjugate(rotation.Conjugate());

	//Quaternion w((rotation * (*this)) * conjugate);

	//this->m_x = w.GetX();
	//this->m_y = w.GetY();
	//this->m_z = w.GetZ();

	//return *this;

	/* ==================== METHOD #2 ==================== */
	Real sinAngle = static_cast<Real>(sin(-angle.GetAngleInRadians()));
	Real cosAngle = static_cast<Real>(cos(-angle.GetAngleInRadians()));

	return this->Cross(axis * sinAngle) + // rotation on local X
		((*this * cosAngle) + // rotation on local Z
		(axis * (this->Dot(axis * (1.0f - cosAngle))))); // rotation on local Y
}

Vector3D Vector3D::Rotate(const Quaternion& rotation) const
{
	//CHECKED
	Quaternion conjugate = rotation.Conjugate();
	Quaternion w = rotation * (*this) * conjugate;

	return Vector3D(w.GetX(), w.GetY(), w.GetZ());
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

inline bool Vector3D::operator==(const Vector3D& v) const
{
	// TODO: Create a floating-point comparer and use it here
	return ( (this->GetX() == v.GetX()) && (this->GetY() == v.GetY()) && (this->GetZ() == v.GetZ()) );
}

Vector3D Vector3D::Lerp(const Vector3D& vec, Real lerpFactor) const
{
	return ((vec - (*this)) * lerpFactor) + (*this);
}

void Vector3D::ApproachX(Real step, Real approachedValue)
{
	if (m_x > approachedValue)
	{
		m_x -= step;
		if (m_x < approachedValue)
		{
			m_x = approachedValue;
		}
	}
	else
	{
		m_x += step;
		if (m_x > approachedValue)
		{
			m_x = approachedValue;
		}
	}
}

void Vector3D::ApproachY(Real step, Real approachedValue)
{
	if (m_y > approachedValue)
	{
		m_y -= step;
		if (m_y < approachedValue)
		{
			m_y = approachedValue;
		}
	}
	else
	{
		m_y += step;
		if (m_y > approachedValue)
		{
			m_y = approachedValue;
		}
	}
}

void Vector3D::ApproachZ(Real step, Real approachedValue)
{
	if (m_z > approachedValue)
	{
		m_z -= step;
		if (m_z < approachedValue)
		{
			m_z = approachedValue;
		}
	}
	else
	{
		m_z += step;
		if (m_z > approachedValue)
		{
			m_z = approachedValue;
		}
	}
}

void Vector3D::Threshold(Real maxLength)
{
	Real length = Length();
	if (length > maxLength)
	{
		Real quotient = length / maxLength;
		m_x /= quotient;
		m_y /= quotient;
		m_z /= quotient;
	}
}