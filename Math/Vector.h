#ifndef __MATH_VECTOR_H__
#define __MATH_VECTOR_H__

#include "Math.h"
#include "Angle.h"
//#include "FloatingPoint.h"
#include <string> // for ToString() method

namespace Math
{
class Quaternion;

class Vector2D
{
/* ==================== Constructors and destructors begin ==================== */
public:
	MATH_API Vector2D();
	MATH_API explicit Vector2D(Real xy);
	MATH_API Vector2D(Real x, Real y);
	MATH_API Vector2D(const Vector2D& v);
	MATH_API ~Vector2D();
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	inline Real GetX() const { return m_x; }
	inline Real GetY() const { return m_y; }
	inline void SetX(Real x) { m_x = x; }
	inline void SetY(Real y) { m_y = y; }
	inline void Set(Real x, Real y) { m_x = x; m_y = y; }

	Real SumOfComponents() const { return m_x + m_y; }
	Real SumOfAbsoluteComponents() const { return Absolute(m_x) + Absolute(m_y); }

	MATH_API Real Length() const; // CHECKED!
	MATH_API Real LengthSquared() const; // CHECKED!
	
	/// <summary>
	/// Creates a negation of the vector and returns it. The current vector stays untouched.
	/// </summary>
	Vector2D Negated() const
	{
		return Vector2D(-m_x, -m_y);
	}
	/// <summary>
	/// Negates all vector components and returns itself.
	/// </summary>
	Vector2D& Negate()
	{
		m_x = -m_x;
		m_y = -m_y;
		return *this;
	}
	
	Vector2D operator+(const Vector2D& v) const { return Vector2D(m_x + v.GetX(), m_y + v.GetY()); }
	Vector2D operator-() const { return Vector2D(-m_x, -m_y); }
	Vector2D operator-(const Vector2D& v) const { return Vector2D(m_x - v.GetX(), m_y - v.GetY()); }
	Vector2D operator*(Real s) const { return Vector2D(s * m_x, s * m_y); }
	Vector2D operator*(const Vector2D& v) const { return Vector2D(m_x * v.GetX(), m_y * v.GetY()); }
	Vector2D operator/(Real s) const { return Vector2D(m_x / s, m_y / s); }
	Vector2D operator/(const Vector2D& v) const { return Vector2D(m_x / v.GetX(), m_y / v.GetY()); }
	
	MATH_API Vector2D& operator+=(const Vector2D& v);
	MATH_API Vector2D& operator-=(const Vector2D& v);
	MATH_API Vector2D& operator*=(Real s);
	MATH_API Vector2D& operator*=(const Vector2D& v);
	MATH_API Vector2D& operator/=(Real s);
	MATH_API Vector2D& operator/=(const Vector2D& v);
	MATH_API Vector2D& operator=(const Vector2D& v);
	MATH_API bool operator!=(const Vector2D& v) const;
	MATH_API bool operator==(const Vector2D& v) const;

	Vector2D Normalized() const
	{
		//Real length = LengthSquared();
		//if (AlmostEqual(length, REAL_ZERO))
		//{
		//	WARNING_LOG("Trying to normalize the vector with 0 length. 0 length vector is returned.");
		//	return (*this);
		//}
		// return (*this) / static_cast<Real>(sqrt(length));
		Real length = Length();
		return Vector2D(m_x / length, m_y / length);
	}
	Vector2D& Normalize()
	{
		//Real length = LengthSquared();
		//if (AlmostEqual(length, REAL_ZERO))
		//{
		//	return;
		//}
		//*this /= static_cast<Real>(sqrt(length));

		Real length = Length();
		m_x /= length;
		m_y /= length;
		return *this;
	}
	MATH_API bool IsNormalized() const;

	/// <summary>
	/// Given a two-dimensional vector (x, y) the function returns a two-dimensional vector perpendicular to it.
	/// This is done by swapping the components and changing the sign of one of them. <code>Perp((x, y)) = (y, -x)</code>.
	/// </summary>
	Vector2D Perp(bool normalizingEnabled = false) const;
	
	Real Cross(const Vector2D& v) const
	{
		return m_x * v.GetY() - m_y * v.GetX();
	}

	Real Dot(const Vector2D& v) const
	{
		return (m_x * v.GetX() + m_y * v.GetY());
	}

	Real Max() const;
	Vector2D Max(const Vector2D& v) const;
	Vector2D Rotate(const Angle& angle);

