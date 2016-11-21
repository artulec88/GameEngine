#ifndef __MATH_VECTOR_H__
#define __MATH_VECTOR_H__

#include "Math.h"
#include "Angle.h"
//#include "FloatingPoint.h"
#include <iomanip>

#define PASS_VECTOR_BY_VALUE

namespace Math
{
	class Quaternion;

	/// <summary>
	/// Two dimensional vector representation.
	/// </summary>
	struct Vector2D
	{
		/* ==================== Static variables and functions begin ==================== */
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		/// <summary> Two-dimensional vector default constructor. </summary>
		MATH_API constexpr Vector2D() noexcept:
			x(REAL_ZERO),
			y(REAL_ZERO)
		{
		}

		/// <summary> Two-dimensional vector constructor that initializes all its components to the specified <paramref name="xy"/> value. </summary>
		/// <param name="xy"> The value that will be used to initialize all components of the 2D vector. </param>
		MATH_API constexpr explicit Vector2D(Real xy) noexcept:
			x(xy),
			y(xy)
		{
		}

		/// <summary>
		/// Two-dimensional vector constructor that initializes its X and Y components to the
		/// specified <paramref name="_x"/> and <paramref name="y"/> values respectively.
		/// </summary>
		/// <param name="_x"> The value that will be used to initialize the X component of the 2D vector. </param>
		/// <param name="_y"> The value that will be used to initialize the Y component of the 2D vector. </param>
		MATH_API constexpr Vector2D(Real _x, Real _y) noexcept:
			x(_x),
			y(_y)
		{
		}

		/// <summary>
		/// Two-dimensional vector copy constructor that initializes its X and Y components using the specified 2D vector <paramref name="v"/>.
		/// </summary>
		/// <param name="v"> The reference to the 2D vector that will be used to copy construct new two-dimensional vector. </param>
		MATH_API constexpr Vector2D(const Vector2D& v) noexcept = default;

		/// <summary>
		/// Two-dimensional vector move constructor that initializes its X and Y components using the specified 2D vector <paramref name="v"/>.
		/// </summary>
		/// <param name="v"> The r-value reference to the 2D vector that will be used to move construct new two-dimensional vector. </param>
		MATH_API constexpr Vector2D(Vector2D&& v) noexcept = default;

		/// <summary> Two-dimensional vector destructor. </summary>
		MATH_API ~Vector2D() = default;


		/// <summary>
		/// Two-dimensional vector copy assignment operator that initializes its X and Y components using the specified 2D vector <paramref name="v"/>.
		/// </summary>
		/// <param name="v"> The reference to the 2D vector we will copy assign from. </param>
		/// <returns> The reference to the newly copy-assigned two-dimensional vector. </returns>
		MATH_API Vector2D& operator=(const Vector2D& v) noexcept = default;

		/// <summary>
		/// Two-dimensional vector move assignment operator that initializes its X and Y components using the specified 2D vector <paramref name="v"/>.
		/// </summary>
		/// <param name="v"> The r-value reference to the 2D vector we will move assign from. </param>
		/// <returns> The reference to the newly move-assigned two-dimensional vector. </returns>
		MATH_API Vector2D& operator=(Vector2D&& v) noexcept = default;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		constexpr Real SumOfComponents() const noexcept { return x + y; }
		constexpr Real SumOfAbsoluteComponents() const noexcept { return Absolute(x) + Absolute(y); }

		MATH_API Real Length() const { return sqrt(LengthSquared()); }
		MATH_API constexpr Real LengthSquared() const { return static_cast<Real>(x * x + y * y); }

