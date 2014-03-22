#pragma once

#include "Math.h"
//#include "Utility\ISerializable.h"
#include <string>
#include <sstream>

namespace Math
{
class MATH_API Vector2D
{
public: // constructors and destructors
	Vector2D();
	Vector2D(Real x, Real y);
	Vector2D(const Vector2D& v);
	~Vector2D();

public: // public member functions
	Real GetX() const { return m_x; };
	Real GetY() const { return m_y; };
	void SetX(Real x) { m_x = x; };
	void SetY(Real y) { m_y = y; };

	Real Length() const;
	Real LengthSquared() const;
	
	void Negate() { m_x = -m_x; m_y = -m_y; }; // creates negation vector
	
	Vector2D operator+(const Vector2D& v) const { return Vector2D(m_x + v.GetX(), m_y + v.GetY()); };
	Vector2D operator-() const { return Vector2D(-m_x, -m_y); };
	Vector2D operator-(const Vector2D& v) const { return Vector2D(m_x - v.GetX(), m_y - v.GetY()); };
	Vector2D operator*(Real s) const { return Vector2D(s * m_x, s * m_y); };
	Vector2D operator/(Real s) const { return Vector2D(m_x / s, m_y / s); };
	
	Vector2D& operator+=(const Vector2D& v);
	Vector2D& operator-=(const Vector2D& v);
	Vector2D& operator*=(const Vector2D& v);
	Vector2D& operator/=(const Vector2D& v);
	Vector2D& operator=(const Vector2D& v);
	bool operator==(const Vector2D& v) const;

	Vector2D Normalized() const;
	void Normalize();

	Vector2D Rotate(Real angle);
	
	Real Dot(const Vector2D& v) const;

public:
	std::string ToString() const;

protected: // member variables
	Real m_x;
	Real m_y;
}; /* end class Vector2D */


class MATH_API Vector3D// : public Utility::ISerializable
{
public: // constructors and destructors
	Vector3D();
	Vector3D(Real x, Real y, Real z);
	Vector3D(const Vector3D& v);
	~Vector3D();

public: // public member functions
	Real GetX() const { return m_x; };
	Real GetY() const { return m_y; };
	Real GetZ() const { return m_z; };
	void SetX(Real x) { m_x = x; };
	void SetY(Real y) { m_y = y; };
	void SetZ(Real z) { m_z = z; };

	Real Length() const;
	Real LengthSquared() const;
	
	void Negate() { m_x = -m_x; m_y = -m_y; m_z = -m_z; }; // creates negation vector
	
	Vector3D operator+(const Vector3D& v) const { return Vector3D(m_x + v.GetX(), m_y + v.GetY(), m_z + v.GetZ()); };
	Vector3D operator-() const { return Vector3D(-m_x, -m_y, -m_z); };
	Vector3D operator-(const Vector3D& v) const { return Vector3D(m_x - v.GetX(), m_y - v.GetY(), m_z - v.GetZ()); };
	Vector3D operator*(Real s) const { return Vector3D(s * m_x, s * m_y, s * m_z); };
	Vector3D operator/(Real s) const { return Vector3D(m_x / s, m_y / s, m_z / s); };

	Vector3D& operator+=(const Vector3D& v);
	Vector3D& operator-=(const Vector3D& v);
	Vector3D& operator*=(const Vector3D& v);
	Vector3D& operator/=(const Vector3D& v);
	Vector3D& operator=(const Vector3D& v);
	bool operator==(const Vector3D& v) const;

	Vector3D Normalized() const;
	void Normalize();

	//Vector3D Rotate(Real angle);
	Vector3D& Rotate(Real angle, const Vector3D& axis);
	
	Real Dot(const Vector3D& v) const;
	Vector3D Cross(const Vector3D& v) const;

public:
	std::string ToString() const;

protected: // member variables
	Real m_x;
	Real m_y;
	Real m_z;
	//Real padding;
}; /* end class Vector3D */

} /* end namespace Math */