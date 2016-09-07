#include "StdAfx.h"
#include "Quaternion.h"
#include "FloatingPoint.h"
#include <sstream>

Math::Quaternion::Quaternion(const Vector3D& axis, const Angle& angle)
{
	Angle halfAngle(angle / 2);
	Real sinHalfAngle = halfAngle.Sin();

	m_x = axis.GetX() * sinHalfAngle;
	m_y = axis.GetY() * sinHalfAngle;
	m_z = axis.GetZ() * sinHalfAngle;
	m_w = halfAngle.Cos();
}

// From Ken Shoemake's "Quaternion Calculus and Fast Animation" article
Math::Quaternion::Quaternion(const Matrix4D& rotMatrix)
{
	// FIXME: TRANSFORMATIONS ORDER
	/*Real m00 = rotMatrix.GetElement(0, 0);
	Real m11 = rotMatrix.GetElement(1, 1);
	Real m22 = rotMatrix.GetElement(2, 2);

	Real trace = m00 + m11 + m22;

	//DEBUG_LOG_MATH("trace = ", trace);
	if (trace > 0)
	{
		Real s = 0.5f / static_cast<Real>(sqrt(trace + 1.0f));
		m_w = 0.25f / s;
		m_x = (rotMatrix.GetElement(1, 2) - rotMatrix.GetElement(2, 1)) * s;
		m_y = (rotMatrix.GetElement(2, 0) - rotMatrix.GetElement(0, 2)) * s;
		m_z = (rotMatrix.GetElement(0, 1) - rotMatrix.GetElement(1, 0)) * s;
		//DEBUG_LOG_MATH("temp = ", temp, "; s = ", s, "; quaternion = ", ToString());
	}
	else if ( (m00 > m11) && (m00 > m22) )
	{
		Real s = 2.0f * static_cast<Real>(sqrt(1.0f + m00 - m11 - m22));
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

	Real length = static_cast<Real>(sqrt(m_x * m_x + m_y * m_y + m_z * m_z + m_w * m_w));
	m_x /= length;
	m_y /= length;
	m_z /= length;
	m_w /= length;*/

	Real m00 = rotMatrix.GetElement(0, 0);
	Real m11 = rotMatrix.GetElement(1, 1);
	Real m22 = rotMatrix.GetElement(2, 2);

	Real trace = m00 + m11 + m22;

	//DEBUG_LOG_MATH("trace = ", trace);
	if (trace > 0)
	{
		Real s = static_cast<Real>(0.5f) / static_cast<Real>(sqrt(trace + REAL_ONE));
		m_w = static_cast<Real>(0.25f) / s;
		m_x = (rotMatrix.GetElement(1, 2) - rotMatrix.GetElement(2, 1)) * s;
		m_y = (rotMatrix.GetElement(2, 0) - rotMatrix.GetElement(0, 2)) * s;
		m_z = (rotMatrix.GetElement(0, 1) - rotMatrix.GetElement(1, 0)) * s;
		//DEBUG_LOG_MATH("temp = ", temp, "; s = ", s, "; quaternion = ", ToString());
	}
	else if ( (m00 > m11) && (m00 > m22) )
	{
		Real s = 2.0f * static_cast<Real>(sqrt(REAL_ONE + m00 - m11 - m22));
		m_w = (rotMatrix.GetElement(1, 2) - rotMatrix.GetElement(2, 1)) / s;
		m_x = static_cast<Real>(0.25f) * s;
		m_y = (rotMatrix.GetElement(1, 0) + rotMatrix.GetElement(0, 1)) / s;
		m_z = (rotMatrix.GetElement(2, 0) + rotMatrix.GetElement(0, 2)) / s;
	}
	else if (m11 > m22)
	{
		Real s = 2.0f * static_cast<Real>(sqrt(REAL_ONE + m11 - m00 - m22));
		m_w = (rotMatrix.GetElement(2, 0) - rotMatrix.GetElement(0, 2)) / s;
		m_x = (rotMatrix.GetElement(1, 0) + rotMatrix.GetElement(0, 1)) / s;
		m_y = static_cast<Real>(0.25f) * s;
		m_z = (rotMatrix.GetElement(2, 1) + rotMatrix.GetElement(1, 2)) / s;
	}
	else
	{
		Real s = 2.0f * static_cast<Real>(sqrt(REAL_ONE + m22 - m11 - m00));
		m_w = (rotMatrix.GetElement(0, 1) - rotMatrix.GetElement(1, 0)) / s;
		m_x = (rotMatrix.GetElement(2, 0) + rotMatrix.GetElement(0, 2)) / s;
		m_y = (rotMatrix.GetElement(1, 2) + rotMatrix.GetElement(2, 1)) / s;
		m_z = static_cast<Real>(0.25f) * s;
	}

	Real length = static_cast<Real>(sqrt(m_x * m_x + m_y * m_y + m_z * m_z + m_w * m_w));
	m_x /= length;
	m_y /= length;
	m_z /= length;
	m_w /= length;
}

