#include "stdafx.h"
#ifndef __MATH_INTERPOLATION_IMPL_H__
#define __MATH_INTERPOLATION_IMPL_H__

#include "Interpolation.h"
#include "Quaternion.h"
#include "FloatingPoint.h"
#include "Utility/ILogger.h"

/* ==================== class Interpolator begin ==================== */
template <class T>
math::interpolation::Interpolator<T>::Interpolator()
{
	//m_interpolationObjects.push_back(firstInterpolationObject);
	//m_times.push_back(REAL_ZERO);
}

template <class T>
math::interpolation::Interpolator<T>::Interpolator(const T* interpolationObjects, const Real* times, int interpolationObjectsCount)
{
	for (auto i = 0; i < interpolationObjectsCount; ++i)
	{
		m_interpolationObjects.push_back(interpolationObjects[i]);
		m_times.push_back(times[i]);
	}
}

template <class T>
math::interpolation::Interpolator<T>::~Interpolator()
{
}

template <typename T>
void math::interpolation::Interpolator<T>::PushInterpolationObject(const T& interpolationObject, Real time)
{
	std::vector<T>::iterator interpolationObjectItr = m_interpolationObjects.begin();
	for (auto timeItr = m_times.begin(); timeItr != m_times.end(); ++timeItr, ++interpolationObjectItr)
	{
		if (time < *timeItr)
		{
			m_interpolationObjects.insert(interpolationObjectItr, interpolationObject);
			m_times.insert(timeItr, time);
			return;
		}
		if (AlmostEqual(time, *timeItr))
		{
			*interpolationObjectItr = interpolationObject;
			return;
		}
	}
}
/* ==================== class Interpolator end ==================== */

/* ==================== class LinearInterpolator begin ==================== */
template <class T>
math::interpolation::LinearInterpolator<T>::LinearInterpolator() :
	Interpolator()
{
}

template <class T>
math::interpolation::LinearInterpolator<T>::LinearInterpolator(const T* interpolationObjects, const Real* times, int interpolationObjectsCount) :
	Interpolator(interpolationObjects, times, interpolationObjectsCount)
{
}

template <class T>
math::interpolation::LinearInterpolator<T>::~LinearInterpolator()
{
}

template <class T>
T math::interpolation::LinearInterpolator<T>::Interpolate(Real time) const
{
	// Handle boundary conditions
	if (time < GetStartTime() || (m_times.size() == 1))
	{
		return m_interpolationObjects.front();
	}
	if (time > GetEndTime())
	{
		return m_interpolationObjects.back();
	}

	// Find segment and parameter
	unsigned int index = 1;
	for (std::vector<Real>::const_iterator timeItr = m_times.begin() + 1; timeItr != m_times.end(); ++timeItr, ++index)
	{
		if (time < *timeItr)
		{
			break;
		}
	}
	auto time0 = m_times[index - 1];
	auto time1 = m_times[index];
	auto interpolationFactor = (time - time0) / (time1 - time0);

	return InterpolateLinear(m_interpolationObjects[index - 1], m_interpolationObjects[index], interpolationFactor);
}
/* ==================== class LinearInterpolator end ==================== */

/* ==================== class CosineInterpolator begin ==================== */
template <class T>
math::interpolation::CosineInterpolator<T>::CosineInterpolator() :
	Interpolator()
{
}

template <class T>
math::interpolation::CosineInterpolator<T>::CosineInterpolator(const T* interpolationObjects, const Real* times, int interpolationObjectsCount) :
	Interpolator(interpolationObjects, times, interpolationObjectsCount)
{
}

template <class T>
math::interpolation::CosineInterpolator<T>::~CosineInterpolator()
{
}

