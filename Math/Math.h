#ifndef __MATH_MATH_H__
#define __MATH_MATH_H__

#ifdef MATH_EXPORTS
#define MATH_API __declspec(dllexport)
#else
#define MATH_API __declspec(dllimport)
#endif

#define CALCULATE_MATH_STATS
#ifndef CALCULATE_MATH_STATS
#define START_PROFILING_STATIC
#define STOP_PROFILING_STATIC
#define START_PROFILING
#define STOP_PROFILING
#endif

#define EPSILON		static_cast<Math::Real>(1e-6)
#define REAL_ZERO	static_cast<Math::Real>(0)
#define REAL_ONE	static_cast<Math::Real>(1)

//#define NOMINMAX
#include <iostream>
#include <limits>

#undef min
#undef max
#define REAL_MIN	static_cast<Math::Real>(std::numeric_limits<Math::Real>::min())
#define REAL_MAX	static_cast<Math::Real>(std::numeric_limits<Math::Real>::max())

#define GET_CONFIG_VALUE_MATH(cfgName, defValue) GET_CONFIG_VALUE("Math", cfgName, defValue)
#define GET_CONFIG_VALUE_STR_MATH(cfgName, defValue) GET_CONFIG_VALUE_STR("Math", cfgName, defValue)

#define CRITICAL_LOG_MATH(...) CRITICAL_LOG("Math", ##__VA_ARGS__)
#define EMERGENCY_LOG_MATH(...) EMERGENCY_LOG("Math", ##__VA_ARGS__)
#define ERROR_LOG_MATH(...) ERROR_LOG("Math", ##__VA_ARGS__)
#define WARNING_LOG_MATH(...) WARNING_LOG("Math", ##__VA_ARGS__)
#define NOTICE_LOG_MATH(...) NOTICE_LOG("Math", ##__VA_ARGS__)
#define INFO_LOG_MATH(...) INFO_LOG("Math", ##__VA_ARGS__)
#define DEBUG_LOG_MATH(...) DEBUG_LOG("Math", ##__VA_ARGS__)
#define DELOCUST_LOG_MATH(...) DELOCUST_LOG("Math", ##__VA_ARGS__)

#define CHECK_CONDITION_MATH(expr, logLevel, ...) CHECK_CONDITION(expr, "Math", logLevel, ##__VA_ARGS__)
#define CHECK_CONDITION_ALWAYS_MATH(expr, logLevel, ...) CHECK_CONDITION_ALWAYS(expr, "Math", logLevel, ##__VA_ARGS__)
#define CHECK_CONDITION_EXIT_MATH(expr, logLevel, ...) CHECK_CONDITION_EXIT(expr, "Math", logLevel, ##__VA_ARGS__)
#define CHECK_CONDITION_EXIT_ALWAYS_MATH(expr, logLevel, ...) CHECK_CONDITION_EXIT_ALWAYS(expr, "Math", logLevel, ##__VA_ARGS__)
#define CHECK_CONDITION_RETURN_VOID_MATH(expr, logLevel, ...) CHECK_CONDITION_RETURN_VOID(expr, "Math", logLevel, ##__VA_ARGS__)
#define CHECK_CONDITION_RETURN_VOID_ALWAYS_MATH(expr, logLevel, ...) CHECK_CONDITION_RETURN_VOID_ALWAYS(expr, "Math", logLevel, ##__VA_ARGS__)
#define CHECK_CONDITION_RETURN_MATH(expr, returnValue, logLevel, ...) CHECK_CONDITION_RETURN(expr, returnValue, "Math", logLevel, ##__VA_ARGS__)
#define CHECK_CONDITION_RETURN_ALWAYS_MATH(expr, returnValue, logLevel, ...) CHECK_CONDITION_RETURN_ALWAYS(expr, returnValue, "Math", logLevel, ##__VA_ARGS__)

#include <math.h>
#include <cmath>

namespace Math
{
	typedef float Real;

	/// <summary> The constant value of PI (https://pl.wikipedia.org/wiki/Pi) </summary>
	/// <remarks>
	/// The reason why the value of PI is computed rather than explictly specified is to try maintain
	/// as much precision as possible for the numbers and to maintain consistency of the values across files.
	/// As an example of inconsistency, I have seen code distributions with one file containing:
	/// <code>#define PI 3.1415</code>
	/// and another file containing:
	/// <code>#define PI 3.141593</code>
	/// </remarks>
	const Real PI = static_cast<Real>(3.14159265358979323846); //static_cast<Real>(4.0 * atan(1.0));
	
	/// <summary> The constant multiplier that converts degrees to radians </summary>
	const Real DEG_TO_RAD_MULTIPLIER = PI / 180;

	/// <summary> The constant multiplier that converts radians to degrees </summary>
	const Real RAD_TO_DEG_MULTIPLIER = 180 / PI;

	template<typename Type>
	Type Absolute(Type v)
	{
		return (v < 0) ? -v : v;
	}

	template<typename Type> Type ToRad(Type angleInDegrees)
	{
		return static_cast<Type>(Math::DEG_TO_RAD_MULTIPLIER * angleInDegrees);
	}

	template<typename Type> Type ToDeg(Type angleInRadians)
	{
		return static_cast<Type>(Math::RAD_TO_DEG_MULTIPLIER * angleInRadians);
	}

	template<typename Type> int Floor(Type value)
	{
		return static_cast<int>(floor(value));
	}

//	Math::Real RealMin()
//	{
//#undef max
//		return static_cast<Math::Real>(std::numeric_limits<float>::max());
//	}

	template<typename Type> Type Clamp(const Type& a, const Type& min, const Type& max)
	{
		if (a < min) return min;
		else if (a > max) return max;
		else return a;
	}

	template<typename Type> Type Lerp(const Type& a, const Type& b, const Type& mixFactor)
	{
		return ((1 - mixFactor) * a) + (mixFactor * b);
	}

	template<typename Type> Type RoundUpPow2(Type number)
	{
		if (number < 0)
		{
			return 0;
		}
		--number;
		number |= number >> 1;
		number |= number >> 2;
		number |= number >> 4;
		number |= number >> 8;
		number |= number >> 16;
		return number + 1;
	}
} /* end namespace Math */

#endif /* __MATH_MATH_H__ */