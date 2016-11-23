#include "stdafx.h"
#include "Vector.h"
#include "Quaternion.h"
#include "FloatingPoint.h"
#include <math.h>

Math::Vector2D& Math::Vector2D::operator+=(const Vector2D& v) noexcept
{
	x += v.x;
	y += v.y;

	return *this;
}

Math::Vector2D& Math::Vector2D::operator-=(const Vector2D& v) noexcept
{
	x -= v.x;
	y -= v.y;

	return *this;
}

Math::Vector2D& Math::Vector2D::operator*=(Math::Real value) noexcept
{
	x *= value;
	y *= value;
	return *this;
}

Math::Vector2D& Math::Vector2D::operator*=(const Vector2D& v) noexcept
{
	x *= v.x;
	y *= v.y;

	return *this;
}

Math::Vector2D& Math::Vector2D::operator/=(Real s) noexcept
{
	CHECK_CONDITION_RETURN_MATH(!AlmostEqual(s, REAL_ZERO), *this, Utility::Logging::ERR, "Dividing the 2D vector by zero is impossible. Returning the unmodified vector.");
	x /= s;
	y /= s;

	return *this;
}

Math::Vector2D& Math::Vector2D::operator/=(const Vector2D& v) noexcept
{
	CHECK_CONDITION_RETURN_MATH(!AlmostEqual(v.GetX(), REAL_ZERO), *this, Utility::Logging::ERR, "Cannot perform the division of the vector (x == 0). Returning the unmodified vector.");
	CHECK_CONDITION_RETURN_MATH(!AlmostEqual(v.GetY(), REAL_ZERO), *this, Utility::Logging::ERR, "Cannot perform the division of the vector (y == 0). Returning the unmodified vector.");
	x /= v.x;
	y /= v.y;

	return *this;
}

bool Math::Vector2D::operator==(const Vector2D& v) const
{
	return ( AlmostEqual(x, v.x) && AlmostEqual(y, v.y) );
}

bool Math::Vector2D::operator!=(const Vector2D& v) const
{
	return (!((*this) == v));
}

bool Math::Vector2D::IsNormalized() const
{
	return AlmostEqual(LengthSquared(), REAL_ONE);
}

Math::Vector2D Math::Vector2D::Perp(bool normalizingEnabled /* = false */) const
{
	if (normalizingEnabled)
	{
		return Vector2D(y, -x).Normalized();
	}
	return Vector2D(y, -x);
}

Math::Real Math::Vector2D::Max() const
{
	return (x > y) ? x : y;
}

#ifdef PASS_VECTOR_BY_VALUE
Math::Vector2D Math::Vector2D::Max(Vector2D v) const
{
	if (x > v.x)
	{
		v.x = x;
	}
	if (y > v.y)
	{
		v.y = y;
	}
	return v;
}

Math::Vector2D Math::Vector2D::Lerp(Vector2D vec, Real lerpFactor) const
{
	CHECK_CONDITION_MATH(!(lerpFactor < REAL_ZERO || lerpFactor > REAL_ONE), Utility::Logging::ERR,
		"Vector2D linear interpolation performed with the incorrect factor ", lerpFactor);
	vec.x = x * (REAL_ONE - lerpFactor) + vec.x * lerpFactor;
	vec.y = y * (REAL_ONE - lerpFactor) + vec.y * lerpFactor;
	return vec;
	
	//vec = ((vec - (*this)) * lerpFactor) + (*this);
	//return vec;
}
#else
Math::Vector2D Math::Vector2D::Max(const Vector2D& v) const
{
	return Vector2D((m_x > v.GetX()) ? m_x : v.GetX(), m_y > v.GetY() ? m_y : v.GetY());
}

Math::Vector2D Math::Vector2D::Lerp(const Vector2D& vec, Real lerpFactor) const
{
	CHECK_CONDITION_MATH(!(lerpFactor < REAL_ZERO || lerpFactor > REAL_ONE), Utility::Logging::ERR,
		"Vector2D linear interpolation performed with the incorrect factor ", lerpFactor);
	return Math::Vector2D(m_x * (REAL_ONE - lerpFactor) + vec.GetX() * lerpFactor,
		m_y * (REAL_ONE - lerpFactor) + vec.GetY() * lerpFactor);
	//return ((vec - (*this)) * lerpFactor) + (*this);
}
#endif