template <class T>
T math::interpolation::CosineInterpolator<T>::Interpolate(Real time) const
{
	// Handle boundary conditions
	if (time < GetStartTime() || (m_times.size() == 1))
	{
		return m_interpolationObjects.front();
	}
	if (time > GetEndTime())
	{
		return m_interpolationObjects.back();
	}

	// Find segment and parameter
	unsigned int index = 1;
	for (std::vector<Real>::const_iterator timeItr = m_times.begin() + 1; timeItr != m_times.end(); ++timeItr, ++index)
	{
		if (time < *timeItr)
		{
			break;
		}
	}
	Real time0 = m_times[index - 1];
	Real time1 = m_times[index];
	auto interpolationFactor = (time - time0) / (time1 - time0);

	return InterpolateCosine(m_interpolationObjects[index - 1], m_interpolationObjects[index], interpolationFactor);
}
/* ==================== class CosineInterpolator end ==================== */

/* ==================== class HermiteInterpolator begin ==================== */
template <class T>
math::interpolation::HermiteInterpolator<T>::HermiteInterpolator() :
	Interpolator(),
	m_derivatives(nullptr)
{
}

template <class T>
math::interpolation::HermiteInterpolator<T>::HermiteInterpolator(const T* interpolationObjects, const Real* times, int interpolationObjectsCount) :
	Interpolator(interpolationObjects, times, interpolationObjectsCount),
	m_derivatives(nullptr)
{
	CalculateDerivatives(interpolationObjects, interpolationObjectsCount, T(REAL_ZERO), T(REAL_ZERO));
}

template <class T>
math::interpolation::HermiteInterpolator<T>::HermiteInterpolator(const T* interpolationObjects, const Real* times, int interpolationObjectsCount, const T& derivative0, const T& derivativeN) :
	Interpolator(interpolationObjects, times, interpolationObjectsCount),
	m_derivatives(interpolationObjectsCount)
{
	CalculateDerivatives(interpolationObjects, interpolationObjectsCount, derivative0, derivativeN);
}

template <class T>
math::interpolation::HermiteInterpolator<T>::HermiteInterpolator(const T* interpolationObjects, const Real* times, const T* derivatives, int interpolationObjectsCount) :
	Interpolator(interpolationObjects, times, interpolationObjectsCount),
	m_derivatives(derivatives, derivatives + interpolationObjectsCount)
{
}

template <class T>
math::interpolation::HermiteInterpolator<T>::~HermiteInterpolator()
{
}

template <class T>
void math::interpolation::HermiteInterpolator<T>::CalculateDerivatives(const T* interpolationObjects, int interpolationObjectsCount, const T& derivative0, const T& derivativeN)
{
	// TODO: Calculate derivatives according to the "Essential Mathematics for Games and Interactive Applications 2nd Edition"- section 10.2.3 ("Hermite curves").
	WARNING_LOG_MATH("This function has not been tested yet");
	std::vector<Real> c, d; // auxiliary vectors
	c.reserve(interpolationObjectsCount - 1);
	d.reserve(interpolationObjectsCount);
	c.push_back(REAL_ZERO);
	d.push_back(derivative0);
	for (auto i = 1; i < interpolationObjectsCount - 1; ++i) // TODO: Try to change into the std::vector<Real>::const_iterator loop
	{
		c.push_back(REAL_ONE / (4.0f - c[i - 1]));
		d.push_back((3.0f * (interpolationObjects[i + 1] - interpolationObjects[i - 1]) - d[i - 1]) / (4.0f - c[i - 1]));
	}
	d.push_back(derivativeN - d[interpolationObjectsCount - 2]);

	m_derivatives[interpolationObjectsCount - 1] = d[interpolationObjectsCount - 1];
	for (auto i = interpolationObjectsCount - 1; i >= 0; --i)
	{
		m_derivatives[i] = d[i] - c[i] * m_derivatives[i + 1];
	}
}

