#include "stdafx.h"
#include "ISort.h"
#include "Sort.h"

using namespace Math::Sorting;

/* static */ ISort* ISort::GetSortingObject(SortingAlgorithm sortingAlgorithm)
{
	switch (sortingAlgorithm)
	{
	case Sorting::BUBBLE_SORT: LOG(Utility::Debug, LOGPLACE, "Bubble sort specified as the sorting algorithm"); return new BubbleSort();
	case Sorting::INSERTION_SORT: LOG(Utility::Debug, LOGPLACE, "Insertion sort specified as the sorting algorithm"); return new InsertionSort();
	case Sorting::SELECTION_SORT: LOG(Utility::Debug, LOGPLACE, "Selection sort specified as the sorting algorithm"); return new SelectionSort();
	case Sorting::MERGE_SORT: LOG(Utility::Debug, LOGPLACE, "Merge sort specified as the sorting algorithm"); return new MergeSort();
	case Sorting::HEAP_SORT: LOG(Utility::Debug, LOGPLACE, "Heap sort specified as the sorting algorithm"); return new HeapSort();
	case Sorting::QUICK_SORT: LOG(Utility::Debug, LOGPLACE, "Quick sort specified as the sorting algorithm"); return new QuickSort();
	case Sorting::SHELL_SORT: LOG(Utility::Debug, LOGPLACE, "Shell sort specified as the sorting algorihtm"); return new ShellSort();
	case Sorting::COMB_SORT: LOG(Utility::Debug, LOGPLACE, "Comb sort specified as the sorting algorithm."); return new CombSort();
	case Sorting::COUNTING_SORT: LOG(Utility::Debug, LOGPLACE, "Counting sort specified as the sorting algorithm"); return new CountingSort();
	case Sorting::RADIX_SORT: LOG(Utility::Error, LOGPLACE, "Radix sort is not yet supported by the Game engine. Insertion sort will be used instead."); return new InsertionSort();
	case Sorting::BUCKET_SORT: LOG(Utility::Debug, LOGPLACE, "Bucket sort specified as the sorting algorithm."); return new BucketSort();
	default: LOG(Utility::Emergency, LOGPLACE, "Unknown sorting algorithm specified. Insertion sort will be used as a default."); return new InsertionSort();
	}
}

ISort::ISort(SortingAlgorithm sortingAlgorithm) :
	m_sortingAlgorithm(sortingAlgorithm)
{
}

ISort::~ISort()
{
}