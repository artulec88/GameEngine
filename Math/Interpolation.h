#ifndef __MATH_INTERPOLATION_H__
#define __MATH_INTERPOLATION_H__

#include "Math.h"
#include "Vector.h"

namespace Math
{

namespace Interpolation
{
	MATH_API Math::Real BarycentricInterpolation(const Math::Vector3D& pos1, const Math::Vector3D& pos2, const Math::Vector3D& pos3, Math::Real xPos, Math::Real zPos);
	MATH_API Math::Real BarycentricInterpolation(Math::Real xPos1, Math::Real yPos1, Math::Real zPos1,
		Math::Real xPos2, Math::Real yPos2, Math::Real zPos2,
		Math::Real xPos3, Math::Real yPos3, Math::Real zPos3,
		Math::Real xPos, Math::Real zPos);

	/// <summary> Interpolator class serves as a base class for all types of interpolation methods. </summary>
	template <class T>
	class Interpolator
	{
	/* ==================== Constructors and destructors begin ==================== */
	public:
		MATH_API Interpolator(const T* interpolationObjects, int interpolationObjectsCount);
		MATH_API virtual ~Interpolator();
	/* ==================== Constructors and destructors end ==================== */

	/* ==================== Non-static member functions begin ==================== */
	public:
		MATH_API virtual T Interpolate(Real time, const Real* times) const = 0;
	/* ==================== Non-static member functions end ==================== */

	/* ==================== Non-static member variables begin ==================== */
	protected:
		const T* m_interpolationObjects;
		const int m_interpolationObjectsCount;
	/* ==================== Non-static member variables end ==================== */
	}; /* end class Interpolator */

	/// <summary> Linear interpolator class. </summary>
	template <class T>
	class LinearInterpolator : public Interpolator<T>
	{
	/* ==================== Constructors and destructors begin ==================== */
	public:
		MATH_API LinearInterpolator(const T* interpolationObjects, int interpolationObjectsCount);
		MATH_API virtual ~LinearInterpolator();
	/* ==================== Constructors and destructors end ==================== */

	/* ==================== Non-static member functions begin ==================== */
	public:
		MATH_API virtual T Interpolate(Real time, const Real* times) const;
	/* ==================== Non-static member functions end ==================== */
	}; /* end class LinearInterpolator */

	/// <summary> Hermite interpolator class. </summary>
	template <class T>
	class HermiteInterpolator : public Interpolator<T>
	{
	/* ==================== Constructors and destructors begin ==================== */
	public:
		MATH_API HermiteInterpolator(const T* interpolationObjects, int interpolationObjectsCount);
		MATH_API HermiteInterpolator(const T* interpolationObjects, int interpolationObjectsCount, const T& derivative0, const T& derivativeN);
		MATH_API HermiteInterpolator(const T* interpolationObjects, const T* derivatives, int interpolationObjectsCount);
		MATH_API virtual ~HermiteInterpolator();
	/* ==================== Constructors and destructors end ==================== */

	/* ==================== Non-static member functions begin ==================== */
	public:
		MATH_API virtual T Interpolate(Real time, const Real* times) const;
	private:
		void CalculateDerivatives(const T* interpolationObjects, int interpolationObjectsCount, const T& derivative0, const T& derivativeN);
	/* ==================== Non-static member functions end ==================== */

	/* ==================== Non-static member variables begin ==================== */
	private:
		T* m_derivatives;
	/* ==================== Non-static member variables end ==================== */
	}; /* end class HermiteInterpolator */

	/// <summary> Barycentric interpolator class. </summary>
	template <class T>
	class BarycentricInterpolator : public Interpolator<T>
	{
	/* ==================== Constructors and destructors begin ==================== */
	public:
		MATH_API BarycentricInterpolator(const T* interpolationObjects, int interpolationObjectsCount);
		MATH_API virtual ~BarycentricInterpolator();
	/* ==================== Constructors and destructors end ==================== */

	/* ==================== Non-static member functions begin ==================== */
	public:
		MATH_API virtual T Interpolate(Real time, const Real* times) const;
	/* ==================== Non-static member functions end ==================== */

	/* ==================== Non-static member variables begin ==================== */
	private:
	/* ==================== Non-static member variables end ==================== */
	}; /* end class BarycentricInterpolator */

} /* end namespace Interpolation */

} /* end namespace Math */

#endif /* __MATH_INTERPOLATION_H__ */