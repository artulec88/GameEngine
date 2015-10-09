#pragma once

#include "Angle.h"
#include "Math.h"
#include "Vector.h"
#include "Matrix.h"
#include <string> // for ToString() method

namespace Math
{

class MATH_API Quaternion
{
/* ==================== Static variables and functions begin ==================== */
/* ==================== Static variables and functions end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	Quaternion() : m_x(REAL_ZERO), m_y(REAL_ZERO), m_z(REAL_ZERO), m_w(REAL_ONE) { };
	Quaternion(Real x, Real y, Real z, Real w) : m_x(x), m_y(y), m_z(z), m_w(w) { };
	Quaternion(const Vector3D& axis, const Angle& angle);
	Quaternion(const Quaternion& q) : m_x(q.GetX()), m_y(q.GetY()), m_z(q.GetZ()), m_w(q.GetW()) { };
	Quaternion(const Matrix4D& rotMatrix);
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	inline Real GetX() const { return m_x; };
	inline Real GetY() const { return m_y; };
	inline Real GetZ() const { return m_z; };
	inline Real GetW() const { return m_w; };

	Real Length() const;
	Real LengthSquared() const;
	
	inline Quaternion Conjugate() const { return Quaternion(-m_x, -m_y, -m_z, m_w); };
	
	// See http://www.euclideanspace.com/maths/algebra/realNormedAlgebra/quaternions/arithmetic/index.htm for quaternion arithmetics.
	Quaternion operator+(const Quaternion& q) const { return Quaternion(m_x + q.GetX(), m_y + q.GetY(), m_z + q.GetZ(), m_w + q.GetW()); };
	Quaternion operator-() const { return Quaternion(-m_x, -m_y, -m_z, -m_w); };
	Quaternion operator-(const Quaternion& q) const { return Quaternion(m_x - q.GetX(), m_y - q.GetY(), m_z - q.GetZ(), m_w - q.GetW()); };
	Quaternion operator*(Real s) const { return Quaternion(s * m_x, s * m_y, s * m_z, s * m_w); };
	Quaternion operator*(const Quaternion& q) const;
	Quaternion operator*(const Vector3D& vec) const;
	Quaternion operator/(Real s) const { return Quaternion(m_x / s, m_y / s, m_z / s, m_w / s); };
	Quaternion& operator=(const Quaternion& v);
	bool operator==(const Quaternion& v) const;
	bool operator!=(const Quaternion& v) const;

	Quaternion Normalized() const;
	void Normalize();
	bool IsNormalized() const;

	Real Dot(const Quaternion& q) const;

	Quaternion Nlerp1(const Quaternion& q, Real nlerpFactor, bool shortest) const;
	Quaternion Nlerp2(const Quaternion& q, Real nlerpFactor, bool shortest) const;
	Quaternion Slerp(const Quaternion& q, Real slerpFactor, bool shortest) const;

	inline Vector3D GetForward() const
	{
		return Vector3D(REAL_ZERO, REAL_ZERO, REAL_ONE).Rotate(*this);
	}
	Vector3D GetUp() const;
	Vector3D GetRight() const;

	Vector3D GetBack() const;
	Vector3D GetDown() const;
	Vector3D GetLeft() const;

	Matrix4D ToRotationMatrix() const;

	std::string ToString() const;
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
private:
	Real m_x;
	Real m_y;
	Real m_z;
	Real m_w;
/* ==================== Non-static member variables end ==================== */
}; /* end class Quaternion */

} /* end namespace Math */