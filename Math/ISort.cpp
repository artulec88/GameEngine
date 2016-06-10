#include "stdafx.h"
#include "ISort.h"
#include "Sort.h"

using namespace Math::Sorting;

/* static */ std::unique_ptr<ISort> ISort::GetSortingObject(SortingAlgorithm sortingAlgorithm)
{
	switch (sortingAlgorithm)
	{
	case Sorting::BUBBLE_SORT: DELOCUST_LOG_MATH("Bubble sort specified as the sorting algorithm"); return std::make_unique<BubbleSort>();
	case Sorting::INSERTION_SORT: DELOCUST_LOG_MATH("Insertion sort specified as the sorting algorithm"); return std::make_unique<InsertionSort>();
	case Sorting::SELECTION_SORT: DELOCUST_LOG_MATH("Selection sort specified as the sorting algorithm"); return std::make_unique<SelectionSort>();
	case Sorting::MERGE_SORT: DELOCUST_LOG_MATH("Merge sort specified as the sorting algorithm"); return std::make_unique<MergeSort>();
	case Sorting::HEAP_SORT: DELOCUST_LOG_MATH("Heap sort specified as the sorting algorithm"); return std::make_unique<HeapSort>();
	case Sorting::QUICK_SORT: DELOCUST_LOG_MATH("Quick sort specified as the sorting algorithm"); return std::make_unique<QuickSort>();
	case Sorting::SHELL_SORT: DELOCUST_LOG_MATH("Shell sort specified as the sorting algorihtm"); return std::make_unique<ShellSort>();
	case Sorting::COMB_SORT: DELOCUST_LOG_MATH("Comb sort specified as the sorting algorithm."); return std::make_unique<CombSort>();
	case Sorting::COUNTING_SORT: DELOCUST_LOG_MATH("Counting sort specified as the sorting algorithm"); return std::make_unique<CountingSort>();
	case Sorting::RADIX_SORT: ERROR_LOG_MATH("Radix sort is not yet supported by the Game engine. Insertion sort will be used instead."); return std::make_unique<InsertionSort>();
	case Sorting::BUCKET_SORT: DELOCUST_LOG_MATH("Bucket sort specified as the sorting algorithm."); return std::make_unique<BucketSort>();
	default: EMERGENCY_LOG_MATH("Unknown sorting algorithm specified. Insertion sort will be used as a default."); return std::make_unique<InsertionSort>();
	}
}

ISort::ISort(SortingAlgorithm sortingAlgorithm) :
	m_sortingAlgorithm(sortingAlgorithm)
{
}

ISort::~ISort()
{
}