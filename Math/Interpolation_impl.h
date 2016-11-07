#include "stdafx.h"
#include "Interpolation.h"
#include "Vector.h"
#include "Quaternion.h"
#include "FloatingPoint.h"
#include "Utility\ILogger.h"
#include <sstream>

/* ==================== class Interpolator begin ==================== */
template <class T>
Math::Interpolation::Interpolator<T>::Interpolator()
{
	//m_interpolationObjects.push_back(firstInterpolationObject);
	//m_times.push_back(REAL_ZERO);
}

template <class T>
Math::Interpolation::Interpolator<T>::Interpolator(const T* interpolationObjects, const Real* times, int interpolationObjectsCount)
{
	for (int i = 0; i < interpolationObjectsCount; ++i)
	{
		m_interpolationObjects.push_back(interpolationObjects[i]);
		m_times.push_back(times[i]);
	}
}

template <class T>
Math::Interpolation::Interpolator<T>::~Interpolator()
{
}

template <class T>
void Math::Interpolation::Interpolator<T>::PushInterpolationObject(const T& interpolationObject, Real time)
{
	std::vector<T>::iterator interpolationObjectItr = m_interpolationObjects.begin();
	for (std::vector<Math::Real>::iterator timeItr = m_times.begin(); timeItr != m_times.end(); ++timeItr, ++interpolationObjectItr)
	{
		if (time < *timeItr)
		{
			m_interpolationObjects.insert(interpolationObjectItr, interpolationObject);
			m_times.insert(timeItr, time);
			return;
		}
		else if (AlmostEqual(time, *timeItr))
		{
			*interpolationObjectItr = interpolationObject;
			return;
		}
	}
}
/* ==================== class Interpolator end ==================== */

/* ==================== class LinearInterpolator begin ==================== */
template <class T>
Math::Interpolation::LinearInterpolator<T>::LinearInterpolator() :
	Interpolator()
{
}

template <class T>
Math::Interpolation::LinearInterpolator<T>::LinearInterpolator(const T* interpolationObjects, const Real* times, int interpolationObjectsCount) :
	Interpolator(interpolationObjects, times, interpolationObjectsCount)
{
}

template <class T>
Math::Interpolation::LinearInterpolator<T>::~LinearInterpolator()
{
}

template <class T>
T Math::Interpolation::LinearInterpolator<T>::Interpolate(const T& value1, const T& value2, Real factor) const
{
	return (value1 * (REAL_ONE - factor)) + (value2 * factor);
}

template <class T>
T Math::Interpolation::LinearInterpolator<T>::Interpolate(Math::Real time) const
{
	// Handle boundary conditions
	if (time < GetStartTime() || (m_times.size() == 1))
	{
		return m_interpolationObjects.front();
	}
	else if (time > GetEndTime())
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
	Math::Real time0 = m_times[index - 1];
	Math::Real time1 = m_times[index];
	Math::Real interpolationFactor = (time - time0) / (time1 - time0);

	return Interpolate(m_interpolationObjects[index - 1], m_interpolationObjects[index], interpolationFactor);
}
/* ==================== class LinearInterpolator end ==================== */

/* ==================== class CosineInterpolator begin ==================== */
template <class T>
Math::Interpolation::CosineInterpolator<T>::CosineInterpolator() :
	Interpolator()
{
}

template <class T>
Math::Interpolation::CosineInterpolator<T>::CosineInterpolator(const T* interpolationObjects, const Real* times, int interpolationObjectsCount) :
	Interpolator(interpolationObjects, times, interpolationObjectsCount)
{
}

template <class T>
Math::Interpolation::CosineInterpolator<T>::~CosineInterpolator()
{
}

template <class T>
T Math::Interpolation::CosineInterpolator<T>::Interpolate(const T& value1, const T& value2, Real factor) const
{
	const Angle angle(factor * PI, Math::Unit::RADIAN);
	const Real cosineFactor = (REAL_ONE - angle.Cos()) * 0.5f; // value from 0 to 1.
	return (value1 * (REAL_ONE - cosineFactor)) + (value2 * cosineFactor);
}

template <class T>
T Math::Interpolation::CosineInterpolator<T>::Interpolate(Math::Real time) const
{
	// Handle boundary conditions
	if (time < GetStartTime() || (m_times.size() == 1))
	{
		return m_interpolationObjects.front();
	}
	else if (time > GetEndTime())
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
	Math::Real time0 = m_times[index - 1];
	Math::Real time1 = m_times[index];
	Math::Real interpolationFactor = (time - time0) / (time1 - time0);

	return Interpolate(m_interpolationObjects[index - 1], m_interpolationObjects[index], interpolationFactor);
}
/* ==================== class CosineInterpolator end ==================== */

