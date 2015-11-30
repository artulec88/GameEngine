#ifndef __MATH_ISORT_H__
#define __MATH_ISORT_H__

#include "Math.h"
#include "Vector.h"
#include "FloatingPoint.h"
#include "Utility\ILogger.h"
#include "SortingParameters.h"

namespace Math { namespace Sorting
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
	BUCKET_SORT
};

/// <summary>The base class for all sorting classes. It also serves as a "factory", that is creating specific instances of ISort classes.</summary>
class ISort
{
/* ==================== Static variables and functions begin ==================== */
public:
	MATH_API static ISort* GetSortingObject(SortingAlgorithm sortingMethod);
/* ==================== Static variables and functions end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	MATH_API ISort(SortingAlgorithm sortingMethod);
	MATH_API virtual ~ISort(void);
private:
	ISort(const ISort& sortObject) {} // don't implement
	void operator=(const ISort& sortObject) {} // don't implement
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	MATH_API virtual void Sort(int* values, int valuesCount, SortingDirection sortingDirection = ASCENDING) = 0;
	MATH_API virtual void Sort(Real* values, int valuesCount, SortingDirection sortingDirection = ASCENDING) = 0;
	MATH_API virtual void Sort(Vector2D* vectors, int vectorSize, SortingKey sortingKey = COMPONENT_X, SortingDirection sortingDirection = ASCENDING) = 0;
	MATH_API virtual void Sort(Vector3D* vectors, int vectorSize, SortingKey sortingKey = COMPONENT_X, SortingDirection sortingDirection = ASCENDING) = 0;
	MATH_API virtual void Sort(Vector2D* vectors, int vectorSize, const SortingParametersChain& sortingParameters) = 0;
	MATH_API virtual void Sort(Vector3D* vectors, int vectorSize, const SortingParametersChain& sortingParameters) = 0;
	MATH_API SortingAlgorithm GetSortingAlgorithm() const { return m_sortingAlgorithm; }
protected:
	template <typename T>
	bool NeedSwapping(const T& v1, const T& v2, const SortingParametersChain& sortingParameters)
	{
		return NeedSwapping<T>(v1, v2, sortingParameters.GetSortingDirection());
	}

	template <typename T>
	bool NeedSwapping(const T& v1, const T& v2, SortingDirection sortingDirection)
	{
		switch (sortingDirection)
		{
		case ASCENDING:
			return (v2 < v1);
		case DESCENDING:
			return (v2 > v1);
		default:
			ERROR_LOG("Unknown sorting direction specified.");
			return false;
			break;
		}
	}

	template <>
	bool NeedSwapping(const Math::Vector2D& vec1, const Math::Vector2D& vec2, const SortingParametersChain& sortingParameters)
	{
		/* Checking parameters */
		SortingDirection sortingDirection = sortingParameters.GetSortingDirection();
		SortingKey sortingKey = sortingParameters.GetSortingKey();
		if (sortingKey == COMPONENT_Z)
		{
			ERROR_LOG("Sorting 2D vectors by Z component is not possible. 2D vectors are defined with XY components.");
			return false;
		}

		Math::Real v1 = CollectValueByKey(vec1, sortingKey);
		Math::Real v2 = CollectValueByKey(vec2, sortingKey);

		if (AlmostEqual(v1, v2) && sortingParameters.GetSortingParametersChain() != NULL)
		{
			return NeedSwapping(vec1, vec2, *sortingParameters.GetSortingParametersChain());
		}
		return NeedSwapping(v1, v2, sortingDirection);
	}

	template <>
	bool NeedSwapping(const Math::Vector3D& vec1, const Math::Vector3D& vec2, const SortingParametersChain& sortingParameters)
	{
		SortingDirection sortingDirection = sortingParameters.GetSortingDirection();
		SortingKey sortingKey = sortingParameters.GetSortingKey();

		Math::Real v1 = CollectValueByKey(vec1, sortingKey);
		Math::Real v2 = CollectValueByKey(vec2, sortingKey);

		if (AlmostEqual(v1, v2) && sortingParameters.GetSortingParametersChain() != NULL)
		{
			return NeedSwapping(vec1, vec2, *sortingParameters.GetSortingParametersChain());
		}
		return NeedSwapping(v1, v2, sortingDirection);
	}

	Math::Real CollectValueByKey(const Math::Vector2D& v, SortingKey sortingKey)
	{
		Math::Real result;
		switch (sortingKey)
		{
		case VALUE:
			WARNING_LOG("VALUE sorting key is incorrect for the 2D vector. Returning X component instead.");
			result = v.GetX();
			break;
		case COMPONENT_X:
			result = v.GetX();
			break;
		case COMPONENT_Y:
			result = v.GetY();
			break;
		case COMPONENT_Z:
			EMERGENCY_LOG("Cannot determine the value of the Z component for the 2D vector.");
			exit(EXIT_FAILURE);
		case SUM_OF_SQUARED_COMPONENTS:
			result = v.LengthSquared();
			break;
		case SUM_OF_COMPONENTS:
			result = v.SumOfComponents();
			break;
		case SUM_OF_ABSOLUTE_COMPONENTS:
			result = v.SumOfAbsoluteComponents();
			break;
		default:
			EMERGENCY_LOG("Unknown sorting key specified. Returning X component value by default.");
			result = v.GetX();
			break;
		}
		return result;
	}

	Math::Real CollectValueByKey(const Math::Vector3D& v, SortingKey sortingKey)
	{
		Math::Real result;
		switch (sortingKey)
		{
		case VALUE:
			WARNING_LOG("VALUE sorting key is incorrect for the 2D vector. Returning X component instead.");
			result = v.GetX();
			break;
		case COMPONENT_X:
			result = v.GetX();
			break;
		case COMPONENT_Y:
			result = v.GetY();
			break;
		case COMPONENT_Z:
			result = v.GetZ();
			break;
		case SUM_OF_SQUARED_COMPONENTS:
			result = v.LengthSquared();
			break;
		case SUM_OF_COMPONENTS:
			result = v.SumOfComponents();
			break;
		case SUM_OF_ABSOLUTE_COMPONENTS:
			result = v.SumOfAbsoluteComponents();
			break;
		default:
			EMERGENCY_LOG("Unknown sorting key specified. Returning X component value by default.");
			result = v.GetX();
			break;
		}
		return result;
	}
	
	template <typename T>
	void Swap(T& v1, T& v2)
	{
		T v1Copy = v1;
		v1 = v2;
		v2 = v1Copy;
	}
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
protected:
	SortingAlgorithm m_sortingAlgorithm;
/* ==================== Non-static member variables end ==================== */
}; /* end class ISort */

} /* end namespace Sorting */

} /* end namespace Math */

#endif /* __MATH_ISORT_H__ */