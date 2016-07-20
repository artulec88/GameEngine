#include "stdafx.h"
#include "Vector.h"
#include "Quaternion.h"
#include "FloatingPoint.h"
#include <math.h>
#include <sstream>
#include <iomanip>

Math::Vector2D::Vector2D() :
	m_x(REAL_ZERO),
	m_y(REAL_ZERO)
{
}

Math::Vector2D::Vector2D(Real xy) :
	m_x(xy),
	m_y(xy)
{
}

Math::Vector2D::Vector2D(Real x, Real y) :
	m_x(x),
	m_y(y)
{
}

Math::Vector2D::Vector2D(const Vector2D& v) :
	m_x(v.GetX()),
	m_y(v.GetY())
{
}

Math::Vector2D::Vector2D(Vector2D&& v) :
	m_x(std::move(v.GetX())),
	m_y(std::move(v.GetY()))
{
}

Math::Vector2D::~Vector2D()
{
}

Math::Real Math::Vector2D::Length() const
{
	return static_cast<Real>(sqrt(static_cast<Real>(LengthSquared())));
}

Math::Real Math::Vector2D::LengthSquared() const
{
	return static_cast<Real>(m_x * m_x + m_y * m_y);
}

Math::Vector2D& Math::Vector2D::operator+=(const Vector2D& v)
{
	m_x += v.GetX();
	m_y += v.GetY();

	return *this;
}

Math::Vector2D& Math::Vector2D::operator-=(const Vector2D& v)
{
	m_x -= v.GetX();
	m_y -= v.GetY();

	return *this;
}

Math::Vector2D& Math::Vector2D::operator*=(Math::Real value)
{
	m_x *= value;
	m_y *= value;
	return *this;
}

Math::Vector2D& Math::Vector2D::operator*=(const Vector2D& v)
{
	m_x *= v.GetX();
	m_y *= v.GetY();

	return *this;
}

Math::Vector2D& Math::Vector2D::operator/=(Real s)
{
	CHECK_CONDITION_RETURN_MATH(!AlmostEqual(s, REAL_ZERO), *this, Utility::ERR, "Dividing the 2D vector by zero is impossible. Returning the unmodified vector.");
	m_x /= s;
	m_y /= s;

	return *this;
}

Math::Vector2D& Math::Vector2D::operator/=(const Vector2D& v)
{
	CHECK_CONDITION_RETURN_MATH(!AlmostEqual(v.GetX(), REAL_ZERO), *this, Utility::ERR, "Cannot perform the division of the vector (x == 0). Returning the unmodified vector.");
	CHECK_CONDITION_RETURN_MATH(!AlmostEqual(v.GetY(), REAL_ZERO), *this, Utility::ERR, "Cannot perform the division of the vector (y == 0). Returning the unmodified vector.");
	m_x /= v.GetX();
	m_y /= v.GetY();

	return *this;
}

Math::Vector2D& Math::Vector2D::operator=(const Vector2D& v)
{
	m_x = v.GetX();
	m_y = v.GetY();

	return *this;
}

bool Math::Vector2D::operator==(const Vector2D& v) const
{
	return ( AlmostEqual(this->GetX(), v.GetX()) && AlmostEqual(this->GetY(), v.GetY()) );
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
		return Vector2D(m_y, -m_x).Normalized();
	}
	return Vector2D(m_y, -m_x);
}

Math::Real Math::Vector2D::Max() const
{
	return (m_x > m_y) ? m_x : m_y;
}

Math::Vector2D Math::Vector2D::Max(const Vector2D& v) const
{
	return Vector2D((m_x > v.GetX()) ? m_x : v.GetX(), m_y > v.GetY() ? m_y : v.GetY());
}

Math::Vector2D Math::Vector2D::Rotate(const Angle& angle)
{
	const Real cosine = angle.Cos();
	const Real sine = angle.Sin();

	return Vector2D(m_x * cosine - m_y * sine, m_x * sine + m_y * cosine);
}

Math::Vector2D Math::Vector2D::Lerp(const Vector2D& vec, Real lerpFactor) const
{
	CHECK_CONDITION_MATH(!(lerpFactor < REAL_ZERO || lerpFactor > REAL_ONE), Utility::ERR,
		"Vector2D linear interpolation performed with the incorrect factor ", lerpFactor);
	return ((vec - (*this)) * lerpFactor) + (*this);
}

std::string Math::Vector2D::ToString() const
{
	std::stringstream ss("");
	ss << "(x=" << m_x << "; y=" << m_y << ")";
	return ss.str();
}