		/// <summary>
		/// Creates a negation of the vector and returns it. The current vector stays untouched.
		/// </summary>
		constexpr Vector2D Negated() const noexcept
		{
			return Vector2D(-x, -y);
		}
		/// <summary>
		/// Negates all vector components and returns itself.
		/// </summary>
		Vector2D& Negate() noexcept
		{
			x = -x;
			y = -y;
			return *this;
		}

#ifdef PASS_VECTOR_BY_VALUE
		Vector2D operator+(Vector2D v) const
		{
			v.x += x;
			v.y += y;
			return v;
		}
		Vector2D operator-(Vector2D v) const
		{
			v.x = x - v.x;
			v.y = y - v.y;
			return v;
		}
		Vector2D operator*(Vector2D v) const
		{
			v.x *= x;
			v.y *= y;
			return v;
		}
		Vector2D operator/(Vector2D v) const
		{
			v.x = x / v.x;
			v.y = y / v.y;
			return v;
		}
		Vector2D Max(Vector2D v) const;
		Vector2D Lerp(Vector2D vec, Real lerpFactor) const; // TODO: Write tests!
#else
		Vector2D operator+(const Vector2D& v) const { return Math::Vector2D(m_x + v.GetX(), m_y + v.GetY()); }
		Vector2D operator-(const Vector2D& v) const { return Vector2D(m_x - v.GetX(), m_y - v.GetY()); }
		Vector2D operator*(const Vector2D& v) const { return Vector2D(m_x * v.GetX(), m_y * v.GetY()); }
		Vector2D operator/(const Vector2D& v) const { return Vector2D(m_x / v.GetX(), m_y / v.GetY()); }
		Vector2D Max(const Vector2D& v) const;
		Vector2D Lerp(const Vector2D& vec, Real lerpFactor) const; // TODO: Write tests!
#endif
		Vector2D operator-() const { return Vector2D(-x, -y); }
		Vector2D operator*(Real s) const { return Vector2D(s * x, s * y); }
		Vector2D operator/(Real s) const { return Vector2D(x / s, y / s); }

		MATH_API Vector2D& operator+=(const Vector2D& v);
		MATH_API Vector2D& operator-=(const Vector2D& v);
		MATH_API Vector2D& operator*=(Real s);
		MATH_API Vector2D& operator*=(const Vector2D& v);
		MATH_API Vector2D& operator/=(Real s);
		MATH_API Vector2D& operator/=(const Vector2D& v);
		MATH_API bool operator!=(const Vector2D& v) const;
		MATH_API bool operator==(const Vector2D& v) const;

		Vector2D Normalized() const
		{
			//Real length = LengthSquared();
			//if (AlmostEqual(length, REAL_ZERO))
			//{
			//	WARNING_LOG_MATH("Trying to normalize the vector with 0 length. 0 length vector is returned.");
			//	return (*this);
			//}
			// return (*this) / static_cast<Real>(sqrt(length));
			Real length = Length();
			return Vector2D(x / length, y / length);
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
			x /= length;
			y /= length;
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
			return x * v.y - y * v.x;
		}

		Real Dot(const Vector2D& v) const
		{
			return (x * v.x + y * v.y);
		}

		Real Max() const;
		Vector2D Rotate(const Angle& angle);
	public:
		friend std::ostream& operator<<(std::ostream& out, const Vector2D& vector)
		{
			out << std::setprecision(4) << "(x=" << vector.x << "; y=" << vector.x << ")";
			return out;
		}
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	public:
		/// <summary>The X component value of the two-dimensional vector.</summary>
		Real x;
		/// <summary>The Y component value of the two-dimensional vector.</summary>
		Real y;
		// TODO: Read about memory layout. A good way to start is the book "3D Game Engine Architecture" by David H. Eberly (section 2.2.3. "Vectors").
	/* ==================== Non-static member variables end ==================== */
	}; /* end struct Vector2D */


	/// <summary>
	/// Three dimensional vector representation.
	/// </summary>
	class Vector3D
	{
		/* ==================== Static variables and functions begin ==================== */
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		MATH_API constexpr Vector3D() :
			m_x(REAL_ZERO),
			m_y(REAL_ZERO),
			m_z(REAL_ZERO)
		{
		}
		MATH_API constexpr explicit Vector3D(Real xyz) :
			m_x(xyz),
			m_y(xyz),
			m_z(xyz)
		{
		}
		MATH_API constexpr Vector3D(Real x, Real y, Real z) :
			m_x(x),
			m_y(y),
			m_z(z)
		{
		}
		MATH_API constexpr Vector3D(const Vector3D& v) = default;
		MATH_API constexpr Vector3D(Vector3D&& v) = default;
		MATH_API ~Vector3D() = default;

