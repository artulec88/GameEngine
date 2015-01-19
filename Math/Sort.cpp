#include "StdAfx.h"
#include "Utility\ILogger.h"
#include "Sort.h"

using namespace Math;

/* ==================== BubbleSort class implementation begin ==================== */
BubbleSort::BubbleSort() : ISort(ISort::BUBBLE_SORT)
{
}

BubbleSort::~BubbleSort(void)
{
}

void BubbleSort::Sort(Math::Vector2D* vectors, int vectorSize, SortingKey sortingKey /* = COMPONENT_X */, SortingDirection sortingDirection /* = ASCENDING */)
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

void BubbleSort::Sort(Math::Vector3D* vectors, int vectorSize, SortingKey sortingKey /* = COMPONENT_X */, SortingDirection sortingDirection /* = ASCENDING */)
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
/* ==================== BubbleSort class implementation end ==================== */

/* ==================== InsertionSort class implementation begin ==================== */
InsertionSort::InsertionSort() : ISort(ISort::INSERTION_SORT)
{
}

InsertionSort::~InsertionSort(void)
{
}

void InsertionSort::Sort(Math::Vector2D* vectors, int vectorSize, SortingKey sortingKey /* = COMPONENT_X */, SortingDirection sortingDirection /* = ASCENDING */)
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

void InsertionSort::Sort(Math::Vector3D* vectors, int vectorSize, SortingKey sortingKey /* = COMPONENT_X */, SortingDirection sortingDirection /* = ASCENDING */)
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
/* ==================== InsertionSort class implementation end ==================== */

/* ==================== SelectionSort class implementation begin ==================== */
SelectionSort::SelectionSort() : ISort(ISort::SELECTION_SORT)
{
}

SelectionSort::~SelectionSort(void)
{
}

void SelectionSort::Sort(Math::Vector2D* vectors, int vectorSize, SortingKey sortingKey /* = COMPONENT_X */, SortingDirection sortingDirection /* = ASCENDING */)
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
		Math::Vector2D vecToSwap = vectors[indexToSwap];
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

void SelectionSort::Sort(Math::Vector3D* vectors, int vectorSize, SortingKey sortingKey /* = COMPONENT_X */, SortingDirection sortingDirection /* = ASCENDING */)
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
		Math::Vector3D vecToSwap = vectors[indexToSwap];
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
/* ==================== SelectionSort class implementation end ==================== */

/* ==================== MergeSort class implementation begin ==================== */
MergeSort::MergeSort() : ISort(ISort::MERGE_SORT)
{
}

MergeSort::~MergeSort(void)
{
}

void MergeSort::Sort(Math::Vector2D* vectors, int vectorSize, SortingKey sortingKey /* = COMPONENT_X */, SortingDirection sortingDirection /* = ASCENDING */)
{
	if (vectors == NULL)
	{
		LOG(Utility::Emergency, LOGPLACE, "Cannot sort the table using the merge sort algorithm. The specified table is NULL");
		return;
	}
	Math::Vector2D* auxiliaryTable = new Math::Vector2D [vectorSize];
	Sort(vectors, 0, vectorSize - 1, sortingKey, sortingDirection, auxiliaryTable);
	delete [] auxiliaryTable;
}

void MergeSort::Sort(Math::Vector3D* vectors, int vectorSize, SortingKey sortingKey /* = COMPONENT_X */, SortingDirection sortingDirection /* = ASCENDING */)
{
	if (vectors == NULL)
	{
		LOG(Utility::Emergency, LOGPLACE, "Cannot sort the table using the merge sort algorithm. The specified table is NULL");
		return;
	}
	Math::Vector3D* auxiliaryTable = new Math::Vector3D [vectorSize];
	Sort(vectors, 0, vectorSize - 1, sortingKey, sortingDirection, auxiliaryTable);
	delete [] auxiliaryTable;
}
/* ==================== MergeSort class implementation end ==================== */

/* ==================== HeapSort class implementation begin ==================== */
HeapSort::HeapSort() : ISort(ISort::HEAP_SORT)
{
}

HeapSort::~HeapSort(void)
{
}

void HeapSort::Sort(Math::Vector2D* vectors, int vectorSize, SortingKey sortingKey /* = COMPONENT_X */, SortingDirection sortingDirection /* = ASCENDING */)
{
	if (vectors == NULL)
	{
		LOG(Utility::Emergency, LOGPLACE, "Cannot sort the table using the heap sort algorithm. The specified table is NULL");
		return;
	}
}

void HeapSort::Sort(Math::Vector3D* vectors, int vectorSize, SortingKey sortingKey /* = COMPONENT_X */, SortingDirection sortingDirection /* = ASCENDING */)
{
	if (vectors == NULL)
	{
		LOG(Utility::Emergency, LOGPLACE, "Cannot sort the table using the heap sort algorithm. The specified table is NULL");
		return;
	}
}
/* ==================== HeapSort class implementation end ==================== */

/* ==================== QuickSort class implementation begin ==================== */
QuickSort::QuickSort() : ISort(ISort::QUICK_SORT)
{
}

