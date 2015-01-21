#pragma once

#ifdef MATH_EXPORTS
#define MATH_API __declspec(dllexport)
#else
#define MATH_API __declspec(dllimport)
#endif

#define EPSILON		1e3
#define REAL_ZERO	static_cast<Math::Real>(0)
#define REAL_ONE	static_cast<Math::Real>(1)
#include <iostream>
#include <limits>
//#define REAL_MAX	static_cast<Math::Real>(std::numeric_limits<float /* replace with double when needed */>::max());


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

	template<typename Type> Type Clamp(const Type& a, const Type& min, const Type& max)
	{
		if (a < min) return min;
		else if (a > max) return max;
		else return a;
	}
} /* end namespace Math */