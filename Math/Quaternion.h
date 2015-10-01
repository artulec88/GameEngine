#pragma once

#include "Angle.h"
#include "Math.h"
#include "Vector.h"
#include "Matrix.h"
#ifdef TO_STRING_ENABLED
#include <string>
#endif

namespace Math
{

class MATH_API Quaternion// : public Utility::ISerializable
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
	inline void SetX(Real x) { m_x = x; };
	inline void SetY(Real y) { m_y = y; };
	inline void SetZ(Real z) { m_x = z; };
	inline void SetW(Real w) { m_y = w; };

	Real Length() const;
	Real LengthSquared() const;
	
	Quaternion Conjugate() const { return Quaternion(-m_x, -m_y, -m_z, m_w); };
	
	Quaternion operator+(const Quaternion& q) const { return Quaternion(m_x + q.GetX(), m_y + q.GetY(), m_z + q.GetZ(), m_w + q.GetW()); };
	Quaternion operator-() const { return Quaternion(-m_x, -m_y, -m_z, m_w /* TODO: Find out whether m_w component should be negated */); };
	Quaternion operator-(const Quaternion& q) const { return Quaternion(m_x - q.GetX(), m_y - q.GetY(), m_z - q.GetZ(), m_w - q.GetW()); };
	Quaternion operator*(Real s) const { return Quaternion(s * m_x, s * m_y, s * m_z, s * m_w); };
	Quaternion operator*(const Quaternion& q) const;
	Quaternion operator*(const Vector3D& vec) const;
	//Quaternion operator/(Real s) const { return Quaternion(m_x / s, m_y / s, m_z / s, m_w / s); };
	Quaternion& operator=(const Quaternion& v);
	bool operator==(const Quaternion& v) const;

	Quaternion Normalized() const;
	void Normalize();

	Real Dot(const Quaternion& q) const;

	Quaternion Nlerp(const Quaternion& q, Real nlerpFactor, bool shortest) const;
	Quaternion Slerp(const Quaternion& q, Real slerpFactor, bool shortest) const;

	inline Vector3D GetForward() const
	{
		return Vector3D(REAL_ZERO, REAL_ZERO, REAL_ONE).Rotate(*this);
	}
	inline Vector3D GetUp() const
	{
		return Vector3D(REAL_ZERO, REAL_ONE, REAL_ZERO).Rotate(*this);
	}
	inline Vector3D GetRight() const
	{
		return Vector3D(REAL_ONE, REAL_ZERO, REAL_ZERO).Rotate(*this);
	}

	Vector3D GetBack() const;
	Vector3D GetDown() const;
	Vector3D GetLeft() const;

	inline Matrix4D ToRotationMatrix1() const
	{
		Real xForward = 2.0f * (GetX() * GetZ() - GetW() * GetY());
		Real yForward = 2.0f * (GetY() * GetZ() + GetW() * GetX());
		Real zForward = 1.0f - 2.0f * (GetX() * GetX() + GetY() * GetY());
		Vector3D forward(xForward, yForward, zForward);

		Real xUp = 2.0f * (GetX()*GetY() + GetW()*GetZ());
		Real yUp = 1.0f - 2.0f * (GetX()*GetX() + GetZ()*GetZ());
		Real zUp = 2.0f * (GetY()*GetZ() - GetW()*GetX());
		Vector3D up(xUp, yUp, zUp);

		Real xRight = 1.0f - 2.0f * (GetY()*GetY() + GetZ()*GetZ());
		Real yRight = 2.0f * (GetX()*GetY() - GetW()*GetZ());
		Real zRight = 2.0f * (GetX()*GetZ() + GetW()*GetY());
		Vector3D right(xRight, yRight, zRight);
		
		return Matrix4D(forward, up, right);
	}

	inline Matrix4D ToRotationMatrix2() const
	{
		Vector3D forward(2.0f * (GetX() * GetZ() - GetW() * GetY()), 2.0f * (GetY() * GetZ() + GetW() * GetX()), 1.0f - 2.0f * (GetX() * GetX() + GetY() * GetY()));
		Vector3D up(2.0f * (GetX()*GetY() + GetW()*GetZ()), 1.0f - 2.0f * (GetX()*GetX() + GetZ()*GetZ()), 2.0f * (GetY()*GetZ() - GetW()*GetX()));
		Vector3D right(1.0f - 2.0f * (GetY()*GetY() + GetZ()*GetZ()), 2.0f * (GetX()*GetY() - GetW()*GetZ()), 2.0f * (GetX()*GetZ() + GetW()*GetY()));
		
		return Matrix4D(forward, up, right);
	}

	inline Matrix4D ToRotationMatrix3() const
	{
		return Matrix4D(Vector3D(2.0f * (GetX() * GetZ() - GetW() * GetY()), 2.0f * (GetY() * GetZ() + GetW() * GetX()), 1.0f - 2.0f * (GetX() * GetX() + GetY() * GetY())),
			Vector3D(2.0f * (GetX()*GetY() + GetW()*GetZ()), 1.0f - 2.0f * (GetX()*GetX() + GetZ()*GetZ()), 2.0f * (GetY()*GetZ() - GetW()*GetX())),
			Vector3D(1.0f - 2.0f * (GetY()*GetY() + GetZ()*GetZ()), 2.0f * (GetX()*GetY() - GetW()*GetZ()), 2.0f * (GetX()*GetZ() + GetW()*GetY())));
	}

	Matrix4D ToRotationMatrix4() const;

#ifdef TO_STRING_ENABLED
	std::string ToString() const;
#endif
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