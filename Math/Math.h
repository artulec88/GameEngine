#pragma once

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

#include <math.h>
#include <cmath>

namespace Math
{
	typedef float Real;

	const Real M_PI = 3.14159265358979323846f;
	const Real DegToRadFactor = M_PI / 180;
	const Real RadToDegFactor = 180 / M_PI;

	template<typename Type>
	Type Absolute(Type v)
	{
		return (v < 0) ? -v : v;
	}

	template<typename Type> Type ToRad(Type angleInDegrees)
	{
		return static_cast<Type>(Math::DegToRadFactor * angleInDegrees);
	}

	template<typename Type> Type ToDeg(Type angleInRadians)
	{
		return static_cast<Type>(Math::RadToDegFactor * angleInRadians);
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