/* ==================== class HermiteInterpolator begin ==================== */
template <class T>
Math::Interpolation::HermiteInterpolator<T>::HermiteInterpolator(const T* interpolationObjects, const Real* times, int interpolationObjectsCount) :
	Interpolator(interpolationObjects, times, interpolationObjectsCount),
	m_derivatives(NULL)
{
	CalculateDerivatives(interpolationObjects, interpolationObjectsCount, T(REAL_ZERO), T(REAL_ZERO));
}

template <class T>
Math::Interpolation::HermiteInterpolator<T>::HermiteInterpolator(const T* interpolationObjects, const Real* times, int interpolationObjectsCount, const T& derivative0, const T& derivativeN) :
	Interpolator(interpolationObjects, times, interpolationObjectsCount),
	m_derivatives(interpolationObjectsCount)
{
	CalculateDerivatives(interpolationObjects, interpolationObjectsCount, derivative0, derivativeN);
}

template <class T>
Math::Interpolation::HermiteInterpolator<T>::HermiteInterpolator(const T* interpolationObjects, const Real* times, const T* derivatives, int interpolationObjectsCount) :
	Interpolator(interpolationObjects, times, interpolationObjectsCount),
	m_derivatives(derivatives, derivatives + interpolationObjectsCount)
{
}

template <class T>
Math::Interpolation::HermiteInterpolator<T>::~HermiteInterpolator()
{
}

template <class T>
void Math::Interpolation::HermiteInterpolator<T>::CalculateDerivatives(const T* interpolationObjects, int interpolationObjectsCount, const T& derivative0, const T& derivativeN)
{
	// TODO: Calculate derivatives according to the "Essential Mathematics for Games and Interactive Applications 2nd Edition"- section 10.2.3 ("Hermite curves").
	WARNING_LOG_MATH("This function has not been tested yet");
	std::vector<Real> c, d; // auxiliary vectors
	c.reserve(interpolationObjectsCount - 1);
	d.reserve(interpolationObjectsCount);
	c.push_back(REAL_ZERO);
	d.push_back(derivative0);
	for (int i = 1; i < interpolationObjectsCount - 1; ++i) // TODO: Try to change into the std::vector<Real>::const_iterator loop
	{
		c.push_back(REAL_ONE / (4.0f - c[i - 1]));
		d.push_back((3.0f * (interpolationObjects[i + 1] - interpolationObjects[i - 1]) - d[i - 1]) / (4.0f - c[i - 1]));
	}
	d.push_back(derivativeN - d[interpolationObjectsCount - 2]);

	m_derivatives[interpolationObjectsCount - 1] = d[interpolationObjectsCount - 1];
	for (int i = interpolationObjectsCount - 1; i >= 0; --i)
	{
		m_derivatives[i] = d[i] - c[i] * m_derivatives[i + 1];
	}
}

template <class T>
T Math::Interpolation::HermiteInterpolator<T>::Interpolate(Math::Real time) const
{
	WARNING_LOG_MATH("This function has not been tested yet");
	// Handle boundary conditions
	if (time < GetStartTime())
	{
		return m_interpolationObjects[0];
	}
	else if (time > GetEndTime())
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
	Math::Real time0 = m_times[index];
	Math::Real time1 = m_times[index + 1];
	Math::Real interpolationFactor = (time - time0) / (time1 - time0);

	// Evaluate the interpolation
	return ((REAL_ONE - interpolationFactor) * m_interpolationObjects[index]) + (interpolationFactor * m_interpolationObjects[index + 1]);
}
/* ==================== class HermiteInterpolator end ==================== */

/* ==================== class BarycentricInterpolator begin ==================== */
template <class T>
Math::Interpolation::BarycentricInterpolator<T>::BarycentricInterpolator(const T* interpolationObjects, const Real* times, int interpolationObjectsCount) :
	Interpolator(interpolationObjects, times, interpolationObjectsCount)
{
}

template <class T>
Math::Interpolation::BarycentricInterpolator<T>::~BarycentricInterpolator()
{
}

template <class T>
T Math::Interpolation::BarycentricInterpolator<T>::Interpolate(Math::Real time) const
{
	WARNING_LOG_MATH("This function has not been tested yet");
	// Handle boundary conditions
	if (time < GetStartTime())
	{
		return m_interpolationObjects[0];
	}
	else if (time > GetEndTime())
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
	Math::Real time0 = m_times[index];
	Math::Real time1 = m_times[index + 1];
	Math::Real interpolationFactor = (time - time0) / (time1 - time0);

	// Evaluate the interpolation
	return ((REAL_ONE - interpolationFactor) * m_interpolationObjects[index]) + (interpolationFactor * m_interpolationObjects[index + 1]);
}
/* ==================== class BarycentricInterpolator end ==================== */
