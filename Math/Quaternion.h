#pragma once

#include "Angle.h"
#include "Math.h"
#include "Vector.h"
#include "Matrix.h"
//#include "Utility\ISerializable.h"
#include <string>

namespace Math
{

class MATH_API Quaternion// : public Utility::ISerializable
{
private:
	Real m_x;
	Real m_y;
	Real m_z;
	Real m_w;

public: // constructors and destructors
	Quaternion() : m_x(0.0), m_y(0.0), m_z(0.0), m_w(0.0) { };
	Quaternion(Real x, Real y, Real z, Real w) : m_x(x), m_y(y), m_z(z), m_w(w) { };
	Quaternion(const Vector3D& axis, const Angle& angle);
	Quaternion(const Quaternion& q) : m_x(q.GetX()), m_y(q.GetY()), m_z(q.GetZ()), m_w(q.GetW()) { };
	Quaternion(const Matrix4D& rotMatrix);

public: // public member functions
	Real GetX() const { return m_x; };
	Real GetY() const { return m_y; };
	Real GetZ() const { return m_z; };
	Real GetW() const { return m_w; };
	void SetX(Real x) { m_x = x; };
	void SetY(Real y) { m_y = y; };
	void SetZ(Real z) { m_x = z; };
	void SetW(Real w) { m_y = w; };

	Real Length() const;
	Real LengthSquared() const;
	
	Quaternion Conjugate() const { return Quaternion(-m_x, -m_y, -m_z, m_w); }; // creates negation vector
	
	Quaternion operator+(const Quaternion& q) const { return Quaternion(m_x + q.GetX(), m_y + q.GetY(), m_z + q.GetZ(), m_w + q.GetW()); };
	Quaternion operator-() const { return Quaternion(-m_x, -m_y, -m_z, -m_w); };
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
		return Vector3D(0, 0, 1).Rotate(*this);
	}
	
	inline Vector3D GetUp() const
	{
		return Vector3D(0, 1, 0).Rotate(*this);
	}

	inline Vector3D GetRight() const
	{
		return Vector3D(1, 0, 0).Rotate(*this);
	}

	inline Matrix4D ToRotationMatrix() const;

public:
	std::string ToString() const;
}; /* end class Quaternion */

} /* end namespace Math */