#pragma once

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
	
	Quaternion Conjugate() { return Quaternion(-m_x, -m_y, -m_z, m_w); }; // creates negation vector
	
	//Quaternion operator+(const Quaternion& q) const { return Quaternion(m_x + q.GetX(), m_y + q.GetY(), m_z + q.GetZ(), m_w + q.GetW()); };
	//Quaternion operator-() const { return Quaternion(-m_x, -m_y, -m_z, -m_w); };
	//Quaternion operator-(const Quaternion& q) const { return Quaternion(m_x - q.GetX(), m_y - q.GetY(), m_z - q.GetZ(), m_w - q.GetW()); };
	//Quaternion operator*(Real s) const { return Quaternion(s * m_x, s * m_y, s * m_z, s * m_w); };
	Quaternion operator*(const Quaternion& q) const;
	Quaternion operator*(const Vector3D& vec) const;
	//Quaternion operator/(Real s) const { return Quaternion(m_x / s, m_y / s, m_z / s, m_w / s); };

	Quaternion Normalized() const;
	void Normalize();

	inline Vector3D GetForward() const
	{
		Real x = 2.0f * (GetX() * GetZ() - GetW() * GetY());
		Real y = 2.0f * (GetY() * GetZ() + GetW() * GetX());
		Real z = 1.0f - 2.0f * (GetX() * GetX() + GetY() * GetY());
		return Vector3D(x, y, z);
	}

	inline Vector3D GetUp() const
	{
		Real x = 2.0f * (GetX() * GetY() + GetW() * GetZ());
		Real y = 1.0f - 2.0f * (GetX() * GetX() + GetZ() * GetZ());
		Real z = 2.0f * (GetY() * GetZ() - GetW() * GetX());
		return Vector3D(x, y, z);
	}

	inline Vector3D GetRight() const
	{
		Real x = 1.0f - 2.0f * (GetY() * GetY() + GetZ() * GetZ());
		Real y = 2.0f * (GetX() * GetY() - GetW() * GetZ());
		Real z = 2.0f * (GetX() * GetZ() + GetW() * GetY());
		return Vector3D(x, y, z);
	}

	inline Matrix4D ToRotationMatrix() const { return Matrix4D::Rotation(GetForward(), GetUp(), GetRight()); }

public:
	std::string ToString() const;
}; /* end class Quaternion */

} /* end namespace Math */