#ifndef __MATH_ISORT_H__
#define __MATH_ISORT_H__

#include "Math.h"
#include "Vector.h"
#include "FloatingPoint.h"
#include "Utility\ILogger.h"
#include "SortingParameters.h"

namespace math {
	namespace sorting
	{
		namespace sorting_algorithms
		{
			/// <summary>
			/// Possible algorithms for sorting.
			/// </summary>
			enum SortingAlgorithm
			{
				/// <summary>
				/// Bubble sort algorithm (see https://en.wikipedia.org/wiki/Bubble_sort).
				/// </summary>
				BUBBLE_SORT = 0,
				/// <summary>
				/// Insertion sort algorithm (see https://en.wikipedia.org/wiki/Insertion_sort).
				/// </summary>
				INSERTION_SORT,
				/// <summary>
				/// Selection sort algorithm (see https://en.wikipedia.org/wiki/Selection_sort).
				/// </summary>
				SELECTION_SORT,
				/// <summary>
				/// Merge sort algorithm (see https://en.wikipedia.org/wiki/Merge_sort).
				/// </summary>
				MERGE_SORT,
				/// <summary>
				/// Heap sort algorithm (see https://en.wikipedia.org/wiki/Heapsort).
				/// </summary>
				HEAP_SORT,
				/// <summary>
				/// Quick sort algorithm (see https://en.wikipedia.org/wiki/Quicksort).
				/// </summary>
				QUICK_SORT,
				/// <summary>
				/// Shell sort algorithm (see https://en.wikipedia.org/wiki/Shellsort).
				/// </summary>
				SHELL_SORT,
				/// <summary>
				/// Comb sort algorithm (see https://en.wikipedia.org/wiki/Comb_sort).
				/// </summary>
				COMB_SORT,
				/// <summary>
				/// Counting sort algorithm (see https://en.wikipedia.org/wiki/Counting_sort).
				/// </summary>
				COUNTING_SORT,
				/// <summary>
				/// Radix sort algorithm (see https://en.wikipedia.org/wiki/Radix_sort).
				/// </summary>
				RADIX_SORT,
				/// <summary>
				/// Bucket sort algorithm (see https://en.wikipedia.org/wiki/Bucket_sort).
				/// </summary>
				BUCKET_SORT,

				COUNT
			}; /* end enum SortingAlgorithm */
		} /* end enum sorting_algorithms */

		/// <summary>
		/// The base class for all sorting classes. It also serves as a "factory", that is creating specific instances of <code>ISort</code> classes.
		/// </summary>
		class ISort
		{
			/* ==================== Static variables and functions begin ==================== */
		public:
			/// <summary>
			/// A factory-like method for creating specific sorting objects based on the given <paramref name="sortingAlgorithm"/>.
			/// </summary>
			MATH_API static std::unique_ptr<ISort> GetSortingObject(sorting_algorithms::SortingAlgorithm sortingAlgorithm);
			/* ==================== Static variables and functions end ==================== */

			/* ==================== Constructors and destructors begin ==================== */
		public:
			/// <summary> The constructor of the sorting object. </summary>
			MATH_API ISort()
			{
			}

			/// <summary> The destructor of the sorting object. </summary>
			MATH_API virtual ~ISort()
			{
			}
			
			/// <summary> The copy constructor of the sorting object. </summary>
			/// <param name="sortObject"> The sorting object to copy construct from. </param>
			ISort(const ISort& sortObject) = delete;

			/// <summary> The move constructor of the sorting object. </summary>
			/// <param name="sortObject"> The r-value reference of the sorting object to move construct from. </param>
			ISort(ISort&& sortObject) = delete;

			/// <summary> The copy assignment operator of the sorting object. </summary>
			/// <param name="sortObject"> The sorting object to copy assign from. </param>
			/// <returns> The reference to the newly copy-assigned <code>ISort</code> object. </returns>
			ISort& operator=(const ISort& sortObject) = delete;

			/// <summary> The move assignment operator of the sorting object. </summary>
			/// <param name="sortObject"> The r-value reference of the sorting object to move assign from. </param>
			/// <returns> The reference to the newly move-assigned <code>ISort</code> object. </returns>
			ISort& operator=(ISort&& sortObject) = delete;
		/* ==================== Constructors and destructors end ==================== */
			
		/* ==================== Non-static member functions begin ==================== */
		public:
			MATH_API virtual void Sort(int* values, size_t valuesCount, orders::Order sortingOrder = orders::ASCENDING) = 0;
			MATH_API virtual void Sort(Real* values, size_t valuesCount, orders::Order sortingOrder = orders::ASCENDING) = 0;
			MATH_API virtual void Sort(Vector2D* vectors, size_t vectorSize, keys::Key sortingKey = keys::COMPONENT_X, orders::Order sortingOrder = orders::ASCENDING) = 0;
			MATH_API virtual void Sort(Vector3D* vectors, size_t vectorSize, keys::Key sortingKey = keys::COMPONENT_X, orders::Order sortingOrder = orders::ASCENDING) = 0;
			MATH_API virtual void Sort(Vector2D* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters) = 0;
			MATH_API virtual void Sort(Vector3D* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters) = 0;
		protected:
			template <typename T>
			bool NeedSwapping(const T& v1, const T& v2, const SortingParametersChain& sortingParameters)
			{
				return NeedSwapping<T>(v1, v2, sortingParameters.GetOrder());
			}

