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
			/// <summary> Interpolator default constructor. </summary>
			Interpolator();
			
			/// <summary> Interpolator constructor. </summary>
			/// <param name="interpolationObjects">
			/// The pointer to values of the function for the <paramref name="times"/> arguments which the interpolator will use.
			/// </param>
			/// <param name="times"> The pointer to arguments the <paramref name="interpolationObjects"/> stores values of. </param>
			/// <param name="interpolationObjectsCount">
			/// The number of elements in both <paramref name="interpolationObjects"/> and <paramref name="times"/> arrays.
			/// </param>
			Interpolator(const T* interpolationObjects, const Real* times, int interpolationObjectsCount);

			/// <summary> Interpolator destructor. </summary>
			virtual ~Interpolator();

			/// <summary> Interpolator copy constructor. </summary>
			/// <param name="interpolator"> The interpolator to copy construct from. </param>
			Interpolator(const Interpolator<T>& interpolator) = delete;

			/// <summary> Interpolator move constructor. </summary>
			/// <param name="interpolator"> The interpolator to move construct from. </param>
			Interpolator(Interpolator<T>&& interpolator) = delete;

			/// <summary> Interpolator copy assignment operator. </summary>
			/// <param name="interpolator"> The interpolator to copy assign from. </param>
			/// <returns> The newly copy-assigned interpolator. </returns>
			Interpolator& operator=(const Interpolator<T>& interpolator) = delete;

			/// <summary> Interpolator move assignment operator. </summary>
			/// <param name="interpolator"> The interpolator to move assign from. </param>
			/// <returns> The newly move-assigned interpolator. </returns>
			Interpolator& operator=(Interpolator<T>&& interpolator) = delete;
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
			/// <summary> Linear interpolator default constructor. </summary>
			LinearInterpolator();

			/// <summary> Linear interpolator constructor. </summary>
			/// <param name="interpolationObjects">
			/// The pointer to values of the function for the <paramref name="times"/> arguments which the interpolator will use.
			/// </param>
			/// <param name="times"> The pointer to arguments the <paramref name="interpolationObjects"/> stores values of. </param>
			/// <param name="interpolationObjectsCount">
			/// The number of elements in both <paramref name="interpolationObjects"/> and <paramref name="times"/> arrays.
			/// </param>
			LinearInterpolator(const T* interpolationObjects, const Real* times, int interpolationObjectsCount);

			/// <summary> Linear interpolator destructor. </summary>
			virtual ~LinearInterpolator();

			/// <summary> Linear interpolator copy constructor. </summary>
			/// <param name="linearInterpolator"> The linear interpolator to copy construct from. </param>
			LinearInterpolator(const LinearInterpolator<T>& linearInterpolator) = delete;

			/// <summary> Linear interpolator move constructor. </summary>
			/// <param name="linearInterpolator"> The linear interpolator to move construct from. </param>
			LinearInterpolator(LinearInterpolator<T>&& linearInterpolator) = delete;

			/// <summary> Linear interpolator copy assignment operator. </summary>
			/// <param name="linearInterpolator"> The linear interpolator to copy assign from. </param>
			/// <returns> The newly copy-assigned linear interpolator. </returns>
			LinearInterpolator& operator=(const LinearInterpolator<T>& linearInterpolator) = delete;

			/// <summary> Linear interpolator move assignment operator. </summary>
			/// <param name="linearInterpolator"> The linear interpolator to move assign from. </param>
			/// <returns> The newly move-assigned linear interpolator. </returns>
			LinearInterpolator& operator=(LinearInterpolator<T>&& linearInterpolator) = delete;
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
		public:
			virtual T Interpolate(const T& value1, const T& value2, Real factor) const override;
			virtual T Interpolate(Real time) const override;
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
			/// <summary> Cosine interpolator default constructor. </summary>
			CosineInterpolator();

			/// <summary> Cosine interpolator constructor. </summary>
			/// <param name="interpolationObjects">
			/// The pointer to values of the function for the <paramref name="times"/> arguments which the interpolator will use.
			/// </param>
			/// <param name="times"> The pointer to arguments the <paramref name="interpolationObjects"/> stores values of. </param>
			/// <param name="interpolationObjectsCount">
			/// The number of elements in both <paramref name="interpolationObjects"/> and <paramref name="times"/> arrays.
			/// </param>
			CosineInterpolator(const T* interpolationObjects, const Real* times, int interpolationObjectsCount);

			/// <summary> Cosine interpolator destructor. </summary>
			virtual ~CosineInterpolator();

			/// <summary> Cosine interpolator copy constructor. </summary>
			/// <param name="cosineInterpolator"> The cosine interpolator to copy construct from. </param>
			CosineInterpolator(const CosineInterpolator<T>& cosineInterpolator) = delete;

			/// <summary> Cosine interpolator move constructor. </summary>
			/// <param name="cosineInterpolator"> The cosine interpolator to move construct from. </param>
			CosineInterpolator(CosineInterpolator<T>&& cosineInterpolator) = delete;

			/// <summary> Cosine interpolator copy assignment operator. </summary>
			/// <param name="cosineInterpolator"> The cosine interpolator to copy assign from. </param>
			/// <returns> The newly copy-assigned cosine interpolator. </returns>
			CosineInterpolator& operator=(const CosineInterpolator<T>& cosineInterpolator) = delete;

			/// <summary> Cosine interpolator move assignment operator. </summary>
			/// <param name="cosineInterpolator"> The cosine interpolator to move assign from. </param>
			/// <returns> The newly move-assigned cosine interpolator. </returns>
			CosineInterpolator& operator=(CosineInterpolator<T>&& cosineInterpolator) = delete;
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
		public:
			virtual T Interpolate(const T& value1, const T& value2, Real factor) const override;
			virtual T Interpolate(Real time) const override;
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
			/// <summary> Hermite interpolator default constructor. </summary>
			HermiteInterpolator();

			/// <summary> Hermite interpolator constructor. </summary>
			/// <param name="interpolationObjects">
			/// The pointer to values of the function for the <paramref name="times"/> arguments which the interpolator will use.
			/// </param>
			/// <param name="times"> The pointer to arguments the <paramref name="interpolationObjects"/> stores values of. </param>
			/// <param name="interpolationObjectsCount">
			/// The number of elements in both <paramref name="interpolationObjects"/> and <paramref name="times"/> arrays.
			/// </param>
			HermiteInterpolator(const T* interpolationObjects, const Real* times, int interpolationObjectsCount);

			/// <summary> Hermite interpolator constructor. </summary>
			/// <param name="interpolationObjects">
			/// The pointer to values of the function for the <paramref name="times"/> arguments which the interpolator will use.
			/// </param>
			/// <param name="times"> The pointer to arguments the <paramref name="interpolationObjects"/> stores values of. </param>
			/// <param name="interpolationObjectsCount">
			/// The number of elements in both <paramref name="interpolationObjects"/> and <paramref name="times"/> arrays.
			/// </param>
			/// <param name="derivative0">
			/// The value of the first derivative for the first data input in the <paramref name="interpolationObjects"/> array.
			/// </param>
			/// <param name="derivativeN">
			/// The value of the first derivative for the last data input in the <paramref name="interpolationObjects"/> array.
			/// </param>
			HermiteInterpolator(const T* interpolationObjects, const Real* times, int interpolationObjectsCount, const T& derivative0, const T& derivativeN);

			/// <summary> Hermite interpolator constructor. </summary>
			/// <param name="interpolationObjects">
			/// The pointer to values of the function for the <paramref name="times"/> arguments which the interpolator will use.
			/// </param>
			/// <param name="times"> The pointer to arguments the <paramref name="interpolationObjects"/> stores values of. </param>
			/// <param name="derivatives">
			/// The values of the first derivatives for all data in the <paramref name="interpolationObjects"/> array.
			/// </param>
			/// <param name="interpolationObjectsCount">
			/// The number of elements in <paramref name="interpolationObjects"/>, <paramref name="times"/> and <paramref name="derivatives"/> arrays.
			/// </param>
			HermiteInterpolator(const T* interpolationObjects, const Real* times, const T* derivatives, int interpolationObjectsCount);

			/// <summary> Hermite interpolator destructor. </summary>
			virtual ~HermiteInterpolator();

			/// <summary> Hermite interpolator copy constructor. </summary>
			/// <param name="hermiteInterpolator"> The Hermite interpolator to copy construct from. </param>
			HermiteInterpolator(const HermiteInterpolator<T>& hermiteInterpolator) = delete;

			/// <summary> Hermite interpolator move constructor. </summary>
			/// <param name="hermiteInterpolator"> The Hermite interpolator to move construct from. </param>
			HermiteInterpolator(HermiteInterpolator<T>&& hermiteInterpolator) = delete;

			/// <summary> Hermite interpolator copy assignment operator. </summary>
			/// <param name="hermiteInterpolator"> The Hermite interpolator to copy assign from. </param>
			/// <returns> The newly copy-assigned Hermite interpolator. </returns>
			HermiteInterpolator& operator=(const HermiteInterpolator<T>& hermiteInterpolator) = delete;

			/// <summary> Hermite interpolator move assignment operator. </summary>
			/// <param name="hermiteInterpolator"> The cosine interpolator to move assign from. </param>
			/// <returns> The newly move-assigned Hermite interpolator. </returns>
			HermiteInterpolator& operator=(HermiteInterpolator<T>&& hermiteInterpolator) = delete;
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
		public:
			virtual T Interpolate(const T& value1, const T& value2, Real factor) const override;
			virtual T Interpolate(Real time) const override;
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
			/// <summary> Barycentric interpolator default constructor. </summary>
			BarycentricInterpolator();

			/// <summary> Barycentric interpolator constructor. </summary>
			/// <param name="interpolationObjects">
			/// The pointer to values of the function for the <paramref name="times"/> arguments which the interpolator will use.
			/// </param>
			/// <param name="times"> The pointer to arguments the <paramref name="interpolationObjects"/> stores values of. </param>
			/// <param name="interpolationObjectsCount">
			/// The number of elements in both <paramref name="interpolationObjects"/> and <paramref name="times"/> arrays.
			/// </param>
			BarycentricInterpolator(const T* interpolationObjects, const Real* times, int interpolationObjectsCount);

			/// <summary> Barycentric interpolator destructor. </summary>
			virtual ~BarycentricInterpolator();

			/// <summary> Barycentric interpolator copy constructor. </summary>
			/// <param name="barycentricInterpolator"> The barycentric interpolator to copy construct from. </param>
			BarycentricInterpolator(const BarycentricInterpolator<T>& barycentricInterpolator) = delete;

			/// <summary> Barycentric interpolator move constructor. </summary>
			/// <param name="barycentricInterpolator"> The barycentric interpolator to move construct from. </param>
			BarycentricInterpolator(BarycentricInterpolator<T>&& barycentricInterpolator) = delete;

			/// <summary> Barycentric interpolator copy assignment operator. </summary>
			/// <param name="barycentricInterpolator"> The barycentric interpolator to copy assign from. </param>
			/// <returns> The newly copy-assigned barycentric interpolator. </returns>
			BarycentricInterpolator& operator=(const BarycentricInterpolator<T>& barycentricInterpolator) = delete;

			/// <summary> Barycentric interpolator move assignment operator. </summary>
			/// <param name="barycentricInterpolator"> The barycentric interpolator to move assign from. </param>
			/// <returns> The newly move-assigned barycentric interpolator. </returns>
			BarycentricInterpolator& operator=(BarycentricInterpolator<T>&& barycentricInterpolator) = delete;
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
		public:
			virtual T Interpolate(const T& value1, const T& value2, Real factor) const override; // TODO: Implement that
			virtual T Interpolate(Real time) const override;
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
		private:
			/* ==================== Non-static member variables end ==================== */
		}; /* end class BarycentricInterpolator */

	} /* end namespace Interpolation */

} /* end namespace Math */

#endif /* __MATH_INTERPOLATION_H__ */