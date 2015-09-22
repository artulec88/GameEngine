#ifndef __MATH_VECTOR_H__
#define __MATH_VECTOR_H__

#include "Math.h"
#include "Angle.h"
//#include "FloatingPoint.h"
#include <string>
#include <sstream>

namespace Math
{
class Quaternion;

class MATH_API Vector2D
{
/* ==================== Constructors and destructors begin ==================== */
public:
	Vector2D();
	Vector2D(Real x, Real y);
	Vector2D(const Vector2D& v);
	~Vector2D();
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	Real GetX() const { return m_x; }
	Real GetY() const { return m_y; }
	void SetX(Real x) { m_x = x; }
	void SetY(Real y) { m_y = y; }

	Real SumOfComponents() const { return m_x + m_y; }
	Real SumOfAbsoluteComponents() const { return Absolute(m_x) + Absolute(m_y); }

	Real Length() const; // CHECKED!
	Real LengthSquared() const; // CHECKED!
	
	void Negate() { m_x = -m_x; m_y = -m_y; }; // creates negation vector
	
	Vector2D operator+(const Vector2D& v) const { return Vector2D(m_x + v.GetX(), m_y + v.GetY()); }
	Vector2D operator-() const { return Vector2D(-m_x, -m_y); }
	Vector2D operator-(const Vector2D& v) const { return Vector2D(m_x - v.GetX(), m_y - v.GetY()); }
	Vector2D operator*(Real s) const { return Vector2D(s * m_x, s * m_y); }
	Vector2D operator/(Real s) const { return Vector2D(m_x / s, m_y / s); }
	
	Vector2D& operator+=(const Vector2D& v);
	Vector2D& operator-=(const Vector2D& v);
	Vector2D& operator*=(const Vector2D& v);
	Vector2D& operator/=(Real s);
	Vector2D& operator/=(const Vector2D& v);
	Vector2D& operator=(const Vector2D& v);
	bool operator!=(const Vector2D& v) const;
	bool operator==(const Vector2D& v) const;

	Vector2D Normalized() const
	{
		Real length = LengthSquared();
		//if (AlmostEqual(length, REAL_ZERO))
		//{
		//	WARNING_LOG("Trying to normalize the vector with 0 length. 0 length vector is returned.");
		//	return (*this);
		//}
		return (*this) / static_cast<Real>(sqrt(length));
	}
	void Normalize()
	{
		Real length = LengthSquared();
		//if (AlmostEqual(length, REAL_ZERO))
		//{
		//	return;
		//}
		*this /= static_cast<Real>(sqrt(length));
	}
	bool IsNormalized() const;
	
	Real Cross(const Vector2D& v) const
	{
		return m_x * v.GetY() - m_y * v.GetX();
	}

	Real Dot(const Vector2D& v) const
	{
		return (m_x * v.GetX() + m_y * v.GetY());
	}

	Vector2D Rotate(const Angle& angle);

	Vector2D Lerp(const Vector2D& vec, Real lerpFactor) const; // TODO: Write tests!

public:
	std::string ToString() const;
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
protected:
	Real m_x;
	Real m_y;
/* ==================== Non-static member variables end ==================== */
}; /* end class Vector2D */


class MATH_API Vector3D
{
/* ==================== Constructors and destructors begin ==================== */
public:
	Vector3D();
	Vector3D(Real x, Real y, Real z);
	Vector3D(const Vector3D& v);
	~Vector3D();
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	inline Real GetX() const { return m_x; }
	inline Real GetY() const { return m_y; }
	inline Real GetZ() const { return m_z; }
	void SetX(Real x) { m_x = x; }
	void SetY(Real y) { m_y = y; }
	void SetZ(Real z) { m_z = z; }

	Real SumOfComponents() const { return m_x + m_y + m_z; }
	Real SumOfAbsoluteComponents() const { return Absolute(m_x) + Absolute(m_y) + Absolute(m_z); }

	Real Length() const; // CHECKED!
	Real LengthSquared() const; // CHECKED!
	
	/// <summary> Negates all components in the vector. </summary>
	void Negate() { m_x = -m_x; m_y = -m_y; m_z = -m_z; };
	/// <summary> Negates all components and returns the reference to the vector. </summary>
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

