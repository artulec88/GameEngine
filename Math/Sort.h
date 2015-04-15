#pragma once

#include "Math.h"
//#include <string>
//#include <sstream>
//#include <map>
#include "ISort.h"
#include <vector>

namespace Math { namespace Sorting {

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
	virtual void Sort(Math::Vector2D* vectors, int vectorSize, const SortingParametersChain& sortingParameters);
	virtual void Sort(Math::Vector3D* vectors, int vectorSize, const SortingParametersChain& sortingParameters);
private:
	template <typename T>
	void Sort(T* vectors, int vectorSize, SortingKey sortingKey = COMPONENT_X, SortingDirection sortingDirection = ASCENDING)
	{
		Sort(vectors, vectorSize, SortingParametersChain(sortingKey, sortingDirection));
	}

	template <typename T>
	void Sort(T* vectors, int vectorSize, const SortingParametersChain& sortingParameters)
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
				if (NeedSwapping(vectors[i - 1], vectors[i], sortingParameters))
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
	virtual void Sort(Math::Vector2D* vectors, int vectorSize, const SortingParametersChain& sortingParameters);
	virtual void Sort(Math::Vector3D* vectors, int vectorSize, const SortingParametersChain& sortingParameters);
private:
	template <typename T>
	void Sort(T* vectors, int vectorSize, SortingKey sortingKey = COMPONENT_X, SortingDirection sortingDirection = ASCENDING)
	{
		Sort(vectors, vectorSize, SortingParametersChain(sortingKey, sortingDirection));
	}

