#pragma once

#include "Math.h"
//#include <string>
//#include <sstream>
//#include <map>
#include "ISort.h"

namespace Math
{

class BubbleSort : public ISort
{
/* ==================== Static variables and functions begin ==================== */
/* ==================== Static variables and functions end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	BubbleSort();
	virtual ~BubbleSort(void);
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	virtual void Sort(Math::Vector2D* vectors, int vectorSize, SortingKey sortingKey = COMPONENT_X, SortingDirection sortingDirection = ASCENDING);
	virtual void Sort(Math::Vector3D* vectors, int vectorSize, SortingKey sortingKey = COMPONENT_X, SortingDirection sortingDirection = ASCENDING);
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
/* ==================== Non-static member variables end ==================== */
}; /* end class BubbleSort */

class InsertionSort : public ISort
{
/* ==================== Static variables and functions begin ==================== */
/* ==================== Static variables and functions end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	InsertionSort();
	virtual ~InsertionSort(void);
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	virtual void Sort(Math::Vector2D* vectors, int vectorSize, SortingKey sortingKey = COMPONENT_X, SortingDirection sortingDirection = ASCENDING);
	virtual void Sort(Math::Vector3D* vectors, int vectorSize, SortingKey sortingKey = COMPONENT_X, SortingDirection sortingDirection = ASCENDING);
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
/* ==================== Non-static member variables end ==================== */
}; /* end class InsertionSort */

class SelectionSort : public ISort
{
/* ==================== Static variables and functions begin ==================== */
/* ==================== Static variables and functions end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	SelectionSort();
	virtual ~SelectionSort(void);
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	virtual void Sort(Math::Vector2D* vectors, int vectorSize, SortingKey sortingKey = COMPONENT_X, SortingDirection sortingDirection = ASCENDING);
	virtual void Sort(Math::Vector3D* vectors, int vectorSize, SortingKey sortingKey = COMPONENT_X, SortingDirection sortingDirection = ASCENDING);
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
/* ==================== Non-static member variables end ==================== */
}; /* end class SelectionSort */

class MergeSort : public ISort
{
/* ==================== Static variables and functions begin ==================== */
/* ==================== Static variables and functions end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	MergeSort();
	virtual ~MergeSort(void);
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	virtual void Sort(Math::Vector2D* vectors, int vectorSize, SortingKey sortingKey = COMPONENT_X, SortingDirection sortingDirection = ASCENDING);
	virtual void Sort(Math::Vector3D* vectors, int vectorSize, SortingKey sortingKey = COMPONENT_X, SortingDirection sortingDirection = ASCENDING);
private:
	template <typename T>
	void Sort(T* values, int left, int right, SortingKey sortingKey, SortingDirection sortingDirection, T* auxiliaryTable)
	{
		if (left >= right)
		{
			return;
		}
		int pivot = (left + right) / 2;
		Sort(values, left, pivot, sortingKey, sortingDirection, auxiliaryTable);
		Sort(values, pivot + 1, right, sortingKey, sortingDirection, auxiliaryTable);
		Merge(values, left, pivot, right, sortingKey, sortingDirection, auxiliaryTable);
	}

	template <typename T>
	void Merge(T* values, int left, int pivot, int right, SortingKey sortingKey, SortingDirection sortingDirection, T* auxiliaryTable)
	{
		int i, j, q;
		//Math::Vector2D* t = new Math::Vector2D [right - left + 1];
		int k = 0;
		for (int i = left; i <= right; ++i, ++k)
		{
			auxiliaryTable[k] = values[i]; // copying the data to auxiliary table
		}
		i = 0;
		j = pivot + 1 - left;
		q = left;
		while ( (i <= pivot - left) && (j <= right - left) ) // transferring the data with sorting from auxiliary tables to the result table
		{
			if (NeedSwapping(auxiliaryTable[j], auxiliaryTable[i], sortingKey, sortingDirection))
			{
				values[q] = auxiliaryTable[i++];
			}
			else
			{
				values[q] = auxiliaryTable[j++];
			}
			++q;
		}
		while (i <= pivot - left) // transferring the not-yet-copied data from the first set when the second one is already empty
		{
			values[q] = auxiliaryTable[i];
			++q;
			++i;
		}
		//delete [] t;
	}
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
/* ==================== Non-static member variables end ==================== */
}; /* end class MergeSort */

class HeapSort : public ISort
{
/* ==================== Static variables and functions begin ==================== */
/* ==================== Static variables and functions end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	HeapSort();
	virtual ~HeapSort(void);
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	virtual void Sort(Math::Vector2D* vectors, int vectorSize, SortingKey sortingKey = COMPONENT_X, SortingDirection sortingDirection = ASCENDING);
	virtual void Sort(Math::Vector3D* vectors, int vectorSize, SortingKey sortingKey = COMPONENT_X, SortingDirection sortingDirection = ASCENDING);
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
/* ==================== Non-static member variables end ==================== */
}; /* end class HeapSort */

class QuickSort : public ISort
{
/* ==================== Static variables and functions begin ==================== */
/* ==================== Static variables and functions end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	QuickSort();
	virtual ~QuickSort(void);
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	virtual void Sort(Math::Vector2D* vectors, int vectorSize, SortingKey sortingKey = COMPONENT_X, SortingDirection sortingDirection = ASCENDING);
	virtual void Sort(Math::Vector3D* vectors, int vectorSize, SortingKey sortingKey = COMPONENT_X, SortingDirection sortingDirection = ASCENDING);
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
/* ==================== Non-static member variables end ==================== */
}; /* end class QuickSort */

class ShellSort : public ISort
{
/* ==================== Static variables and functions begin ==================== */
/* ==================== Static variables and functions end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	ShellSort();
	virtual ~ShellSort(void);
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	virtual void Sort(Math::Vector2D* vectors, int vectorSize, SortingKey sortingKey = COMPONENT_X, SortingDirection sortingDirection = ASCENDING);
	virtual void Sort(Math::Vector3D* vectors, int vectorSize, SortingKey sortingKey = COMPONENT_X, SortingDirection sortingDirection = ASCENDING);
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
/* ==================== Non-static member variables end ==================== */
}; /* end class ShellSort */

class CombSort : public ISort
{
/* ==================== Static variables and functions begin ==================== */
/* ==================== Static variables and functions end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	CombSort();
	virtual ~CombSort(void);
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	virtual void Sort(Math::Vector2D* vectors, int vectorSize, SortingKey sortingKey = COMPONENT_X, SortingDirection sortingDirection = ASCENDING);
	virtual void Sort(Math::Vector3D* vectors, int vectorSize, SortingKey sortingKey = COMPONENT_X, SortingDirection sortingDirection = ASCENDING);
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
/* ==================== Non-static member variables end ==================== */
}; /* end class CombSort */

/**
 * Counting sort is only useful for sorting integer values
 */
class CountingSort : public ISort
{
/* ==================== Static variables and functions begin ==================== */
/* ==================== Static variables and functions end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	CountingSort();
	virtual ~CountingSort(void);
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	virtual void Sort(Math::Vector2D* vectors, int vectorSize, SortingKey sortingKey = COMPONENT_X, SortingDirection sortingDirection = ASCENDING);
	virtual void Sort(Math::Vector3D* vectors, int vectorSize, SortingKey sortingKey = COMPONENT_X, SortingDirection sortingDirection = ASCENDING);
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
/* ==================== Non-static member variables end ==================== */
}; /* end class CountingSort */

class RadixSort : public ISort
{
/* ==================== Static variables and functions begin ==================== */
/* ==================== Static variables and functions end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	RadixSort();
	virtual ~RadixSort(void);
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	virtual void Sort(Math::Vector2D* vectors, int vectorSize, SortingKey sortingKey = COMPONENT_X, SortingDirection sortingDirection = ASCENDING);
	virtual void Sort(Math::Vector3D* vectors, int vectorSize, SortingKey sortingKey = COMPONENT_X, SortingDirection sortingDirection = ASCENDING);
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
/* ==================== Non-static member variables end ==================== */
}; /* end class RadixSort */

class BucketSort : public ISort
{
/* ==================== Static variables and functions begin ==================== */
/* ==================== Static variables and functions end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	BucketSort();
	virtual ~BucketSort(void);
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	virtual void Sort(Math::Vector2D* vectors, int vectorSize, SortingKey sortingKey = COMPONENT_X, SortingDirection sortingDirection = ASCENDING);
	virtual void Sort(Math::Vector3D* vectors, int vectorSize, SortingKey sortingKey = COMPONENT_X, SortingDirection sortingDirection = ASCENDING);
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
/* ==================== Non-static member variables end ==================== */
}; /* end class BucketSort */

} /* end namespace Math */