Math::Vector2D Math::Vector2D::Rotate(const Angle& angle)
{
	const Real cosine = angle.Cos();
	const Real sine = angle.Sin();

	return Vector2D(x * cosine - y * sine, x * sine + y * cosine);
}

/* ==================== Vector3D ==================== */
Math::Real Math::Vector3D::Max() const
{
	return (x > y) ? ((x > z) ? x : z) : ((y > z) ? y : z);
}

#ifdef PASS_VECTOR_BY_VALUE
Math::Vector3D Math::Vector3D::Lerp(Vector3D vec, Real lerpFactor) const
{
	CHECK_CONDITION_MATH(!(lerpFactor < REAL_ZERO || lerpFactor > REAL_ONE), Utility::Logging::ERR,
		"Vector3D linear interpolation performed with the incorrect factor ", lerpFactor);
	const Math::Real oneMinusLerpFactor = REAL_ONE - lerpFactor;
	vec.x = x * oneMinusLerpFactor + vec.x * lerpFactor;
	vec.y = y * oneMinusLerpFactor + vec.y * lerpFactor;
	vec.z = z * oneMinusLerpFactor + vec.z * lerpFactor;
	return vec;
	
	// vec = ((vec - (*this)) * lerpFactor) + (*this);
	// return vec;
}
Math::Vector3D Math::Vector3D::Max(Vector3D v) const
{
	if (x > v.x)
	{
		v.x = x;
	}
	if (y > v.y)
	{
		v.y = y;
	}
	if (z > v.z)
	{
		v.z = z;
	}
	return v;
}
#else
Math::Vector3D Math::Vector3D::Lerp(const Vector3D& vec, Real lerpFactor) const
{
	CHECK_CONDITION_MATH(!(lerpFactor < REAL_ZERO || lerpFactor > REAL_ONE), Utility::Logging::ERR,
		"Vector3D linear interpolation performed with the incorrect factor ", lerpFactor);
	const Math::Real oneMinusLerpFactor = REAL_ONE - lerpFactor;
	return Math::Vector3D(m_x * oneMinusLerpFactor + vec.GetX() * lerpFactor,
		m_y * oneMinusLerpFactor + vec.GetY() * lerpFactor,
		m_z * oneMinusLerpFactor + vec.GetZ() * lerpFactor);
	//return ((vec - (*this)) * lerpFactor) + (*this);
}
Math::Vector3D Math::Vector3D::Max(const Vector3D& v) const
{
	return Vector3D((m_x > v.GetX()) ? m_x : v.GetX(), m_y > v.GetY() ? m_y : v.GetY(), m_z > v.GetZ() ? m_z : v.GetZ());
}
#endif

Math::Vector3D Math::Vector3D::Rotate(const Vector3D& axis, const Angle& angle)
{
	// TODO: Test which method is faster and use it instead of the other

	/* ==================== METHOD #1 ==================== */
	//Quaternion rotation(axis, angle);
	//Quaternion conjugate(rotation.Conjugate());

	//Quaternion w((rotation * (*this)) * conjugate);

	//m_x = w.GetX();
	//m_y = w.GetY();
	//m_z = w.GetZ();

	//return *this;

	/* ==================== METHOD #2 ==================== */
	Real sinAngle = static_cast<Real>(-angle.Sin()); // sin(-alpha) == -sin(alpha)
	Real cosAngle = static_cast<Real>(angle.Cos()); // cos(-alpha) == cos(alpha)

	return Cross(axis * sinAngle) + // rotation on local X
		(*this * cosAngle) + // rotation on local Z
		(axis * (Dot(axis * (REAL_ONE - cosAngle)))); // rotation on local Y
}

Math::Vector3D Math::Vector3D::Rotate(const Quaternion& rotation) const
{
	// TODO: This function should be reimplemented to avoid unnecessary copies of quaternions.
	Quaternion w = rotation * (*this) * rotation.Conjugate(); // FIXME: Check quaternion multiplication
	return Vector3D(w.GetX(), w.GetY(), w.GetZ());
}