	Vector2D Lerp(const Vector2D& vec, Real lerpFactor) const; // TODO: Write tests!

public:
	MATH_API std::string ToString() const;
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
private:
	Real m_x;
	Real m_y;
	// TODO: Read about memory layout. A good way to start is the book "3D Game Engine Architecture" by David H. Eberly (section 2.2.3. "Vectors").
/* ==================== Non-static member variables end ==================== */
}; /* end class Vector2D */


class Vector3D
{
/* ==================== Constructors and destructors begin ==================== */
public:
	MATH_API Vector3D();
	MATH_API explicit Vector3D(Real xyz);
	MATH_API Vector3D(Real x, Real y, Real z);
	MATH_API Vector3D(const Vector3D& v);
	MATH_API Vector3D(Vector3D&& v);
	MATH_API ~Vector3D();
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	inline Real GetX() const { return m_x; }
	inline Real GetY() const { return m_y; }
	inline Real GetZ() const { return m_z; }
	inline void SetX(Real x) { m_x = x; }
	inline void SetY(Real y) { m_y = y; }
	inline void SetZ(Real z) { m_z = z; }
	inline void Set(Real x, Real y, Real z) { m_x = x; m_y = y; m_z = z; }

	Real SumOfComponents() const { return m_x + m_y + m_z; }
	Real SumOfAbsoluteComponents() const { return Absolute(m_x) + Absolute(m_y) + Absolute(m_z); }

	MATH_API Real Length() const; // CHECKED!
	MATH_API Real LengthSquared() const; // CHECKED!
	
	/// <summary>
	/// Creates a negation of the vector and returns it. The current vector stays untouched.
	/// </summary>
	Vector3D Negated() const
	{
		return Vector3D(-m_x, -m_y, -m_z);
	}
	/// <summary>
	/// Negates all vector components and returns itself.
	/// </summary>
	Vector3D& Negate()
	{
		m_x = -m_x;
		m_y = -m_y;
		m_z = -m_z;
		return *this;
	}
	
	Vector3D operator+(const Vector3D& v) const { return Vector3D(m_x + v.GetX(), m_y + v.GetY(), m_z + v.GetZ()); }
	Vector3D operator-() const { return Vector3D(-m_x, -m_y, -m_z); };
	Vector3D operator-(const Vector3D& v) const { return Vector3D(m_x - v.GetX(), m_y - v.GetY(), m_z - v.GetZ()); }
	Vector3D operator*(Real s) const { return Vector3D(s * m_x, s * m_y, s * m_z); };
	Vector3D operator*(const Vector3D& v) const { return Vector3D(m_x * v.GetX(), m_y * v.GetY(), m_z * v.GetZ()); }
	Vector3D operator/(Real s) const { return Vector3D(m_x / s, m_y / s, m_z / s); };
	Vector3D operator/(const Vector3D v) const { return Vector3D(m_x / v.GetX(), m_y / v.GetY(), m_z / v.GetZ()); };

	MATH_API Vector3D& operator+=(const Vector3D& v);
	MATH_API Vector3D& operator-=(const Vector3D& v);
	MATH_API Vector3D& operator*=(Real s);
	MATH_API Vector3D& operator*=(const Vector3D& v);
	MATH_API Vector3D& operator/=(Real s);
	MATH_API Vector3D& operator/=(const Vector3D& v);
	MATH_API Vector3D& operator=(Vector3D v);
	MATH_API bool operator!=(const Vector3D& v) const;
	MATH_API bool operator==(const Vector3D& v) const;
	
	/**
	 * Type cast operator created to allow using std::unordered_set in the TerrainMesh::SavePositions function
	 * See the following links:
	 * http://stackoverflow.com/questions/8157937/how-to-specialize-stdhashkeyoperator-for-user-defined-type-in-unordered
	 * http://stackoverflow.com/questions/6585077/size-t-an-operator-and-a-way-to-use-unordered-set
	 * http://stackoverflow.com/questions/13485979/hash-function-of-unordered-set
	 */
	std::size_t to_size_t() const
	{
		return static_cast<std::size_t>(m_x * 31 + m_y * 17 + m_z);
	}

	Vector3D Normalized() const
	{
		Real length = Length();
		return Vector3D(m_x / length, m_y / length, m_z / length);
	}
	Vector3D& Normalize()
	{
//#ifdef _DEBUG
//		Real length = LengthSquared();
//		if (AlmostEqual(length, REAL_ZERO))
//		{
//			WARNING_LOG("Trying to normalize the vector with 0 length. 0 length vector is returned.");
//			return;
//		}
//#endif
		(*this) /= Length();
		return *this;
	}
	MATH_API bool IsNormalized() const;