template <class T>
T math::interpolation::HermiteInterpolator<T>::Interpolate(Real time) const
{
	WARNING_LOG_MATH("This function has not been tested yet");
	// Handle boundary conditions
	if (time < GetStartTime())
	{
		return m_interpolationObjects[0];
	}
	if (time > GetEndTime())
	{
		return m_interpolationObjects[m_interpolationObjectsCount - 1];
	}

	// Find segment and parameter
	unsigned int index = 0;
	for (; index < m_interpolationObjectsCount; ++index)
	{
		if (t < m_times[index + 1])
		{
			break;
		}
	}
	Real time0 = m_times[index];
	Real time1 = m_times[index + 1];
	auto interpolationFactor = (time - time0) / (time1 - time0);

	// Evaluate the interpolation
	return InterpolateHermite(m_interpolationObjects[index + 1], m_interpolationObjects[index], interpolationFactor);
	//return ((REAL_ONE - interpolationFactor) * m_interpolationObjects[index]) + (interpolationFactor * m_interpolationObjects[index + 1]);
}
/* ==================== class HermiteInterpolator end ==================== */

/* ==================== class BarycentricInterpolator begin ==================== */
template <class T>
math::interpolation::BarycentricInterpolator<T>::BarycentricInterpolator() :
	Interpolator()
{
}

template <class T>
math::interpolation::BarycentricInterpolator<T>::BarycentricInterpolator(const T* interpolationObjects, const Real* times, int interpolationObjectsCount) :
	Interpolator(interpolationObjects, times, interpolationObjectsCount)
{
}

template <class T>
math::interpolation::BarycentricInterpolator<T>::~BarycentricInterpolator()
{
}

template <class T>
T math::interpolation::BarycentricInterpolator<T>::Interpolate(Real time) const
{
	WARNING_LOG_MATH("This function has not been tested yet");
	// Handle boundary conditions
	if (time < GetStartTime())
	{
		return m_interpolationObjects[0];
	}
	if (time > GetEndTime())
	{
		return m_interpolationObjects[m_interpolationObjectsCount - 1];
	}

	// Find segment and parameter
	unsigned int index = 0;
	for (; index < m_interpolationObjectsCount; ++index)
	{
		if (t < m_times[index + 1])
		{
			break;
		}
	}
	Real time0 = m_times[index];
	Real time1 = m_times[index + 1];
	auto interpolationFactor = (time - time0) / (time1 - time0);

	// Evaluate the interpolation
	return InterpolateBarycentric(m_interpolationObjects[index + 1], m_interpolationObjects[index], interpolationFactor);
	//return ((REAL_ONE - interpolationFactor) * m_interpolationObjects[index]) + (interpolationFactor * m_interpolationObjects[index + 1]);
}
/* ==================== class BarycentricInterpolator end ==================== */

template <class T>
T math::interpolation::Interpolate(interpolation_types::InterpolationType interpolationType, const T& value1, const T& value2, Real factor)
{
	switch (interpolationType)
	{
	case interpolation_types::LINEAR:
		return InterpolateLinear(value1, value2, factor);
	case interpolation_types::COSINE:
		return InterpolateCosine(value1, value2, factor);
	case interpolation_types::HERMITE:
		return InterpolateHermite(value1, value2, factor);
	case interpolation_types::BARYCENTRIC:
		return InterpolateBarycentric(value1, value2, factor);
	default:
		WARNING_LOG_MATH("Incorrect interpolation type specified (", interpolationType, "). Linear interpolation will be used instead.");
		return InterpolateLinear(value1, value2, factor);
	}
}

template <class T>
T math::interpolation::InterpolateLinear(const T& value1, const T& value2, Real factor)
{
	return (value1 * (REAL_ONE - factor)) + (value2 * factor);
}

template <class T>
T math::interpolation::InterpolateCosine(const T& value1, const T& value2, Real factor)
{
	const Angle angle(factor * PI, units::RADIAN);
	const auto cosineFactor = (REAL_ONE - angle.Cos()) * 0.5f; // value from 0 to 1.
	return (value1 * (REAL_ONE - cosineFactor)) + (value2 * cosineFactor);
}

template <class T>
T math::interpolation::InterpolateHermite(const T& value1, const T& value2, Real factor)
{
	return value1;
}

template <class T>
T math::interpolation::InterpolateBarycentric(const T& value1, const T& value2, Real factor)
{
	return value1;
}

#endif /* __MATH_INTERPOLATION_IMPL_H__ */