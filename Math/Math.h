#pragma once

#ifdef MATH_EXPORTS
#define MATH_API __declspec(dllexport)
#else
#define MATH_API __declspec(dllimport)
#endif

#include <math.h>
#include <cmath>
#include <iostream>
#include <limits>

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
} /* end namespace Math */