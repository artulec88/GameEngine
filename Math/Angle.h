#pragma once

#include "Math.h"
//#include "Utility\ISerializable.h"
#include <string>
#include <sstream>

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
public: // constructors and destructors
	explicit Angle(Real angle = REAL_ZERO, Unit::UnitType unit = Unit::DEGREE); // Explicit constructor, so that Real cannot be easily cast to Angle
	Angle(const Angle& angle); // copy constructor

public: // non-static member functions
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
			return cos(ToRad(m_angle));
			break;
		case Unit::RADIAN:
		default:
			return cos(m_angle);
			break;
		}
	}

	Angle operator-() const { return Angle(-m_angle, m_unit); };
	bool operator==(const Angle& angle) const;
	bool operator!=(const Angle& angle) const;
	bool operator>(const Angle& angle) const;
	bool operator>=(const Angle& angle) const;
	bool operator<(const Angle& angle) const;
	bool operator<=(const Angle& angle) const;
    
private: // non-static member variables
	Real m_angle;
	Unit::UnitType m_unit;
}; /* end class Angle */
  
} /* end namespace Math */