		MATH_API Vector3D& operator=(const Vector3D& v) = default;
		MATH_API Vector3D& operator=(Vector3D&& v) = default;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		constexpr inline Real GetX() const { return m_x; }
		constexpr inline Real GetY() const { return m_y; }
		constexpr inline Real GetZ() const { return m_z; }
		inline void SetX(Real x) { m_x = x; }
		inline void SetY(Real y) { m_y = y; }
		inline void SetZ(Real z) { m_z = z; }
		inline void Set(Real x, Real y, Real z) { m_x = x; m_y = y; m_z = z; }
		inline void Increase(Real x, Real y, Real z) { m_x += x; m_y += y; m_z += z; }
		inline void Increase(const Math::Vector3D& translation) { m_x += translation.GetX(); m_y += translation.GetY(); m_z += translation.GetZ(); }
		inline void IncreaseX(Real x) { m_x += x; }
		inline void IncreaseY(Real y) { m_y += y; }
		inline void IncreaseZ(Real z) { m_z += z; }

		constexpr Real SumOfComponents() const { return m_x + m_y + m_z; }
		constexpr Real SumOfAbsoluteComponents() const { return Absolute(m_x) + Absolute(m_y) + Absolute(m_z); }

		MATH_API Real Length() const { return sqrt(LengthSquared()); }
		MATH_API constexpr Real LengthSquared() const { return static_cast<Real>(m_x * m_x + m_y * m_y + m_z * m_z); }

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

#ifdef PASS_VECTOR_BY_VALUE
		Vector3D operator+(Vector3D v) const { v.Set(m_x + v.GetX(), m_y + v.GetY(), m_z + v.GetZ()); return v; }
		Vector3D operator-(Vector3D v) const { v.Set(m_x - v.GetX(), m_y - v.GetY(), m_z - v.GetZ()); return v; }
		Vector3D operator*(Vector3D v) const { v.Set(m_x * v.GetX(), m_y * v.GetY(), m_z * v.GetZ()); return v; }
		Vector3D operator/(Vector3D v) const { v.Set(m_x / v.GetX(), m_y / v.GetY(), m_z / v.GetZ()); return v; }
		Vector3D Cross(Vector3D v) const
		{
			v.Set(m_y * v.GetZ() - m_z * v.GetY(),
				m_z * v.GetX() - m_x * v.GetZ(),
				m_x * v.GetY() - m_y * v.GetX());
			return v;
		}
		/// <summary>
		/// Calculates linear interpolation between two three-dimensional vectors.
		/// </summary>
		MATH_API Vector3D Lerp(Vector3D vec, Real lerpFactor) const;
		Vector3D Max(Vector3D v) const;
#else
		Vector3D operator+(const Vector3D& v) const { return Vector3D(m_x + v.GetX(), m_y + v.GetY(), m_z + v.GetZ()); }
		Vector3D operator-(const Vector3D& v) const { return Vector3D(m_x - v.GetX(), m_y - v.GetY(), m_z - v.GetZ()); }
		Vector3D operator*(const Vector3D& v) const { return Vector3D(m_x * v.GetX(), m_y * v.GetY(), m_z * v.GetZ()); }
		Vector3D operator/(const Vector3D& v) const { return Vector3D(m_x / v.GetX(), m_y / v.GetY(), m_z / v.GetZ()); }
		Vector3D Cross(const Vector3D& v) const
		{
			return Vector3D(m_y * v.GetZ() - m_z * v.GetY(),
				m_z * v.GetX() - m_x * v.GetZ(),
				m_x * v.GetY() - m_y * v.GetX());
		}
		/// <summary>
		/// Calculates linear interpolation between two three-dimensional vectors.
		/// </summary>
		MATH_API Vector3D Lerp(const Vector3D& vec, Real lerpFactor) const;
		Vector3D Max(const Vector3D& v) const;
#endif
		