	//Vector3D Rotate(Real angle);
	Vector3D Rotate(const Vector3D& axis, const Angle& angle);
	MATH_API Vector3D Rotate(const Quaternion& rotation) const;
	
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
	MATH_API void ApproachX(Real step, Real approachedValue);
	MATH_API void ApproachY(Real step, Real approachedValue);
	MATH_API void ApproachZ(Real step, Real approachedValue);
	MATH_API void Threshold(Real maxLength);

	// interpolation LERP
	MATH_API Vector3D Lerp(const Vector3D& vec, Real lerpFactor) const; // TODO: Write tests!

	MATH_API std::string ToString() const;
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
private:
	Real m_x;
	Real m_y;
	Real m_z;
	//Real padding; // TODO: Read about memory layout. A good way to start is the book "3D Game Engine Architecture" by David H. Eberly (section 2.2.3. "Vectors").
/* ==================== Non-static member variables end ==================== */
}; /* end class Vector3D */

class Vector4D
{
/* ==================== Constructors and destructors begin ==================== */
public:
	MATH_API Vector4D();
	MATH_API explicit Vector4D(Real xyzw);
	MATH_API Vector4D(Real x, Real y, Real z, Real w);
	MATH_API Vector4D(const Vector4D& v);
	MATH_API ~Vector4D();
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	inline Real GetX() const { return m_x; }
	inline Real GetY() const { return m_y; }
	inline Real GetZ() const { return m_z; }
	inline Real GetW() const { return m_w; }
	inline void SetX(Real x) { m_x = x; }
	inline void SetY(Real y) { m_y = y; }
	inline void SetZ(Real z) { m_z = z; }
	inline void SetW(Real w) { m_w = w; }
	inline void Set(Real x, Real y, Real z, Real w) { m_x = x; m_y = y; m_z = z; m_w = w; }

	Real SumOfComponents() const { return m_x + m_y + m_z + m_w; }
	Real SumOfAbsoluteComponents() const { return Absolute(m_x) + Absolute(m_y) + Absolute(m_z) + Absolute(m_w); }

	MATH_API Real Length() const;
	MATH_API Real LengthSquared() const;
	
	/// <summary>
	/// Creates a negation of the vector and returns it. The current vector stays untouched.
	/// </summary>
	Vector4D Negated() const
	{
		return Vector4D(-m_x, -m_y, -m_z, -m_w);
	}
	/// <summary>
	/// Negates all vector components and returns itself.
	/// </summary>
	Vector4D& Negate()
	{
		m_x = -m_x;
		m_y = -m_y;
		m_z = -m_z;
		m_w = -m_w;
		return *this;
	}
	
	Vector4D operator+(const Vector4D& v) const { return Vector4D(m_x + v.GetX(), m_y + v.GetY(), m_z + v.GetZ(), m_w + v.GetW()); }
	Vector4D operator-() const { return Vector4D(-m_x, -m_y, -m_z, -m_w); }
	Vector4D operator-(const Vector4D& v) const { return Vector4D(m_x - v.GetX(), m_y - v.GetY(), m_z - v.GetZ(), m_w - v.GetW()); }
	Vector4D operator*(Real s) const { return Vector4D(s * m_x, s * m_y, s * m_z, s * m_w); }
	Vector4D operator*(const Vector4D& v) const { return Vector4D(m_x * v.GetX(), m_y * v.GetY(), m_z * v.GetZ(), m_w * v.GetW()); }
	Vector4D operator/(Real s) const { return Vector4D(m_x / s, m_y / s, m_z / s, m_w / s); }
	Vector4D operator/(const Vector4D v) const { return Vector4D(m_x / v.GetX(), m_y / v.GetY(), m_z / v.GetZ(), m_w / v.GetW()); };

	MATH_API Vector4D& operator+=(const Vector4D& v);
	MATH_API Vector4D& operator-=(const Vector4D& v);
	MATH_API Vector4D& operator*=(Real s);
	MATH_API Vector4D& operator*=(const Vector4D& v);
	MATH_API Vector4D& operator/=(Real s);
	MATH_API Vector4D& operator/=(const Vector4D& v);
	MATH_API Vector4D& operator=(const Vector4D& v);
	MATH_API bool operator!=(const Vector4D& v) const;
	MATH_API bool operator==(const Vector4D& v) const;

	Vector4D Normalized() const
	{
		Vector4D copyVec(*this);
		copyVec.Normalize();
		return copyVec;
	}
	Vector4D& Normalize()
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
		return *this;
	}
	MATH_API bool IsNormalized() const;
	
