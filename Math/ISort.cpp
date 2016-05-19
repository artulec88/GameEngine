#include "stdafx.h"
#include "ISort.h"
#include "Sort.h"

using namespace Math::Sorting;

/* static */ ISort* ISort::GetSortingObject(SortingAlgorithm sortingAlgorithm)
{
	switch (sortingAlgorithm)
	{
	case Sorting::BUBBLE_SORT: DELOCUST_LOG_MATH("Bubble sort specified as the sorting algorithm"); return new BubbleSort();
	case Sorting::INSERTION_SORT: DELOCUST_LOG_MATH("Insertion sort specified as the sorting algorithm"); return new InsertionSort();
	case Sorting::SELECTION_SORT: DELOCUST_LOG_MATH("Selection sort specified as the sorting algorithm"); return new SelectionSort();
	case Sorting::MERGE_SORT: DELOCUST_LOG_MATH("Merge sort specified as the sorting algorithm"); return new MergeSort();
	case Sorting::HEAP_SORT: DELOCUST_LOG_MATH("Heap sort specified as the sorting algorithm"); return new HeapSort();
	case Sorting::QUICK_SORT: DELOCUST_LOG_MATH("Quick sort specified as the sorting algorithm"); return new QuickSort();
	case Sorting::SHELL_SORT: DELOCUST_LOG_MATH("Shell sort specified as the sorting algorihtm"); return new ShellSort();
	case Sorting::COMB_SORT: DELOCUST_LOG_MATH("Comb sort specified as the sorting algorithm."); return new CombSort();
	case Sorting::COUNTING_SORT: DELOCUST_LOG_MATH("Counting sort specified as the sorting algorithm"); return new CountingSort();
	case Sorting::RADIX_SORT: ERROR_LOG_MATH("Radix sort is not yet supported by the Game engine. Insertion sort will be used instead."); return new InsertionSort();
	case Sorting::BUCKET_SORT: DELOCUST_LOG_MATH("Bucket sort specified as the sorting algorithm."); return new BucketSort();
	default: EMERGENCY_LOG_MATH("Unknown sorting algorithm specified. Insertion sort will be used as a default."); return new InsertionSort();
	}
}

ISort::ISort(SortingAlgorithm sortingAlgorithm) :
	m_sortingAlgorithm(sortingAlgorithm)
{
}

ISort::~ISort()
{
}