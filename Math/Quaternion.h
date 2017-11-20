#ifndef __MATH_QUATERNION_H__
#define __MATH_QUATERNION_H__

#include "Math.h"
#include "Angle.h"
#include "Vector.h"
#include "Matrix.h"

#define PASS_QUATERNION_BY_VALUE

namespace math
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
		MATH_API constexpr Quaternion() : Quaternion(REAL_ZERO, REAL_ZERO, REAL_ZERO, REAL_ONE)
		{
		}

		/// <summary>
		/// Quaternion constructor. It takes four floating-point parameters representing each of the components of the quaternion.
		/// </summary>
		/// <param name="x"> The X component of the quaternion. </param>
		/// <param name="y"> The Y component of the quaternion. </param>
		/// <param name="z"> The Z component of the quaternion. </param>
		/// <param name="w"> The W component of the quaternion. </param>
		MATH_API constexpr Quaternion(Real x, Real y, Real z, Real w) : m_x(x), m_y(y), m_z(z), m_w(w)
		{
		}

		/// <summary>
		/// Creates a new quaternion based on the axis given as parameter <paramref name="axis"/> around which we want to rotate by the angle <paramref name="angle"/>.
		/// </summary>
		/// <param name="axis">An axis around which we want to rotate.</param>
		/// <param name="angle">An angle by which we want to rotate.</param>
		MATH_API Quaternion(const Vector3D& axis, const Angle& angle);

		/// <summary>
		/// Quaternion constructor that converts given rotation matrix <paramref name="rotMatrix"/> into a quaternion representing the same rotation.
		/// </summary>
		/// <param name="rotMatrix">
		/// The rotation matrix whose values will be used to initialize a quaternion in a way to represent the same rotation as the matrix itself.
		/// </param>
		MATH_API explicit Quaternion(const Matrix4D& rotMatrix);

		/// <summary> Quaternion copy constructor. </summary>
		/// <param name="q"> The quaternion to copy construct from. </param>
		MATH_API Quaternion(const Quaternion& q) = default;

		/// <summary> Quaternion move constructor. </summary>
		/// <param name="q"> The quaternion to move construct from. </param>
		MATH_API Quaternion(Quaternion&& q) = default;

		/// <summary> Quaternion copy assignment operator. </summary>
		/// <param name="q"> The quaternion to copy assign from. </param>
		/// <returns> The reference to the newly copy-assigned quaternion. </returns>
		MATH_API Quaternion& operator=(const Quaternion& q) = default;

		/// <summary> Quaternion move assignment operator. </summary>
		/// <param name="q"> The quaternion to move assign from. </param>
		/// <returns> The reference to the newly move-assigned quaternion. </returns>
		MATH_API Quaternion& operator=(Quaternion&& q) = default;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
		/// <summary>
		/// Returns the imaginary X component of the quaternion.
		/// </summary>
		/// <returns> The value of the imaginary X component of the quaternion. </returns>
		constexpr Real GetX() const noexcept { return m_x; }

		/// <summary>
		/// Returns the imaginary Y component of the quaternion.
		/// </summary>
		/// <returns> The value of the imaginary Y component of the quaternion. </returns>
		constexpr Real GetY() const noexcept { return m_y; }

		/// <summary>
		/// Returns the imaginary Z component of the quaternion.
		/// </summary>
		/// <returns> The value of the imaginary Z component of the quaternion. </returns>
		constexpr Real GetZ() const noexcept { return m_z; }

		/// <summary>
		/// Returns the real W component of the quaternion.
		/// </summary>
		/// <returns> The value of the real W component of the quaternion. </returns>
		constexpr Real GetW() const noexcept { return m_w; }

		/// <summary>
		/// A simple setter for the quaternion's components.
		/// </summary>
		/// <param name="x">New value for the X component of the quaternion. </param>
		/// <param name="y">New value for the Y component of the quaternion. </param>
		/// <param name="z">New value for the Z component of the quaternion. </param>
		/// <param name="w">New value for the W component of the quaternion. </param>
		void Set(Real x, Real y, Real z, Real w)
		{
			m_x = x;
			m_y = y;
			m_z = z;
			m_w = w;
		}

		/// <summary>
		/// Calculates the length of the quaternion and returns it.
		/// </summary>
		/// <returns> The length of the quaternion. </returns>
		MATH_API Real Length() const;

		/// <summary>
		/// Calculates the squared length of the quaternion and returns it.
		/// </summary>
		/// <returns> The squared length of the quaternion. </returns>
		MATH_API constexpr Real LengthSquared() const
		{
			return static_cast<Real>(m_x * m_x + m_y * m_y + m_z * m_z + m_w * m_w);
		}

		/// <summary>
		/// Pitch inversion function.
		/// </summary>
		/// <remarks>
		/// The pitch is described in the video: https://www.youtube.com/watch?v=PoxDDZmctnU&list=PLRIWtICgwaX0u7Rf9zkZhLoLuZVfUksDP&index=19.
		/// </remarks>
		MATH_API void InvertPitch();

		/// <summary>
		/// Calculates the conjugate of the quaternion and returns it.
		/// </summary>
		/// <returns> The conjugate of the quaternion. </returns>
		MATH_API constexpr Quaternion Conjugate() const { return Quaternion(-m_x, -m_y, -m_z, m_w); }

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

		Vector3D GetForward() const
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
		/// <returns> The matrix representing the same rotation as the quaternion itself. </returns>
		MATH_API Matrix4D ToRotationMatrix() const;

		/// <summary>
		/// Simplifies logging procedure.
		/// </summary>
		/// <param name="out">
		/// The stream that will be used to output the specified quaternion values.
		/// The same stream will then be returned by the function to allow chaining the logging calls in one go.
		/// </param>
		/// <param name="q">
		/// The quaternion whose values will be put into the stream given in parameter <paramref name="out"/>.
		/// </param>
		/// <returns>
		/// The stream used to log the values of the quaternion. This allows for chaining the logging operations in one call.
		/// </returns>
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

	constexpr Quaternion NO_ROTATION_QUATERNION{ REAL_ZERO, REAL_ZERO, REAL_ZERO, REAL_ONE };

} /* end namespace math */

#endif /* __MATH_QUATERNION_H__ */