Math::Vector3D& Math::Vector3D::operator+=(const Vector3D& v) noexcept
{
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

Math::Vector3D& Math::Vector3D::operator-=(const Vector3D& v) noexcept
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

Math::Vector3D& Math::Vector3D::operator*=(Math::Real value) noexcept
{
	x *= value;
	y *= value;
	z *= value;
	return *this;
}

Math::Vector3D& Math::Vector3D::operator*=(const Vector3D& v) noexcept
{
	x *= v.x;
	y *= v.y;
	z *= v.z;
	return *this;
}

Math::Vector3D& Math::Vector3D::operator/=(Real s) noexcept
{
	CHECK_CONDITION_RETURN_MATH(!AlmostEqual(s, REAL_ZERO), *this, Utility::Logging::ERR, "Dividing the 3D vector by zero is impossible. Returning the unmodified vector.");
	x /= s;
	y /= s;
	z /= s;
	return *this;
}

Math::Vector3D& Math::Vector3D::operator/=(const Vector3D& v) noexcept
{
	CHECK_CONDITION_RETURN_MATH(!AlmostEqual(v.GetX(), REAL_ZERO), *this, Utility::Logging::ERR, "Cannot perform the division of the vector (x == 0). Returning the unmodified vector.");
	CHECK_CONDITION_RETURN_MATH(!AlmostEqual(v.GetY(), REAL_ZERO), *this, Utility::Logging::ERR, "Cannot perform the division of the vector (y == 0). Returning the unmodified vector.");
	CHECK_CONDITION_RETURN_MATH(!AlmostEqual(v.GetZ(), REAL_ZERO), *this, Utility::Logging::ERR, "Cannot perform the division of the vector (z == 0). Returning the unmodified vector.");
	x /= v.x;
	y /= v.y;
	z /= v.z;
	return *this;
}

inline bool Math::Vector3D::operator==(const Vector3D& v) const
{
	if (!AlmostEqual(x, v.x))
		return false;
	if (!AlmostEqual(y, v.y))
		return false;
	if (!AlmostEqual(z, v.z))
		return false;
	return true;
}

bool Math::Vector3D::operator!=(const Vector3D& v) const
{
	return (!((*this) == v));
}

bool Math::Vector3D::IsNormalized() const
{
	return AlmostEqual(LengthSquared(), REAL_ONE);
}

void Math::Vector3D::Approach(Real step, const Vector3D& approachedVector)
{
	ApproachX(step, approachedVector.x);
	ApproachY(step, approachedVector.y);
	ApproachZ(step, approachedVector.z);
}

void Math::Vector3D::ApproachX(Real step, Real approachedValue)
{
	if (x > approachedValue)
	{
		x -= step;
		if (x < approachedValue)
		{
			x = approachedValue;
		}
	}
	else
	{
		x += step;
		if (x > approachedValue)
		{
			x = approachedValue;
		}
	}
}

void Math::Vector3D::ApproachY(Real step, Real approachedValue)
{
	if (y > approachedValue)
	{
		y -= step;
		if (y < approachedValue)
		{
			y = approachedValue;
		}
	}
	else
	{
		y += step;
		if (y > approachedValue)
		{
			y = approachedValue;
		}
	}
}

void Math::Vector3D::ApproachZ(Real step, Real approachedValue)
{
	if (z > approachedValue)
	{
		z -= step;
		if (z < approachedValue)
		{
			z = approachedValue;
		}
	}
	else
	{
		z += step;
		if (z > approachedValue)
		{
			z = approachedValue;
		}
	}
}

void Math::Vector3D::Threshold(Real maxLength)
{
	Real length = Length();
	if (length > maxLength)
	{
		CHECK_CONDITION_RETURN_VOID_MATH(!AlmostEqual(maxLength, REAL_ZERO), Utility::Logging::ERR, "Cannot perform the threshold operation (the specified threshold is 0). Returning the unmodified vector.");
		Real quotient = length / maxLength;
		CHECK_CONDITION_RETURN_VOID_MATH(!AlmostEqual(quotient, REAL_ZERO), Utility::Logging::ERR, "Cannot perform the threshold operation (the specified quotient is 0). Returning the unmodified vector.");
		x /= quotient;
		y /= quotient;
		z /= quotient;
	}
}

/* ==================== Vector4D ==================== */
Math::Real Math::Vector4D::Max() const
{
	return (x > y) ? ((x > z) ? x : z) : ((y > z) ? y : z);
}

#ifdef PASS_VECTOR_BY_VALUE
Math::Vector4D Math::Vector4D::Max(Vector4D v) const
{
	if (x > v.x)
	{
		v.x = x;
	}
	if (y > v.y)
	{
		v.y = y;
	}
	if (z > v.z)
	{
		v.z = z;
	}
	if (w > v.w)
	{
		v.w = w;
	}
	return v;
}
Math::Vector4D Math::Vector4D::Lerp(Vector4D vec, Real lerpFactor) const
{
	CHECK_CONDITION_MATH(!(lerpFactor < REAL_ZERO || lerpFactor > REAL_ONE), Utility::Logging::ERR,
		"Vector2D linear interpolation performed with the incorrect factor ", lerpFactor);
	const Real oneMinusLerpFactor = REAL_ONE - lerpFactor;
	vec.x = x * oneMinusLerpFactor + vec.x * lerpFactor;
	vec.y = y * oneMinusLerpFactor + vec.y * lerpFactor;
	vec.z = z * oneMinusLerpFactor + vec.z * lerpFactor;
	vec.w = w * oneMinusLerpFactor + vec.w * lerpFactor;
	return vec;
	
	//return ((vec - (*this)) * lerpFactor) + (*this);
}
#else
Math::Vector4D Math::Vector4D::Max(const Vector4D& v) const
{
	return Vector4D((m_x > v.GetX()) ? m_x : v.GetX(), m_y > v.GetY() ? m_y : v.GetY(), m_z > v.GetZ() ? m_z : v.GetZ(), m_w > v.GetW() ? m_w : v.GetW());
}
Math::Vector4D Math::Vector4D::Lerp(const Vector4D& vec, Real lerpFactor) const
{
	CHECK_CONDITION_MATH(!(lerpFactor < REAL_ZERO || lerpFactor > REAL_ONE), Utility::Logging::ERR,
		"Vector2D linear interpolation performed with the incorrect factor ", lerpFactor);
	return ((vec - (*this)) * lerpFactor) + (*this);
}
#endif

Math::Vector4D& Math::Vector4D::operator+=(const Vector4D& v) noexcept
{
	x += v.x;
	y += v.y;
	z += v.z;
	w += v.w;

	return *this;
}

Math::Vector4D& Math::Vector4D::operator-=(const Vector4D& v) noexcept
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	w -= v.w;

	return *this;
}

