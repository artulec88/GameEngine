#ifndef __MATH_QUATERNION_H__
#define __MATH_QUATERNION_H__

#include "Math.h"
#include "Angle.h"
#include "Vector.h"
#include "Matrix.h"

#define PASS_QUATERNION_BY_VALUE

namespace Math
{

	/// <summary>
	/// Quaternion representation. Quaternion uses the imaginary component (in contrary to a simple 4D vector) which makes rotation a lot easier to do.
	/// The rotation when using the quaternions can be performed by simple multiplication.
	/// </summary>
	class Quaternion
	{
		/* ==================== Static variables and functions begin ==================== */
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		/// <summary>
		/// Default constructor of the quaternion. The quaternion is set up in a way to represent no rotation at all.
		/// </summary>
		MATH_API constexpr Quaternion() : m_x(REAL_ZERO), m_y(REAL_ZERO), m_z(REAL_ZERO), m_w(REAL_ONE)
		{
		}
		MATH_API constexpr Quaternion(Real x, Real y, Real z, Real w) : m_x(x), m_y(y), m_z(z), m_w(w)
		{
		}
		/// <summary>
		/// Creates a new quaternion based on the axis given as parameter <paramref name="axis"/> around we want to rotate by the angle <paramref name="angle"/>.
		/// </summary>
		/// <param name="axis">An axis around which we want to rotate.</param>
		/// <param name="angle">An angle by which we want to rotate.</param>
		MATH_API Quaternion(const Vector3D& axis, const Angle& angle);
		MATH_API explicit Quaternion(const Matrix4D& rotMatrix);
		MATH_API Quaternion(const Quaternion& q) = default;
		MATH_API Quaternion(Quaternion&& q) = default;
		MATH_API Quaternion& operator=(const Quaternion& q) = default;
		MATH_API Quaternion& operator=(Quaternion&& q) = default;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		inline constexpr Real GetX() const { return m_x; };
		inline constexpr Real GetY() const { return m_y; };
		inline constexpr Real GetZ() const { return m_z; };
		inline constexpr Real GetW() const { return m_w; };

		inline void Set(Math::Real x, Math::Real y, Math::Real z, Math::Real w)
		{
			m_x = x;
			m_y = y;
			m_z = z;
			m_w = w;
		}

		MATH_API Real Length() const;
		MATH_API constexpr Real LengthSquared() const
		{
			return static_cast<Real>(m_x * m_x + m_y * m_y + m_z * m_z + m_w * m_w);
		}

		/// <summary>
		/// Pitch inversion function.
		/// </summary>
		/// <remarks>
		/// The pitch is described in the video: https://www.youtube.com/watch?v=PoxDDZmctnU&list=PLRIWtICgwaX0u7Rf9zkZhLoLuZVfUksDP&index=19.
		/// </remarks
		MATH_API void InvertPitch();

		/// <summary>
		/// Calculates the conjugate of the quaternion.
		/// </summary>
		MATH_API inline constexpr Quaternion Conjugate() const { return Quaternion(-m_x, -m_y, -m_z, m_w); };

		// See http://www.euclideanspace.com/maths/algebra/realNormedAlgebra/quaternions/arithmetic/index.htm for quaternion arithmetics.
#ifdef PASS_QUATERNION_BY_VALUE
		MATH_API Quaternion operator+(Quaternion q) const { q.Set(m_x + q.GetX(), m_y + q.GetY(), m_z + q.GetZ(), m_w + q.GetW()); return q; }
		MATH_API Quaternion operator-(Quaternion q) const { q.Set(m_x - q.GetX(), m_y - q.GetY(), m_z - q.GetZ(), m_w - q.GetW()); return q; }
		MATH_API Quaternion operator*(Quaternion q) const;
		MATH_API Quaternion Nlerp(Quaternion q, Real nlerpFactor, bool shortest) const;
		MATH_API Quaternion Slerp(Quaternion q, Real slerpFactor, bool shortest) const;
#else
		MATH_API Quaternion operator+(const Quaternion& q) const { return Quaternion(m_x + q.GetX(), m_y + q.GetY(), m_z + q.GetZ(), m_w + q.GetW()); }
		MATH_API Quaternion operator-(const Quaternion& q) const { return Quaternion(m_x - q.GetX(), m_y - q.GetY(), m_z - q.GetZ(), m_w - q.GetW()); }
		MATH_API Quaternion operator*(const Quaternion& q) const;
		MATH_API Quaternion Nlerp(const Quaternion& q, Real nlerpFactor, bool shortest) const;
		MATH_API Quaternion Slerp(const Quaternion& q, Real slerpFactor, bool shortest) const;
#endif
		Quaternion operator-() const { return Quaternion(-m_x, -m_y, -m_z, -m_w); }
		MATH_API Quaternion operator*(Real s) const { return Quaternion(s * m_x, s * m_y, s * m_z, s * m_w); }
		MATH_API Quaternion operator*(const Vector3D& vec) const;
		Quaternion operator/(Real s) const { return Quaternion(m_x / s, m_y / s, m_z / s, m_w / s); }
		MATH_API bool operator==(const Quaternion& v) const;
		MATH_API bool operator!=(const Quaternion& v) const;

		MATH_API Quaternion Normalized() const;
		MATH_API Quaternion& Normalize();
		MATH_API bool IsNormalized() const;

		MATH_API Quaternion Negated() const;
		MATH_API Quaternion& Negate();

		MATH_API Real Dot(const Quaternion& q) const;

		inline Vector3D GetForward() const
		{
			// TODO: Maybe instead of creating new vectors and rotating them each time we call this function,
			// we should store the results in the member variables of the quaternion.
			return Vector3D(REAL_ZERO, REAL_ZERO, REAL_ONE).Rotate(*this);
		}
		MATH_API Vector3D GetUp() const;
		MATH_API Vector3D GetRight() const;

		MATH_API Vector3D GetBack() const;
		MATH_API Vector3D GetDown() const;
		MATH_API Vector3D GetLeft() const;

		/// <summary>
		/// Converts the quaternion to the respective matrix representing the same rotation.
		/// </summary>
		MATH_API Matrix4D ToRotationMatrix() const;

		friend std::ostream& operator<<(std::ostream& out, const Quaternion& q)
		{
			out << std::setprecision(4) << "(x=" << q.m_x << "; y=" << q.m_y << "; z=" << q.m_z << "; w=" << q.m_w << ")";
			return out;
		}
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	private:
		/// <summary>
		/// Imaginary X component of the quaternion.
		/// </summary>
		Real m_x;

		/// <summary>
		/// Imaginary Y component of the quaternion.
		/// </summary>
		Real m_y;

		/// <summary>
		/// Imaginary Z component of the quaternion.
		/// </summary>
		Real m_z;

		/// <summary>
		/// Real W component of the quaternion.
		/// </summary>
		Real m_w;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class Quaternion */

} /* end namespace Math */

#endif /* __MATH_QUATERNION_H__ */