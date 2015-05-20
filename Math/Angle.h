#pragma once

#include "Math.h"
//#include "Utility\ISerializable.h"
#include <string>
#include "Statistics.h"
#include "IStatisticsStorage.h"

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
	// Explicit constructor, so that Real cannot be easily cast to Angle
	explicit Angle(Real angle = REAL_ZERO, Unit::UnitType unit = Unit::DEGREE) :
		m_angle(angle),
		m_unit(unit)
#ifdef CALCULATE_MATH_STATS
		,m_degToRadConversionCount(0),
		m_radToDegConversionCount(0),
		m_classStats(STATS_STORAGE.GetClassStats("Angle"))
#endif
	{
	}

	Angle(const Angle& angle); // copy constructor
	
	~Angle()
	{
//#ifdef CALCULATE_MATH_STATS
	//	if ((m_degToRadConversionCount > m_radToDegConversionCount + 1) || (m_radToDegConversionCount > m_degToRadConversionCount + 1))
	//	{
	//		LOG(Utility::Info, LOGPLACE, "The conversions between radians and degrees units report: ToRad: %d\t ToDeg: %d", m_degToRadConversionCount, m_radToDegConversionCount);
	//	}
//#endif
	}
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	Real GetAngleInDegrees() const;
	Real GetAngleInRadians() const;
	bool IsInDegrees() const { return (m_unit == Unit::DEGREE); }
	Unit::UnitType GetUnit() const { return m_unit; }

	void SetAngleInDegrees(Real angle) { m_angle = angle; m_unit = Unit::DEGREE; }
	void SetAngleInRadians(Real angle) { m_angle = angle; m_unit = Unit::RADIAN; }

	Real Sin() const
	{
		START_PROFILING;
		switch(m_unit)
		{
		case Unit::DEGREE:
#ifdef CALCULATE_MATH_STATS
			++m_degToRadConversionCount;
#endif
			STOP_PROFILING;
			return sin(ToRad(m_angle));
			break;
		case Unit::RADIAN:
		default:
			STOP_PROFILING;
			return sin(m_angle);
			break;
		}
	}
	
	Real Cos() const
	{
		START_PROFILING;
		switch(m_unit)
		{
		case Unit::DEGREE:
#ifdef CALCULATE_MATH_STATS
			++m_degToRadConversionCount;
#endif
			STOP_PROFILING;
			return cos(ToRad(m_angle));
			break;
		case Unit::RADIAN:
		default:
			STOP_PROFILING;
			return cos(m_angle);
			break;
		}
	}

	Angle operator-() const;

	Angle operator+(const Angle& angle) const;
	Angle operator-(const Angle& angle) const;
	Angle operator*(Real s) const;
	Angle operator/(Real s) const;

	Angle& operator+=(const Angle& angle);
	Angle& operator-=(const Angle& angle);
	Angle& operator*=(Real s);
	Angle& operator/=(Real s);

	Angle& operator=(const Angle& angle);
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

#ifdef CALCULATE_MATH_STATS
	mutable int m_degToRadConversionCount;
	mutable int m_radToDegConversionCount;
	mutable Statistics::ClassStats& m_classStats;
#endif
/* ==================== Non-static member variables end ==================== */

}; /* end class Angle */
  
} /* end namespace Math */