Math::Vector4D& Math::Vector4D::operator*=(Math::Real value) noexcept
{
	x *= value;
	y *= value;
	z *= value;
	w *= value;
	return *this;
}

Math::Vector4D& Math::Vector4D::operator*=(const Vector4D& v) noexcept
{
	x *= v.x;
	y *= v.y;
	z *= v.z;
	w *= v.w;

	return *this;
}

Math::Vector4D& Math::Vector4D::operator/=(Real s) noexcept
{
	CHECK_CONDITION_RETURN_MATH(!AlmostEqual(s, REAL_ZERO), *this, Utility::Logging::ERR, "Dividing the 4D vector by zero is impossible. Returning the unmodified vector.");
	x /= s;
	y /= s;
	z /= s;
	w /= s;
	return *this;
}

Math::Vector4D& Math::Vector4D::operator/=(const Vector4D& v) noexcept
{
	CHECK_CONDITION_RETURN_MATH(!AlmostEqual(v.GetX(), REAL_ZERO), *this, Utility::Logging::ERR, "Cannot perform the division of the vector (x == 0). Returning the unmodified vector.");
	CHECK_CONDITION_RETURN_MATH(!AlmostEqual(v.GetY(), REAL_ZERO), *this, Utility::Logging::ERR, "Cannot perform the division of the vector (y == 0). Returning the unmodified vector.");
	CHECK_CONDITION_RETURN_MATH(!AlmostEqual(v.GetZ(), REAL_ZERO), *this, Utility::Logging::ERR, "Cannot perform the division of the vector (z == 0). Returning the unmodified vector.");
	CHECK_CONDITION_RETURN_MATH(!AlmostEqual(v.GetW(), REAL_ZERO), *this, Utility::Logging::ERR, "Cannot perform the division of the vector (w == 0). Returning the unmodified vector.");
	x /= v.x;
	y /= v.y;
	z /= v.z;
	w /= v.w;

	return *this;
}