	Real Dot(const Vector4D& v) const
	{
		return (m_x * v.GetX() + m_y * v.GetY() + m_z * v.GetZ() + m_w * v.GetW());
	}

	Real Max() const;
	Vector4D Max(const Vector4D& v) const;

	/* ==================== Vector swizzling begin ==================== */
	MATH_API Vector2D GetXY() const { return Vector2D(m_x, m_y); }
	Vector2D GetXZ() const { return Vector2D(m_x, m_z); }
	Vector2D GetYZ() const { return Vector2D(m_y, m_z); }
	Vector2D GetYX() const { return Vector2D(m_y, m_x); }
	Vector2D GetZX() const { return Vector2D(m_z, m_x); }
	Vector2D GetZY() const { return Vector2D(m_z, m_y); }
	MATH_API Vector3D GetXYZ() const { return Vector3D(m_x, m_y, m_z); }
	Vector3D GetXYW() const { return Vector3D(m_x, m_y, m_w); }
	Vector3D GetXZY() const { return Vector3D(m_x, m_z, m_y); }
	Vector3D GetXZW() const { return Vector3D(m_x, m_z, m_w); }
	Vector3D GetXWY() const { return Vector3D(m_x, m_w, m_y); }
	Vector3D GetXWZ() const { return Vector3D(m_x, m_w, m_z); }
	Vector3D GetYXZ() const { return Vector3D(m_y, m_x, m_z); }
	Vector3D GetYXW() const { return Vector3D(m_y, m_x, m_w); }
	Vector3D GetYZX() const { return Vector3D(m_y, m_z, m_x); }
	Vector3D GetYZW() const { return Vector3D(m_y, m_z, m_w); }
	Vector3D GetYWX() const { return Vector3D(m_y, m_w, m_x); }
	Vector3D GetYWZ() const { return Vector3D(m_y, m_w, m_z); }
	Vector3D GetZXY() const { return Vector3D(m_z, m_x, m_y); }
	Vector3D GetZXW() const { return Vector3D(m_z, m_x, m_w); }
	Vector3D GetZYX() const { return Vector3D(m_z, m_y, m_x); }
	Vector3D GetZYW() const { return Vector3D(m_z, m_y, m_w); }
	Vector3D GetZWX() const { return Vector3D(m_z, m_w, m_x); }
	Vector3D GetZWY() const { return Vector3D(m_z, m_w, m_y); }
	Vector3D GetWXY() const { return Vector3D(m_w, m_x, m_y); }
	Vector3D GetWXZ() const { return Vector3D(m_w, m_x, m_z); }
	Vector3D GetWYX() const { return Vector3D(m_w, m_y, m_x); }
	Vector3D GetWYZ() const { return Vector3D(m_w, m_y, m_z); }
	Vector3D GetWZX() const { return Vector3D(m_w, m_z, m_x); }
	Vector3D GetWZY() const { return Vector3D(m_w, m_z, m_y); }
	/* ==================== Vector swizzling end ==================== */

	void Approach(Real step, const Vector4D& approachedVector);
	void ApproachX(Real step, Real approachedValue);
	void ApproachY(Real step, Real approachedValue);
	void ApproachZ(Real step, Real approachedValue);
	void ApproachW(Real step, Real approachedValue);
	void Threshold(Real maxLength);

	// interpolation LERP
	Vector4D Lerp(const Vector4D& vec, Real lerpFactor) const; // TODO: Write tests!

	MATH_API std::string ToString() const;
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
private:
	Real m_x;
	Real m_y;
	Real m_z;
	Real m_w;
	//Real padding; // TODO: Read about memory layout. A good way to start is the book "3D Game Engine Architecture" by David H. Eberly (section 2.2.3. "Vectors").
/* ==================== Non-static member variables end ==================== */
}; /* end class Vector4D */

namespace Interpolation
{
	MATH_API Math::Real BarycentricInterpolation(const Math::Vector3D& pos1, const Math::Vector3D& pos2, const Math::Vector3D& pos3, Math::Real xPos, Math::Real zPos);
	MATH_API Math::Real BarycentricInterpolation(Math::Real xPos1, Math::Real yPos1, Math::Real zPos1,
		Math::Real xPos2, Math::Real yPos2, Math::Real zPos2,
		Math::Real xPos3, Math::Real yPos3, Math::Real zPos3,
		Math::Real xPos, Math::Real zPos);
} /* end namespace Interpolation */
} /* end namespace Math */

#endif /* __MATH_VECTOR_H__ */