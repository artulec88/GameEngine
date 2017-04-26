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
		/// <summary>
		/// Two-dimensional vector default constructor. It initializes the X and Y components to value <code>0.0</code>.
		/// </summary>
		MATH_API constexpr Vector2D() noexcept:
			Vector2D(REAL_ZERO)
		{
		}

		/// <summary> Two-dimensional vector constructor that initializes all its components to the specified <paramref name="xy"/> value. </summary>
		/// <param name="xy"> The value that will be used to initialize all components of the 2D vector. </param>
		MATH_API constexpr explicit Vector2D(Real xy) noexcept:
			Vector2D(xy, xy)
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
		/// <summary>
		/// Sets the values of the X and Y components.
		/// </summary>
		/// <param name="_x"> The new value for the X component of the vector. </param>
		/// <param name="_y"> The new value for the Y component of the vector. </param>
		inline void Set(Real _x, Real _y) noexcept { x = _x; y = _y; }

		/// <summary>
		/// Increases the values of the X and Y components.
		/// </summary>
		/// <param name="_x"> The increase of the X component of the vector. </param>
		/// <param name="_y"> The increase of the Y component of the vector. </param>
		inline void Increase(Real _x, Real _y) noexcept { x += _x; y += _y; }

		/// <summary>
		/// Increases the values of the X and Y components.
		/// </summary>
		/// <param name="translation"> The 2D vector representing the translation of the current vector. </param>
		inline void Increase(const Math::Vector2D& translation) noexcept { operator+=(translation); }

		/// <summary>
		/// Resets the values of both X and Y components of the vector.
		/// </summary>
		inline void Zero() noexcept { Set(REAL_ZERO, REAL_ZERO); }

		/// <summary> Calculates and returns the sum of components of the vector. </summary>
		constexpr Real SumOfComponents() const noexcept { return x + y; }

		/// <summary> Calculates and returns the sum of vector components' absolute values. </summary>
		constexpr Real SumOfAbsoluteComponents() const noexcept { return Absolute(x) + Absolute(y); }

		/// <summary> Calculates and returns the length of the vector. </summary>
		MATH_API Real Length() const { return sqrt(LengthSquared()); }

		/// <summary>
		/// Calculates and returns the squared length of the vector.
		/// It is advisable to use this function instead of <code>Length()</code> because it is less time-consuming.
		/// </summary>
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
		constexpr Vector2D operator+(const Vector2D& v) const { return Vector2D(m_x + v.GetX(), m_y + v.GetY()); }
		constexpr Vector2D operator-(const Vector2D& v) const { return Vector2D(m_x - v.GetX(), m_y - v.GetY()); }
		constexpr Vector2D operator*(const Vector2D& v) const { return Vector2D(m_x * v.GetX(), m_y * v.GetY()); }
		constexpr Vector2D operator/(const Vector2D& v) const { return Vector2D(m_x / v.GetX(), m_y / v.GetY()); }
		Vector2D Max(const Vector2D& v) const;
		Vector2D Lerp(const Vector2D& vec, Real lerpFactor) const; // TODO: Write tests!
