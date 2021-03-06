#ifndef __MATH_MATH_H__
#define __MATH_MATH_H__

#ifdef MATH_EXPORTS
#define MATH_API __declspec(dllexport)
#else
#define MATH_API __declspec(dllimport)
#endif

#define EPSILON		static_cast<math::Real>(1e-6)
#define REAL_ZERO	static_cast<math::Real>(0)
#define REAL_ONE	static_cast<math::Real>(1)

//#define NOMINMAX

#undef min
#undef max
#define REAL_MIN	static_cast<math::Real>(std::numeric_limits<math::Real>::min())
#define REAL_MAX	static_cast<math::Real>(std::numeric_limits<math::Real>::max())

#define GET_CONFIG_VALUE_MATH(cfgName, defValue) GET_CONFIG_VALUE("Math", cfgName, defValue)
#define GET_CONFIG_VALUE_STR_MATH(cfgName, defValue) GET_CONFIG_VALUE_STR("Math", cfgName, defValue)
#define HAS_CONFIG_VALUE_MATH(cfgName) HAS_CONFIG_VALUE("Math", cfgName)

#define CRITICAL_LOG_MATH(...) CRITICAL_LOG("Math", ##__VA_ARGS__)
#define EMERGENCY_LOG_MATH(...) EMERGENCY_LOG("Math", ##__VA_ARGS__)
#define ERROR_LOG_MATH(...) ERROR_LOG("Math", ##__VA_ARGS__)
#define WARNING_LOG_MATH(...) WARNING_LOG("Math", ##__VA_ARGS__)
#define NOTICE_LOG_MATH(...) NOTICE_LOG("Math", ##__VA_ARGS__)
#define INFO_LOG_MATH(...) INFO_LOG("Math", ##__VA_ARGS__)
#define DEBUG_LOG_MATH(...) DEBUG_LOG("Math", ##__VA_ARGS__)
#define DELOCUST_LOG_MATH(...) DELOCUST_LOG("Math", ##__VA_ARGS__)

//#define PROFILING_MATH_MODULE_ENABLED
#ifdef PROFILING_MATH_MODULE_ENABLED
#define START_PROFILING_STATIC_MATH(measureTime, param) START_PROFILING_STATIC("Math", measureTime, param)
#define STOP_PROFILING_STATIC_MATH(param) STOP_PROFILING_STATIC("Math", param)
#define START_PROFILING_MATH(measureTime, param) START_PROFILING("Math", measureTime, param)
#define STOP_PROFILING_MATH(param) STOP_PROFILING("Math", param)
#define CONSTEXPR_IF_PROFILING_DISABLED_MATH
#define CONSTEXPR_IF_PROFILING_DISABLED_OR_CONST_MATH const
#else
#define START_PROFILING_STATIC_MATH(measureTime, param)
#define STOP_PROFILING_STATIC_MATH(param)
#define START_PROFILING_MATH(measureTime, param)
#define STOP_PROFILING_MATH(param)
#define CONSTEXPR_IF_PROFILING_DISABLED_MATH constexpr
#define CONSTEXPR_IF_PROFILING_DISABLED_OR_CONST_MATH constexpr
#endif

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

namespace math
{
	using Real = float;

	/// <summary> The constant value of PI (https://pl.wikipedia.org/wiki/Pi) </summary>
	/// <remarks>
	/// The reason why the value of PI is computed rather than explictly specified is to try maintain
	/// as much precision as possible for the numbers and to maintain consistency of the values across files.
	/// As an example of inconsistency, I have seen code distributions with one file containing:
	/// <code>#define PI 3.1415</code>
	/// and another file containing:
	/// <code>#define PI 3.141593</code>
	/// </remarks>
	constexpr Real PI = static_cast<Real>(3.14159265358979323846); //static_cast<Real>(4.0 * atan(1.0));
	
	/// <summary> The constant multiplier that converts degrees to radians </summary>
	constexpr Real DEG_TO_RAD_MULTIPLIER = PI / 180;

	/// <summary> The constant multiplier that converts radians to degrees </summary>
	constexpr Real RAD_TO_DEG_MULTIPLIER = 180 / PI;

	template<typename Type>
	constexpr Type Absolute(Type v) noexcept
	{
		return (v < 0) ? -v : v;
	}

	template<typename Type>
	constexpr Type ToRad(Type angleInDegrees) noexcept
	{
		return static_cast<Type>(DEG_TO_RAD_MULTIPLIER * angleInDegrees);
	}

	template<typename Type>
	constexpr Type ToDeg(Type angleInRadians) noexcept
	{
		return static_cast<Type>(RAD_TO_DEG_MULTIPLIER * angleInRadians);
	}

	template<typename Type>
	constexpr Type Avg(Type value1, Type value2) noexcept
	{
		return (value1 + value2) / 2;
	}

	template<typename Type>
	constexpr int Floor(Type value)
	{
		return static_cast<int>(floor(value));
	}

	template<typename Type>
	constexpr int Ceil(Type value)
	{
		return static_cast<int>(ceil(value));
	}

//	Real RealMin()
//	{
//#undef max
//		return static_cast<Real>(std::numeric_limits<float>::max());
//	}

	template<typename Type>
	constexpr Type Clamp(const Type& a, const Type& min, const Type& max) noexcept
	{
		return (a < min) ? min : ((a > max) ? max : a);
	}

	//template<typename Type> Type Lerp(const Type& a, const Type& b, const Type& mixFactor)
	//{
	//	return ((1 - mixFactor) * a) + (mixFactor * b);
	//}

	/// <summary>
	/// For unsigned integer-like values it returns the minimum integer-like value that is both a power of <code>2</code>
	/// and greater or equal to the given <paramref name="number"/>. For example, for a given set of values [ -3, 0, 1, 2, 5, 10 ]
	/// the function would return [ 0, 0, 1, 2, 8, 16 ] respectively.
	/// </summary>
	template<typename Type>
	Type RoundUpPow2(Type number) noexcept
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
} /* end namespace math */

#endif /* __MATH_MATH_H__ */