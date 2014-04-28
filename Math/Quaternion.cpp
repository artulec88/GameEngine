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

// From Ken Shoemake's "Quaternion Calculus and Fast Animation" article
Quaternion::Quaternion(const Matrix4D& rotMatrix)
{
	Real m00 = rotMatrix.GetElement(0, 0);
	Real m11 = rotMatrix.GetElement(1, 1);
	Real m22 = rotMatrix.GetElement(2, 2);

	Real trace = m00 + m11 + m22;

	stdlog(Utility::Debug, LOGPLACE, "trace = %.3f", trace);
	if (trace > 0)
	{
		Real s = 0.5f / static_cast<Real>(sqrt(trace + 1.0f));
		m_w = 0.25f / s;
		m_x = (rotMatrix.GetElement(1, 2) - rotMatrix.GetElement(2, 1)) * s;
		m_y = (rotMatrix.GetElement(2, 0) - rotMatrix.GetElement(0, 2)) * s;
		m_z = (rotMatrix.GetElement(0, 1) - rotMatrix.GetElement(1, 0)) * s;
		stdlog(Utility::Debug, LOGPLACE, "s = %.3f; (x, y, z, w) = (%.3f, %.3f, %.3f, %.3f)", s, m_x, m_y, m_z, m_w);
	}
	else /* trace <= 0 */
	{
		if ( (m00 > m11) && (m00 > m22) )
		{
			Real s = 2.0f * static_cast<Real>(sqrt(1.0 + m00 - m11 - m22));
			m_w = (rotMatrix.GetElement(1, 2) - rotMatrix.GetElement(2, 1)) / s;
			m_x = 0.25f * s;
			m_y = (rotMatrix.GetElement(1, 0) + rotMatrix.GetElement(0, 1)) / s;
			m_z = (rotMatrix.GetElement(2, 0) + rotMatrix.GetElement(0, 2)) / s;
		}
		else if (m11 > m22)
		{
			Real s = 2.0f * static_cast<Real>(sqrt(1.0 + m11 - m00 - m22));
			m_w = (rotMatrix.GetElement(2, 0) - rotMatrix.GetElement(0, 2)) / s;
			m_x = (rotMatrix.GetElement(1, 0) + rotMatrix.GetElement(0, 1)) / s;
			m_y = 0.25f * s;
			m_z = (rotMatrix.GetElement(2, 1) + rotMatrix.GetElement(1, 2)) / s;
		}
		else
		{
			Real s = 2.0f * static_cast<Real>(sqrt(1.0 + m22 - m00 - m11));
			m_w = (rotMatrix.GetElement(0, 1) - rotMatrix.GetElement(1, 0)) / s;
			m_x = (rotMatrix.GetElement(2, 0) + rotMatrix.GetElement(0, 2)) / s;
			m_y = (rotMatrix.GetElement(1, 2) + rotMatrix.GetElement(2, 1)) / s;
			m_z = 0.25f * s;
		}
	}

	Real length = static_cast<Real>(sqrt(m_x * m_x + m_y * m_y + m_z * m_z + m_w * m_w));
	m_x /= length;
	m_y /= length;
	m_z /= length;
	m_w /= length;
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

Real Quaternion::Dot(const Quaternion& q) const
{
	return m_x * q.GetX() + m_y * q.GetY() + m_z * q.GetZ() + m_w * q.GetW();
}

Quaternion Quaternion::Nlerp(const Quaternion& q, Real nlerpFactor, bool shortest) const
{
	Quaternion fixedQ(q);

	if (shortest && (this->Dot(q) < 0))
	{
		fixedQ = -q;
	}

	return (((fixedQ - *this) * nlerpFactor) + *this).Normalized();
}

Quaternion Quaternion::Slerp(const Quaternion& q, Real slerpFactor, bool shortest) const
{
	const Real EPSILON = 1e3f;

	Real cos = this->Dot(q);
	Quaternion fixedQ(q);

	if (shortest && cos < 0)
	{
		cos = -cos;
		fixedQ = -q;
	}

	if (abs(cos) >= 1 - EPSILON)
	{
		return Nlerp(fixedQ, slerpFactor, false);
	}

	Real sinus = static_cast<Real>(sqrt(1.0f - cos * cos));
	Real angle = atan2(sinus, cos);
	Real invSinus = 1.0f / sinus;

	Real srcFactor = static_cast<Real>(sin((1.0f - slerpFactor) * angle)) * invSinus;
	Real destFactor = static_cast<Real>(sin(slerpFactor * angle)) * invSinus;

	return (*this * srcFactor) + (fixedQ * destFactor);
}

Quaternion& Quaternion::operator=(const Quaternion& q)
{
	this->m_x = q.GetX();
	this->m_y = q.GetY();
	this->m_z = q.GetZ();
	this->m_w = q.GetW();

	return *this;
}

bool Quaternion::operator==(const Quaternion& q) const
{
	// TODO: Create a floating-point comparer and use it here
	return ( (this->GetX() == q.GetX()) && (this->GetY() == q.GetY()) && (this->GetZ() == q.GetZ()) && (this->GetW() == q.GetW()) );
}