/* ==================== Vector3D ==================== */
Math::Vector3D::Vector3D() :
	m_x(REAL_ZERO),
	m_y(REAL_ZERO),
	m_z(REAL_ZERO)
{
}

Math::Vector3D::Vector3D(Real xyz) :
	m_x(xyz),
	m_y(xyz),
	m_z(xyz)
{
}

Math::Vector3D::Vector3D(Real x, Real y, Real z) :
	m_x(x),
	m_y(y),
	m_z(z)
{
}

Math::Vector3D::Vector3D(const Vector3D& v) :
	m_x(v.GetX()),
	m_y(v.GetY()),
	m_z(v.GetZ())
{
}

Math::Vector3D::Vector3D(Vector3D&& v) :
	m_x(std::move(v.m_x)),
	m_y(std::move(v.m_y)),
	m_z(std::move(v.m_z))
{
}

Math::Vector3D::~Vector3D()
{
}

std::string Math::Vector3D::ToString() const
{
	std::stringstream ss("");
	ss << std::setprecision(4) << "(x=" << m_x << "; y=" << m_y << "; z=" << m_z << ")";
	return ss.str();
}

Math::Real Math::Vector3D::Length() const
{
	return static_cast<Real>(sqrt(LengthSquared()));
}

Math::Real Math::Vector3D::LengthSquared() const
{
	return static_cast<Real>(m_x * m_x + m_y * m_y + m_z * m_z);
}

Math::Real Math::Vector3D::Max() const
{
	return (m_x > m_y) ? ((m_x > m_z) ? m_x : m_z) : ((m_y > m_z) ? m_y : m_z);
}

Math::Vector3D Math::Vector3D::Max(const Vector3D& v) const
{
	return Vector3D((m_x > v.GetX()) ? m_x : v.GetX(), m_y > v.GetY() ? m_y : v.GetY(), m_z > v.GetZ() ? m_z : v.GetZ());
}

Math::Vector3D Math::Vector3D::Rotate(const Vector3D& axis, const Angle& angle)
{
	// TODO: Test which method is faster and use it instead of the other

	/* ==================== METHOD #1 ==================== */
	//Quaternion rotation(axis, angle);
	//Quaternion conjugate(rotation.Conjugate());

	//Quaternion w((rotation * (*this)) * conjugate);

	//this->m_x = w.GetX();
	//this->m_y = w.GetY();
	//this->m_z = w.GetZ();

	//return *this;

	/* ==================== METHOD #2 ==================== */
	Real sinAngle = static_cast<Real>(-angle.Sin()); // sin(-alpha) == -sin(alpha)
	Real cosAngle = static_cast<Real>(angle.Cos()); // cos(-alpha) == cos(alpha)

	return this->Cross(axis * sinAngle) + // rotation on local X
		(*this * cosAngle) + // rotation on local Z
		(axis * (this->Dot(axis * (REAL_ONE - cosAngle)))); // rotation on local Y
}

Math::Vector3D Math::Vector3D::Rotate(const Quaternion& rotation) const
{
	Quaternion w = rotation * (*this) * rotation.Conjugate(); // FIXME: Check quaternion multiplication
	return Vector3D(w.GetX(), w.GetY(), w.GetZ());
}

Math::Vector3D& Math::Vector3D::operator+=(const Vector3D& v)
{
	m_x += v.GetX();
	m_y += v.GetY();
	m_z += v.GetZ();

	return *this;
}

Math::Vector3D& Math::Vector3D::operator-=(const Vector3D& v)
{
	m_x -= v.GetX();
	m_y -= v.GetY();
	m_z -= v.GetZ();

	return *this;
}

Math::Vector3D& Math::Vector3D::operator*=(Math::Real value)
{
	m_x *= value;
	m_y *= value;
	m_z *= value;
	return *this;
}

Math::Vector3D& Math::Vector3D::operator*=(const Vector3D& v)
{
	m_x *= v.GetX();
	m_y *= v.GetY();
	m_z *= v.GetZ();

	return *this;
}

Math::Vector3D& Math::Vector3D::operator/=(Real s)
{
	CHECK_CONDITION_RETURN_MATH(!AlmostEqual(s, REAL_ZERO), *this, Utility::ERR, "Dividing the 3D vector by zero is impossible. Returning the unmodified vector.");
	m_x /= s;
	m_y /= s;
	m_z /= s;
	return *this;
}

