#ifndef __MATH_ANGLE_H__
#define __MATH_ANGLE_H__

#include "Math.h"
#include "Utility\ILogger.h"
#include <string>
#ifdef PROFILING_MATH_MODULE_ENABLED
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
		/// <summary> Angle constructor. Angle object is created based on the given value and the angle unit type. </summary>
		/// <param name="angle"> The value of the angle. </param>
		/// <param name="unit"> The unit in which the value of the angle is presented. </param>
		/// <remarks>
		/// Explicit constructor is used, so that <code>Real</code> cannot be easily cast to Angle without the developer knowing about that.
		/// </remarks>
		MATH_API explicit Angle(Real angle, Unit::UnitType unit = Unit::DEGREE) :
			m_angle((unit == Unit::RADIAN) ? angle : ToRad(angle))
#ifdef PROFILING_MATH_MODULE_ENABLED
			, m_classStats(STATS_STORAGE.GetClassStats("Angle"))
#endif
		{
		}

		/// <summary> The angle copy constructor. </summary>
		/// <param name="angle"> The angle we copy construct from. </param>
		MATH_API Angle(const Angle& angle);

		/// <summary> The angle move constructor. </summary>
		/// <param name="angle"> The angle we move construct from. </param>
		MATH_API Angle(Angle&& angle);

		/// <summary> The angle destructor. </summary>
		MATH_API ~Angle()
		{
		}
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		/// <summary> Returns the value of the angle converted accordingly to the specified <paramref name="unitType"/>. </summary>
		/// <returns> The value of the angle in the specified <paramref name="unitType"/>. </returns>
		MATH_API inline Real Get(Unit::UnitType unitType) const
		{
#ifdef PROFILING_MATH_MODULE_ENABLED
			START_PROFILING_MATH(false, "");
			Real angleValue = (unitType == Unit::DEGREE) ? ToDeg(m_angle) : m_angle;
			STOP_PROFILING_MATH("");
			return angleValue;
#else
			switch (unitType)
			{
			case Unit::DEGREE:
				return ToDeg(m_angle);
			case Unit::RADIAN:
				return m_angle;
			default:
				ERROR_LOG_MATH("Unknown unit type specified: ", unitType);
				return m_angle;
			}
#endif
		}

		/// <summary> Sets the value of the angle to the given <paramref name="angle"/> specified in the passed <paramref name="unitType"/>. </summary>
		MATH_API void Set(Real angle, Unit::UnitType unitType)
		{
			START_PROFILING_MATH(false, "");
			switch (unitType)
			{
			case Unit::DEGREE:
				m_angle = ToRad(angle);
				break;
			case Unit::RADIAN:
				m_angle = angle;
				break;
			default:
				ERROR_LOG_MATH("Unknown unit type specified: ", unitType);
			}
			STOP_PROFILING_MATH("");
		}

		/// <summary>
		/// Calculates and returns the sine of the current angle.
		/// </summary>
		/// <returns>The precise value of the sine of the current angle.</returns>
		/// <remarks>
		/// This function is more precise than FastSin1() or FastSin2() at the expense of its speed. It is way slower than the other ones.
		/// </remarks>
		MATH_API inline Real Sin() const
		{
#ifdef PROFILING_MATH_MODULE_ENABLED
			START_PROFILING_MATH(false, "");
			Math::Real sinValue = sin(m_angle);
			STOP_PROFILING_MATH("");
			return sinValue;
#else
			return sin(m_angle);
#endif
		}

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
		MATH_API inline Real Cos() const
		{
#ifdef PROFILING_MATH_MODULE_ENABLED
			START_PROFILING_MATH(false, "");
			Math::Real cosValue = cos(m_angle);
			STOP_PROFILING_MATH("");
			return cosValue;
#else
			return cos(m_angle);
#endif
		}

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
		MATH_API inline Real Tan() const
		{
#ifdef PROFILING_MATH_MODULE_ENABLED
			START_PROFILING_MATH(false, "");
			Math::Real tanValue = tan(m_angle);
			STOP_PROFILING_MATH("");
			return tanValue;
#else
			return tan(m_angle);
#endif
		}

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
		/// For correct results the angle must be in range [0; pi / 2 (although the book "3D Game Engine Architecture" by David H. Eberly says "2" may be an error, it is likely to be 4 as in FastTan1())].
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

		/// <summary> Angle copy assignment operator. </summary>
		MATH_API Angle& operator=(const Angle& angle);
		/// <summary> Angle move assignment operator. </summary>
		MATH_API Angle& operator=(Angle&& angle);
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
		/// <summary>
		/// The value of an angle, specified in <code>Unit::RADIAN</code>.
		/// </summary>
		Real m_angle;