inline bool Math::Vector4D::operator==(const Vector4D& v) const
{
	if (!AlmostEqual(x, v.x))
		return false;
	if (!AlmostEqual(y, v.y))
		return false;
	if (!AlmostEqual(z, v.z))
		return false;
	if (!AlmostEqual(w, v.w))
		return false;
	return true;
}

bool Math::Vector4D::operator!=(const Vector4D& v) const
{
	return (!((*this) == v));
}

bool Math::Vector4D::IsNormalized() const
{
	return AlmostEqual(LengthSquared(), REAL_ONE);
}

void Math::Vector4D::Approach(Real step, const Vector4D& approachedVector)
{
	ApproachX(step, approachedVector.x);
	ApproachY(step, approachedVector.y);
	ApproachZ(step, approachedVector.z);
	ApproachW(step, approachedVector.w);
}

void Math::Vector4D::ApproachX(Real step, Real approachedValue)
{
	if (x > approachedValue)
	{
		x -= step;
		if (x < approachedValue)
		{
			x = approachedValue;
		}
	}
	else
	{
		x += step;
		if (x > approachedValue)
		{
			x = approachedValue;
		}
	}
}

void Math::Vector4D::ApproachY(Real step, Real approachedValue)
{
	if (y > approachedValue)
	{
		y -= step;
		if (y < approachedValue)
		{
			y = approachedValue;
		}
	}
	else
	{
		y += step;
		if (y > approachedValue)
		{
			y = approachedValue;
		}
	}
}

void Math::Vector4D::ApproachZ(Real step, Real approachedValue)
{
	if (z > approachedValue)
	{
		z -= step;
		if (z < approachedValue)
		{
			z = approachedValue;
		}
	}
	else
	{
		z += step;
		if (z > approachedValue)
		{
			z = approachedValue;
		}
	}
}

void Math::Vector4D::ApproachW(Real step, Real approachedValue)
{
	if (w > approachedValue)
	{
		w -= step;
		if (w < approachedValue)
		{
			w = approachedValue;
		}
	}
	else
	{
		w += step;
		if (w > approachedValue)
		{
			w = approachedValue;
		}
	}
}

void Math::Vector4D::Threshold(Real maxLength)
{
	Real length = Length();
	if (length > maxLength)
	{
		CHECK_CONDITION_RETURN_VOID_MATH(!AlmostEqual(maxLength, REAL_ZERO), Utility::Logging::ERR, "Cannot perform the threshold operation (the specified threshold is 0). Returning the unmodified vector.");
		const Real quotient = length / maxLength;
		CHECK_CONDITION_RETURN_VOID_MATH(!AlmostEqual(quotient, REAL_ZERO), Utility::Logging::ERR, "Cannot perform the threshold operation (the specified quotient is 0). Returning the unmodified vector.");
		x /= quotient;
		y /= quotient;
		z /= quotient;
		w /= quotient;
	}
}

// TODO: Move these to BarycentricInterpolator class.
Math::Real Math::Interpolation::BarycentricInterpolation(const Vector3D& pos1, const Vector3D& pos2, const Vector3D& pos3, Real xPos, Real zPos)
{
	return BarycentricInterpolation(pos1.x, pos1.y, pos1.z, pos2.x, pos2.y, pos2.z, pos3.x, pos3.y, pos3.z, xPos, zPos);
}

Math::Real Math::Interpolation::BarycentricInterpolation(Real xPos1, Real yPos1, Real zPos1,
	Real xPos2, Real yPos2, Real zPos2,
	Real xPos3, Real yPos3, Real zPos3,
	Real xPos, Real zPos)
{
	Math::Real det = (zPos2 - zPos3) * (xPos1 - xPos3) + (xPos3 - xPos2) * (zPos1 - zPos3);
	Math::Real l1 = ((zPos2 - zPos3) * (xPos - xPos3) + (xPos3 - xPos2) * (zPos - zPos3)) / det;
	Math::Real l2 = ((zPos3 - zPos1) * (xPos - xPos3) + (xPos1 - xPos3) * (zPos - zPos3)) / det;
	Math::Real l3 = 1.0f - l1 - l2;
	return l1 * yPos1 + l2 * yPos2 + l3 * yPos3;
}