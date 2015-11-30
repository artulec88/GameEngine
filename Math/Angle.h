#ifndef __MATH_ANGLE_H__
#define __MATH_ANGLE_H__

#include "Math.h"
//#include "Utility\ISerializable.h"
#include <string>
#ifdef CALCULATE_MATH_STATS
#include "Statistics.h"
#include "IStatisticsStorage.h"
#endif

namespace Math
{

namespace Unit
{
	/// <summary>
	/// Possible units for the angles.
	/// </summary>
	enum UnitType
	{
		DEGREE = 0,
		RADIAN
	};
} /* end namespace Unit */

/// <summary>
/// Angle class is responsible for handling the angle.
/// </summary>
class MATH_API Angle
{
/* ==================== Static variables and functions begin ==================== */
/* ==================== Static variables and functions end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	// Explicit constructor, so that Real cannot be easily cast to Angle
	explicit Angle(Real angle = REAL_ZERO, Unit::UnitType unit = Unit::DEGREE) :
		m_angle((unit == Unit::RADIAN) ? angle : ToRad(angle)),
		m_unit(Unit::RADIAN)
#ifdef CALCULATE_MATH_STATS
		,m_classStats(STATS_STORAGE.GetClassStats("Angle"))
#endif
	{
	}

	Angle(const Angle& angle); // copy constructor
	
	~Angle()
	{
	}
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	inline Real GetAngleInDegrees() const;
	inline Real GetAngleInRadians() const;
	bool IsInDegrees() const { return (m_unit == Unit::DEGREE); }
	Unit::UnitType GetUnit() const { return m_unit; }

	void SetAngleInDegrees(Real angleInDegrees) { m_angle = angleInDegrees; m_unit = Unit::DEGREE; }
	void SetAngleInRadians(Real angleInRadians) { m_angle = angleInRadians; m_unit = Unit::RADIAN; }

	inline Real Sin() const;	
	inline Real Cos() const;

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
	Statistics::ClassStats& m_classStats;
#endif
/* ==================== Non-static member variables end ==================== */

}; /* end class Angle */
  
inline Real Angle::GetAngleInDegrees() const
{
	START_PROFILING;
	switch (m_unit)
	{
	case Unit::DEGREE:
		STOP_PROFILING;
		return m_angle;
	case Unit::RADIAN:
		STOP_PROFILING;
		return ToDeg(m_angle);
	default:
		STOP_PROFILING;
		ERROR_LOG("Incorrect unit type for angle with amount=%.2f and unit=%d", m_angle, m_unit);
		return m_angle;
	}
}

inline Real Angle::GetAngleInRadians() const
{
	START_PROFILING;
	switch (m_unit)
	{
	case Unit::DEGREE:
		STOP_PROFILING;
		return ToRad(m_angle);
	case Unit::RADIAN:
		STOP_PROFILING;
		return m_angle;
	default:
		ERROR_LOG("Incorrect unit type for angle with amount=%.2f and unit=%d", m_angle, m_unit);
		STOP_PROFILING;
		return m_angle;
	}
}

inline Real Angle::Sin() const
{
	START_PROFILING;
	switch (m_unit)
	{
	case Unit::DEGREE:
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

inline Real Angle::Cos() const
{
	START_PROFILING;
	switch (m_unit)
	{
	case Unit::DEGREE:
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

} /* end namespace Math */

#endif /* __MATH_ANGLE_H__ */