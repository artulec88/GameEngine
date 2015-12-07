#ifndef __MATH_INTERPOLATION_H__
#define __MATH_INTERPOLATION_H__

#include "Math.h"

namespace Math {
namespace Interpolation
{
	/// <summary> Interpolator class serves as a base class for all types of interpolation methods. </summary>
	template <class T>
	class Interpolator
	{
	/* ==================== Constructors and destructors begin ==================== */
	public:
		MATH_API Interpolator();
		MATH_API virtual ~Interpolator();
	/* ==================== Constructors and destructors end ==================== */

	/* ==================== Non-static member functions begin ==================== */
	public:
		MATH_API virtual T Interpolate(Real time, int interpolationObjectsCount, const T* interpolationObjects, const Real* times) const = 0;
	/* ==================== Non-static member functions end ==================== */
	}; /* end class Interpolator */

	/// <summary> Linear interpolator class. </summary>
	template <class T>
	class LinearInterpolator : public Interpolator<T>
	{
		/* ==================== Constructors and destructors begin ==================== */
	public:
		MATH_API LinearInterpolator();
		MATH_API virtual ~LinearInterpolator();
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		MATH_API virtual T Interpolate(Real time, int interpolationObjectsCount, const T* interpolationObjects, const Real* times) const;
		/* ==================== Non-static member functions end ==================== */
	}; /* end class LinearInterpolator */

} /* end namespace Interpolation */

} /* end namespace Math */

#endif /* __MATH_INTERPOLATION_H__ */