#ifdef PROFILING_MATH_MODULE_ENABLED
		Statistics::ClassStats& m_classStats;
#endif
		/* ==================== Non-static member variables end ==================== */
	}; /* end class Angle */

	inline Real Angle::FastSin1() const
	{
#ifdef PROFILING_MATH_MODULE_ENABLED
		START_PROFILING_MATH(false, "");
		// TODO: Range checking (the angle, converted to radians, must be in range [0; pi / 2]!).
		// In case the angle is outside specified range use: https://pl.wikipedia.org/wiki/Trygonometryczne_wzory_redukcyjne.
		constexpr Real ANGLE_SECOND_POWER_FACTOR = static_cast<Real>(0.16605);
		constexpr Real ANGLE_FOURTH_POWER_FACTOR = static_cast<Real>(0.00761);
		Real sinValue = m_angle - (ANGLE_SECOND_POWER_FACTOR * m_angle * m_angle * m_angle) +
			(ANGLE_FOURTH_POWER_FACTOR * m_angle * m_angle * m_angle * m_angle * m_angle);
		STOP_PROFILING_MATH("");
		return sinValue;
#else
		// TODO: Range checking (the angle, converted to radians, must be in range [0; pi / 2]!).
		// In case the angle is outside specified range use: https://pl.wikipedia.org/wiki/Trygonometryczne_wzory_redukcyjne.
		constexpr Real ANGLE_SECOND_POWER_FACTOR = static_cast<Real>(0.16605);
		constexpr Real ANGLE_FOURTH_POWER_FACTOR = static_cast<Real>(0.00761);
		return m_angle - (ANGLE_SECOND_POWER_FACTOR * m_angle * m_angle * m_angle) +
			(ANGLE_FOURTH_POWER_FACTOR * m_angle * m_angle * m_angle * m_angle * m_angle);
#endif
	}

	inline Real Angle::FastSin2() const
	{
#ifdef PROFILING_MATH_MODULE_ENABLED
		START_PROFILING_MATH(false, "");
		// TODO: Range checking (the angle, converted to radians, must be in range [0; pi / 2]!).
		constexpr Real ANGLE_SECOND_POWER_FACTOR = static_cast<Real>(0.1666666664);
		constexpr Real ANGLE_FOURTH_POWER_FACTOR = static_cast<Real>(0.0083333315);
		constexpr Real ANGLE_SIXTH_POWER_FACTOR = static_cast<Real>(0.0001984090);
		constexpr Real ANGLE_EIGHTH_POWER_FACTOR = static_cast<Real>(0.0000027526);
		constexpr Real ANGLE_TENTH_POWER_FACTOR = static_cast<Real>(0.0000000239);
		Real sinValue = m_angle - (ANGLE_SECOND_POWER_FACTOR * pow(m_angle, 3)) + (ANGLE_FOURTH_POWER_FACTOR * pow(m_angle, 5)) - (ANGLE_SIXTH_POWER_FACTOR * pow(m_angle, 7)) +
			(ANGLE_EIGHTH_POWER_FACTOR * pow(m_angle, 9)) - (ANGLE_TENTH_POWER_FACTOR * pow(m_angle, 11));
		STOP_PROFILING_MATH("");
		return sinValue;
#else
		// TODO: Range checking (the angle, converted to radians, must be in range [0; pi / 2]!).
		constexpr Real ANGLE_SECOND_POWER_FACTOR = static_cast<Real>(0.1666666664);
		constexpr Real ANGLE_FOURTH_POWER_FACTOR = static_cast<Real>(0.0083333315);
		constexpr Real ANGLE_SIXTH_POWER_FACTOR = static_cast<Real>(0.0001984090);
		constexpr Real ANGLE_EIGHTH_POWER_FACTOR = static_cast<Real>(0.0000027526);
		constexpr Real ANGLE_TENTH_POWER_FACTOR = static_cast<Real>(0.0000000239);
		return m_angle - (ANGLE_SECOND_POWER_FACTOR * pow(m_angle, 3)) + (ANGLE_FOURTH_POWER_FACTOR * pow(m_angle, 5)) - (ANGLE_SIXTH_POWER_FACTOR * pow(m_angle, 7)) +
			(ANGLE_EIGHTH_POWER_FACTOR * pow(m_angle, 9)) - (ANGLE_TENTH_POWER_FACTOR * pow(m_angle, 11));
#endif
	}

	inline Real Angle::FastCos1() const
	{
#ifdef PROFILING_MATH_MODULE_ENABLED
		START_PROFILING_MATH(false, "");
		// TODO: Range checking (the angle, converted to radians, must be in range [0; pi / 2]!).
		// In case the angle is outside specified range use: https://pl.wikipedia.org/wiki/Trygonometryczne_wzory_redukcyjne.
		constexpr Real ANGLE_SECOND_POWER_FACTOR = static_cast<Real>(0.4967);
		constexpr Real ANGLE_FOURTH_POWER_FACTOR = static_cast<Real>(0.03705);
		Real cosValue = REAL_ONE - (ANGLE_SECOND_POWER_FACTOR * m_angle * m_angle) + (ANGLE_FOURTH_POWER_FACTOR * m_angle * m_angle * m_angle * m_angle);
		STOP_PROFILING_MATH("");
		return cosValue;
#else
		// TODO: Range checking (the angle, converted to radians, must be in range [0; pi / 2]!).
		// In case the angle is outside specified range use: https://pl.wikipedia.org/wiki/Trygonometryczne_wzory_redukcyjne.
		constexpr Real ANGLE_SECOND_POWER_FACTOR = static_cast<Real>(0.4967);
		constexpr Real ANGLE_FOURTH_POWER_FACTOR = static_cast<Real>(0.03705);
		return REAL_ONE - (ANGLE_SECOND_POWER_FACTOR * m_angle * m_angle) + (ANGLE_FOURTH_POWER_FACTOR * m_angle * m_angle * m_angle * m_angle);
#endif
	}

	inline Real Angle::FastCos2() const
	{
#ifdef PROFILING_MATH_MODULE_ENABLED
		START_PROFILING_MATH(false, "");
		// TODO: Range checking (the angle, converted to radians, must be in range [0; pi / 2]!).
		constexpr Real ANGLE_SECOND_POWER_FACTOR = static_cast<Real>(0.4999999963);
		constexpr Real ANGLE_FOURTH_POWER_FACTOR = static_cast<Real>(0.0416666418);
		constexpr Real ANGLE_SIXTH_POWER_FACTOR = static_cast<Real>(0.0013888397);
		constexpr Real ANGLE_EIGHTH_POWER_FACTOR = static_cast<Real>(0.0000247609);
		constexpr Real ANGLE_TENTH_POWER_FACTOR = static_cast<Real>(0.0000002605);
		Real cosValue = REAL_ONE - (ANGLE_SECOND_POWER_FACTOR * pow(m_angle, 2)) + (ANGLE_FOURTH_POWER_FACTOR * pow(m_angle, 4)) -
			(ANGLE_SIXTH_POWER_FACTOR * pow(m_angle, 6)) + (ANGLE_EIGHTH_POWER_FACTOR * pow(m_angle, 8)) - (ANGLE_TENTH_POWER_FACTOR * pow(m_angle, 10));
		STOP_PROFILING_MATH("");
		return cosValue;
#else
		// TODO: Range checking (the angle, converted to radians, must be in range [0; pi / 2]!).
		constexpr Real ANGLE_SECOND_POWER_FACTOR = static_cast<Real>(0.4999999963);
		constexpr Real ANGLE_FOURTH_POWER_FACTOR = static_cast<Real>(0.0416666418);
		constexpr Real ANGLE_SIXTH_POWER_FACTOR = static_cast<Real>(0.0013888397);
		constexpr Real ANGLE_EIGHTH_POWER_FACTOR = static_cast<Real>(0.0000247609);
		constexpr Real ANGLE_TENTH_POWER_FACTOR = static_cast<Real>(0.0000002605);
		return REAL_ONE - (ANGLE_SECOND_POWER_FACTOR * pow(m_angle, 2)) + (ANGLE_FOURTH_POWER_FACTOR * pow(m_angle, 4)) -
			(ANGLE_SIXTH_POWER_FACTOR * pow(m_angle, 6)) + (ANGLE_EIGHTH_POWER_FACTOR * pow(m_angle, 8)) - (ANGLE_TENTH_POWER_FACTOR * pow(m_angle, 10));
#endif
	}

	inline Real Angle::FastTan1() const
	{
#ifdef PROFILING_MATH_MODULE_ENABLED
		START_PROFILING_MATH(false, "");
		// TODO: Range checking (the angle, converted to radians, must be in range [0; pi / 4]!).
		constexpr Real ANGLE_SECOND_POWER_FACTOR = static_cast<Real>(0.31755);
		constexpr Real ANGLE_FOURTH_POWER_FACTOR = static_cast<Real>(0.2033);
		Real tanValue = static_cast<Real>(m_angle + ANGLE_SECOND_POWER_FACTOR * pow(m_angle, 3) + ANGLE_FOURTH_POWER_FACTOR * pow(m_angle, 5));
		STOP_PROFILING_MATH("");
		return tanValue;
#else
		// TODO: Range checking (the angle, converted to radians, must be in range [0; pi / 4]!).
		constexpr Real ANGLE_SECOND_POWER_FACTOR = static_cast<Real>(0.31755);
		constexpr Real ANGLE_FOURTH_POWER_FACTOR = static_cast<Real>(0.2033);
		return static_cast<Real>(m_angle + ANGLE_SECOND_POWER_FACTOR * pow(m_angle, 3) + ANGLE_FOURTH_POWER_FACTOR * pow(m_angle, 5));
#endif
	}

	inline Real Angle::FastTan2() const
	{
#ifdef PROFILING_MATH_MODULE_ENABLED
		START_PROFILING_MATH(false, "");
		// TODO: Range checking (the angle, converted to radians, must be in range [0; pi / 2]!).
		//constexpr Real ANGLE_SECOND_POWER_FACTOR = static_cast<Real>(0.4999999963);
		//constexpr Real ANGLE_FOURTH_POWER_FACTOR = static_cast<Real>(0.0416666418);
		//constexpr Real ANGLE_SIXTH_POWER_FACTOR = static_cast<Real>(0.0013888397);
		//constexpr Real ANGLE_EIGHTH_POWER_FACTOR = static_cast<Real>(0.0000247609);
		//constexpr Real ANGLE_TENTH_POWER_FACTOR = static_cast<Real>(0.0000002605);
		Real tanValue = static_cast<Real>(m_angle + 0.3333314036 * pow(m_angle, 3) + 0.1333923995 * pow(m_angle, 5) +
			0.0533740603 * pow(m_angle, 7) + 0.0245650893 * pow(m_angle, 9) + 0.0029005250 * pow(m_angle, 11) + 0.0095168091 * pow(m_angle, 13));
		STOP_PROFILING_MATH("");
		return tanValue;
#else
		// TODO: Range checking (the angle, converted to radians, must be in range [0; pi / 2]!).
		//constexpr Real ANGLE_SECOND_POWER_FACTOR = static_cast<Real>(0.4999999963);
		//constexpr Real ANGLE_FOURTH_POWER_FACTOR = static_cast<Real>(0.0416666418);
		//constexpr Real ANGLE_SIXTH_POWER_FACTOR = static_cast<Real>(0.0013888397);
		//constexpr Real ANGLE_EIGHTH_POWER_FACTOR = static_cast<Real>(0.0000247609);
		//constexpr Real ANGLE_TENTH_POWER_FACTOR = static_cast<Real>(0.0000002605);
		return static_cast<Real>(m_angle + 0.3333314036 * pow(m_angle, 3) + 0.1333923995 * pow(m_angle, 5) +
			0.0533740603 * pow(m_angle, 7) + 0.0245650893 * pow(m_angle, 9) + 0.0029005250 * pow(m_angle, 11) + 0.0095168091 * pow(m_angle, 13));
#endif
	}

} /* end namespace Math */

#endif /* __MATH_ANGLE_H__ */