Math::Vector3D& Math::Vector3D::operator/=(const Vector3D& v)
{
	CHECK_CONDITION_RETURN_MATH(!AlmostEqual(v.GetX(), REAL_ZERO), *this, Utility::ERR, "Cannot perform the division of the vector (x == 0). Returning the unmodified vector.");
	CHECK_CONDITION_RETURN_MATH(!AlmostEqual(v.GetY(), REAL_ZERO), *this, Utility::ERR, "Cannot perform the division of the vector (y == 0). Returning the unmodified vector.");
	CHECK_CONDITION_RETURN_MATH(!AlmostEqual(v.GetZ(), REAL_ZERO), *this, Utility::ERR, "Cannot perform the division of the vector (z == 0). Returning the unmodified vector.");
	m_x /= v.GetX();
	m_y /= v.GetY();
	m_z /= v.GetZ();

	return *this;
}

Math::Vector3D& Math::Vector3D::operator=(Vector3D v)
{
	m_x = v.m_x;
	m_y = v.m_y;
	m_z = v.m_z;
	return *this;
}

inline bool Math::Vector3D::operator==(const Vector3D& v) const
{
	if (!AlmostEqual(m_x, v.GetX()))
		return false;
	if (!AlmostEqual(m_y, v.GetY()))
		return false;
	if (!AlmostEqual(m_z, v.GetZ()))
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

Math::Vector3D Math::Vector3D::Lerp(const Vector3D& vec, Real lerpFactor) const
{
	CHECK_CONDITION_MATH(!(lerpFactor < REAL_ZERO || lerpFactor > REAL_ONE), Utility::ERR,
		"Vector3D linear interpolation performed with the incorrect factor ", lerpFactor);
	return ((vec - (*this)) * lerpFactor) + (*this);
}

void Math::Vector3D::Approach(Real step, const Vector3D& approachedVector)
{
	ApproachX(step, approachedVector.GetX());
	ApproachY(step, approachedVector.GetY());
	ApproachZ(step, approachedVector.GetZ());
}

void Math::Vector3D::ApproachX(Real step, Real approachedValue)
{
	if (m_x > approachedValue)
	{
		m_x -= step;
		if (m_x < approachedValue)
		{
			m_x = approachedValue;
		}
	}
	else
	{
		m_x += step;
		if (m_x > approachedValue)
		{
			m_x = approachedValue;
		}
	}
}

void Math::Vector3D::ApproachY(Real step, Real approachedValue)
{
	if (m_y > approachedValue)
	{
		m_y -= step;
		if (m_y < approachedValue)
		{
			m_y = approachedValue;
		}
	}
	else
	{
		m_y += step;
		if (m_y > approachedValue)
		{
			m_y = approachedValue;
		}
	}
}

void Math::Vector3D::ApproachZ(Real step, Real approachedValue)
{
	if (m_z > approachedValue)
	{
		m_z -= step;
		if (m_z < approachedValue)
		{
			m_z = approachedValue;
		}
	}
	else
	{
		m_z += step;
		if (m_z > approachedValue)
		{
			m_z = approachedValue;
		}
	}
}

void Math::Vector3D::Threshold(Real maxLength)
{
	Real length = Length();
	if (length > maxLength)
	{
		CHECK_CONDITION_RETURN_VOID_MATH(!AlmostEqual(maxLength, REAL_ZERO), Utility::ERR, "Cannot perform the threshold operation (the specified threshold is 0). Returning the unmodified vector.");
		Real quotient = length / maxLength;
		CHECK_CONDITION_RETURN_VOID_MATH(!AlmostEqual(quotient, REAL_ZERO), Utility::ERR, "Cannot perform the threshold operation (the specified quotient is 0). Returning the unmodified vector.");
		m_x /= quotient;
		m_y /= quotient;
		m_z /= quotient;
	}
}

/* ==================== Vector4D ==================== */
Math::Vector4D::Vector4D() :
	m_x(REAL_ZERO),
	m_y(REAL_ZERO),
	m_z(REAL_ZERO),
	m_w(REAL_ZERO)
{
}

Math::Vector4D::Vector4D(Real xyzw) :
	m_x(xyzw),
	m_y(xyzw),
	m_z(xyzw),
	m_w(xyzw)
{
}

Math::Vector4D::Vector4D(Real x, Real y, Real z, Real w) :
	m_x(x),
	m_y(y),
	m_z(z),
	m_w(w)
{
}

Math::Vector4D::Vector4D(const Vector4D& v) :
	m_x(v.GetX()),
	m_y(v.GetY()),
	m_z(v.GetZ()),
	m_w(v.GetW())
{
}

Math::Vector4D::Vector4D(Vector4D&& v) :
	m_x(std::move(v.GetX())),
	m_y(std::move(v.GetY())),
	m_z(std::move(v.GetZ())),
	m_w(std::move(v.GetW()))
{
}

Math::Vector4D::~Vector4D()
{
}

std::string Math::Vector4D::ToString() const
{
	std::stringstream ss("");
	ss << "(x=" << m_x << "; y=" << m_y << "; z=" << m_z << "; w=" << m_w << ")";
	return ss.str();
}

Math::Real Math::Vector4D::Length() const
{
	return static_cast<Real>(sqrt(static_cast<Real>(LengthSquared())));
}

Math::Real Math::Vector4D::LengthSquared() const
{
	return static_cast<Real>(m_x * m_x + m_y * m_y + m_z * m_z + m_w * m_w);
}

Math::Real Math::Vector4D::Max() const
{
	Real maxComponent = (m_x > m_y) ? m_x : m_y;
	if (m_z > maxComponent)
	{
		maxComponent = m_z;
	}
	if (m_w > maxComponent)
	{
		maxComponent = m_w;
	}
	return maxComponent;
}

Math::Vector4D Math::Vector4D::Max(const Vector4D& v) const
{
	return Vector4D((m_x > v.GetX()) ? m_x : v.GetX(), m_y > v.GetY() ? m_y : v.GetY(), m_z > v.GetZ() ? m_z : v.GetZ(), m_w > v.GetW() ? m_w : v.GetW());
}

Math::Vector4D& Math::Vector4D::operator+=(const Vector4D& v)
{
	m_x += v.GetX();
	m_y += v.GetY();
	m_z += v.GetZ();
	m_w += v.GetW();

	return *this;
}

Math::Vector4D& Math::Vector4D::operator-=(const Vector4D& v)
{
	m_x -= v.GetX();
	m_y -= v.GetY();
	m_z -= v.GetZ();
	m_w -= v.GetW();

	return *this;
}

Math::Vector4D& Math::Vector4D::operator*=(Math::Real value)
{
	m_x *= value;
	m_y *= value;
	m_z *= value;
	m_w *= value;
	return *this;
}

Math::Vector4D& Math::Vector4D::operator*=(const Vector4D& v)
{
	m_x *= v.GetX();
	m_y *= v.GetY();
	m_z *= v.GetZ();
	m_w *= v.GetW();

	return *this;
}

Math::Vector4D& Math::Vector4D::operator/=(Real s)
{
	CHECK_CONDITION_RETURN_MATH(!AlmostEqual(s, REAL_ZERO), *this, Utility::ERR, "Dividing the 4D vector by zero is impossible. Returning the unmodified vector.");
	m_x /= s;
	m_y /= s;
	m_z /= s;
	m_w /= s;
	return *this;
}

Math::Vector4D& Math::Vector4D::operator/=(const Vector4D& v)
{
	CHECK_CONDITION_RETURN_MATH(!AlmostEqual(v.GetX(), REAL_ZERO), *this, Utility::ERR, "Cannot perform the division of the vector (x == 0). Returning the unmodified vector.");
	CHECK_CONDITION_RETURN_MATH(!AlmostEqual(v.GetY(), REAL_ZERO), *this, Utility::ERR, "Cannot perform the division of the vector (y == 0). Returning the unmodified vector.");
	CHECK_CONDITION_RETURN_MATH(!AlmostEqual(v.GetZ(), REAL_ZERO), *this, Utility::ERR, "Cannot perform the division of the vector (z == 0). Returning the unmodified vector.");
	CHECK_CONDITION_RETURN_MATH(!AlmostEqual(v.GetW(), REAL_ZERO), *this, Utility::ERR, "Cannot perform the division of the vector (w == 0). Returning the unmodified vector.");
	m_x /= v.GetX();
	m_y /= v.GetY();
	m_z /= v.GetZ();
	m_w /= v.GetW();

	return *this;
}

Math::Vector4D& Math::Vector4D::operator=(const Vector4D& v)
{
	m_x = v.GetX();
	m_y = v.GetY();
	m_z = v.GetZ();
	m_w = v.GetW();

	return *this;
}

inline bool Math::Vector4D::operator==(const Vector4D& v) const
{
	if (!AlmostEqual(m_x, v.GetX()))
		return false;
	if (!AlmostEqual(m_y, v.GetY()))
		return false;
	if (!AlmostEqual(m_z, v.GetZ()))
		return false;
	if (!AlmostEqual(m_w, v.GetW()))
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

Math::Vector4D Math::Vector4D::Lerp(const Vector4D& vec, Real lerpFactor) const
{
	CHECK_CONDITION_MATH(!(lerpFactor < REAL_ZERO || lerpFactor > REAL_ONE), Utility::ERR,
		"Vector2D linear interpolation performed with the incorrect factor ", lerpFactor);
	return ((vec - (*this)) * lerpFactor) + (*this);
}

void Math::Vector4D::Approach(Real step, const Vector4D& approachedVector)
{
	ApproachX(step, approachedVector.GetX());
	ApproachY(step, approachedVector.GetY());
	ApproachZ(step, approachedVector.GetZ());
	ApproachW(step, approachedVector.GetW());
}

void Math::Vector4D::ApproachX(Real step, Real approachedValue)
{
	if (m_x > approachedValue)
	{
		m_x -= step;
		if (m_x < approachedValue)
		{
			m_x = approachedValue;
		}
	}
	else
	{
		m_x += step;
		if (m_x > approachedValue)
		{
			m_x = approachedValue;
		}
	}
}

void Math::Vector4D::ApproachY(Real step, Real approachedValue)
{
	if (m_y > approachedValue)
	{
		m_y -= step;
		if (m_y < approachedValue)
		{
			m_y = approachedValue;
		}
	}
	else
	{
		m_y += step;
		if (m_y > approachedValue)
		{
			m_y = approachedValue;
		}
	}
}

void Math::Vector4D::ApproachZ(Real step, Real approachedValue)
{
	if (m_z > approachedValue)
	{
		m_z -= step;
		if (m_z < approachedValue)
		{
			m_z = approachedValue;
		}
	}
	else
	{
		m_z += step;
		if (m_z > approachedValue)
		{
			m_z = approachedValue;
		}
	}
}

void Math::Vector4D::ApproachW(Real step, Real approachedValue)
{
	if (m_w > approachedValue)
	{
		m_w -= step;
		if (m_w < approachedValue)
		{
			m_w = approachedValue;
		}
	}
	else
	{
		m_w += step;
		if (m_w > approachedValue)
		{
			m_w = approachedValue;
		}
	}
}

void Math::Vector4D::Threshold(Real maxLength)
{
	Real length = Length();
	if (length > maxLength)
	{
		CHECK_CONDITION_RETURN_VOID_MATH(!AlmostEqual(maxLength, REAL_ZERO), Utility::ERR, "Cannot perform the threshold operation (the specified threshold is 0). Returning the unmodified vector.");
		const Real quotient = length / maxLength;
		CHECK_CONDITION_RETURN_VOID_MATH(!AlmostEqual(quotient, REAL_ZERO), Utility::ERR, "Cannot perform the threshold operation (the specified quotient is 0). Returning the unmodified vector.");
		m_x /= quotient;
		m_y /= quotient;
		m_z /= quotient;
		m_w /= quotient;
	}
}

// TODO: Move these to BarycentricInterpolator class.
Math::Real Math::Interpolation::BarycentricInterpolation(const Math::Vector3D& pos1, const Math::Vector3D& pos2, const Math::Vector3D& pos3, Math::Real xPos, Math::Real zPos)
{
	return BarycentricInterpolation(pos1.GetX(), pos1.GetY(), pos1.GetZ(), pos2.GetX(), pos2.GetY(), pos2.GetZ(), pos3.GetX(), pos3.GetY(), pos3.GetZ(), xPos, zPos);
}

Math::Real Math::Interpolation::BarycentricInterpolation(Math::Real xPos1, Math::Real yPos1, Math::Real zPos1,
	Math::Real xPos2, Math::Real yPos2, Math::Real zPos2,
	Math::Real xPos3, Math::Real yPos3, Math::Real zPos3,
	Math::Real xPos, Math::Real zPos)
{
	Math::Real det = (zPos2 - zPos3) * (xPos1 - xPos3) + (xPos3 - xPos2) * (zPos1 - zPos3);
	Math::Real l1 = ((zPos2 - zPos3) * (xPos - xPos3) + (xPos3 - xPos2) * (zPos - zPos3)) / det;
	Math::Real l2 = ((zPos3 - zPos1) * (xPos - xPos3) + (xPos1 - xPos3) * (zPos - zPos3)) / det;
	Math::Real l3 = 1.0f - l1 - l2;
	return l1 * yPos1 + l2 * yPos2 + l3 * yPos3;
}