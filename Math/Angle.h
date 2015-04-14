#pragma once

#include "Math.h"
//#include "Utility\ISerializable.h"
#include <string>

namespace Math
{

namespace Unit
{
	/// <summary>
	/// Possible units for the angles
	/// </summary>
	enum UnitType
	{
		DEGREE = 0,
		RADIAN
	};
} /* end namespace Unit */

class MATH_API Angle
{
/* ==================== Static variables and functions begin ==================== */
/* ==================== Static variables and functions end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	explicit Angle(Real angle = REAL_ZERO, Unit::UnitType unit = Unit::DEGREE); // Explicit constructor, so that Real cannot be easily cast to Angle
	Angle(const Angle& angle); // copy constructor
	~Angle();
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	Real GetAngleInDegrees() const;
	Real GetAngleInRadians() const;
	bool IsInDegrees() const { return (m_unit == Unit::DEGREE); }

	void SetAngleInDegrees(Real angle) { m_angle = angle; m_unit = Unit::DEGREE; }
	void SetAngleInRadians(Real angle) { m_angle = angle; m_unit = Unit::RADIAN; }

	Real Sin() const
	{
		switch(m_unit)
		{
		case Unit::DEGREE:
#ifdef COUNT_STATS
			++m_degToRadConversionCount;
#endif
			return sin(ToRad(m_angle));
			break;
		case Unit::RADIAN:
		default:
			return sin(m_angle);
			break;
		}
	}
	
	Real Cos() const
	{
		switch(m_unit)
		{
		case Unit::DEGREE:
#ifdef COUNT_STATS
			++m_degToRadConversionCount;
#endif
			return cos(ToRad(m_angle));
			break;
		case Unit::RADIAN:
		default:
			return cos(m_angle);
			break;
		}
	}

	Angle operator-() const;

	Angle operator+(const Angle& angle) const;
	Angle operator-(const Angle& angle) const;
	Angle operator*(Real s) const;
	Angle operator/(Real s) const;

	Angle& operator+=(const Angle& angle) const;
	Angle& operator-=(const Angle& angle) const;
	Angle& operator*=(Real s) const;
	Angle& operator/=(Real s) const;

	bool operator==(const Angle& angle) const;
	bool operator!=(const Angle& angle) const;
	bool operator>(const Angle& angle) const;
	bool operator>=(const Angle& angle) const;
	bool operator<(const Angle& angle) const;
	bool operator<=(const Angle& angle) const;

	std::string ToString() const;
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
private:
	Real m_angle;
	Unit::UnitType m_unit;

#ifdef COUNT_STATS
	mutable int m_degToRadConversionCount;
	mutable int m_radToDegConversionCount;
#endif
/* ==================== Non-static member variables end ==================== */

}; /* end class Angle */
  
} /* end namespace Math */
