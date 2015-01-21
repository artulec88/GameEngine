#pragma once

#include "Math.h"
//#include <string>
//#include <sstream>
//#include <map>
#include "ISort.h"
#include <vector>

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
private:
	template <typename T>
	void Sort(T* vectors, int vectorSize, SortingKey sortingKey = COMPONENT_X, SortingDirection sortingDirection = ASCENDING)
	{
		if (vectors == NULL)
		{
			LOG(Utility::Emergency, LOGPLACE, "Cannot sort the table. The specified table is NULL");
			return;
		}

		bool swapOccured = false;
		int n = vectorSize;
		do
		{
			swapOccured = false;
			for (int i = 1; i < n; ++i)
			{
				if (NeedSwapping(vectors[i - 1], vectors[i], sortingKey, sortingDirection))
				{
					Swap(vectors[i - 1], vectors[i]);
					swapOccured = true;
				}
			}
			--n;
		} while (swapOccured);
	}
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
private:
	template <typename T>
	void Sort(T* vectors, int vectorSize, SortingKey sortingKey = COMPONENT_X, SortingDirection sortingDirection = ASCENDING)
	{
		if (vectors == NULL)
		{
			LOG(Utility::Emergency, LOGPLACE, "Cannot sort the table using the insertion sort algorithm. The specified table is NULL");
			return;
		}

		for (int j = 1; j < vectorSize; j++)
		{
			int i = j - 1;
			while ( (i >= 0) && (NeedSwapping(vectors[i], vectors[j], sortingKey, sortingDirection)) )
			{
				vectors[i + 1] = vectors[i];
				i--;
			}
			vectors[i + 1] = vectors[j];
		}
	}
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
private:
	template <typename T>
	void Sort(T* vectors, int vectorSize, SortingKey sortingKey = COMPONENT_X, SortingDirection sortingDirection = ASCENDING)
	{
		if (vectors == NULL)
		{
			LOG(Utility::Emergency, LOGPLACE, "Cannot sort the table using the selection sort algorithm. The specified table is NULL");
			return;
		}

		if (vectorSize < 2)
		{
			return;
		}

		for (int k = 0; k < vectorSize - 1; ++k)
		{
			int indexToSwap = k + 1;
			T vecToSwap = vectors[indexToSwap];
			for (int i = k + 2; i < vectorSize; ++i)
			{
				if (NeedSwapping(vecToSwap, vectors[i], sortingKey, sortingDirection))
				{
					indexToSwap = i;
					vecToSwap = vectors[indexToSwap];
				}
			}
			if (NeedSwapping(vectors[k], vectors[indexToSwap], sortingKey, sortingDirection))
			{
				Swap(vectors[k], vectors[indexToSwap]);
			}
		}
	}
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
	void Sort(T* vectors, int vectorSize, SortingKey sortingKey = COMPONENT_X, SortingDirection sortingDirection = ASCENDING)
	{
		if (vectors == NULL)
		{
			LOG(Utility::Emergency, LOGPLACE, "Cannot sort the table using the merge sort algorithm. The specified table is NULL");
			return;
		}
		T* auxiliaryTable = new T [vectorSize];
		Sort<T>(vectors, 0, vectorSize - 1, sortingKey, sortingDirection, auxiliaryTable);
		delete [] auxiliaryTable;
	}

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
private:
	template <typename T>
	void Sort(T* vectors, int vectorSize, SortingKey sortingKey, SortingDirection sortingDirection)
	{
		int j = vectorSize - 1;
		int heapHeight = 0;
		while (j > 1)
		{
			j /= 2; // specifying the height of the heap based on the number of elements
			++heapHeight;
		}
		for (j = 0; j <= heapHeight; ++j)
		{
			int i = vectorSize - 1;
			while (i > 0)
			{
				int k = i / 2; // the heap is started being constructed here in the input vector
				if (NeedSwapping(vectors[i], vectors[k], sortingKey, sortingDirection)) // checking the condition of heap existance in the input vector
				{
					Swap(vectors[i], vectors[k]);
				}
				--i;
			}
		}
		for (j = vectorSize - 1; j >= 0; --j)
		{
			int i = 0;
			bool swap = true;
			while (swap)
			{
				if ( (2 * i + 1 <= j) && (NeedSwapping(vectors[2 * i + 1], vectors[2 * i], sortingKey, sortingDirection)) )
				{
					if (NeedSwapping(vectors[2 * i + 1], vectors[i], sortingKey, sortingDirection))
					{
						Swap(vectors[i], vectors[2 * i + 1]);
						i = 2 * i + 1;
					}
					else
					{
						swap = false;
					}
				}
				else if ( (2 * i <= j) && (NeedSwapping(vectors[2 * i], vectors[i], sortingKey, sortingDirection)) )
				{
					Swap(vectors[i], vectors[2 * i]);
					i = 2 * i;
				}
				else
				{
					swap = false;
				}
			}
			Swap(vectors[0], vectors[j]);
		}
	}
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
private:
	template <typename T>
	void Sort(T* vectors, int vectorSize, SortingKey sortingKey, SortingDirection sortingDirection)
	{
		if (vectors == NULL)
		{
			LOG(Utility::Emergency, LOGPLACE, "Cannot sort the table using the quick sort algorithm. The specified table is NULL");
			return;
		}
		Sort<T>(vectors, 0, vectorSize - 1, sortingKey, sortingDirection);
	}

	template <typename T>
	void Sort(T* vectors, int left, int right, SortingKey sortingKey, SortingDirection sortingDirection)
	{
		if (left >= right)
		{
			return;
		}

		T pivot = vectors[left];
		int i = left - 1;
		int j = right + 1;
		do
		{
			do
			{
				if (i > right)
					break;
				++i;
				//LOG(Utility::Debug, LOGPLACE, "i = %d; left = %d; right = %d; vectors[%d] = %s; pivot = %s",
				//	i, left, right, i, vectors[i].ToString().c_str(), pivot.ToString().c_str());
			} while ((i != left) && !NeedSwapping(vectors[i], pivot, sortingKey, sortingDirection)); // TODO: Check this condition more thoroughly
			do
			{
				if (j <= left)
					break;
				--j;
				//LOG(Utility::Debug, LOGPLACE, "j = %d; left = %d; right = %d; vectors[%d] = %s; pivot = %s",
				//	j, left, right, j, vectors[j].ToString().c_str(), pivot.ToString().c_str());
			} while(!NeedSwapping(pivot, vectors[j], sortingKey, sortingDirection)); // TODO: Check this condition more thoroughly
			if (i < j)
			{
				//LOG(Utility::Debug, LOGPLACE, "Swapping vectors[%d] (%s) with vectors[%d] (%s)", i, vectors[i].ToString().c_str(), j, vectors[j].ToString().c_str());
				Swap(vectors[i], vectors[j]);
			}
		} while (!(i > j));
		//LOG(Utility::Debug, LOGPLACE, "left1 = %d; right1 = %d", left, j);
		//if (j == right)
		//{
		//	LOG(Utility::Critical, LOGPLACE, "Calling the quick sort with the same indices. Stack overflow exception will occur.");
		//	exit(EXIT_FAILURE);
		//}
		Sort(vectors, left, j, sortingKey, sortingDirection);
		//LOG(Utility::Debug, LOGPLACE, "left2 = %d; right2 = %d", i, right);
		//if (i == left)
		//{
		//	LOG(Utility::Critical, LOGPLACE, "Calling the quick sort with the same indices. Stack overflow exception will occur.");
		//	exit(EXIT_FAILURE);
		//}
		Sort(vectors, i, right, sortingKey, sortingDirection);
	}
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
private:
	template <typename T>
	void Sort(T* vectors, int vectorSize, SortingKey sortingKey, SortingDirection sortingDirection)
	{
		if (vectors == NULL)
		{
			LOG(Utility::Emergency, LOGPLACE, "Cannot sort the table using the insertion sort algorithm. The specified table is NULL");
			return;
		}
		const int LAST_CONST_INDEX = 701;
		const Math::Real RECURSIVE_INDEX_STEP = 2.25f;
		int constIndices[] = {1, 4, 10, 23, 57, 132, 301, LAST_CONST_INDEX};
		std::vector<int> indices(constIndices, constIndices + sizeof(constIndices) / sizeof(int));
		int nextIndex = LAST_CONST_INDEX;
		while (nextIndex < vectorSize)
		{
			// TODO: use floor function instead of static_cast<int>
			nextIndex = static_cast<int>(static_cast<Math::Real>(nextIndex) * RECURSIVE_INDEX_STEP);
			indices.push_back(nextIndex);
		}
		
		/*bool swapOccured = false;
		do
		{
			for ()
			int index = 1;
			int indexStep = indices.back();
			for (int j = index; j < vectorSize; j += indexStep)
			{
				int i = j - 1;
				while ( (i >= 0) && (NeedSwapping(vectors[i], vectors[j], sortingKey, sortingDirection)) )
				{
					vectors[i + 1] = vectors[i];
					--i;
				}
				vectors[i + 1] = vectors[j];
			}
		} while ((index != 1) && swapOccured);*/
	}
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
