#include "stdafx.h"
#include "Interpolation.h"
#include "Vector.h"
#include "Quaternion.h"
#include "Utility\ILogger.h"

Math::Real Math::Interpolation::BarycentricInterpolation(const Math::Vector3D& pos1, const Math::Vector3D& pos2, const Math::Vector3D& pos3, Math::Real xPos, Math::Real zPos)
{
	return BarycentricInterpolation(pos1.GetX(), pos1.GetY(), pos1.GetZ(), pos2.GetX(), pos2.GetY(), pos2.GetZ(), pos3.GetX(), pos3.GetY(), pos3.GetZ(), xPos, zPos);
}

Math::Real Math::Interpolation::BarycentricInterpolation(Math::Real xPos1, Math::Real yPos1, Math::Real zPos1,
	Math::Real xPos2, Math::Real yPos2, Math::Real zPos2,
	Math::Real xPos3, Math::Real yPos3, Math::Real zPos3,
	Math::Real xPos, Math::Real zPos)
{
	Math::Real det = (zPos2 - zPos3) * (xPos1 - xPos3) + (xPos3 - xPos2) * (zPos1 - zPos3);
	Math::Real l1 = ((zPos2 - zPos3) * (xPos - xPos3) + (xPos3 - xPos2) * (zPos - zPos3)) / det;
	Math::Real l2 = ((zPos3 - zPos1) * (xPos - xPos3) + (xPos1 - xPos3) * (zPos - zPos3)) / det;
	Math::Real l3 = 1.0f - l1 - l2;
	return l1 * yPos1 + l2 * yPos2 + l3 * yPos3;
}

/* ==================== class Interpolator begin ==================== */
template <class T>
Math::Interpolation::Interpolator<T>::Interpolator(const T* interpolationObjects, int interpolationObjectsCount) :
	m_interpolationObjects(interpolationObjects),
	m_interpolationObjectsCount(interpolationObjectsCount)
{
	CHECK_CONDITION(m_interpolationObjects != NULL, Utility::Warning, "Specified interpolation object array is NULL.");
}

template <class T>
Math::Interpolation::Interpolator<T>::~Interpolator()
{
	SAFE_DELETE_JUST_TABLE(m_interpolationObjects);
}
/* ==================== class Interpolator end ==================== */

/* ==================== class LinearInterpolator begin ==================== */
template <class T>
Math::Interpolation::LinearInterpolator<T>::LinearInterpolator(const T* interpolationObjects, int interpolationObjectsCount) :
	Interpolator(interpolationObjects, interpolationObjectsCount)
{
}

template <class T>
Math::Interpolation::LinearInterpolator<T>::~LinearInterpolator()
{
}

template <class T>
T Math::Interpolation::LinearInterpolator<T>::Interpolate(Math::Real time, const Math::Real* times) const
{
	// Handle boundary conditions
	if (time < times[0])
	{
		return m_interpolationObjects[0];
	}
	else if (time > times[m_interpolationObjectsCount - 1])
	{
		return m_interpolationObjects[m_interpolationObjectsCount - 1];
	}

	// Find segment and parameter
	unsigned int index = 0;
	for (; index < m_interpolationObjectsCount; ++index)
	{
		if (t < times[index + 1])
		{
			break;
		}
	}
	Math::Real time0 = times[index];
	Math::Real time1 = times[index + 1];
	Math::Real interpolationFactor = (time - time0) / (time1 - time0);

	// Evaluate the interpolation
	return ((REAL_ONE - interpolationFactor) * m_interpolationObjects[index]) + (interpolationFactor * m_interpolationObjects[index + 1]);
}
/* ==================== class LinearInterpolator end ==================== */


/* ==================== class HermiteInterpolator begin ==================== */
template <class T>
Math::Interpolation::HermiteInterpolator<T>::HermiteInterpolator(const T* interpolationObjects, int interpolationObjectsCount) :
	Interpolator(interpolationObjects, interpolationObjectsCount),
	m_derivatives(NULL)
{
	CalculateDerivatives(interpolationObjects, interpolationObjectsCount, T(REAL_ZERO), T(REAL_ZERO));
}

template <class T>
Math::Interpolation::HermiteInterpolator<T>::HermiteInterpolator(const T* interpolationObjects, int interpolationObjectsCount, const T& derivative0, const T& derivativeN) :
	Interpolator(interpolationObjects, interpolationObjectsCount),
	m_derivatives(NULL)
{
	CalculateDerivatives(interpolationObjects, interpolationObjectsCount, derivative0, derivativeN);
}

template <class T>
Math::Interpolation::HermiteInterpolator<T>::HermiteInterpolator(const T* interpolationObjects, const T* derivatives, int interpolationObjectsCount) :
	Interpolator(interpolationObjects, interpolationObjectsCount),
	m_derivatives(derivatives)
{
}

