#ifndef __MATH_ANGLE_H__
#define __MATH_ANGLE_H__

#include "Math.h"
#include "Utility\ILogger.h"
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
/// <remarks>
/// The fast trigonometric and inverse trigonometric functions are all based on approximations that appear in
/// "Handbook of Mathematical Functions with Formulas, Graphs, and Mathematical Tables" by Milton Abramowitz and Irene A. Stegun.
/// They are adopted in the book "3D Game Engine Architecture" by David H. Eberly in chapter 2.2.2 ("Fast functions").
/// </remarks>
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
	
	/// <summary>
	/// The fast trigonometric function calculating the sine of an angle. Given <code>x</code> is the angle in radians the result is the following:
	/// sin(x) = x - 0.16605x^2 + 0.00761x^4 + eps(x).
	/// </summary>
	/// <remarks>
	/// For correct results the angle must be in range [0; pi / 2].
	/// The error term eps(x) is bounded by |eps(x)| <= 1.7 * 10^(-4).
	/// </remarks>
	inline Real FastSin1() const;
	
	/// <summary>
	/// The fast trigonometric function calculating the sine of an angle. Given <code>x</code> is the angle in radians the result is the following:
	/// sin(x) = x - 0.1666666664x^2 + 0.0083333315x^4 - 0.0001984090x^6 + 0.0000027526x^8 - 0.0000000239x^10 + eps(x).
	/// </summary>
	/// <remarks>
	/// For correct results the angle must be in range [0; pi / 2].
	/// The error term eps(x) is bounded by |eps(x)| <= 1.9 * 10^(-8).
	/// </remarks>
	inline Real FastSin2() const;

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

inline Real Angle::FastSin1() const
{
	// TODO: Range checking (the angle, converted to radians, must be in range [0; pi / 2]!).
	// In case the angle is outside specified range use: https://pl.wikipedia.org/wiki/Trygonometryczne_wzory_redukcyjne.
	static const Real ANGLE_SECOND_POWER_FACTOR = 0.16605;
	static const Real ANGLE_FOURTH_POWER_FACTOR = 0.00761;

	START_PROFILING;
	switch (m_unit)
	{
	case Unit::DEGREE:
		STOP_PROFILING;
		const Real angleInRad = ToRad(m_angle);
		return angleInRad - (ANGLE_SECOND_POWER_FACTOR * angleInRad * angleInRad * angleInRad) + (ANGLE_FOURTH_POWER_FACTOR * angleInRad * angleInRad * angleInRad * angleInRad * angleInRad);
		break;
	case Unit::RADIAN:
	default:
		STOP_PROFILING;
		return m_angle - (ANGLE_SECOND_POWER_FACTOR * m_angle * m_angle * m_angle) + (ANGLE_FOURTH_POWER_FACTOR * m_angle * m_angle * m_angle * m_angle * m_angle);
		break;
	}
}

inline Real Angle::FastSin2() const
{
	// TODO: Range checking (the angle, converted to radians, must be in range [0; pi / 2]!).
	static const Real ANGLE_SECOND_POWER_FACTOR = 0.1666666664;
	static const Real ANGLE_FOURTH_POWER_FACTOR = 0.0083333315;
	static const Real ANGLE_SIXTH_POWER_FACTOR = 0.0001984090;
	static const Real ANGLE_EIGHTH_POWER_FACTOR = 0.0000027526;
	static const Real ANGLE_TENTH_POWER_FACTOR = 0.0000000239;

	START_PROFILING;
	switch (m_unit)
	{
	case Unit::DEGREE:
		STOP_PROFILING;
		const Real angleInRad = ToRad(m_angle);
		return angleInRad - (ANGLE_SECOND_POWER_FACTOR * pow(angleInRad, 3)) + (ANGLE_FOURTH_POWER_FACTOR * pow(angleInRad, 5)) - (ANGLE_SIXTH_POWER_FACTOR * pow(angleInRad, 7)) + (ANGLE_EIGHTH_POWER_FACTOR * pow(angleInRad, 9)) - (ANGLE_TENTH_POWER_FACTOR * pow(angleInRad, 11));
		break;
	case Unit::RADIAN:
	default:
		STOP_PROFILING;
		return m_angle - (ANGLE_SECOND_POWER_FACTOR * pow(m_angle, 3)) + (ANGLE_FOURTH_POWER_FACTOR * pow(m_angle, 5)) - (ANGLE_SIXTH_POWER_FACTOR * pow(m_angle, 7)) + (ANGLE_EIGHTH_POWER_FACTOR * pow(m_angle, 9)) - (ANGLE_TENTH_POWER_FACTOR * pow(m_angle, 11));
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