	template <typename T>
	void Sort(T* vectors, int vectorSize, const SortingParametersChain& sortingParameters)
	{
		if (vectors == NULL)
		{
			LOG(Utility::Emergency, LOGPLACE, "Cannot sort the table using the insertion sort algorithm. The specified table is NULL");
			return;
		}

		for (int i = 1; i < vectorSize; ++i)
		{
			//for (int k = 0; k < vectorSize; ++k)
			//{
			//	LOG(Utility::Debug, LOGPLACE, "i = %d) vectors[%d] = %s", i, k, vectors[k].ToString().c_str());
			//}
			T key(vectors[i]);
			int j = i - 1;
			while ( (j >= 0) && (NeedSwapping(vectors[j], key, sortingParameters)) )
			{
				vectors[j + 1] = vectors[j];
				vectors[j] = key;
				--j;
			}
			//vectors[i + 1] = vectors[j];
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
	virtual void Sort(Math::Vector2D* vectors, int vectorSize, const SortingParametersChain& sortingParameters);
	virtual void Sort(Math::Vector3D* vectors, int vectorSize, const SortingParametersChain& sortingParameters);
private:
	template <typename T>
	void Sort(T* vectors, int vectorSize, SortingKey sortingKey = COMPONENT_X, SortingDirection sortingDirection = ASCENDING)
	{
		Sort(vectors, vectorSize, SortingParametersChain(sortingKey, sortingDirection));
	}

	template <typename T>
	void Sort(T* vectors, int vectorSize, const SortingParametersChain& sortingParameters)
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
				if (NeedSwapping(vecToSwap, vectors[i], sortingParameters))
				{
					indexToSwap = i;
					vecToSwap = vectors[indexToSwap];
				}
			}
			if (NeedSwapping(vectors[k], vectors[indexToSwap], sortingParameters))
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
	virtual void Sort(Math::Vector2D* vectors, int vectorSize, const SortingParametersChain& sortingParameters);
	virtual void Sort(Math::Vector3D* vectors, int vectorSize, const SortingParametersChain& sortingParameters);
private:
	template <typename T>
	void Sort(T* vectors, int vectorSize, SortingKey sortingKey = COMPONENT_X, SortingDirection sortingDirection = ASCENDING)
	{
		Sort(vectors, vectorSize, SortingParametersChain(sortingKey, sortingDirection));
	}

	template <typename T>
	void Sort(T* vectors, int vectorSize, const SortingParametersChain& sortingParameters)
	{
		if (vectors == NULL)
		{
			LOG(Utility::Emergency, LOGPLACE, "Cannot sort the table using the merge sort algorithm. The specified table is NULL");
			return;
		}
		T* auxiliaryTable = new T [vectorSize];
		Sort<T>(vectors, 0, vectorSize - 1, sortingParameters, auxiliaryTable);
		delete [] auxiliaryTable;
	}

	template <typename T>
	void Sort(T* values, int left, int right, const SortingParametersChain& sortingParameters, T* auxiliaryTable)
	{
		if (left >= right)
		{
			return;
		}
		int pivot = (left + right) / 2;
		Sort(values, left, pivot, sortingParameters, auxiliaryTable);
		Sort(values, pivot + 1, right, sortingParameters, auxiliaryTable);
		Merge(values, left, pivot, right, sortingParameters, auxiliaryTable);
	}

	template <typename T>
	void Merge(T* values, int left, int pivot, int right, const SortingParametersChain& sortingParameters, T* auxiliaryTable)
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
			if (NeedSwapping(auxiliaryTable[j], auxiliaryTable[i], sortingParameters))
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
	virtual void Sort(Math::Vector2D* vectors, int vectorSize, const SortingParametersChain& sortingParameters);
	virtual void Sort(Math::Vector3D* vectors, int vectorSize, const SortingParametersChain& sortingParameters);
private:
	template <typename T>
	void Sort(T* vectors, int vectorSize, SortingKey sortingKey, SortingDirection sortingDirection)
	{
		Sort(vectors, vectorSize, SortingParametersChain(sortingKey, sortingDirection));
	}

	template <typename T>
	void Sort(T* vectors, int vectorSize, const SortingParametersChain& sortingParameters)
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
				if (NeedSwapping(vectors[i], vectors[k], sortingParameters)) // checking the condition of heap existance in the input vector
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
				if ( (2 * i + 1 <= j) && (NeedSwapping(vectors[2 * i + 1], vectors[2 * i], sortingParameters)) )
				{
					if (NeedSwapping(vectors[2 * i + 1], vectors[i], sortingParameters))
					{
						Swap(vectors[i], vectors[2 * i + 1]);
						i = 2 * i + 1;
					}
					else
					{
						swap = false;
					}
				}
				else if ( (2 * i <= j) && (NeedSwapping(vectors[2 * i], vectors[i], sortingParameters)) )
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
	virtual void Sort(Math::Vector2D* vectors, int vectorSize, const SortingParametersChain& sortingParameters);
	virtual void Sort(Math::Vector3D* vectors, int vectorSize, const SortingParametersChain& sortingParameters);
private:
	template <typename T>
	void Sort(T* vectors, int vectorSize, SortingKey sortingKey, SortingDirection sortingDirection)
	{
		Sort(vectors, vectorSize, SortingParametersChain(sortingKey, sortingDirection));
	}

	template <typename T>
	void Sort(T* vectors, int vectorSize, const SortingParametersChain& sortingParameters)
	{
		if (vectors == NULL)
		{
			LOG(Utility::Emergency, LOGPLACE, "Cannot sort the table using the quick sort algorithm. The specified table is NULL");
			return;
		}
		Sort<T>(vectors, 0, vectorSize - 1, sortingParameters);
	}

	template <typename T>
	void Sort(T* vectors, int left, int right, const SortingParametersChain& sortingParameters)
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
			} while ((i != left) && !NeedSwapping(vectors[i], pivot, sortingParameters)); // TODO: Check this condition more thoroughly
			do
			{
				if (j <= left)
					break;
				--j;
				//LOG(Utility::Debug, LOGPLACE, "j = %d; left = %d; right = %d; vectors[%d] = %s; pivot = %s",
				//	j, left, right, j, vectors[j].ToString().c_str(), pivot.ToString().c_str());
			} while(!NeedSwapping(pivot, vectors[j], sortingParameters)); // TODO: Check this condition more thoroughly
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
		Sort(vectors, left, j, sortingParameters);
		//LOG(Utility::Debug, LOGPLACE, "left2 = %d; right2 = %d", i, right);
		//if (i == left)
		//{
		//	LOG(Utility::Critical, LOGPLACE, "Calling the quick sort with the same indices. Stack overflow exception will occur.");
		//	exit(EXIT_FAILURE);
		//}
		Sort(vectors, i, right, sortingParameters);
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
	virtual void Sort(Math::Vector2D* vectors, int vectorSize, const SortingParametersChain& sortingParameters);
	virtual void Sort(Math::Vector3D* vectors, int vectorSize, const SortingParametersChain& sortingParameters);
private:
	template <typename T>
	void Sort(T* vectors, int vectorSize, SortingKey sortingKey, SortingDirection sortingDirection)
	{
		Sort(vectors, vectorSize, SortingParametersChain(sortingKey, sortingDirection));
	}