#endif
		constexpr Vector2D operator-() const { return Vector2D(-x, -y); }
		constexpr Vector2D operator*(Real s) const { return Vector2D(s * x, s * y); }
		constexpr Vector2D operator/(Real s) const { return Vector2D(x / s, y / s); }

		MATH_API Vector2D& operator+=(const Vector2D& v) noexcept;
		MATH_API Vector2D& operator-=(const Vector2D& v) noexcept;
		MATH_API Vector2D& operator*=(Real s) noexcept;
		MATH_API Vector2D& operator*=(const Vector2D& v) noexcept;
		MATH_API Vector2D& operator/=(Real s) noexcept;
		MATH_API Vector2D& operator/=(const Vector2D& v) noexcept;
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
			const Real length = Length();
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

		/// <summary>
		/// Gives information about whether or not the vector is normalized or not (i.e. its length is equal to <code>1</code> or not).
		/// </summary>
		/// <returns> <code>true</code> when vector's length is equal to <code>1</code> and <code>false</code> otherwise. </returns>
		MATH_API bool IsNormalized() const;

		/// <summary>
		/// Given a two-dimensional vector [x, y] the function returns a two-dimensional vector perpendicular to it.
		/// This is done by swapping the components and changing the sign of one of them. <code>Perp([x, y]) = [y, -x]</code>.
		/// </summary>
		/// <param name="isNormalizingEnabled">
		/// A simple <code>bool</code> flag indicating whether the result should be normalized (<code>true</code>) or not (<code>false</code>).
		/// </param>
		/// <returns> The two-dimensional vector being perpendicular to the current vector. </returns>
		constexpr Vector2D Perp(bool isNormalizingEnabled = false) const
		{
			return (isNormalizingEnabled) ? Vector2D(y, -x).Normalized() : Vector2D(y, -x);
		}

		/// <summary>
		/// Calculates the cross product of the current vector with the specified <paramref name="v"/>.
		/// </summary>
		/// <param name="v"> The two-dimensional vector to be used to compute the cross product. </param>
		/// <returns> The cross product of the current vector with the given <paramref name="v"/>. </returns>
		constexpr Real Cross(const Vector2D& v) const noexcept
		{
			return x * v.y - y * v.x;
		}

		/// <summary> Calculates the dot product of the current vector with the specified <paramref name="v"/>. </summary>
		/// <param name="v"> The two-dimensional vector to be used to compute the dot product. </param>
		/// <returns> The dot product of the current vector with the given <paramref name="v"/>. </returns>
		constexpr Real Dot(const Vector2D& v) const noexcept
		{
			return (x * v.x + y * v.y);
		}

		/// <summary> Finds and returns the component that is the greatest. </summary>
		/// <returns> The greatest component of the current two-dimensional vector. </returns>
		constexpr Real Max() const noexcept
		{
			return (x > y) ? x : y;
		}

		/// <summary> Rotates the vector by a given <paramref name="angle"/>. </summary>
		/// <param name="angle"> The angle to rotate the vector by. </param>
		/// <returns> The two-dimensional vector of the result of rotation. </returns>
		Vector2D Rotate(const Angle& angle);
	public:
		friend std::ostream& operator<<(std::ostream& out, const Vector2D& vector)
		{
			out << std::setprecision(4) << "(x=" << vector.x << "; y=" << vector.y << ")";
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
	struct Vector3D
	{
		/* ==================== Static variables and functions begin ==================== */
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		/// <summary>
		/// Three-dimensional vector default constructor. It initializes the X, Y and Z components to value <code>0.0</code>.
		/// </summary>
		MATH_API constexpr Vector3D() noexcept:
			Vector3D(REAL_ZERO)
		{
		}

		/// <summary>
		/// Three-dimensional vector constructor. It initializes all vector components with the specified <paramref name="xyz"/> value.
		/// </summary>
		/// <param name="xyz"> The value that each component of the vector will be initialized with. </param>
		MATH_API constexpr explicit Vector3D(Real xyz) noexcept :
			Vector3D(xyz, xyz, xyz)
		{
		}

		/// <summary>
		/// Three-dimensional vector constructor. It initializes X, Y and Z components of the vector with the given <paramref name="_x"/>,
		/// <paramref name="_y"/> and <paramref name="_z"/> values.
		/// </summary>
		/// <param name="_x"> The value X component of the vector will be initialized with. </param>
		/// <param name="_y"> The value Y component of the vector will be initialized with. </param>
		/// <param name="_z"> The value Z component of the vector will be initialized with. </param>
		MATH_API constexpr Vector3D(Real _x, Real _y, Real _z) noexcept :
			x(_x),
			y(_y),
			z(_z)
		{
		}

		/// <summary>
		/// Three-dimensional vector copy constructor.
		/// </summary>
		/// <param name="v"> The 3D vector that will be used to copy construct a new vector. </param>
		MATH_API constexpr Vector3D(const Vector3D& v) noexcept = default;

		/// <summary>
		/// Three-dimensional vector move constructor.
		/// </summary>
		/// <param name="v"> The r-value reference of the 3D vector that will be used to move construct a new vector. </param>
		MATH_API constexpr Vector3D(Vector3D&& v) noexcept = default;

		/// <summary>
		/// Three-dimensional vector destructor.
		/// </summary>
		MATH_API ~Vector3D() = default;

		/// <summary>
		/// Three-dimensional vector copy assignment operator.
		/// </summary>
		/// <param name="v"> The 3D vector that we will copy assign from. </param>
		/// <returns> The reference to the newly copy-assigned 3D vector. </returns>
		MATH_API Vector3D& operator=(const Vector3D& v) noexcept = default;

		/// <summary>
		/// Three-dimensional vector move assignment operator.
		/// </summary>
		/// <param name="v"> The r-value reference of the 3D vector that we will move assign from. </param>
		/// <returns> The reference to the newly move-assigned 3D vector. </returns>
		MATH_API Vector3D& operator=(Vector3D&& v) noexcept = default;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		inline void Set(Real _x, Real _y, Real _z) noexcept { x = _x; y = _y; z = _z; }
		inline void Increase(Real _x, Real _y, Real _z) noexcept { x += _x; y += _y; z += _z; }
		inline void Increase(const Math::Vector3D& translation) noexcept { operator+=(translation); }
		inline void Zero() noexcept { Set(REAL_ZERO, REAL_ZERO, REAL_ZERO); }

		constexpr Real SumOfComponents() const noexcept { return x + y + z; }
		constexpr Real SumOfAbsoluteComponents() const noexcept { return Absolute(x) + Absolute(y) + Absolute(z); }

		MATH_API Real Length() const { return sqrt(LengthSquared()); }
		MATH_API constexpr Real LengthSquared() const { return static_cast<Real>(x * x + y * y + z * z); }

		/// <summary>
		/// Creates a negation of the vector and returns it. The current vector stays untouched.
		/// </summary>
		Vector3D Negated() const
		{
			return Vector3D(-x, -y, -z);
		}
		/// <summary>
		/// Negates all vector components and returns itself.
		/// </summary>
		Vector3D& Negate()
		{
			x = -x;
			y = -y;
			z = -z;
			return *this;
		}

#ifdef PASS_VECTOR_BY_VALUE
		Vector3D operator+(Vector3D v) const
		{
			v.x += x;
			v.y += y;
			v.z += z;
			return v;
		}
		Vector3D operator-(Vector3D v) const
		{
			v.x = x - v.x;
			v.y = y - v.y;
			v.z = z - v.z;
			return v;
		}
		Vector3D operator*(Vector3D v) const
		{
			v.x *= x;
			v.y *= y;
			v.z *= z;
			return v;
		}
		Vector3D operator/(Vector3D v) const
		{
			v.x = x / v.x;
			v.y = y / v.y;
			v.z = z / v.z;
			return v;
		}
		/// <summary>
		/// Calculates and returns the cross product of the two vectors (the current one and the given <paramref name="v"/> vector).
		/// The magnitude of the cross product of two vectors <code>a</code> and <code>b</code> is equal to the area of the parallelogram
		/// formed on two sides by <code>a</code> and <code>b</code>.
		/// </summary>
		/// <param name="v"> The vector for cross product calculation. </param>
		/// <returns> The cross product of the current vector and the specified vector <paramref name="v"/>. </returns>
		Vector3D Cross(Vector3D v) const
		{
			v.x = y * v.z - z * v.y;
			v.y = z * v.x - x * v.z;
			v.z = x * v.y - y * v.x;
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
		
		Vector3D operator-() const { return Vector3D(-x, -y, -z); };
		Vector3D operator*(Real s) const { return Vector3D(s * x, s * y, s * z); };
		Vector3D operator/(Real s) const { return Vector3D(x / s, y / s, z / s); };

		MATH_API Vector3D& operator+=(const Vector3D& v) noexcept;
		MATH_API Vector3D& operator-=(const Vector3D& v) noexcept;
		MATH_API Vector3D& operator*=(Real s) noexcept;
		MATH_API Vector3D& operator*=(const Vector3D& v) noexcept;
		MATH_API Vector3D& operator/=(Real s) noexcept;
		MATH_API Vector3D& operator/=(const Vector3D& v) noexcept;
		MATH_API bool operator!=(const Vector3D& v) const;
		MATH_API bool operator==(const Vector3D& v) const;

		Vector3D Normalized() const
		{
			Real length = Length();
			return Vector3D(x / length, y / length, z / length);
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
			return (x * v.x + y * v.y + z * v.z);
		}

		Real Max() const;

		// vector swizzling
		Vector2D GetXY() const { return Vector2D(x, y); }
		Vector2D GetXZ() const { return Vector2D(x, z); }
		Vector2D GetYZ() const { return Vector2D(y, z); }
		Vector2D GetYX() const { return Vector2D(y, x); }
		Vector2D GetZX() const { return Vector2D(z, x); }
		Vector2D GetZY() const { return Vector2D(z, y); }

		void Approach(Real step, const Vector3D& approachedVector);
		MATH_API void ApproachX(Real step, Real approachedValue);
		MATH_API void ApproachY(Real step, Real approachedValue);
		MATH_API void ApproachZ(Real step, Real approachedValue);
		MATH_API void Threshold(Real maxLength);

		friend std::ostream& operator<<(std::ostream& out, const Vector3D& vector)
		{
			out << std::setprecision(4) << "(x=" << vector.x << "; y=" << vector.y << "; z=" << vector.z << ")";
			return out;
		}
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	public:
		Real x;
		Real y;
		Real z;
		//Real padding; // TODO: Read about memory layout. A good way to start is the book "3D Game Engine Architecture" by David H. Eberly (section 2.2.3. "Vectors").
	/* ==================== Non-static member variables end ==================== */
	}; /* end struct Vector3D */

	/// <summary>
	/// Four dimensional vector representation.
	/// </summary>
	struct Vector4D
	{
		/* ==================== Constructors and destructors begin ==================== */
	public:
		MATH_API constexpr Vector4D() noexcept :
			Vector4D(REAL_ZERO)
		{
		}
		MATH_API constexpr explicit Vector4D(Real xyzw) noexcept :
			Vector4D(xyzw, xyzw, xyzw, xyzw)
		{
		}
		MATH_API constexpr Vector4D(Real _x, Real _y, Real _z, Real _w) noexcept :
			x(_x),
			y(_y),
			z(_z),
			w(_w)
		{
		}
		MATH_API constexpr Vector4D(const Vector4D& v) noexcept = default;
		MATH_API constexpr Vector4D(Vector4D&& v) noexcept = default;
		MATH_API ~Vector4D() = default;

		MATH_API Vector4D& operator=(const Vector4D& v) noexcept = default;
		MATH_API Vector4D& operator=(Vector4D&& v) noexcept = default;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		inline void Set(Real _x, Real _y, Real _z, Real _w) noexcept { x = _x; y = _y; z = _z; w = _w; }
		inline void Increase(Real _x, Real _y, Real _z, Real _w) noexcept { x += _x; y += _y; z += _z; w += _w; }
		inline void Increase(const Math::Vector4D& translation) noexcept { operator+=(translation); }
		inline void Zero() noexcept { Set(REAL_ZERO, REAL_ZERO, REAL_ZERO, REAL_ZERO); }
		
		constexpr Real SumOfComponents() const noexcept { return x + y + z + w; }
		constexpr Real SumOfAbsoluteComponents() const noexcept { return Absolute(x) + Absolute(y) + Absolute(z) + Absolute(w); }

		MATH_API Real Length() const { return sqrt(LengthSquared()); }
		MATH_API constexpr Real LengthSquared() const { return static_cast<Real>(x * x + y * y + z * z + w * w); }

		/// <summary>
		/// Creates a negation of the vector and returns it. The current vector stays untouched.
		/// </summary>
		Vector4D Negated() const
		{
			return Vector4D(-x, -y, -z, -w);
		}
		/// <summary>
		/// Negates all vector components and returns itself.
		/// </summary>
		Vector4D& Negate()
		{
			x = -x;
			y = -y;
			z = -z;
			w = -w;
			return *this;
		}

#ifdef PASS_VECTOR_BY_VALUE
		Vector4D operator+(Vector4D v) const
		{
			v.x += x;
			v.y += y;
			v.z += z;
			v.w += w;
			return v;
		}
		Vector4D operator-(Vector4D v) const
		{
			v.x = x - v.x;
			v.y = y - v.y;
			v.z = z - v.z;
			v.w = w - v.w;
			return v;
		}
		Vector4D operator*(Vector4D v) const
		{
			v.x *= x;
			v.y *= y;
			v.z *= z;
			v.w *= w;
			return v;
		}
		Vector4D operator/(Vector4D v) const
		{
			v.x = x / v.x;
			v.y = y / v.y;
			v.z = z / v.z;
			v.w = w / v.w;
			return v;
		}
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
		Vector4D operator-() const { return Vector4D(-x, -y, -z, -w); }
		Vector4D operator*(Real s) const { return Vector4D(s * x, s * y, s * z, s * w); }
		Vector4D operator/(Real s) const { return Vector4D(x / s, y / s, z / s, w / s); }

		MATH_API Vector4D& operator+=(const Vector4D& v) noexcept;
		MATH_API Vector4D& operator-=(const Vector4D& v) noexcept;
		MATH_API Vector4D& operator*=(Real s) noexcept;
		MATH_API Vector4D& operator*=(const Vector4D& v) noexcept;
		MATH_API Vector4D& operator/=(Real s) noexcept;
		MATH_API Vector4D& operator/=(const Vector4D& v) noexcept;
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
			return (x * v.x + y * v.y + z * v.z + w * v.w);
		}

		Real Max() const;

		/* ==================== Vector swizzling begin ==================== */
		MATH_API Vector2D GetXY() const { return Vector2D(x, y); }
		Vector2D GetXZ() const { return Vector2D(x, z); }
		Vector2D GetYZ() const { return Vector2D(y, z); }
		Vector2D GetYX() const { return Vector2D(y, x); }
		Vector2D GetZX() const { return Vector2D(z, x); }
		Vector2D GetZY() const { return Vector2D(z, y); }
		MATH_API Vector3D GetXYZ() const { return Vector3D(x, y, z); }
		Vector3D GetXYW() const { return Vector3D(x, y, w); }
		Vector3D GetXZY() const { return Vector3D(x, z, y); }
		Vector3D GetXZW() const { return Vector3D(x, z, w); }
		Vector3D GetXWY() const { return Vector3D(x, w, y); }
		Vector3D GetXWZ() const { return Vector3D(x, w, z); }
		Vector3D GetYXZ() const { return Vector3D(y, x, z); }
		Vector3D GetYXW() const { return Vector3D(y, x, w); }
		Vector3D GetYZX() const { return Vector3D(y, z, x); }
		Vector3D GetYZW() const { return Vector3D(y, z, w); }
		Vector3D GetYWX() const { return Vector3D(y, w, x); }
		Vector3D GetYWZ() const { return Vector3D(y, w, z); }
		Vector3D GetZXY() const { return Vector3D(z, x, y); }
		Vector3D GetZXW() const { return Vector3D(z, x, w); }
		Vector3D GetZYX() const { return Vector3D(z, y, x); }
		Vector3D GetZYW() const { return Vector3D(z, y, w); }
		Vector3D GetZWX() const { return Vector3D(z, w, x); }
		Vector3D GetZWY() const { return Vector3D(z, w, y); }
		Vector3D GetWXY() const { return Vector3D(w, x, y); }
		Vector3D GetWXZ() const { return Vector3D(w, x, z); }
		Vector3D GetWYX() const { return Vector3D(w, y, x); }
		Vector3D GetWYZ() const { return Vector3D(w, y, z); }
		Vector3D GetWZX() const { return Vector3D(w, z, x); }
		Vector3D GetWZY() const { return Vector3D(w, z, y); }
		/* ==================== Vector swizzling end ==================== */

		void Approach(Real step, const Vector4D& approachedVector);
		void ApproachX(Real step, Real approachedValue);
		void ApproachY(Real step, Real approachedValue);
		void ApproachZ(Real step, Real approachedValue);
		void ApproachW(Real step, Real approachedValue);
		void Threshold(Real maxLength);

		friend std::ostream& operator<<(std::ostream& out, const Vector4D& vector)
		{
			out << std::setprecision(4) << "(x=" << vector.x << "; y=" << vector.y << "; z=" << vector.z << "; w=" << vector.w << ")";
			return out;
		}
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	public:
		Real x;
		Real y;
		Real z;
		Real w;
		//Real padding; // TODO: Read about memory layout. A good way to start is the book "3D Game Engine Architecture" by David H. Eberly (section 2.2.3. "Vectors").
	/* ==================== Non-static member variables end ==================== */
	}; /* end struct Vector4D */

	namespace Interpolation
	{
		MATH_API Math::Real BarycentricInterpolation(const Vector3D& pos1, const Vector3D& pos2, const Vector3D& pos3, Real xPos, Real zPos);
		MATH_API Math::Real BarycentricInterpolation(Real xPos1, Real yPos1, Real zPos1,
			Real xPos2, Real yPos2, Real zPos2,
			Real xPos3, Real yPos3, Real zPos3,
			Real xPos, Real zPos);
	} /* end namespace Interpolation */

	constexpr Vector2D ZERO_VECTOR_2D{ REAL_ZERO, REAL_ZERO };
	constexpr Vector3D ZERO_VECTOR_3D{ REAL_ZERO, REAL_ZERO, REAL_ZERO };

} /* end namespace Math */

#endif /* __MATH_VECTOR_H__ */