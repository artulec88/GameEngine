#ifndef __MATH_SORT_H__
#define __MATH_SORT_H__

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
	virtual void Sort(int* values, size_t valuesCount, SortingDirection sortingDirection = ASCENDING);
	virtual void Sort(Real* values, size_t valuesCount, SortingDirection sortingDirection = ASCENDING);
	virtual void Sort(Vector2D* vectors, size_t vectorSize, SortingKey sortingKey = COMPONENT_X, SortingDirection sortingDirection = ASCENDING);
	virtual void Sort(Vector3D* vectors, size_t vectorSize, SortingKey sortingKey = COMPONENT_X, SortingDirection sortingDirection = ASCENDING);
	virtual void Sort(Vector2D* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters);
	virtual void Sort(Vector3D* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters);
private:
	template <typename T>
	void Sort(T* vectors, size_t vectorSize, SortingKey sortingKey = COMPONENT_X, SortingDirection sortingDirection = ASCENDING)
	{
		Sort(vectors, vectorSize, SortingParametersChain(sortingKey, sortingDirection));
	}

	template <typename T>
	void Sort(T* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters)
	{
		if (vectors == NULL)
		{
			EMERGENCY_LOG_MATH("Cannot sort the table. The specified table is NULL");
			return;
		}

		bool swapOccured = false;
		size_t n = vectorSize;
		do
		{
			swapOccured = false;
			for (size_t i = 1; i < n; ++i)
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
	virtual void Sort(int* values, size_t valuesCount, SortingDirection sortingDirection = ASCENDING);
	virtual void Sort(Real* values, size_t valuesCount, SortingDirection sortingDirection = ASCENDING);
	virtual void Sort(Math::Vector2D* vectors, size_t vectorSize, SortingKey sortingKey = COMPONENT_X, SortingDirection sortingDirection = ASCENDING);
	virtual void Sort(Math::Vector3D* vectors, size_t vectorSize, SortingKey sortingKey = COMPONENT_X, SortingDirection sortingDirection = ASCENDING);
	virtual void Sort(Math::Vector2D* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters);
	virtual void Sort(Math::Vector3D* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters);
private:
	template <typename T>
	void Sort(T* vectors, size_t vectorSize, SortingKey sortingKey = COMPONENT_X, SortingDirection sortingDirection = ASCENDING)
	{
		Sort(vectors, vectorSize, SortingParametersChain(sortingKey, sortingDirection));
	}

	template <typename T>
	void Sort(T* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters)
	{
		if (vectors == NULL)
		{
			EMERGENCY_LOG_MATH("Cannot sort the table using the insertion sort algorithm. The specified table is NULL");
			return;
		}

		for (int i = 1; i < vectorSize; ++i)
		{
			T key(vectors[i]);
			int j = i - 1;
			while ( (j >= 0) && (NeedSwapping(vectors[j], key, sortingParameters)) )
			{
				vectors[j + 1] = vectors[j];
				vectors[j] = key;
				--j;
			}
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
	virtual void Sort(int* values, size_t valuesCount, SortingDirection sortingDirection = ASCENDING);
	virtual void Sort(Real* values, size_t valuesCount, SortingDirection sortingDirection = ASCENDING);
	virtual void Sort(Math::Vector2D* vectors, size_t vectorSize, SortingKey sortingKey = COMPONENT_X, SortingDirection sortingDirection = ASCENDING);
	virtual void Sort(Math::Vector3D* vectors, size_t vectorSize, SortingKey sortingKey = COMPONENT_X, SortingDirection sortingDirection = ASCENDING);
	virtual void Sort(Math::Vector2D* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters);
	virtual void Sort(Math::Vector3D* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters);
private:
	template <typename T>
	void Sort(T* vectors, size_t vectorSize, SortingKey sortingKey = COMPONENT_X, SortingDirection sortingDirection = ASCENDING)
	{
		Sort(vectors, vectorSize, SortingParametersChain(sortingKey, sortingDirection));
	}

	template <typename T>
	void Sort(T* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters)
	{
		if (vectors == NULL)
		{
			EMERGENCY_LOG_MATH("Cannot sort the table using the selection sort algorithm. The specified table is NULL");
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
	virtual void Sort(int* values, size_t valuesCount, SortingDirection sortingDirection = ASCENDING);
	virtual void Sort(Real* values, size_t valuesCount, SortingDirection sortingDirection = ASCENDING);
	virtual void Sort(Math::Vector2D* vectors, size_t vectorSize, SortingKey sortingKey = COMPONENT_X, SortingDirection sortingDirection = ASCENDING);
	virtual void Sort(Math::Vector3D* vectors, size_t vectorSize, SortingKey sortingKey = COMPONENT_X, SortingDirection sortingDirection = ASCENDING);
	virtual void Sort(Math::Vector2D* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters);
	virtual void Sort(Math::Vector3D* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters);
private:
	template <typename T>
	void Sort(T* vectors, size_t vectorSize, SortingKey sortingKey = COMPONENT_X, SortingDirection sortingDirection = ASCENDING)
	{
		Sort(vectors, vectorSize, SortingParametersChain(sortingKey, sortingDirection));
	}

	template <typename T>
	void Sort(T* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters)
	{
		if (vectors == NULL)
		{
			EMERGENCY_LOG_MATH("Cannot sort the table using the merge sort algorithm. The specified table is NULL");
			return;
		}
		std::vector<T> auxiliaryTable(vectorSize);
		Sort<T>(vectors, 0, vectorSize - 1, sortingParameters, auxiliaryTable.data());
	}

	template <typename T>
	void Sort(T* values, size_t left, size_t right, const SortingParametersChain& sortingParameters, T* auxiliaryTable)
	{
		if (left >= right)
		{
			return;
		}
		size_t pivot = (left + right) / 2;
		Sort(values, left, pivot, sortingParameters, auxiliaryTable);
		Sort(values, pivot + 1, right, sortingParameters, auxiliaryTable);
		Merge(values, left, pivot, right, sortingParameters, auxiliaryTable);
	}

	template <typename T>
	void Merge(T* values, size_t left, size_t pivot, size_t right, const SortingParametersChain& sortingParameters, T* auxiliaryTable)
	{
		size_t i, j, q;
		//Math::Vector2D* t = new Math::Vector2D [right - left + 1];
		size_t k = 0;
		for (i = left; i <= right; ++i, ++k)
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
	virtual void Sort(int* values, size_t valuesCount, SortingDirection sortingDirection = ASCENDING);
	virtual void Sort(Real* values, size_t valuesCount, SortingDirection sortingDirection = ASCENDING);
	virtual void Sort(Math::Vector2D* vectors, size_t vectorSize, SortingKey sortingKey = COMPONENT_X, SortingDirection sortingDirection = ASCENDING);
	virtual void Sort(Math::Vector3D* vectors, size_t vectorSize, SortingKey sortingKey = COMPONENT_X, SortingDirection sortingDirection = ASCENDING);
	virtual void Sort(Math::Vector2D* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters);
	virtual void Sort(Math::Vector3D* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters);
private:
	template <typename T>
	void Sort(T* vectors, size_t vectorSize, SortingKey sortingKey, SortingDirection sortingDirection)
	{
		Sort(vectors, vectorSize, SortingParametersChain(sortingKey, sortingDirection));
	}

	template <typename T>
	void Sort(T* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters)
	{
		size_t j = vectorSize - 1;
		int heapHeight = 0;
		while (j > 1)
		{
			j /= 2; // specifying the height of the heap based on the number of elements
			++heapHeight;
		}
		for (j = 0; j <= heapHeight; ++j)
		{
			size_t i = vectorSize - 1;
			while (i > 0)
			{
				size_t k = i / 2; // the heap is started being constructed here in the input vector
				if (NeedSwapping(vectors[i], vectors[k], sortingParameters)) // checking the condition of heap existance in the input vector
				{
					Swap(vectors[i], vectors[k]);
				}
				--i;
			}
		}
		for (j = vectorSize - 1; j >= 0; --j)
		{
			size_t i = 0;
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
	virtual void Sort(int* values, size_t valuesCount, SortingDirection sortingDirection = ASCENDING);
	virtual void Sort(Real* values, size_t valuesCount, SortingDirection sortingDirection = ASCENDING);
	virtual void Sort(Math::Vector2D* vectors, size_t vectorSize, SortingKey sortingKey = COMPONENT_X, SortingDirection sortingDirection = ASCENDING);
	virtual void Sort(Math::Vector3D* vectors, size_t vectorSize, SortingKey sortingKey = COMPONENT_X, SortingDirection sortingDirection = ASCENDING);
	virtual void Sort(Math::Vector2D* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters);
	virtual void Sort(Math::Vector3D* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters);
private:
	template <typename T>
	void Sort(T* vectors, size_t vectorSize, SortingKey sortingKey, SortingDirection sortingDirection)
	{
		Sort(vectors, vectorSize, SortingParametersChain(sortingKey, sortingDirection));
	}

	template <typename T>
	void Sort(T* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters)
	{
		if (vectors == NULL)
		{
			EMERGENCY_LOG_MATH("Cannot sort the table using the quick sort algorithm. The specified table is NULL");
			return;
		}
		Sort<T>(vectors, 0, vectorSize - 1, sortingParameters);
	}

	template <typename T>
	void Sort(T* vectors, size_t left, size_t right, const SortingParametersChain& sortingParameters)
	{
		if (left >= right)
		{
			return;
		}

		T pivot = vectors[left];
		size_t i = left - 1;
		size_t j = right + 1;
		do
		{
			do
			{
				if (i > right)
					break;
				++i;
				//DEBUG_LOG_MATH("i = ", i, "; left = ", left, "; right = ", right, "; vectors[", i, "] = ", vectors[i].ToString(), "; pivot = ", pivot.ToString());
			} while ((i != left) && !NeedSwapping(vectors[i], pivot, sortingParameters)); // TODO: Check this condition more thoroughly
			do
			{
				if (j <= left)
					break;
				--j;
				//DEBUG_LOG_MATH("j = ", j, "; left = ", left, "; right = ", right, "; vectors[", j, "] = ", vectors[j].ToString(), "; pivot = ", pivot.ToString());
			} while(!NeedSwapping(pivot, vectors[j], sortingParameters)); // TODO: Check this condition more thoroughly
			if (i < j)
			{
				//DEBUG_LOG_MATH("Swapping vectors[", i, "] (", vectors[i].ToString(), ") with vectors[", j, "] (", vectors[j].ToString(), ")");
				Swap(vectors[i], vectors[j]);
			}
		} while (!(i > j));
		//DEBUG_LOG_MATH("left1 = ", left, "; right1 = ", j);
		//if (j == right)
		//{
		//	CRITICAL_LOG_MATH("Calling the quick sort with the same indices. Stack overflow exception will occur.");
		//	exit(EXIT_FAILURE);
		//}
		Sort(vectors, left, j, sortingParameters);
		//DEBUG_LOG_MATH("left2 = ", i, "; right2 = ", right);
		//if (i == left)
		//{
		//	CRITICAL_LOG_MATH("Calling the quick sort with the same indices. Stack overflow exception will occur.");
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
	virtual void Sort(int* values, size_t valuesCount, SortingDirection sortingDirection = ASCENDING);
	virtual void Sort(Real* values, size_t valuesCount, SortingDirection sortingDirection = ASCENDING);
	virtual void Sort(Math::Vector2D* vectors, size_t vectorSize, SortingKey sortingKey = COMPONENT_X, SortingDirection sortingDirection = ASCENDING);
	virtual void Sort(Math::Vector3D* vectors, size_t vectorSize, SortingKey sortingKey = COMPONENT_X, SortingDirection sortingDirection = ASCENDING);
	virtual void Sort(Math::Vector2D* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters);
	virtual void Sort(Math::Vector3D* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters);
private:
	template <typename T>
	void Sort(T* vectors, size_t vectorSize, SortingKey sortingKey, SortingDirection sortingDirection)
	{
		Sort(vectors, vectorSize, SortingParametersChain(sortingKey, sortingDirection));
	}

	template <typename T>
	void Sort(T* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters)
	{
		const size_t QUARTER_VECTOR_SIZE = vectorSize / 4;
		if (vectors == NULL)
		{
			EMERGENCY_LOG_MATH("Cannot sort the table using the insertion sort algorithm. The specified table is NULL");
			return;
		}
		const size_t FIRST_CONST_INDEX = 1;
		const size_t LAST_CONST_INDEX = 701;
		const Math::Real RECURSIVE_INDEX_STEP = 2.25f;
		int constIndices[] = {FIRST_CONST_INDEX, 4, 10, 23, 57, 132, 301, LAST_CONST_INDEX};
		std::vector<size_t> indices(constIndices, constIndices + sizeof(constIndices) / sizeof(size_t));
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
		CHECK_CONDITION_MATH(indices.back() < QUARTER_VECTOR_SIZE, Utility::Logging::ERR, "Incorrect calculation of the initial gap value for the shell sort algorithm. The vector size = ", vectorSize, " and the gap = ", indices.back());
		CHECK_CONDITION_MATH(indices.front() == FIRST_CONST_INDEX, Utility::Logging::ERR, "Incorrect calculation of the last gap value for the shell sort algorithm. The last gap value must be equal to ", FIRST_CONST_INDEX, ", but is equal to ", indices.front());

		while (!indices.empty())
		{
			size_t gap = indices.back();
			for (size_t j = vectorSize - gap - 1; j >= 0; --j)
			{
				T vec = vectors[j];
				size_t i = j + gap;
				while ( (i < vectorSize) && (NeedSwapping(vec, vectors[i], sortingParameters)) )
				{
					vectors[i - gap] = vectors[i];
					i += gap;
				}
				vectors[i - gap] = vec;
			}
			indices.pop_back();
		}
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
	virtual void Sort(int* values, size_t valuesCount, SortingDirection sortingDirection = ASCENDING);
	virtual void Sort(Real* values, size_t valuesCount, SortingDirection sortingDirection = ASCENDING);
	virtual void Sort(Math::Vector2D* vectors, size_t vectorSize, SortingKey sortingKey = COMPONENT_X, SortingDirection sortingDirection = ASCENDING);
	virtual void Sort(Math::Vector3D* vectors, size_t vectorSize, SortingKey sortingKey = COMPONENT_X, SortingDirection sortingDirection = ASCENDING);
	virtual void Sort(Math::Vector2D* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters);
	virtual void Sort(Math::Vector3D* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters);
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
private:
	template <typename T>
	void Sort(T* vectors, size_t vectorSize, SortingKey sortingKey, SortingDirection sortingDirection)
	{
		Sort(vectors, vectorSize, SortingParametersChain(sortingKey, sortingDirection));
	}

	template <typename T>
	void Sort(T* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters)
	{
		if (vectors == NULL)
		{
			EMERGENCY_LOG_MATH("Cannot sort the table using the comb sort algorithm. The specified table is NULL");
			return;
		}

		size_t gap = vectorSize;
		
		// TODO: replace with some predefined const from the configuration file.
		// This value is set by the rule of thumb (see http://en.wikipedia.org/wiki/Comb_sort)
		Math::Real shrink = 1.3f;

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
	virtual void Sort(int* values, size_t valuesCount, SortingDirection sortingDirection = ASCENDING);
	virtual void Sort(Real* values, size_t valuesCount, SortingDirection sortingDirection = ASCENDING);
	virtual void Sort(Math::Vector2D* vectors, size_t vectorSize, SortingKey sortingKey = COMPONENT_X, SortingDirection sortingDirection = ASCENDING);
	virtual void Sort(Math::Vector3D* vectors, size_t vectorSize, SortingKey sortingKey = COMPONENT_X, SortingDirection sortingDirection = ASCENDING);
	virtual void Sort(Math::Vector2D* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters);
	virtual void Sort(Math::Vector3D* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters);
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
	virtual void Sort(int* values, size_t valuesCount, SortingDirection sortingDirection = ASCENDING);
	virtual void Sort(Real* values, size_t valuesCount, SortingDirection sortingDirection = ASCENDING);
	virtual void Sort(Math::Vector2D* vectors, size_t vectorSize, SortingKey sortingKey = COMPONENT_X, SortingDirection sortingDirection = ASCENDING);
	virtual void Sort(Math::Vector3D* vectors, size_t vectorSize, SortingKey sortingKey = COMPONENT_X, SortingDirection sortingDirection = ASCENDING);
	virtual void Sort(Math::Vector2D* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters);
	virtual void Sort(Math::Vector3D* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters);
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
		Bucket() : m_lowBound(REAL_MIN), m_highBound(REAL_MAX)
		{
		}
		Bucket(Math::Real lowBound, Math::Real highBound) : m_lowBound(lowBound), m_highBound(highBound)
		{
		}
	public:
		bool IsEmpty() const { return m_vectors.empty(); }
		Math::Real GetLowBound() const { return m_lowBound; }
		Math::Real GetHighBound() const { return m_highBound; }
		void SetLowBound(Math::Real lowBound) { m_lowBound = lowBound; }
		void SetHighBound(Math::Real highBound) { m_highBound = highBound; }
		void PushVector(const T& vector) { m_vectors.push_back(vector); }
		T* GetVectors() { return &m_vectors[0]; }
		const T& GetVector(int i) const { return m_vectors[i]; }
		size_t GetVectorsSize() const { return m_vectors.size(); }
	private:
		std::vector<T> m_vectors;
		Math::Real m_lowBound; // TODO: Remove this member variable in the future. It is not used anywhere
		Math::Real m_highBound; // TODO: Remove this member variable in the future. It is not used anywhere.
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
	virtual void Sort(int* values, size_t valuesCount, SortingDirection sortingDirection = ASCENDING);
	virtual void Sort(Real* values, size_t valuesCount, SortingDirection sortingDirection = ASCENDING);
	virtual void Sort(Math::Vector2D* vectors, size_t vectorSize, SortingKey sortingKey = COMPONENT_X, SortingDirection sortingDirection = ASCENDING);
	virtual void Sort(Math::Vector3D* vectors, size_t vectorSize, SortingKey sortingKey = COMPONENT_X, SortingDirection sortingDirection = ASCENDING);
	virtual void Sort(Math::Vector2D* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters);
	virtual void Sort(Math::Vector3D* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters);
private:
	void FindMinMax(Math::Vector2D* vectors, size_t vectorSize, SortingKey sortingKey, Math::Real& minValue, Math::Real& maxValue)
	{
		if (vectors == NULL)
		{
			EMERGENCY_LOG_MATH("Cannot sort the table using the comb sort algorithm. The specified table is NULL");
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
		CHECK_CONDITION_MATH(!(maxValue < minValue), Utility::Logging::ERR, "Incorrect values for min and max keys. The minimum = ", minValue, " and the maximum = ", maxValue);
	}

	void FindMinMax(Math::Vector3D* vectors, size_t vectorSize, SortingKey sortingKey, Math::Real& minValue, Math::Real& maxValue)
	{
		if (vectors == NULL)
		{
			EMERGENCY_LOG_MATH("Cannot sort the table using the comb sort algorithm. The specified table is NULL");
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
		CHECK_CONDITION_MATH(!(maxValue < minValue), Utility::Logging::ERR, "Incorrect values for min and max keys. The minimum = ", minValue, " and the maximum = ", maxValue);
	}

	template <typename T>
	void Sort(T* vectors, size_t vectorSize, SortingKey sortingKey, SortingDirection sortingDirection)
	{
		Math::Real minValue, maxValue;
		FindMinMax(vectors, vectorSize, sortingKey, minValue, maxValue);
		//DEBUG_LOG_MATH("minValue = ", minValue, "; maxValue = ", maxValue);

		//const int NUMBER_OF_BUCKETS = ((vectorSize / 20) < 2) ? 2 : vectorSize / 20; // estimated by myself
		const int NUMBER_OF_BUCKETS = static_cast<int>(sqrt(static_cast<Math::Real>(vectorSize)));
		const Math::Real bucketWidth = static_cast<Math::Real>((maxValue - minValue) / NUMBER_OF_BUCKETS);
		std::vector<Bucket<T>> buckets;
		buckets.reserve(NUMBER_OF_BUCKETS);
		Math::Real bucketLowBound = minValue;
		for (int i = 0; i < NUMBER_OF_BUCKETS; ++i)
		{
			buckets.emplace_back(Bucket<T>(bucketLowBound, bucketLowBound + bucketWidth));
			//DEBUG_LOG_MATH("Bucket[", i, "] takes range [", bucketLowBound, "; ", bucketLowBound + bucketWidth, ")");
			bucketLowBound += bucketWidth;
		}

		for (int i = 0; i < vectorSize; ++i)
		{
			Math::Real value = CollectValueByKey(vectors[i], sortingKey);
			//DEBUG_LOG_MATH("vectors[", i, "] = ", vectors[i].ToString(), " and the value = ", value);
			
			// Calculate the index of the bucket to which we will add the vector
			int bucketIndex = static_cast<int>(NUMBER_OF_BUCKETS * ((value - minValue) / (maxValue - minValue))); // TODO: Is it possible for minValue == maxValue? If so, then we have a division by 0 problem.
			if (bucketIndex == NUMBER_OF_BUCKETS)
			{
				--bucketIndex;
			}

			CHECK_CONDITION_MATH((bucketIndex >= 0) && (bucketIndex < NUMBER_OF_BUCKETS), Utility::Logging::CRITICAL, "Miscalculated bucket index. Bucket index must be within range [0; ", NUMBER_OF_BUCKETS, "), but is equal to ", bucketIndex);
			buckets[bucketIndex].PushVector(vectors[i]);
		}
		std::unique_ptr<ISort> insertionSorter = ISort::GetSortingObject(Sorting::SortingAlgorithms::INSERTION_SORT);
		for (int i = 0; i < NUMBER_OF_BUCKETS; ++i)
		{
			//for (int j = 0; j < buckets[i].GetVectorsSize(); ++j)
			//{
			//	DEBUG_LOG_MATH("buckets[", i, "][", j, "] = ", buckets[i].GetVector(j).ToString());
			//}
			if (!buckets[i].IsEmpty())
			{
				insertionSorter->Sort(buckets[i].GetVectors(), buckets[i].GetVectorsSize(), sortingKey, sortingDirection);
			}
		}

		//for (int i = 0; i < NUMBER_OF_BUCKETS; ++i)
		//{
		//	for (int j = 0; j < buckets[i].GetVectorsSize(); ++j)
		//	{
		//		DEBUG_LOG_MATH("buckets[", i, "][", j, "] = ", buckets[i].GetVector(j).ToString());
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
			ERROR_LOG_MATH("Unknown sorting direction value (", sortingDirection, "). Only ASCENDING (", ASCENDING, ") and DESCENDING (", DESCENDING, ") are acceptable.");
			break;
		}
	}

	template <typename T>
	void Sort(T* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters)
	{
		Math::Real minValue, maxValue;
		FindMinMax(vectors, vectorSize, sortingParameters.GetSortingKey(), minValue, maxValue);
		//DEBUG_LOG_MATH("minValue = ", minValue, "; maxValue = ", maxValue);

		//const int NUMBER_OF_BUCKETS = ((vectorSize / 20) < 2) ? 2 : vectorSize / 20; // estimated by myself
		const int NUMBER_OF_BUCKETS = static_cast<int>(sqrt(static_cast<Math::Real>(vectorSize)));
		const Math::Real bucketWidth = static_cast<Math::Real>((maxValue - minValue) / NUMBER_OF_BUCKETS);
		std::vector<Bucket<T>> buckets;
		buckets.reserve(NUMBER_OF_BUCKETS);
		Math::Real bucketLowBound = minValue;
		for (int i = 0; i < NUMBER_OF_BUCKETS; ++i)
		{
			buckets.emplace_back(Bucket<T>(bucketLowBound, bucketLowBound + bucketWidth));
			//DEBUG_LOG_MATH("Bucket[", i, "] takes range [", bucketLowBound, "; ", bucketLowBound + bucketWidth, ")");
			bucketLowBound += bucketWidth;
		}

		for (int i = 0; i < vectorSize; ++i)
		{
			Math::Real value = CollectValueByKey(vectors[i], sortingParameters.GetSortingKey());
			//DEBUG_LOG_MATH("vectors[", i, "] = ", vectors[i].ToString(), " and the value = ", value);
			
			// Calculate the index of the bucket to which we will add the vector
			int bucketIndex = static_cast<int>(NUMBER_OF_BUCKETS * ((value - minValue) / (maxValue - minValue))); // TODO: Is it possible for minValue == maxValue? If so, then we have a division by 0 problem.
			if (bucketIndex == NUMBER_OF_BUCKETS)
			{
				--bucketIndex;
			}

			CHECK_CONDITION_MATH((bucketIndex >= 0) && (bucketIndex < NUMBER_OF_BUCKETS), Utility::Logging::ERR, "Miscalculated bucket index. Bucket index must be within range [0; ", NUMBER_OF_BUCKETS, "), but is equal to ", bucketIndex);
			buckets[bucketIndex].PushVector(vectors[i]);
		}
		std::unique_ptr<ISort> insertionSorter = ISort::GetSortingObject(Sorting::SortingAlgorithms::INSERTION_SORT);
		for (int i = 0; i < NUMBER_OF_BUCKETS; ++i)
		{
			//for (int j = 0; j < buckets[i].GetVectorsSize(); ++j)
			//{
			//	DEBUG_LOG_MATH("buckets[", i, "][", j, "] = ", buckets[i].GetVector(j).ToString());
			//}
			if (!buckets[i].IsEmpty())
			{
				insertionSorter->Sort(buckets[i].GetVectors(), buckets[i].GetVectorsSize(), sortingParameters);
			}
		}

		//for (int i = 0; i < NUMBER_OF_BUCKETS; ++i)
		//{
		//	for (int j = 0; j < buckets[i].GetVectorsSize(); ++j)
		//	{
		//		DEBUG_LOG_MATH("buckets[", i, "][", j, "] = ", buckets[i].GetVector(j).ToString());
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
			ERROR_LOG_MATH("Unknown sorting direction value (", sortingParameters.GetSortingDirection(), "). Only ASCENDING (",
				ASCENDING, ") and DESCENDING (", DESCENDING, ") are acceptable.");
			break;
		}
	}
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
/* ==================== Non-static member variables end ==================== */
}; /* end class BucketSort */

} /* end namespace Sorting */

} /* end namespace Math */

#endif /* __MATH_SORT_H__ */