		Vector3D operator-() const { return Vector3D(-m_x, -m_y, -m_z); };
		Vector3D operator*(Real s) const { return Vector3D(s * m_x, s * m_y, s * m_z); };
		Vector3D operator/(Real s) const { return Vector3D(m_x / s, m_y / s, m_z / s); };

		MATH_API Vector3D& operator+=(const Vector3D& v);
		MATH_API Vector3D& operator-=(const Vector3D& v);
		MATH_API Vector3D& operator*=(Real s);
		MATH_API Vector3D& operator*=(const Vector3D& v);
		MATH_API Vector3D& operator/=(Real s);
		MATH_API Vector3D& operator/=(const Vector3D& v);
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
			//			WARNING_LOG_MATH("Trying to normalize the vector with 0 length. 0 length vector is returned.");
			//			return;
			//		}
			//#endif
			(*this) /= Length();
			return *this;
		}
		MATH_API bool IsNormalized() const;

		//Vector3D Rotate(Real angle);
		Vector3D Rotate(const Vector3D& axis, const Angle& angle); // TODO: Possible improvement by passing axis by value. Check that!
		MATH_API Vector3D Rotate(const Quaternion& rotation) const;

		Real Dot(const Vector3D& v) const
		{
			return (m_x * v.GetX() + m_y * v.GetY() + m_z * v.GetZ());
		}

		Real Max() const;

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