	Vector3D Normalized() const
	{
		Vector3D copyVec(*this);
		copyVec.Normalize();
		return copyVec;
	}
	void Normalize()
	{
		Real length = LengthSquared();
//#ifdef _DEBUG
//		if (AlmostEqual(length, REAL_ZERO))
//		{
//			WARNING_LOG("Trying to normalize the vector with 0 length. 0 length vector is returned.");
//			return;
//		}
//#endif
		(*this) /= static_cast<Real>(sqrt(length));
	}
	bool IsNormalized() const;

	//Vector3D Rotate(Real angle);
	Vector3D Rotate(const Vector3D& axis, const Angle& angle);
	Vector3D Rotate(const Quaternion& rotation) const;
	
	Real Dot(const Vector3D& v) const
	{
		return (m_x * v.GetX() + m_y * v.GetY() + m_z * v.GetZ());
	}
	Vector3D Cross(const Vector3D& v) const
	{
		return Vector3D(m_y * v.GetZ() - m_z * v.GetY(),
			m_z * v.GetX() - m_x * v.GetZ(),
			m_x * v.GetY() - m_y * v.GetX());
	}

	Real Max() const;
	Vector3D Max(const Vector3D& v) const;

	// vector swizzling
	Vector2D GetXY() const { return Vector2D(m_x, m_y); }
	Vector2D GetXZ() const { return Vector2D(m_x, m_z); }
	Vector2D GetYZ() const { return Vector2D(m_y, m_z); }
	Vector2D GetYX() const { return Vector2D(m_y, m_x); }
	Vector2D GetZX() const { return Vector2D(m_z, m_x); }
	Vector2D GetZY() const { return Vector2D(m_z, m_y); }

	void Approach(Real step, const Vector3D& approachedVector);
	void ApproachX(Real step, Real approachedValue);
	void ApproachY(Real step, Real approachedValue);
	void ApproachZ(Real step, Real approachedValue);
	void Threshold(Real maxLength);

	// interpolation LERP
	Vector3D Lerp(const Vector3D& vec, Real lerpFactor) const; // TODO: Write tests!

	std::string ToString() const;
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
protected:
	Real m_x;
	Real m_y;
	Real m_z;
	//Real padding;
/* ==================== Non-static member variables end ==================== */
}; /* end class Vector3D */

class MATH_API Vector4D
{
/* ==================== Constructors and destructors begin ==================== */
public:
	Vector4D();
	Vector4D(Real x, Real y, Real z, Real w);
	Vector4D(const Vector4D& v);
	~Vector4D();
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	inline Real GetX() const { return m_x; }
	inline Real GetY() const { return m_y; }
	inline Real GetZ() const { return m_z; }
	inline Real GetW() const { return m_w; }
	void SetX(Real x) { m_x = x; }
	void SetY(Real y) { m_y = y; }
	void SetZ(Real z) { m_z = z; }
	void SetW(Real w) { m_w = w; }

	Real SumOfComponents() const { return m_x + m_y + m_z + m_w; }
	Real SumOfAbsoluteComponents() const { return Absolute(m_x) + Absolute(m_y) + Absolute(m_z) + Absolute(m_w); }

	Real Length() const;
	Real LengthSquared() const;
	
	/// <summary> Negates all components in the vector. </summary>
	void Negate() { m_x = -m_x; m_y = -m_y; m_z = -m_z; m_w = -m_w; };
	/// <summary> Negates all components and returns the reference to the vector. </summary>
	Vector4D& Negated() { Negate(); return *this; }
	
	Vector4D operator+(const Vector4D& v) const { return Vector4D(m_x + v.GetX(), m_y + v.GetY(), m_z + v.GetZ(), m_w + v.GetW()); }
	Vector4D operator-() const { return Vector4D(-m_x, -m_y, -m_z, -m_w); }
	Vector4D operator-(const Vector4D& v) const { return Vector4D(m_x - v.GetX(), m_y - v.GetY(), m_z - v.GetZ(), m_w - v.GetW()); }
	Vector4D operator*(Real s) const { return Vector4D(s * m_x, s * m_y, s * m_z, s * m_w); }
	Vector4D operator*(const Vector4D& v) const { return Vector4D(m_x * v.GetX(), m_y * v.GetY(), m_z * v.GetZ(), m_w * v.GetW()); }
	Vector4D operator/(Real s) const { return Vector4D(m_x / s, m_y / s, m_z / s, m_w / s); }

	Vector4D& operator+=(const Vector4D& v);
	Vector4D& operator-=(const Vector4D& v);
	Vector4D& operator*=(const Vector4D& v);
	Vector4D& operator/=(Real s);
	Vector4D& operator/=(const Vector4D& v);
	Vector4D& operator=(const Vector4D& v);
	bool operator!=(const Vector4D& v) const;
	bool operator==(const Vector4D& v) const;

