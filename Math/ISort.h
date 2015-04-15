#pragma once

#include "Math.h"
#include "Vector.h"
#include "FloatingPoint.h"
#include "Utility\ILogger.h"
#include "SortingParameters.h"

namespace Math { namespace Sorting
{
/// <summary>
/// Possible algorithms for sorting
/// </summary>
enum SortingAlgorithm
{
	BUBBLE_SORT = 0,
	INSERTION_SORT,
	SELECTION_SORT,
	MERGE_SORT,
	HEAP_SORT,
	QUICK_SORT,
	SHELL_SORT,
	COMB_SORT,
	COUNTING_SORT,
	RADIX_SORT,
	BUCKET_SORT
};

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
	MATH_API virtual void Sort(Vector2D* vectors, int vectorSize, SortingKey sortingKey = COMPONENT_X, SortingDirection sortingDirection = ASCENDING) = 0;
	MATH_API virtual void Sort(Vector3D* vectors, int vectorSize, SortingKey sortingKey = COMPONENT_X, SortingDirection sortingDirection = ASCENDING) = 0;
	MATH_API virtual void Sort(Vector2D* vectors, int vectorSize, const SortingParametersChain& sortingParameters) = 0;
	MATH_API virtual void Sort(Vector3D* vectors, int vectorSize, const SortingParametersChain& sortingParameters) = 0;
	MATH_API SortingAlgorithm GetSortingAlgorithm() const { return m_sortingAlgorithm; }
protected:
	template <typename T>
	bool NeedSwapping(const T& v1, const T& v2, const SortingParametersChain& sortingParameters)
	{
		return NeedSwapping<T>(v1, v2, sortingParameters.GetSortingKey(), sortingParameters.GetSortingDirection());
	}

	template <typename T>
	bool NeedSwapping(const T& v1, const T& v2, SortingKey sortingKey, SortingDirection sortingDirection)
	{
		switch (sortingDirection)
		{
		case ASCENDING:
			return (v2 < v1);
		case DESCENDING:
			return (v2 > v1);
		default:
			LOG(Utility::Critical, LOGPLACE, "Unknown sorting direction specified.");
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
			LOG(Utility::Critical, LOGPLACE, "Sorting 2D vectors by Z component is not possible. 2D vectors are defined with XY components.");
			return false;
		}

		Math::Real v1 = CollectValueByKey(vec1, sortingKey);
		Math::Real v2 = CollectValueByKey(vec2, sortingKey);

		if (AlmostEqual(v1, v2) && sortingParameters.GetSortingParametersChain() != NULL)
		{
			return NeedSwapping(vec1, vec2, *sortingParameters.GetSortingParametersChain());
		}
		return NeedSwapping(v1, v2, sortingKey, sortingDirection);
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
		return NeedSwapping(v1, v2, sortingKey, sortingDirection);
	}

	template <>
	bool NeedSwapping(const Math::Vector2D& vec1, const Math::Vector2D& vec2, SortingKey sortingKey, SortingDirection sortingDirection)
	{
		return NeedSwapping(vec1, vec2, SortingParametersChain(sortingKey, sortingDirection));
	}


	template <>
	bool NeedSwapping(const Math::Vector3D& vec1, const Math::Vector3D& vec2, SortingKey sortingKey, SortingDirection sortingDirection)
	{
		return NeedSwapping(vec1, vec2, SortingParametersChain(sortingKey, sortingDirection));
	}

	Math::Real CollectValueByKey(const Math::Vector2D& v, SortingKey sortingKey)
	{
		switch (sortingKey)
		{
		case COMPONENT_X:
			return v.GetX();
		case COMPONENT_Y:
			return v.GetY();
		case COMPONENT_Z:
			LOG(Utility::Critical, LOGPLACE, "Cannot determine the value of the Z component for the 2D vector.");
			exit(EXIT_FAILURE);
		case SUM_OF_SQUARED_COMPONENTS:
			return v.LengthSquared();
		case SUM_OF_COMPONENTS:
			return v.SumOfComponents();
		case SUM_OF_ABSOLUTE_COMPONENTS:
			return v.SumOfAbsoluteComponents();
		default:
			LOG(Utility::Emergency, LOGPLACE, "Unknown sorting key specified. Returning X component value by default.");
			return v.GetX();
		}
	}

	Math::Real CollectValueByKey(const Math::Vector3D& v, SortingKey sortingKey)
	{
		switch (sortingKey)
		{
		case COMPONENT_X:
			return v.GetX();
		case COMPONENT_Y:
			return v.GetY();
		case COMPONENT_Z:
			return v.GetZ();
		case SUM_OF_SQUARED_COMPONENTS:
			return v.LengthSquared();
		case SUM_OF_COMPONENTS:
			return v.SumOfComponents();
		case SUM_OF_ABSOLUTE_COMPONENTS:
			return v.SumOfAbsoluteComponents();
		default:
			LOG(Utility::Emergency, LOGPLACE, "Unknown sorting key specified. Returning X component value by default.");
			return v.GetX();
		}
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