			template <typename T>
			bool NeedSwapping(const T& v1, const T& v2, orders::Order sortingOrder)
			{
				switch (sortingOrder)
				{
				case orders::ASCENDING:
					return (v2 < v1);
				case orders::DESCENDING:
					return (v2 > v1);
				default:
					ERROR_LOG_MATH("Unknown sorting direction specified.");
					return false;
				}
			}

			template <>
			bool NeedSwapping(const Vector2D& vec1, const Vector2D& vec2, const SortingParametersChain& sortingParameters)
			{
				/* Checking parameters */
				orders::Order sortingOrder = sortingParameters.GetOrder();
				keys::Key sortingKey = sortingParameters.GetKey();
				if (sortingKey == keys::COMPONENT_Z)
				{
					ERROR_LOG_MATH("Sorting 2D vectors by Z component is not possible. 2D vectors are defined with XY components.");
					return false;
				}

				Real v1 = CollectValueByKey(vec1, sortingKey);
				Real v2 = CollectValueByKey(vec2, sortingKey);

				if (AlmostEqual(v1, v2) && sortingParameters.GetSortingParametersChain() != nullptr)
				{
					return NeedSwapping(vec1, vec2, *sortingParameters.GetSortingParametersChain());
				}
				return NeedSwapping(v1, v2, sortingOrder);
			}

			template <>
			bool NeedSwapping(const Vector3D& vec1, const Vector3D& vec2, const SortingParametersChain& sortingParameters)
			{
				orders::Order sortingOrder = sortingParameters.GetOrder();
				keys::Key sortingKey = sortingParameters.GetKey();

				Real v1 = CollectValueByKey(vec1, sortingKey);
				Real v2 = CollectValueByKey(vec2, sortingKey);

				if (AlmostEqual(v1, v2) && sortingParameters.GetSortingParametersChain() != nullptr)
				{
					return NeedSwapping(vec1, vec2, *sortingParameters.GetSortingParametersChain());
				}
				return NeedSwapping(v1, v2, sortingOrder);
			}

			Real CollectValueByKey(const Vector2D& v, keys::Key sortingKey)
			{
				switch (sortingKey)
				{
				case keys::VALUE:
					WARNING_LOG_MATH("VALUE sorting key is incorrect for the 2D vector. Returning X component instead.");
					return v.x;
				case keys::COMPONENT_X: return v.x;
				case keys::COMPONENT_Y: return v.y;
				case keys::COMPONENT_Z:
					EMERGENCY_LOG_MATH("Cannot determine the value of the Z component for the 2D vector.");
					exit(EXIT_FAILURE);
				case keys::SUM_OF_SQUARED_COMPONENTS: return v.LengthSquared();
				case keys::SUM_OF_COMPONENTS: return v.SumOfComponents();
				case keys::SUM_OF_ABSOLUTE_COMPONENTS: return v.SumOfAbsoluteComponents();
				default:
					EMERGENCY_LOG_MATH("Unknown sorting key specified. Returning X component value by default.");
					return v.x;
				}
			}

			Real CollectValueByKey(const Vector3D& v, keys::Key sortingKey)
			{
				switch (sortingKey)
				{
				case keys::VALUE:
					WARNING_LOG_MATH("VALUE sorting key is incorrect for the 2D vector. Returning X component instead.");
					return v.x;
				case keys::COMPONENT_X: return v.x;
				case keys::COMPONENT_Y: return v.y;
				case keys::COMPONENT_Z: return v.z;
				case keys::SUM_OF_SQUARED_COMPONENTS: return v.LengthSquared();
				case keys::SUM_OF_COMPONENTS: return v.SumOfComponents();
				case keys::SUM_OF_ABSOLUTE_COMPONENTS: return v.SumOfAbsoluteComponents();
				default:
					EMERGENCY_LOG_MATH("Unknown sorting key specified. Returning X component value by default.");
					return v.x;
				}
			}

			template <typename T>
			void Swap(T& v1, T& v2)
			{
				std::swap(v1, v2);
				
				//T temp(std::move(v1));
				//v1 = std::move(v2);
				//v2 = std::move(temp);

				//T temp = v1;
				//v1 = v2;
				//v2 = temp;
			}
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
			/* ==================== Non-static member variables end ==================== */
		}; /* end class ISort */

	} /* end namespace Sorting */

} /* end namespace Math */

#endif /* __MATH_ISORT_H__ */