template <class T>
Math::Interpolation::HermiteInterpolator<T>::~HermiteInterpolator()
{
	SAFE_DELETE_JUST_TABLE(m_derivatives);
}

template <class T>
void Math::Interpolation::HermiteInterpolator<T>::CalculateDerivatives(const T* interpolationObjects, int interpolationObjectsCount, const T& derivative0, const T& derivativeN)
{
	// TODO: Calculate derivatives according to the "Essential Mathematics for Games and Interactive Applications 2nd Edition"- section 10.2.3 ("Hermite curves").
	WARNING_LOG("This function has not been tested yet");
	Real* c = new Real[interpolationObjectsCount - 1];
	T* d = new T[interpolationObjectsCount];
	c[0] = REAL_ZERO;
	d[0] = derivative0;
	for (int i = 1; i < interpolationObjectsCount - 1; ++i)
	{
		c[i] = REAL_ONE / (4.0f - c[i - 1]);
		d[i] = (3.0f * (interpolationObjects[i + 1] - interpolationObjects[i - 1]) - d[i - 1]) / (4.0f - c[i - 1]);
	}
	d[interpolationObjectsCount - 1] = derivativeN - d[interpolationObjectsCount - 2];

	m_derivatives = new T[interpolationObjectsCount];
	m_derivatives[interpolationObjectsCount - 1] = d[interpolationObjectsCount - 1];
	for (int i = interpolationObjectsCount - 1; i >= 0; --i)
	{
		m_derivatives[i] = d[i] - c[i] * m_derivatives[i + 1];
	}

	SAFE_DELETE_JUST_TABLE(c);
	SAFE_DELETE_JUST_TABLE(d);
}

template <class T>
T Math::Interpolation::HermiteInterpolator<T>::Interpolate(Math::Real time, const Math::Real* times) const
{
	WARNING_LOG("This function has not been tested yet");
	// Handle boundary conditions
	if (time < times[0])
	{
		return m_interpolationObjects[0];
	}
	else if (time > times[m_interpolationObjectsCount - 1])
	{
		return m_interpolationObjects[m_interpolationObjectsCount - 1];
	}

	// Find segment and parameter
	unsigned int index = 0;
	for (; index < m_interpolationObjectsCount; ++index)
	{
		if (t < times[index + 1])
		{
			break;
		}
	}
	Math::Real time0 = times[index];
	Math::Real time1 = times[index + 1];
	Math::Real interpolationFactor = (time - time0) / (time1 - time0);

	// Evaluate the interpolation
	return ((REAL_ONE - interpolationFactor) * m_interpolationObjects[index]) + (interpolationFactor * m_interpolationObjects[index + 1]);
}
/* ==================== class HermiteInterpolator end ==================== */

/* ==================== class BarycentricInterpolator begin ==================== */
template <class T>
Math::Interpolation::BarycentricInterpolator<T>::BarycentricInterpolator(const T* interpolationObjects, int interpolationObjectsCount) :
	Interpolator(interpolationObjects, interpolationObjectsCount)
{
}

template <class T>
Math::Interpolation::BarycentricInterpolator<T>::~BarycentricInterpolator()
{
}

template <class T>
T Math::Interpolation::BarycentricInterpolator<T>::Interpolate(Math::Real time, const Math::Real* times) const
{
	WARNING_LOG("This function has not been tested yet");
	// Handle boundary conditions
	if (time < times[0])
	{
		return m_interpolationObjects[0];
	}
	else if (time > times[m_interpolationObjectsCount - 1])
	{
		return m_interpolationObjects[m_interpolationObjectsCount - 1];
	}

	// Find segment and parameter
	unsigned int index = 0;
	for (; index < m_interpolationObjectsCount; ++index)
	{
		if (t < times[index + 1])
		{
			break;
		}
	}
	Math::Real time0 = times[index];
	Math::Real time1 = times[index + 1];
	Math::Real interpolationFactor = (time - time0) / (time1 - time0);

	// Evaluate the interpolation
	return ((REAL_ONE - interpolationFactor) * m_interpolationObjects[index]) + (interpolationFactor * m_interpolationObjects[index + 1]);
}
/* ==================== class BarycentricInterpolator end ==================== */

// TODO: For all template classes perform the steps described in https://anteru.net/2008/11/19/318/.
// The same steps have already been performed for the Utility::Container::Array<T> class.
//template class Math::Interpolation::Interpolator<Math::Real>;
template class Math::Interpolation::Interpolator<Math::Vector2D>;
template class Math::Interpolation::Interpolator<Math::Vector3D>;
//template class Math::Interpolation::Interpolator<Math::Quaternion>;
template class Math::Interpolation::Interpolator<Math::Vector4D>;