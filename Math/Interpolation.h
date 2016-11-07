#ifndef __MATH_INTERPOLATION_H__
#define __MATH_INTERPOLATION_H__

#include "Math.h"
#include "Vector.h"

namespace Math
{

	namespace Interpolation
	{

		/// <summary> Interpolator class serves as a base class for all types of interpolation methods. </summary>
		template <class T>
		class Interpolator
		{
			/* ==================== Static variables and functions begin ==================== */
			/* ==================== Static variables and functions end ==================== */

			/* ==================== Constructors and destructors begin ==================== */
		public:
			Interpolator();
			Interpolator(const T* interpolationObjects, const Real* times, int interpolationObjectsCount);
			virtual ~Interpolator();
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
		public:
			virtual T Interpolate(const T& value1, const T& value2, Real factor) const = 0;
			virtual T Interpolate(Real time) const = 0;
			void PushInterpolationObject(const T& interpolationObject, Real time);
			Real GetStartTime() const { return m_times.front(); }
			Real GetEndTime() const { return m_times.back(); }
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
		protected:
			std::vector<T> m_interpolationObjects;
			std::vector<Real> m_times;
			/* ==================== Non-static member variables end ==================== */
		}; /* end class Interpolator */

		/// <summary> Linear interpolator class. </summary>
		template <class T>
		class LinearInterpolator : public Interpolator<T>
		{
			/* ==================== Static variables and functions begin ==================== */
			/* ==================== Static variables and functions end ==================== */

			/* ==================== Constructors and destructors begin ==================== */
		public:
			LinearInterpolator();
			LinearInterpolator(const T* interpolationObjects, const Real* times, int interpolationObjectsCount);
			virtual ~LinearInterpolator();
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
		public:
			virtual T Interpolate(const T& value1, const T& value2, Real factor) const;
			virtual T Interpolate(Real time) const;
			/* ==================== Non-static member functions end ==================== */
		}; /* end class LinearInterpolator */

		/// <summary> Cosine interpolator class. </summary>
		template <class T>
		class CosineInterpolator : public Interpolator<T>
		{
			/* ==================== Static variables and functions begin ==================== */
			/* ==================== Static variables and functions end ==================== */

			/* ==================== Constructors and destructors begin ==================== */
		public:
			CosineInterpolator();
			CosineInterpolator(const T* interpolationObjects, const Real* times, int interpolationObjectsCount);
			virtual ~CosineInterpolator();
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
		public:
			virtual T Interpolate(const T& value1, const T& value2, Real factor) const;
			virtual T Interpolate(Real time) const;
			/* ==================== Non-static member functions end ==================== */
		}; /* end class CosineInterpolator */

		/// <summary> Hermite interpolator class. </summary>
		template <class T>
		class HermiteInterpolator : public Interpolator<T>
		{
			/* ==================== Static variables and functions begin ==================== */
			/* ==================== Static variables and functions end ==================== */

			/* ==================== Constructors and destructors begin ==================== */
		public:
			HermiteInterpolator(const T* interpolationObjects, const Real* times, int interpolationObjectsCount);
			HermiteInterpolator(const T* interpolationObjects, const Real* times, int interpolationObjectsCount, const T& derivative0, const T& derivativeN);
			HermiteInterpolator(const T* interpolationObjects, const Real* times, const T* derivatives, int interpolationObjectsCount);
			virtual ~HermiteInterpolator();
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
		public:
			virtual T Interpolate(const T& value1, const T& value2, Real factor) const = 0; // TODO: Implement that
			virtual T Interpolate(Real time) const;
		private:
			void CalculateDerivatives(const T* interpolationObjects, int interpolationObjectsCount, const T& derivative0, const T& derivativeN);
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
		private:
			std::vector<T> m_derivatives;
			/* ==================== Non-static member variables end ==================== */
		}; /* end class HermiteInterpolator */

		/// <summary> Barycentric interpolator class. </summary>
		template <class T>
		class BarycentricInterpolator : public Interpolator<T>
		{
			/* ==================== Static variables and functions begin ==================== */
			/* ==================== Static variables and functions end ==================== */

			/* ==================== Constructors and destructors begin ==================== */
		public:
			BarycentricInterpolator(const T* interpolationObjects, const Real* times, int interpolationObjectsCount);
			virtual ~BarycentricInterpolator();
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
		public:
			virtual T Interpolate(const T& value1, const T& value2, Real factor) const = 0; // TODO: Implement that
			virtual T Interpolate(Real time) const;
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
		private:
			/* ==================== Non-static member variables end ==================== */
		}; /* end class BarycentricInterpolator */

	} /* end namespace Interpolation */

} /* end namespace Math */

#endif /* __MATH_INTERPOLATION_H__ */