#pragma once

#include "Math.h"
#include "Angle.h"
//#include "Utility\ISerializable.h"
#include <string>
#include <sstream>

namespace Math
{
class Quaternion;

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

	Real SumOfComponents() const { return m_x + m_y; }
	Real SumOfAbsoluteComponents() const { return Absolute(m_x) + Absolute(m_y); }

	Real Length() const; // CHECKED!
	Real LengthSquared() const; // CHECKED!
	
	void Negate() { m_x = -m_x; m_y = -m_y; }; // creates negation vector
	
	Vector2D operator+(const Vector2D& v) const { return Vector2D(m_x + v.GetX(), m_y + v.GetY()); };
	Vector2D operator-() const { return Vector2D(-m_x, -m_y); };
	Vector2D operator-(const Vector2D& v) const { return Vector2D(m_x - v.GetX(), m_y - v.GetY()); };
	Vector2D operator*(Real s) const { return Vector2D(s * m_x, s * m_y); };
	Vector2D operator/(Real s) const { return Vector2D(m_x / s, m_y / s); };
	
	Vector2D& operator+=(const Vector2D& v);
	Vector2D& operator-=(const Vector2D& v);
	Vector2D& operator*=(const Vector2D& v);
	Vector2D& operator/=(Real s);
	Vector2D& operator/=(const Vector2D& v);
	Vector2D& operator=(const Vector2D& v);
	bool operator!=(const Vector2D& v) const;
	bool operator==(const Vector2D& v) const;

	Vector2D Normalized() const;
	void Normalize();

	Vector2D Rotate(const Angle& angle);
	
	Real Cross(const Vector2D& v) const; // CHECKED!
	Real Dot(const Vector2D& v) const; // CHECKED!

	Vector2D Lerp(const Vector2D& vec, Real lerpFactor) const; // TODO: Write tests!

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

	Real SumOfComponents() const { return m_x + m_y + m_z; }
	Real SumOfAbsoluteComponents() const { return Absolute(m_x) + Absolute(m_y) + Absolute(m_z); }

	Real Length() const; // CHECKED!
	Real LengthSquared() const; // CHECKED!
	
	void Negate() { m_x = -m_x; m_y = -m_y; m_z = -m_z; }; // creates negation vector
	Vector3D& Negated() { Negate(); return *this; }
	
	Vector3D operator+(const Vector3D& v) const { return Vector3D(m_x + v.GetX(), m_y + v.GetY(), m_z + v.GetZ()); }
	Vector3D operator-() const { return Vector3D(-m_x, -m_y, -m_z); };
	Vector3D operator-(const Vector3D& v) const { return Vector3D(m_x - v.GetX(), m_y - v.GetY(), m_z - v.GetZ()); }
	Vector3D operator*(Real s) const { return Vector3D(s * m_x, s * m_y, s * m_z); };
	Vector3D operator*(const Vector3D& v) const { return Vector3D(m_x * v.GetX(), m_y * v.GetY(), m_z * v.GetZ()); }
	Vector3D operator/(Real s) const { return Vector3D(m_x / s, m_y / s, m_z / s); }; // CHECKED!

	Vector3D& operator+=(const Vector3D& v);
	Vector3D& operator-=(const Vector3D& v);
	Vector3D& operator*=(const Vector3D& v);
	Vector3D& operator/=(Real s);
	Vector3D& operator/=(const Vector3D& v);
	Vector3D& operator=(const Vector3D& v);
	bool operator!=(const Vector3D& v) const;
	bool operator==(const Vector3D& v) const;
	
	/**
	 * Type cast operator created to allow using std::unordered_set in the TerrainMesh::SavePositions function
	 * See the following links:
	 * http://stackoverflow.com/questions/8157937/how-to-specialize-stdhashkeyoperator-for-user-defined-type-in-unordered
	 * http://stackoverflow.com/questions/6585077/size-t-an-operator-and-a-way-to-use-unordered-set
	 * http://stackoverflow.com/questions/13485979/hash-function-of-unordered-set
	 */
	operator size_t () const
	{
		return static_cast<size_t>(m_x * 31 + m_y);
	}

	Vector3D Normalized() const; // CHECKED!
	void Normalize(); // CHECKED!

	//Vector3D Rotate(Real angle);
	Vector3D Rotate(const Vector3D& axis, const Angle& angle);
	Vector3D Rotate(const Quaternion& rotation) const;
	
	Real Dot(const Vector3D& v) const; // CHECKED!
	Vector3D Cross(const Vector3D& v) const;

	Real Max() const;
	Vector3D Max(const Vector3D& v) const;

	// vector swizzling
	Vector2D GetXY() const { return Vector2D(m_x, m_y); };
	Vector2D GetXZ() const { return Vector2D(m_x, m_z); };
	Vector2D GetYZ() const { return Vector2D(m_y, m_z); };
	Vector2D GetYX() const { return Vector2D(m_y, m_x); };
	Vector2D GetZX() const { return Vector2D(m_z, m_x); };
	Vector2D GetZY() const { return Vector2D(m_z, m_y); };

	void ApproachX(Real step, Real approachedValue);
	void ApproachY(Real step, Real approachedValue);
	void ApproachZ(Real step, Real approachedValue);
	void Threshold(Real MaxLength);

	// interpolation LERP
	Vector3D Lerp(const Vector3D& vec, Real lerpFactor) const; // TODO: Write tests!

	std::string ToString() const;

protected: // member variables
	Real m_x;
	Real m_y;
	Real m_z;
	//Real padding;
}; /* end class Vector3D */

} /* end namespace Math */