Math::Matrix4D Math::Quaternion::ToRotationMatrix() const
{
	// FIXME: TRANSFORMATIONS ORDER
	// This function was tested and proved to be the fastest of 6 variants of the same function.
	// It prooved to be faster even than inline functions.
	return Matrix4D(Vector3D(2.0f * (GetX() * GetZ() - GetW() * GetY()), 2.0f * (GetY() * GetZ() + GetW() * GetX()), 1.0f - 2.0f * (GetX() * GetX() + GetY() * GetY())),
		Vector3D(2.0f * (GetX()*GetY() + GetW()*GetZ()), 1.0f - 2.0f * (GetX()*GetX() + GetZ()*GetZ()), 2.0f * (GetY()*GetZ() - GetW()*GetX())),
		Vector3D(1.0f - 2.0f * (GetY()*GetY() + GetZ()*GetZ()), 2.0f * (GetX()*GetY() - GetW()*GetZ()), 2.0f * (GetX()*GetZ() + GetW()*GetY())));
}

Math::Real Math::Quaternion::Length() const
{
	return static_cast<Real>(sqrt(static_cast<Real>(LengthSquared())));
}

Math::Real Math::Quaternion::LengthSquared() const
{
	return static_cast<Real>(m_x * m_x + m_y * m_y + m_z * m_z + m_w * m_w);
}

void Math::Quaternion::InvertPitch()
{
	// TODO: Warning. This function has not yet been tested. Use with caution.
	m_x = -m_x;
	m_z = -m_z;
}

Math::Quaternion Math::Quaternion::Normalized() const
{
	Real length = Length();
	return Quaternion(m_x / length, m_y / length, m_z / length, m_w / length);
}

void Math::Quaternion::Normalize()
{
	Real length = Length();
	if (AlmostEqual(length , REAL_ONE))
	{
		DEBUG_LOG_MATH("The quaternion is already normalized");
		return;
	}
	m_x /= length;
	m_y /= length;
	m_z /= length;
	m_w /= length;
}

bool Math::Quaternion::IsNormalized() const
{
	Real lengthSquared = LengthSquared();
	return AlmostEqual(lengthSquared, REAL_ONE);
}

Math::Quaternion Math::Quaternion::operator*(const Quaternion& q) const
{
	// The subtractions in these equations come from the imaginary numbers multiplication, because i * i = -1.
	Real x = m_x * q.GetW() + m_w * q.GetX() + m_y * q.GetZ() - m_z * q.GetY();
	Real y = m_y * q.GetW() + m_w * q.GetY() + m_z * q.GetX() - m_x * q.GetZ();
	Real z = m_z * q.GetW() + m_w * q.GetZ() + m_x * q.GetY() - m_y * q.GetX();
	Real w = m_w * q.GetW() - m_x * q.GetX() - m_y * q.GetY() - m_z * q.GetZ();

	return Quaternion(x, y, z, w);
}

Math::Quaternion Math::Quaternion::operator*(const Vector3D& vec) const
{
	// CHECKED
	Real w = -m_x * vec.GetX() - m_y * vec.GetY() - m_z * vec.GetZ();
	Real x = m_w * vec.GetX() + m_y * vec.GetZ() - m_z * vec.GetY();
	Real y = m_w * vec.GetY() + m_z * vec.GetX() - m_x * vec.GetZ();
	Real z = m_w * vec.GetZ() + m_x * vec.GetY() - m_y * vec.GetX();

	return Quaternion(x, y, z, w);
}

Math::Real Math::Quaternion::Dot(const Quaternion& q) const
{
	return m_x * q.GetX() + m_y * q.GetY() + m_z * q.GetZ() + m_w * q.GetW();
}

