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
	class Angle
	{
		/* ==================== Static variables and functions begin ==================== */
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		// Explicit constructor, so that Real cannot be easily cast to Angle
		MATH_API explicit Angle(Real angle, Unit::UnitType unit = Unit::DEGREE) :
			m_angle((unit == Unit::RADIAN) ? angle : ToRad(angle)),
			m_unit(Unit::RADIAN)
#ifdef CALCULATE_MATH_STATS
			, m_classStats(STATS_STORAGE.GetClassStats("Angle"))
#endif
		{
		}

		MATH_API Angle(const Angle& angle); // copy constructor
		MATH_API Angle(Angle&& angle); // move constructor

		MATH_API ~Angle()
		{
		}
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		MATH_API inline Real GetAngleInDegrees() const;
		MATH_API inline Real GetAngleInRadians() const;
		MATH_API bool IsInDegrees() const { return (m_unit == Unit::DEGREE); }
		MATH_API Unit::UnitType GetUnit() const { return m_unit; }

		MATH_API void SetAngleInDegrees(Real angleInDegrees) { m_angle = angleInDegrees; m_unit = Unit::DEGREE; }
		MATH_API void SetAngleInRadians(Real angleInRadians) { m_angle = angleInRadians; m_unit = Unit::RADIAN; }

		/// <summary>
		/// Calculates and returns the sine of the current angle.
		/// </summary>
		/// <returns>The precise value of the sine of the current angle.</returns>
		/// <remarks>
		/// This function is more precise than FastSin1() or FastSin2() at the expense of its speed. It is way slower than the other ones.
		/// </remarks>
		MATH_API inline Real Sin() const;

		/// <summary>
		/// The fast trigonometric function calculating the sine of an angle. Given <code>x</code> is the angle in radians the result is the following:
		/// sin(x) = x - 0.16605x^2 + 0.00761x^4 + eps(x).
		/// </summary>
		/// <returns>The approximated value of the sine of the current angle.</returns>
		/// <remarks>
		/// For correct results the angle must be in range [0; pi / 2].
		/// The error term eps(x) is bounded by |eps(x)| <= 1.7 * 10^(-4).
		/// </remarks>
		MATH_API inline Real FastSin1() const;

		/// <summary>
		/// The fast trigonometric function calculating the sine of an angle. Given <code>x</code> is the angle in radians the result is the following:
		/// sin(x) = x - 0.1666666664x^2 + 0.0083333315x^4 - 0.0001984090x^6 + 0.0000027526x^8 - 0.0000000239x^10 + eps(x).
		/// </summary>
		/// <returns>The approximated value of the sine of the current angle.</returns>
		/// <remarks>
		/// For correct results the angle must be in range [0; pi / 2].
		/// The error term eps(x) is bounded by |eps(x)| <= 1.9 * 10^(-8).
		/// </remarks>
		MATH_API inline Real FastSin2() const;

		/// <summary>
		/// Calculates and returns the cosine of the current angle.
		/// </summary>
		/// <returns>The precise value of the cosine of the current angle.</returns>
		/// <remarks>
		/// This function is more precise than FastSin1() or FastSin2() at the expense of its speed. It is way slower than the other ones.
		/// </remarks>
		MATH_API inline Real Cos() const;

		/// <summary>
		/// The fast trigonometric function calculating the cosine of an angle. Given <code>x</code> is the angle in radians the result is the following:
		/// cos(x) = 1 - 0.4967x^2 + 0.03705x^4 + eps(x).
		/// </summary>
		/// <returns>The approximated value of the cosine of the current angle.</returns>
		/// <remarks>
		/// For correct results the angle must be in range [0; pi / 2].
		/// The error term eps(x) is bounded by |eps(x)| <= 1.2 * 10^(-3).
		/// </remarks>
		MATH_API inline Real FastCos1() const;

		/// <summary>
		/// The fast trigonometric function calculating the cosine of an angle. Given <code>x</code> is the angle in radians the result is the following:
		/// cos(x) = 1 - 0.4999999963x^2 + 0.0416666418x^4 - 0.0013888397x^6 + 0.0000247609x^8 - 0.0000002605x^10 + eps(x).
		/// </summary>
		/// <returns>The approximated value of the cosine of the current angle.</returns>
		/// <remarks>
		/// For correct results the angle must be in range [0; pi / 2].
		/// The error term eps(x) is bounded by |eps(x)| <= 6.5 * 10^(-9).
		/// </remarks>
		MATH_API inline Real FastCos2() const;

		/// <summary>
		/// Calculates and returns the tangent of the current angle.
		/// </summary>
		/// <returns>The precise value of the tangent of the current angle.</returns>
		/// <remarks>
		/// This function is more precise than FastTan1() or FastTan2() at the expense of its speed. It is way slower than the other ones.
		/// </remarks>
		MATH_API inline Real Tan() const;

		/// <summary>
		/// The fast trigonometric function calculating the tangent of an angle. Given <code>x</code> is the angle in radians the result is the following:
		/// tan(x) = x + 0.31755x^3 + 0.2033x^5 + eps(x).
		/// </summary>
		/// <returns>The approximated value of the tangent of the current angle.</returns>
		/// <remarks>
		/// For correct results the angle must be in range [0; pi / 4].
		/// The error term eps(x) is bounded by |eps(x)| <= 8.1 * 10^(-4).
		/// </remarks>
		MATH_API inline Real FastTan1() const;

		/// <summary>
		/// The fast trigonometric function calculating the tangent of an angle. Given <code>x</code> is the angle in radians the result is the following:
		/// tan(x) = x + 0.3333314036x^3 + 0.1333923995x^5 + + 0.0533740603x^7 + 0.0245650893x^9 + 0.0029005250x^11 + 0.0095168091x^13 + eps(x).
		/// </summary>
		/// <returns>The approximated value of the tangent of the current angle.</returns>
		/// <remarks>
		/// For correct results the angle must be in range [0; pi / 2 (although the book "3D Game Engine Architecture" by David H. Eberly says 2 it may be an error, it is likely to be 4 as in FastTan1())].
		/// The error term eps(x) is bounded by |eps(x)| <= 1.9 * 10^(-8).
		/// </remarks>
		MATH_API inline Real FastTan2() const;

		MATH_API Angle operator-() const;
		MATH_API Angle operator+(const Angle& angle) const;
		MATH_API Angle operator-(const Angle& angle) const;
		MATH_API Angle operator*(Real s) const;
		MATH_API Angle operator/(Real s) const;

		MATH_API Angle& operator+=(const Angle& angle);
		MATH_API Angle& operator-=(const Angle& angle);
		MATH_API Angle& operator*=(Real s);
		MATH_API Angle& operator/=(Real s);

		MATH_API Angle& operator=(const Angle& angle);
		MATH_API bool operator==(const Angle& angle) const;
		MATH_API bool operator!=(const Angle& angle) const;
		MATH_API bool operator>(const Angle& angle) const;
		MATH_API bool operator>=(const Angle& angle) const;
		MATH_API bool operator<(const Angle& angle) const;
		MATH_API bool operator<=(const Angle& angle) const;

		MATH_API std::string ToString() const;
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
			ERROR_LOG_MATH("Incorrect unit type for angle with amount=", m_angle, " and unit=", m_unit);
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
			ERROR_LOG_MATH("Incorrect unit type for angle with amount=", m_angle, " and unit=", m_unit);
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
		START_PROFILING;
		// TODO: Range checking (the angle, converted to radians, must be in range [0; pi / 2]!).
		// In case the angle is outside specified range use: https://pl.wikipedia.org/wiki/Trygonometryczne_wzory_redukcyjne.
		static const Real ANGLE_SECOND_POWER_FACTOR = static_cast<Real>(0.16605);
		static const Real ANGLE_FOURTH_POWER_FACTOR = static_cast<Real>(0.00761);

		const Real angleInRad = GetAngleInRadians();
		STOP_PROFILING; // TODO: This profiling makes no sense since most of the computation is done in the return statement itself.
		return angleInRad - (ANGLE_SECOND_POWER_FACTOR * angleInRad * angleInRad * angleInRad) +
			(ANGLE_FOURTH_POWER_FACTOR * angleInRad * angleInRad * angleInRad * angleInRad * angleInRad);
	}

	inline Real Angle::FastSin2() const
	{
		START_PROFILING;
		// TODO: Range checking (the angle, converted to radians, must be in range [0; pi / 2]!).
		static const Real ANGLE_SECOND_POWER_FACTOR = static_cast<Real>(0.1666666664);
		static const Real ANGLE_FOURTH_POWER_FACTOR = static_cast<Real>(0.0083333315);
		static const Real ANGLE_SIXTH_POWER_FACTOR = static_cast<Real>(0.0001984090);
		static const Real ANGLE_EIGHTH_POWER_FACTOR = static_cast<Real>(0.0000027526);
		static const Real ANGLE_TENTH_POWER_FACTOR = static_cast<Real>(0.0000000239);

		const Real angleInRad = GetAngleInRadians();
		STOP_PROFILING; // TODO: This profiling makes no sense since most of the computation is done in the return statement itself.
		return angleInRad - (ANGLE_SECOND_POWER_FACTOR * pow(angleInRad, 3)) + (ANGLE_FOURTH_POWER_FACTOR * pow(angleInRad, 5)) - (ANGLE_SIXTH_POWER_FACTOR * pow(angleInRad, 7)) +
			(ANGLE_EIGHTH_POWER_FACTOR * pow(angleInRad, 9)) - (ANGLE_TENTH_POWER_FACTOR * pow(angleInRad, 11));
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

	inline Real Angle::FastCos1() const
	{
		START_PROFILING;
		// TODO: Range checking (the angle, converted to radians, must be in range [0; pi / 2]!).
		// In case the angle is outside specified range use: https://pl.wikipedia.org/wiki/Trygonometryczne_wzory_redukcyjne.
		static const Real ANGLE_SECOND_POWER_FACTOR = static_cast<Real>(0.4967);
		static const Real ANGLE_FOURTH_POWER_FACTOR = static_cast<Real>(0.03705);

		const Real angleInRad = GetAngleInRadians();
		STOP_PROFILING; // TODO: This profiling makes no sense since most of the computation is done in the return statement itself.
		return REAL_ONE - (ANGLE_SECOND_POWER_FACTOR * angleInRad * angleInRad) + (ANGLE_FOURTH_POWER_FACTOR * angleInRad * angleInRad * angleInRad * angleInRad);
	}

	inline Real Angle::FastCos2() const
	{
		START_PROFILING;
		// TODO: Range checking (the angle, converted to radians, must be in range [0; pi / 2]!).
		static const Real ANGLE_SECOND_POWER_FACTOR = static_cast<Real>(0.4999999963);
		static const Real ANGLE_FOURTH_POWER_FACTOR = static_cast<Real>(0.0416666418);
		static const Real ANGLE_SIXTH_POWER_FACTOR = static_cast<Real>(0.0013888397);
		static const Real ANGLE_EIGHTH_POWER_FACTOR = static_cast<Real>(0.0000247609);
		static const Real ANGLE_TENTH_POWER_FACTOR = static_cast<Real>(0.0000002605);

		const Real angleInRad = GetAngleInRadians();
		STOP_PROFILING; // TODO: This profiling makes no sense since most of the computation is done in the return statement itself.
		return REAL_ONE - (ANGLE_SECOND_POWER_FACTOR * pow(angleInRad, 2)) + (ANGLE_FOURTH_POWER_FACTOR * pow(angleInRad, 4)) -
			(ANGLE_SIXTH_POWER_FACTOR * pow(angleInRad, 6)) + (ANGLE_EIGHTH_POWER_FACTOR * pow(angleInRad, 8)) - (ANGLE_TENTH_POWER_FACTOR * pow(angleInRad, 10));
	}

	inline Real Angle::Tan() const
	{
		START_PROFILING;
		switch (m_unit)
		{
		case Unit::DEGREE:
			STOP_PROFILING; // TODO: This profiling makes no sense since most of the computation is done in the return statement itself.
			return tan(ToRad(m_angle));
			break;
		case Unit::RADIAN:
		default:
			STOP_PROFILING; // TODO: This profiling makes no sense since most of the computation is done in the return statement itself.
			return tan(m_angle);
			break;
		}
	}

	inline Real Angle::FastTan1() const
	{
		START_PROFILING;
		// TODO: Range checking (the angle, converted to radians, must be in range [0; pi / 4]!).
		//static const Real ANGLE_SECOND_POWER_FACTOR = static_cast<Real>(0.31755);
		//static const Real ANGLE_FOURTH_POWER_FACTOR = static_cast<Real>(0.2033);

		const Real angleInRad = GetAngleInRadians();
		STOP_PROFILING; // TODO: This profiling makes no sense since most of the computation is done in the return statement itself.
		return static_cast<Real>(angleInRad + 0.31755 * pow(angleInRad, 3) + 0.2033 * pow(angleInRad, 5));
	}

	inline Real Angle::FastTan2() const
	{
		START_PROFILING;
		// TODO: Range checking (the angle, converted to radians, must be in range [0; pi / 2]!).
		//static const Real ANGLE_SECOND_POWER_FACTOR = static_cast<Real>(0.4999999963);
		//static const Real ANGLE_FOURTH_POWER_FACTOR = static_cast<Real>(0.0416666418);
		//static const Real ANGLE_SIXTH_POWER_FACTOR = static_cast<Real>(0.0013888397);
		//static const Real ANGLE_EIGHTH_POWER_FACTOR = static_cast<Real>(0.0000247609);
		//static const Real ANGLE_TENTH_POWER_FACTOR = static_cast<Real>(0.0000002605);

		const Real angleInRad = GetAngleInRadians();
		STOP_PROFILING; // TODO: This profiling makes no sense since most of the computation is done in the return statement itself.
		return static_cast<Real>(angleInRad + 0.3333314036 * pow(angleInRad, 3) + 0.1333923995 * pow(angleInRad, 5) +
			0.0533740603 * pow(angleInRad, 7) + 0.0245650893 * pow(angleInRad, 9) + 0.0029005250 * pow(angleInRad, 11) + 0.0095168091 * pow(angleInRad, 13));
	}

} /* end namespace Math */

#endif /* __MATH_ANGLE_H__ */