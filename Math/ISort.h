#pragma once

#include "Math.h"
#include "Vector.h"
#include "Utility\ILogger.h"

namespace Math
{

class ISort
{
public:
	enum MATH_API SortingMethod { BUBBLE_SORT = 0, INSERTION_SORT, SELECTION_SORT, MERGE_SORT, HEAP_SORT, QUICK_SORT, SHELL_SORT, COMB_SORT, COUNTING_SORT, RADIX_SORT, BUCKET_SORT };
	enum MATH_API SortingKey { COMPONENT_X, COMPONENT_Y, COMPONENT_Z, SUM_OF_COMPONENTS, SUM_OF_SQUARED_COMPONENTS };
	enum MATH_API SortingDirection { ASCENDING = 0, DESCENDING };
/* ==================== Static variables and functions begin ==================== */
protected:
	MATH_API static ISort* sortObject;
public:
	MATH_API static ISort& GetSortingObject(SortingMethod sortingMethod);
/* ==================== Static variables and functions end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	MATH_API ISort(SortingMethod sortingMethod);
	MATH_API virtual ~ISort(void);
private:
	ISort(const ISort& sortObject) {} // don't implement
	void operator=(const ISort& sortObject) {} // don't implement
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	MATH_API virtual void Sort(Vector2D* vectors, int vectorSize, SortingKey sortingKey = COMPONENT_X, SortingDirection sortingDirection = ASCENDING) = 0;
	MATH_API virtual void Sort(Vector3D* vectors, int vectorSize, SortingKey sortingKey = COMPONENT_X, SortingDirection sortingDirection = ASCENDING) = 0;
	MATH_API SortingMethod GetSortingMethod() const { return sortingMethod; }
protected:
	template <typename T>
	bool NeedSwapping(const T& v1, const T& v2, SortingKey sortingKey, SortingDirection sortingDirection)
	{
		/* Checking parameters */
		if (sortingKey == SUM_OF_COMPONENTS)
		{
			LOG(Utility::Critical, LOGPLACE, "Template sorting by sum of components is yet supported by the Math library");
			return false;
		}

		switch (sortingDirection)
		{
		case ASCENDING:
			switch (sortingKey)
			{
			case COMPONENT_X:
			case COMPONENT_Y:
			case COMPONENT_Z:
			case SUM_OF_SQUARED_COMPONENTS:
				return (v2 < v1);
			case SUM_OF_COMPONENTS:
			default:
				LOG(Utility::Critical, LOGPLACE, "Unknown sorting key given.");
				return false;
			}
			break;
		case DESCENDING:
			switch (sortingKey)
			{
			case COMPONENT_X:
			case COMPONENT_Y:
			case COMPONENT_Z:
			case SUM_OF_SQUARED_COMPONENTS:
				return (v2 > v1);
			case SUM_OF_COMPONENTS:
			default:
				LOG(Utility::Critical, LOGPLACE, "Unknown sorting key given.");
				return false;
			}
			break;
		default:
			LOG(Utility::Critical, LOGPLACE, "Unknown sorting direction specified.");
			return false;
			break;
		}
	}

	template <>
	bool NeedSwapping(const Math::Vector2D& vec1, const Math::Vector2D& vec2, SortingKey sortingKey, SortingDirection sortingDirection)
	{
		/* Checking parameters */
		if (sortingKey == COMPONENT_Z)
		{
			LOG(Utility::Critical, LOGPLACE, "Sorting 2D vectors by Z component is not possible. 2D vectors are defined with XY components.");
			return false;
		}
		if (sortingKey == SUM_OF_COMPONENTS)
		{
			LOG(Utility::Critical, LOGPLACE, "Sorting 2D vectors by sum of components is not yet supported by the Math library");
			return false;
		}

		switch (sortingDirection)
		{
		case ASCENDING:
			switch (sortingKey)
			{
			case COMPONENT_X:
				return (vec2.GetX() < vec1.GetX());
			case COMPONENT_Y:
				return (vec2.GetY() < vec1.GetY());
			case SUM_OF_SQUARED_COMPONENTS:
				return (vec2.LengthSquared() < vec1.LengthSquared());
			case COMPONENT_Z:
			case SUM_OF_COMPONENTS:
			default:
				LOG(Utility::Critical, LOGPLACE, "Unknown sorting key given.");
				return false;
			}
			break;
		case DESCENDING:
			switch (sortingKey)
			{
			case COMPONENT_X:
				return (vec2.GetX() > vec1.GetX());
			case COMPONENT_Y:
				return (vec2.GetY() > vec1.GetY());
			case SUM_OF_SQUARED_COMPONENTS:
				return (vec2.LengthSquared() > vec1.LengthSquared());
			case COMPONENT_Z:
			case SUM_OF_COMPONENTS:
			default:
				LOG(Utility::Critical, LOGPLACE, "Unknown sorting key specified.");
				return false;
			}
			break;
		default:
			LOG(Utility::Critical, LOGPLACE, "Unknown sorting direction specified.");
			return false;
			break;
		}
	}


	template <>
	bool NeedSwapping(const Math::Vector3D& vec1, const Math::Vector3D& vec2, SortingKey sortingKey, SortingDirection sortingDirection)
	{
		/* Checking parameters */
		if (sortingKey == SUM_OF_COMPONENTS)
		{
			LOG(Utility::Critical, LOGPLACE, "Sorting 2D vectors by sum of components is not yet supported by the Math library");
			return false;
		}

		switch (sortingDirection)
		{
		case ASCENDING:
			switch (sortingKey)
			{
			case COMPONENT_X:
				return (vec2.GetX() < vec1.GetX());
			case COMPONENT_Y:
				return (vec2.GetY() < vec1.GetY());
			case COMPONENT_Z:
				return (vec2.GetZ() < vec1.GetZ());
			case SUM_OF_SQUARED_COMPONENTS:
				return (vec2.LengthSquared() < vec1.LengthSquared());
			case SUM_OF_COMPONENTS:
			default:
				LOG(Utility::Critical, LOGPLACE, "Unknown sorting key given.");
				return false;
			}
			break;
		case DESCENDING:
			switch (sortingKey)
			{
			case COMPONENT_X:
				return (vec2.GetX() > vec1.GetX());
			case COMPONENT_Y:
				return (vec2.GetY() > vec1.GetY());
			case COMPONENT_Z:
				return (vec2.GetZ() > vec1.GetZ());
			case SUM_OF_SQUARED_COMPONENTS:
				return (vec2.LengthSquared() > vec1.LengthSquared());
			case SUM_OF_COMPONENTS:
			default:
				LOG(Utility::Critical, LOGPLACE, "Unknown sorting key specified.");
				return false;
			}
			break;
		default:
			LOG(Utility::Critical, LOGPLACE, "Unknown sorting direction specified.");
			return false;
			break;
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
	SortingMethod sortingMethod;
/* ==================== Non-static member variables end ==================== */
}; /* end class ISort */

} /* end namespace Math */