		friend std::ostream& operator<<(std::ostream& out, const Vector3D& vector)
		{
			out << std::setprecision(4) << "(x=" << vector.m_x << "; y=" << vector.m_y << "; z=" << vector.m_z << ")";
			return out;
		}
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	private:
		Real m_x;
		Real m_y;
		Real m_z;
		//Real padding; // TODO: Read about memory layout. A good way to start is the book "3D Game Engine Architecture" by David H. Eberly (section 2.2.3. "Vectors").
	/* ==================== Non-static member variables end ==================== */
	}; /* end class Vector3D */

	/// <summary>
	/// Four dimensional vector representation.
	/// </summary>
	class Vector4D
	{
		/* ==================== Constructors and destructors begin ==================== */
	public:
		MATH_API constexpr Vector4D() :
			m_x(REAL_ZERO),
			m_y(REAL_ZERO),
			m_z(REAL_ZERO),
			m_w(REAL_ZERO)
		{
		}
		MATH_API constexpr explicit Vector4D(Real xyzw) :
			m_x(xyzw),
			m_y(xyzw),
			m_z(xyzw),
			m_w(xyzw)
		{
		}
		MATH_API constexpr Vector4D(Real x, Real y, Real z, Real w) :
			m_x(x),
			m_y(y),
			m_z(z),
			m_w(w)
		{
		}
		MATH_API constexpr Vector4D(const Vector4D& v) = default;
		MATH_API constexpr Vector4D(Vector4D&& v) = default;
		MATH_API ~Vector4D() = default;

		MATH_API Vector4D& operator=(const Vector4D& v) = default;
		MATH_API Vector4D& operator=(Vector4D&& v) = default;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		constexpr inline Real GetX() const { return m_x; }
		constexpr inline Real GetY() const { return m_y; }
		constexpr inline Real GetZ() const { return m_z; }
		constexpr inline Real GetW() const { return m_w; }
		inline void SetX(Real x) { m_x = x; }
		inline void SetY(Real y) { m_y = y; }
		inline void SetZ(Real z) { m_z = z; }
		inline void SetW(Real w) { m_w = w; }
		inline void Set(Real x, Real y, Real z, Real w) { m_x = x; m_y = y; m_z = z; m_w = w; }
		inline void Increase(Real x, Real y, Real z, Real w) { m_x += x; m_y += y; m_z += z; m_w += w; }
		inline void Increase(const Math::Vector4D& translation) { m_x += translation.GetX(); m_y += translation.GetY(); m_z += translation.GetZ(); m_w += translation.GetW(); }
		inline void IncreaseX(Real x) { m_x += x; }
		inline void IncreaseY(Real y) { m_y += y; }
		inline void IncreaseZ(Real z) { m_z += z; }
		inline void IncreaseW(Real w) { m_w += w; }

		constexpr Real SumOfComponents() const { return m_x + m_y + m_z + m_w; }
		constexpr Real SumOfAbsoluteComponents() const { return Absolute(m_x) + Absolute(m_y) + Absolute(m_z) + Absolute(m_w); }

		MATH_API Real Length() const { return sqrt(LengthSquared()); }
		MATH_API constexpr Real LengthSquared() const { return static_cast<Real>(m_x * m_x + m_y * m_y + m_z * m_z + m_w * m_w); }

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

#ifdef PASS_VECTOR_BY_VALUE
		Vector4D operator+(Vector4D v) const { v.Set(m_x + v.GetX(), m_y + v.GetY(), m_z + v.GetZ(), m_w + v.GetW()); return v; }
		Vector4D operator-(Vector4D v) const { v.Set(m_x - v.GetX(), m_y - v.GetY(), m_z - v.GetZ(), m_w - v.GetW()); return v; }
		Vector4D operator*(Vector4D v) const { v.Set(m_x * v.GetX(), m_y * v.GetY(), m_z * v.GetZ(), m_w * v.GetW()); return v; }
		Vector4D operator/(Vector4D v) const { v.Set(m_x / v.GetX(), m_y / v.GetY(), m_z / v.GetZ(), m_w / v.GetW()); return v; };
		Vector4D Max(Vector4D v) const;
		/// <summary>
		/// Calculates linear interpolation between two four-dimensional vectors.
		/// </summary>
		MATH_API Vector4D Lerp(Vector4D vec, Real lerpFactor) const; // TODO: Write tests!
#else
		Vector4D operator+(const Vector4D& v) const { return Vector4D(m_x + v.GetX(), m_y + v.GetY(), m_z + v.GetZ(), m_w + v.GetW()); }
		Vector4D operator-(const Vector4D& v) const { return Vector4D(m_x - v.GetX(), m_y - v.GetY(), m_z - v.GetZ(), m_w - v.GetW()); }
		Vector4D operator*(const Vector4D& v) const { return Vector4D(m_x * v.GetX(), m_y * v.GetY(), m_z * v.GetZ(), m_w * v.GetW()); }
		Vector4D operator/(const Vector4D& v) const { return Vector4D(m_x / v.GetX(), m_y / v.GetY(), m_z / v.GetZ(), m_w / v.GetW()); };
		Vector4D Max(const Vector4D& v) const;
		/// <summary>
		/// Calculates linear interpolation between two four-dimensional vectors.
		/// </summary>
		MATH_API Vector4D Lerp(const Vector4D& vec, Real lerpFactor) const; // TODO: Write tests!
#endif
		Vector4D operator-() const { return Vector4D(-m_x, -m_y, -m_z, -m_w); }
		Vector4D operator*(Real s) const { return Vector4D(s * m_x, s * m_y, s * m_z, s * m_w); }
		Vector4D operator/(Real s) const { return Vector4D(m_x / s, m_y / s, m_z / s, m_w / s); }

		MATH_API Vector4D& operator+=(const Vector4D& v);
		MATH_API Vector4D& operator-=(const Vector4D& v);
		MATH_API Vector4D& operator*=(Real s);
		MATH_API Vector4D& operator*=(const Vector4D& v);
		MATH_API Vector4D& operator/=(Real s);
		MATH_API Vector4D& operator/=(const Vector4D& v);
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
			//			WARNING_LOG_MATH("Trying to normalize the vector with 0 length. 0 length vector is returned.");
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

		friend std::ostream& operator<<(std::ostream& out, const Vector4D& vector)
		{
			out << std::setprecision(4) << "(x=" << vector.m_x << "; y=" << vector.m_y << "; z=" << vector.m_z << "; w=" << vector.m_w << ")";
			return out;
		}
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

	constexpr Vector2D ZERO_VECTOR_2D{ REAL_ZERO, REAL_ZERO };

} /* end namespace Math */

#endif /* __MATH_VECTOR_H__ */