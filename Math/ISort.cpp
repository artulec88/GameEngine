#include "stdafx.h"
#include "ISort.h"
#include "Sort.h"

/* static */ std::unique_ptr<Math::Sorting::ISort> Math::Sorting::ISort::GetSortingObject(SortingAlgorithms::SortingAlgorithm sortingAlgorithm)
{
	switch (sortingAlgorithm)
	{
	case SortingAlgorithms::BUBBLE_SORT: DELOCUST_LOG_MATH("Bubble sort specified as the sorting algorithm"); return std::make_unique<BubbleSort>();
	case SortingAlgorithms::INSERTION_SORT: DELOCUST_LOG_MATH("Insertion sort specified as the sorting algorithm"); return std::make_unique<InsertionSort>();
	case SortingAlgorithms::SELECTION_SORT: DELOCUST_LOG_MATH("Selection sort specified as the sorting algorithm"); return std::make_unique<SelectionSort>();
	case SortingAlgorithms::MERGE_SORT: DELOCUST_LOG_MATH("Merge sort specified as the sorting algorithm"); return std::make_unique<MergeSort>();
	case SortingAlgorithms::HEAP_SORT: DELOCUST_LOG_MATH("Heap sort specified as the sorting algorithm"); return std::make_unique<HeapSort>();
	case SortingAlgorithms::QUICK_SORT: DELOCUST_LOG_MATH("Quick sort specified as the sorting algorithm"); return std::make_unique<QuickSort>();
	case SortingAlgorithms::SHELL_SORT: DELOCUST_LOG_MATH("Shell sort specified as the sorting algorihtm"); return std::make_unique<ShellSort>();
	case SortingAlgorithms::COMB_SORT: DELOCUST_LOG_MATH("Comb sort specified as the sorting algorithm."); return std::make_unique<CombSort>();
	case SortingAlgorithms::COUNTING_SORT: DELOCUST_LOG_MATH("Counting sort specified as the sorting algorithm"); return std::make_unique<CountingSort>();
	case SortingAlgorithms::RADIX_SORT: ERROR_LOG_MATH("Radix sort is not yet supported by the Game engine. Insertion sort will be used instead."); return std::make_unique<InsertionSort>();
	case SortingAlgorithms::BUCKET_SORT: DELOCUST_LOG_MATH("Bucket sort specified as the sorting algorithm."); return std::make_unique<BucketSort>();
	default: EMERGENCY_LOG_MATH("Unknown sorting algorithm specified. Insertion sort will be used as a default."); return std::make_unique<InsertionSort>();
	}
}

Math::Sorting::ISort::ISort(SortingAlgorithms::SortingAlgorithm sortingAlgorithm) :
	m_sortingAlgorithm(sortingAlgorithm)
{
}

Math::Sorting::ISort::~ISort()
{
}