Math::Quaternion Math::Quaternion::Nlerp1(const Quaternion& q, Real nlerpFactor, bool shortest) const
{
	Quaternion fixedQ((shortest && Dot(q) < REAL_ZERO) ? -q : q);
	return (((fixedQ - *this) * nlerpFactor) + *this).Normalized();  // FIXME: Check quaternion multiplication
}

Math::Quaternion Math::Quaternion::Nlerp2(const Quaternion& q, Real nlerpFactor, bool shortest) const
{
	// FIXME: Check quaternion multiplication
	if (shortest && Dot(q) < REAL_ZERO)
	{
		return Quaternion((-q.GetX() - m_x) * nlerpFactor + m_x, (-q.GetY() - m_y) * nlerpFactor + m_y, (-q.GetZ() - m_z) * nlerpFactor + m_z, (-q.GetW() - m_w) * nlerpFactor + m_w).Normalized();
	}
	else
	{
		return Quaternion((q.GetX() - m_x) * nlerpFactor + m_x, (q.GetY() - m_y) * nlerpFactor + m_y, (q.GetZ() - m_z) * nlerpFactor + m_z, (q.GetW() - m_w) * nlerpFactor + m_w).Normalized();
	}
}

Math::Quaternion Math::Quaternion::Slerp(const Quaternion& q, Real slerpFactor, bool shortest) const
{
	// CHECKED
	Real cos = this->Dot(q);
	Quaternion fixedQ(q);

	if (shortest && cos < 0)
	{
		cos = -cos;
		fixedQ = -q; // TODO: Replace with fixedQ.Negate();
	}

	if (abs(cos) > 1 - EPSILON)
	{
		return Nlerp2(fixedQ, slerpFactor, false);
	}

	Real sinus = static_cast<Real>(sqrt(REAL_ONE - cos * cos));
	Real angle = atan2(sinus, cos);
	Real invSinus = REAL_ONE / sinus;

	Real srcFactor = static_cast<Real>(sin((REAL_ONE - slerpFactor) * angle)) * invSinus;
	Real destFactor = static_cast<Real>(sin(slerpFactor * angle)) * invSinus;

	return Quaternion((*this) * srcFactor + fixedQ * destFactor); // FIXME: Check quaternion multiplication
}

Math::Vector3D Math::Quaternion::GetBack() const
{
	return Vector3D(REAL_ZERO, REAL_ZERO, -REAL_ONE).Rotate(*this);
}

Math::Vector3D Math::Quaternion::GetUp() const
{
	return Vector3D(REAL_ZERO, REAL_ONE, REAL_ZERO).Rotate(*this);
}

Math::Vector3D Math::Quaternion::GetDown() const
{
	return Vector3D(REAL_ZERO, -REAL_ONE, REAL_ZERO).Rotate(*this);
}

Math::Vector3D Math::Quaternion::GetRight() const
{
	return Vector3D(REAL_ONE, REAL_ZERO, REAL_ZERO).Rotate(*this);
}

Math::Vector3D Math::Quaternion::GetLeft() const
{
	return Vector3D(-REAL_ONE, REAL_ZERO, REAL_ZERO).Rotate(*this);
}

Math::Quaternion& Math::Quaternion::operator=(const Quaternion& q)
{
	m_x = q.GetX();
	m_y = q.GetY();
	m_z = q.GetZ();
	m_w = q.GetW();
	return *this;
}

Math::Quaternion& Math::Quaternion::operator=(Quaternion&& q)
{
	m_x = std::move(q.m_x);
	m_y = std::move(q.m_y);
	m_z = std::move(q.m_z);
	m_w = std::move(q.m_w);
	return *this;
}

bool Math::Quaternion::operator==(const Quaternion& q) const
{
	return ( AlmostEqual(m_x, q.GetX()) && AlmostEqual(m_y, q.GetY()) && AlmostEqual(m_z, q.GetZ()) && AlmostEqual(m_w, q.GetW()) );
}

bool Math::Quaternion::operator!=(const Quaternion& q) const
{
	return ( !AlmostEqual(m_x, q.GetX()) || !AlmostEqual(m_y, q.GetY()) || !AlmostEqual(m_z, q.GetZ()) || !AlmostEqual(m_w, q.GetW()) );
}

std::string Math::Quaternion::ToString() const
{
	// TODO: Set precision (std::precision)
	std::stringstream ss("");
	ss << m_x << " " << m_y << " " << m_z << " " << m_w << " ";
	return ss.str();
}