	Vector4D Normalized() const
	{
		Vector4D copyVec(*this);
		copyVec.Normalize();
		return copyVec;
	}
	void Normalize()
	{
		Real length = LengthSquared();
//#ifdef _DEBUG
//		if (AlmostEqual(length, REAL_ZERO))
//		{
//			WARNING_LOG("Trying to normalize the vector with 0 length. 0 length vector is returned.");
//			return;
//		}
//#endif
		(*this) /= static_cast<Real>(sqrt(length));
	}
	bool IsNormalized() const;
	
	Real Dot(const Vector4D& v) const
	{
		return (m_x * v.GetX() + m_y * v.GetY() + m_z * v.GetZ() + m_w * v.GetW());
	}

	Real Max() const;
	Vector4D Max(const Vector4D& v) const;

	/* ==================== Vector swizzling begin ==================== */
	//Vector2D GetXY() const { return Vector2D(m_x, m_y); }
	//Vector2D GetXZ() const { return Vector2D(m_x, m_z); }
	//Vector2D GetYZ() const { return Vector2D(m_y, m_z); }
	//Vector2D GetYX() const { return Vector2D(m_y, m_x); }
	//Vector2D GetZX() const { return Vector2D(m_z, m_x); }
	//Vector2D GetZY() const { return Vector2D(m_z, m_y); }
	//Vector3D GetXYZ() const { return Vector3D(m_x, m_y, m_z); }
	//Vector3D GetXYW() const { return Vector3D(m_x, m_y, m_w); }
	//Vector3D GetXZY() const { return Vector3D(m_x, m_z, m_y); }
	//Vector3D GetXZW() const { return Vector3D(m_x, m_z, m_w); }
	//Vector3D GetXWY() const { return Vector3D(m_x, m_w, m_y); }
	//Vector3D GetXWZ() const { return Vector3D(m_x, m_w, m_z); }
	//Vector3D GetYXZ() const { return Vector3D(m_y, m_x, m_z); }
	//Vector3D GetYXW() const { return Vector3D(m_y, m_x, m_w); }
	//Vector3D GetYZX() const { return Vector3D(m_y, m_z, m_x); }
	//Vector3D GetYZW() const { return Vector3D(m_y, m_z, m_w); }
	//Vector3D GetYWX() const { return Vector3D(m_y, m_w, m_x); }
	//Vector3D GetYWZ() const { return Vector3D(m_y, m_w, m_z); }
	//Vector3D GetZXY() const { return Vector3D(m_z, m_x, m_y); }
	//Vector3D GetZXW() const { return Vector3D(m_z, m_x, m_w); }
	//Vector3D GetZYX() const { return Vector3D(m_z, m_y, m_x); }
	//Vector3D GetZYW() const { return Vector3D(m_z, m_y, m_w); }
	//Vector3D GetZWX() const { return Vector3D(m_z, m_w, m_x); }
	//Vector3D GetZWY() const { return Vector3D(m_z, m_w, m_y); }
	//Vector3D GetWXY() const { return Vector3D(m_w, m_x, m_y); }
	//Vector3D GetWXZ() const { return Vector3D(m_w, m_x, m_z); }
	//Vector3D GetWYX() const { return Vector3D(m_w, m_y, m_x); }
	//Vector3D GetWYZ() const { return Vector3D(m_w, m_y, m_z); }
	//Vector3D GetWZX() const { return Vector3D(m_w, m_z, m_x); }
	//Vector3D GetWZY() const { return Vector3D(m_w, m_z, m_y); }
	/* ==================== Vector swizzling end ==================== */

	void Approach(Real step, const Vector4D& approachedVector);
	void ApproachX(Real step, Real approachedValue);
	void ApproachY(Real step, Real approachedValue);
	void ApproachZ(Real step, Real approachedValue);
	void ApproachW(Real step, Real approachedValue);
	void Threshold(Real maxLength);

	// interpolation LERP
	Vector4D Lerp(const Vector4D& vec, Real lerpFactor) const; // TODO: Write tests!

	std::string ToString() const;
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
protected:
	Real m_x;
	Real m_y;
	Real m_z;
	Real m_w;
	//Real padding;
/* ==================== Non-static member variables end ==================== */
}; /* end class Vector4D */

} /* end namespace Math */

#endif /* __MATH_VECTOR_H__ */