	template <typename T>
	void Sort(T* vectors, int vectorSize, const SortingParametersChain& sortingParameters)
	{
		const int QUARTER_VECTOR_SIZE = vectorSize / 4;
		if (vectors == NULL)
		{
			LOG(Utility::Emergency, LOGPLACE, "Cannot sort the table using the insertion sort algorithm. The specified table is NULL");
			return;
		}
		const int FIRST_CONST_INDEX = 1;
		const int LAST_CONST_INDEX = 701;
		const Math::Real RECURSIVE_INDEX_STEP = 2.25f;
		int constIndices[] = {FIRST_CONST_INDEX, 4, 10, 23, 57, 132, 301, LAST_CONST_INDEX};
		std::vector<int> indices(constIndices, constIndices + sizeof(constIndices) / sizeof(int));
		if (indices.back() >= QUARTER_VECTOR_SIZE)
		{
			do
			{
				indices.pop_back();
			} while (indices.back() >= QUARTER_VECTOR_SIZE);
		}
		else
		{
			int nextIndex = Floor(static_cast<Math::Real>(LAST_CONST_INDEX) * RECURSIVE_INDEX_STEP);
			while (nextIndex < QUARTER_VECTOR_SIZE)
			{
				indices.push_back(nextIndex);
				nextIndex = Floor(static_cast<Math::Real>(nextIndex) * RECURSIVE_INDEX_STEP);
			}
		}
		ASSERT(indices.back() < QUARTER_VECTOR_SIZE);
		ASSERT(indices.front() == FIRST_CONST_INDEX);
		if (indices.back() >= QUARTER_VECTOR_SIZE)
		{
			LOG(Utility::Error, LOGPLACE, "Incorrect calculation of the initial gap value for the shell sort algorithm. The vector size = %d and the gap = %d", vectorSize, indices.back());
		}
		if (indices.front() != FIRST_CONST_INDEX)
		{
			LOG(Utility::Error, LOGPLACE, "Incorrect calculation of the last gap value for the shell sort algorithm. The last gap value must be equal to %d, but is equal to %d", FIRST_CONST_INDEX, indices.front());
		}

		while (!indices.empty())
		{
			int gap = indices.back();
			for (int j = vectorSize - gap - 1; j >= 0; --j)
			{
				T vec = vectors[j];
				int i = j + gap;
				while ( (i < vectorSize) && (NeedSwapping(vec, vectors[i], sortingParameters)) )
				{
					vectors[i - gap] = vectors[i];
					i += gap;
				}
				vectors[i - gap] = vec;
			}
			indices.pop_back();
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
	virtual void Sort(Math::Vector2D* vectors, int vectorSize, const SortingParametersChain& sortingParameters);
	virtual void Sort(Math::Vector3D* vectors, int vectorSize, const SortingParametersChain& sortingParameters);
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
private:
	template <typename T>
	void Sort(T* vectors, int vectorSize, SortingKey sortingKey, SortingDirection sortingDirection)
	{
		Sort(vectors, vectorSize, SortingParametersChain(sortingKey, sortingDirection));
	}

	template <typename T>
	void Sort(T* vectors, int vectorSize, const SortingParametersChain& sortingParameters)
	{
		if (vectors == NULL)
		{
			LOG(Utility::Emergency, LOGPLACE, "Cannot sort the table using the comb sort algorithm. The specified table is NULL");
			return;
		}

		int gap = vectorSize;
		Math::Real shrink = 1.3f; // a value set by the rule of thumb (see http://en.wikipedia.org/wiki/Comb_sort)

		bool swapOccured = false;
		while (gap != 1 || swapOccured)
		{
			gap = static_cast<int>(gap / shrink);
			if (gap < 1)
			{
				gap = 1;
			}
			
			int i = 0;
			swapOccured = false;
			for (int i = 0; i + gap < vectorSize; ++i)
			{
				if (NeedSwapping(vectors[i], vectors[i + gap], sortingParameters))
				{
					Swap(vectors[i], vectors[i + gap]);
					swapOccured = true;
				}
			}
		}
	}
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
	virtual void Sort(Math::Vector2D* vectors, int vectorSize, const SortingParametersChain& sortingParameters);
	virtual void Sort(Math::Vector3D* vectors, int vectorSize, const SortingParametersChain& sortingParameters);
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
	virtual void Sort(Math::Vector2D* vectors, int vectorSize, const SortingParametersChain& sortingParameters);
	virtual void Sort(Math::Vector3D* vectors, int vectorSize, const SortingParametersChain& sortingParameters);
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
/* ==================== Non-static member variables end ==================== */
}; /* end class RadixSort */

class BucketSort : public ISort
{
	template <typename T>
	class Bucket
	{
	public:
		Bucket() : lowBound(REAL_MIN), highBound(REAL_MAX)
		{
		}
		Bucket(Math::Real lowBound, Math::Real highBound) : lowBound(lowBound), highBound(highBound)
		{
		}
	public:
		bool IsEmpty() const { return vectors.empty(); }
		Math::Real GetLowBound() const { return lowBound; }
		Math::Real GetHighBound() const { return highBound; }
		void SetLowBound(Math::Real lowBound) { this->lowBound = lowBound; }
		void SetHighBound(Math::Real highBound) { this->highBound = highBound; }
		void PushVector(const T& vector) { this->vectors.push_back(vector); }
		T* GetVectors() { return &vectors[0]; }
		T GetVector(int i) const { return vectors[i]; }
		int GetVectorsSize() const { return vectors.size(); }
	private:
		std::vector<T> vectors;
		Math::Real lowBound; // TODO: Remove this member variable in the future. It is not used anywhere
		Math::Real highBound; // TODO: Remove this member variable in the future. It is not used anywhere.
	};
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
	virtual void Sort(Math::Vector2D* vectors, int vectorSize, const SortingParametersChain& sortingParameters);
	virtual void Sort(Math::Vector3D* vectors, int vectorSize, const SortingParametersChain& sortingParameters);
private:
	void FindMinMax(Math::Vector2D* vectors, int vectorSize, SortingKey sortingKey, Math::Real& minValue, Math::Real& maxValue)
	{
		if (vectors == NULL)
		{
			LOG(Utility::Emergency, LOGPLACE, "Cannot sort the table using the comb sort algorithm. The specified table is NULL");
			return;
		}

		minValue = REAL_MAX;
		maxValue = REAL_MIN;
		for (int i = 0; i < vectorSize; ++i)
		{
			Math::Real value = CollectValueByKey(vectors[i], sortingKey);
			if (minValue > value)
			{
				minValue = value;
			}
			else if (maxValue < value)
			{
				maxValue = value;
			}
		}
		ASSERT(!(maxValue < minValue));
		if (maxValue < minValue)
		{
			LOG(Utility::Error, LOGPLACE, "Incorrect values for min and max keys. The minimum = %.3f and the maximum = %.3f", minValue, maxValue);;
		}
	}

	void FindMinMax(Math::Vector3D* vectors, int vectorSize, SortingKey sortingKey, Math::Real& minValue, Math::Real& maxValue)
	{
		if (vectors == NULL)
		{
			LOG(Utility::Emergency, LOGPLACE, "Cannot sort the table using the comb sort algorithm. The specified table is NULL");
			return;
		}

		minValue = REAL_MAX;
		maxValue = REAL_MIN;
		for (int i = 0; i < vectorSize; ++i)
		{
			Math::Real value = CollectValueByKey(vectors[i], sortingKey);
			if (minValue > value)
			{
				minValue = value;
			}
			else if (maxValue < value)
			{
				maxValue = value;
			}
		}
		ASSERT(!(maxValue < minValue));
		//if (maxValue < minValue)
		//{
		//	LOG(Utility::Error, LOGPLACE, "Incorrect values for min and max keys. The minimum = %.3f and the maximum = %.3f", minValue, maxValue);;
		//}
	}

	template <typename T>
	void Sort(T* vectors, int vectorSize, SortingKey sortingKey, SortingDirection sortingDirection)
	{
		Math::Real minValue, maxValue;
		FindMinMax(vectors, vectorSize, sortingKey, minValue, maxValue);
		//LOG(Utility::Debug, LOGPLACE, "minValue = %.4f; maxValue = %.4f", minValue, maxValue);

		//const int NUMBER_OF_BUCKETS = ((vectorSize / 20) < 2) ? 2 : vectorSize / 20; // estimated by myself
		const int NUMBER_OF_BUCKETS = static_cast<int>(sqrt(static_cast<Math::Real>(vectorSize)));
		const Math::Real bucketWidth = static_cast<Math::Real>((maxValue - minValue) / NUMBER_OF_BUCKETS);
		Bucket<T>* buckets = new Bucket<T> [NUMBER_OF_BUCKETS];
		Math::Real bucketLowBound = minValue;
		for (int i = 0; i < NUMBER_OF_BUCKETS; ++i)
		{
			buckets[i].SetLowBound(bucketLowBound);
			buckets[i].SetHighBound(bucketLowBound + bucketWidth);
			//LOG(Utility::Debug, LOGPLACE, "Bucket[%d] takes range [%.3f; %.3f)", i, bucketLowBound, bucketLowBound + bucketWidth);
			bucketLowBound += bucketWidth;
		}

		for (int i = 0; i < vectorSize; ++i)
		{
			Math::Real value = CollectValueByKey(vectors[i], sortingKey);
			//LOG(Utility::Debug, LOGPLACE, "vectors[%d] = %s and the value = %.4f", i, vectors[i].ToString().c_str(), value);
			
			// Calculate the index of the bucket to which we will add the vector
			int bucketIndex = static_cast<int>(NUMBER_OF_BUCKETS * ((value - minValue) / (maxValue - minValue))); // TODO: Is it possible for minValue == maxValue? If so, then we have a division by 0 problem.
			if (bucketIndex == NUMBER_OF_BUCKETS)
			{
				--bucketIndex;
			}

			ASSERT((bucketIndex >= 0) && (bucketIndex < NUMBER_OF_BUCKETS));

			//if ((bucketIndex < 0) || (bucketIndex >= NUMBER_OF_BUCKETS))
			//{
			//	LOG(Utility::Critical, LOGPLACE, "Miscalculated bucket index. Bucket index must be within range [0; %d), but is equal to %d", NUMBER_OF_BUCKETS, bucketIndex);
			//}
			buckets[bucketIndex].PushVector(vectors[i]);
		}
		ISort* insertionSorter = ISort::GetSortingObject(Sorting::INSERTION_SORT);
		for (int i = 0; i < NUMBER_OF_BUCKETS; ++i)
		{
			//for (int j = 0; j < buckets[i].GetVectorsSize(); ++j)
			//{
			//	LOG(Utility::Debug, LOGPLACE, "buckets[%d][%d] = %s", i, j, buckets[i].GetVector(j).ToString().c_str());
			//}
			if (!buckets[i].IsEmpty())
			{
				insertionSorter->Sort(buckets[i].GetVectors(), buckets[i].GetVectorsSize(), sortingKey, sortingDirection);
			}
		}
		SAFE_DELETE(insertionSorter);

		//for (int i = 0; i < NUMBER_OF_BUCKETS; ++i)
		//{
		//	for (int j = 0; j < buckets[i].GetVectorsSize(); ++j)
		//	{
		//		LOG(Utility::Debug, LOGPLACE, "buckets[%d][%d] = %s", i, j, buckets[i].GetVector(j).ToString().c_str());
		//	}
		//}

		int index = 0;
		switch (sortingDirection)
		{
		case ASCENDING:
			for (int i = 0; i < NUMBER_OF_BUCKETS; ++i)
			{
				for (int j = 0; j < buckets[i].GetVectorsSize(); ++j, ++index)
				{
					vectors[index] = buckets[i].GetVector(j);
				}
			}
			break;
		case DESCENDING:
			for (int i = NUMBER_OF_BUCKETS - 1; i >= 0; --i)
			{
				for (int j = 0; j < buckets[i].GetVectorsSize(); ++j, ++index)
				{
					vectors[index] = buckets[i].GetVector(j);
				}
			}
			break;
		default:
			LOG(Utility::Error, LOGPLACE, "");
			break;
		}
	}

	template <typename T>
	void Sort(T* vectors, int vectorSize, const SortingParametersChain& sortingParameters)
	{
		Math::Real minValue, maxValue;
		FindMinMax(vectors, vectorSize, sortingParameters.GetSortingKey(), minValue, maxValue);
		//LOG(Utility::Debug, LOGPLACE, "minValue = %.4f; maxValue = %.4f", minValue, maxValue);

		//const int NUMBER_OF_BUCKETS = ((vectorSize / 20) < 2) ? 2 : vectorSize / 20; // estimated by myself
		const int NUMBER_OF_BUCKETS = static_cast<int>(sqrt(static_cast<Math::Real>(vectorSize)));
		const Math::Real bucketWidth = static_cast<Math::Real>((maxValue - minValue) / NUMBER_OF_BUCKETS);
		Bucket<T>* buckets = new Bucket<T> [NUMBER_OF_BUCKETS];
		Math::Real bucketLowBound = minValue;
		for (int i = 0; i < NUMBER_OF_BUCKETS; ++i)
		{
			buckets[i].SetLowBound(bucketLowBound);
			buckets[i].SetHighBound(bucketLowBound + bucketWidth);
			//LOG(Utility::Debug, LOGPLACE, "Bucket[%d] takes range [%.3f; %.3f)", i, bucketLowBound, bucketLowBound + bucketWidth);
			bucketLowBound += bucketWidth;
		}

		for (int i = 0; i < vectorSize; ++i)
		{
			Math::Real value = CollectValueByKey(vectors[i], sortingParameters.GetSortingKey());
			//LOG(Utility::Debug, LOGPLACE, "vectors[%d] = %s and the value = %.4f", i, vectors[i].ToString().c_str(), value);
			
			// Calculate the index of the bucket to which we will add the vector
			int bucketIndex = static_cast<int>(NUMBER_OF_BUCKETS * ((value - minValue) / (maxValue - minValue))); // TODO: Is it possible for minValue == maxValue? If so, then we have a division by 0 problem.
			if (bucketIndex == NUMBER_OF_BUCKETS)
			{
				--bucketIndex;
			}

			ASSERT((bucketIndex >= 0) && (bucketIndex < NUMBER_OF_BUCKETS));

			//if ((bucketIndex < 0) || (bucketIndex >= NUMBER_OF_BUCKETS))
			//{
			//	LOG(Utility::Critical, LOGPLACE, "Miscalculated bucket index. Bucket index must be within range [0; %d), but is equal to %d", NUMBER_OF_BUCKETS, bucketIndex);
			//}
			buckets[bucketIndex].PushVector(vectors[i]);
		}
		ISort* insertionSorter = ISort::GetSortingObject(Sorting::INSERTION_SORT);
		for (int i = 0; i < NUMBER_OF_BUCKETS; ++i)
		{
			//for (int j = 0; j < buckets[i].GetVectorsSize(); ++j)
			//{
			//	LOG(Utility::Debug, LOGPLACE, "buckets[%d][%d] = %s", i, j, buckets[i].GetVector(j).ToString().c_str());
			//}
			if (!buckets[i].IsEmpty())
			{
				insertionSorter->Sort(buckets[i].GetVectors(), buckets[i].GetVectorsSize(), sortingParameters);
			}
		}
		SAFE_DELETE(insertionSorter);

		//for (int i = 0; i < NUMBER_OF_BUCKETS; ++i)
		//{
		//	for (int j = 0; j < buckets[i].GetVectorsSize(); ++j)
		//	{
		//		LOG(Utility::Debug, LOGPLACE, "buckets[%d][%d] = %s", i, j, buckets[i].GetVector(j).ToString().c_str());
		//	}
		//}

		int index = 0;
		switch (sortingParameters.GetSortingDirection())
		{
		case ASCENDING:
			for (int i = 0; i < NUMBER_OF_BUCKETS; ++i)
			{
				for (int j = 0; j < buckets[i].GetVectorsSize(); ++j, ++index)
				{
					vectors[index] = buckets[i].GetVector(j);
				}
			}
			break;
		case DESCENDING:
			for (int i = NUMBER_OF_BUCKETS - 1; i >= 0; --i)
			{
				for (int j = 0; j < buckets[i].GetVectorsSize(); ++j, ++index)
				{
					vectors[index] = buckets[i].GetVector(j);
				}
			}
			break;
		default:
			LOG(Utility::Error, LOGPLACE, "");
			break;
		}
	}
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
/* ==================== Non-static member variables end ==================== */
}; /* end class BucketSort */

} /* end namespace Sorting */

} /* end namespace Math */