QuickSort::~QuickSort(void)
{
}

void QuickSort::Sort(Math::Vector2D* vectors, int vectorSize, SortingKey sortingKey /* = COMPONENT_X */, SortingDirection sortingDirection /* = ASCENDING */)
{
	if (vectors == NULL)
	{
		LOG(Utility::Emergency, LOGPLACE, "Cannot sort the table using the quick sort algorithm. The specified table is NULL");
		return;
	}
}

void QuickSort::Sort(Math::Vector3D* vectors, int vectorSize, SortingKey sortingKey /* = COMPONENT_X */, SortingDirection sortingDirection /* = ASCENDING */)
{
	if (vectors == NULL)
	{
		LOG(Utility::Emergency, LOGPLACE, "Cannot sort the table using the quick sort algorithm. The specified table is NULL");
		return;
	}
}
/* ==================== QuickSort class implementation end ==================== */

/* ==================== ShellSort class implementation begin ==================== */
ShellSort::ShellSort() : ISort(ISort::SHELL_SORT)
{
}

ShellSort::~ShellSort(void)
{
}

void ShellSort::Sort(Math::Vector2D* vectors, int vectorSize, SortingKey sortingKey /* = COMPONENT_X */, SortingDirection sortingDirection /* = ASCENDING */)
{
	if (vectors == NULL)
	{
		LOG(Utility::Emergency, LOGPLACE, "Cannot sort the table using the shell sort algorithm. The specified table is NULL");
		return;
	}
}

void ShellSort::Sort(Math::Vector3D* vectors, int vectorSize, SortingKey sortingKey /* = COMPONENT_X */, SortingDirection sortingDirection /* = ASCENDING */)
{
	if (vectors == NULL)
	{
		LOG(Utility::Emergency, LOGPLACE, "Cannot sort the table using the shell sort algorithm. The specified table is NULL");
		return;
	}
}
/* ==================== ShellSort class implementation end ==================== */

/* ==================== CombSort class implementation begin ==================== */
CombSort::CombSort() : ISort(ISort::COMB_SORT)
{
}

CombSort::~CombSort(void)
{
}

void CombSort::Sort(Math::Vector2D* vectors, int vectorSize, SortingKey sortingKey /* = COMPONENT_X */, SortingDirection sortingDirection /* = ASCENDING */)
{
	if (vectors == NULL)
	{
		LOG(Utility::Emergency, LOGPLACE, "Cannot sort the table using the comb sort algorithm. The specified table is NULL");
		return;
	}
}

void CombSort::Sort(Math::Vector3D* vectors, int vectorSize, SortingKey sortingKey /* = COMPONENT_X */, SortingDirection sortingDirection /* = ASCENDING */)
{
	if (vectors == NULL)
	{
		LOG(Utility::Emergency, LOGPLACE, "Cannot sort the table using the comb sort algorithm. The specified table is NULL");
		return;
	}
}
/* ==================== CombSort class implementation end ==================== */

/* ==================== CountingSort class implementation begin ==================== */
CountingSort::CountingSort() : ISort(ISort::COUNTING_SORT)
{
}

CountingSort::~CountingSort(void)
{
}

void CountingSort::Sort(Math::Vector2D* vectors, int vectorSize, SortingKey sortingKey /* = COMPONENT_X */, SortingDirection sortingDirection /* = ASCENDING */)
{
	if (vectors == NULL)
	{
		LOG(Utility::Emergency, LOGPLACE, "Cannot sort the table using the counting sort algorithm. The specified table is NULL");
		return;
	}
}

void CountingSort::Sort(Math::Vector3D* vectors, int vectorSize, SortingKey sortingKey /* = COMPONENT_X */, SortingDirection sortingDirection /* = ASCENDING */)
{
	if (vectors == NULL)
	{
		LOG(Utility::Emergency, LOGPLACE, "Cannot sort the table using the counting sort algorithm. The specified table is NULL");
		return;
	}
}
/* ==================== CountingSort class implementation end ==================== */

/* ==================== RadixSort class implementation begin ==================== */
RadixSort::RadixSort() : ISort(ISort::RADIX_SORT)
{
}

RadixSort::~RadixSort(void)
{
}

void RadixSort::Sort(Math::Vector2D* vectors, int vectorSize, SortingKey sortingKey /* = COMPONENT_X */, SortingDirection sortingDirection /* = ASCENDING */)
{
	if (vectors == NULL)
	{
		LOG(Utility::Emergency, LOGPLACE, "Cannot sort the table using the radix sort algorithm. The specified table is NULL");
		return;
	}
}

void RadixSort::Sort(Math::Vector3D* vectors, int vectorSize, SortingKey sortingKey /* = COMPONENT_X */, SortingDirection sortingDirection /* = ASCENDING */)
{
	if (vectors == NULL)
	{
		LOG(Utility::Emergency, LOGPLACE, "Cannot sort the table using the radix sort algorithm. The specified table is NULL");
		return;
	}
}
/* ==================== RadixSort class implementation end ==================== */