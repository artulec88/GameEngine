#include "stdafx.h"
#include "ISort.h"
#include "Sort.h"

/* static */ std::unique_ptr<math::sorting::ISort> math::sorting::ISort::GetSortingObject(sorting_algorithms::SortingAlgorithm sortingAlgorithm)
{
	switch (sortingAlgorithm)
	{
	case sorting_algorithms::BUBBLE_SORT:
		DELOCUST_LOG_MATH("Bubble sort specified as the sorting algorithm");
		return std::make_unique<BubbleSort>();
	case sorting_algorithms::INSERTION_SORT:
		DELOCUST_LOG_MATH("Insertion sort specified as the sorting algorithm");
		return std::make_unique<InsertionSort>();
	case sorting_algorithms::SELECTION_SORT:
		DELOCUST_LOG_MATH("Selection sort specified as the sorting algorithm");
		return std::make_unique<SelectionSort>();
	case sorting_algorithms::MERGE_SORT:
		DELOCUST_LOG_MATH("Merge sort specified as the sorting algorithm");
		return std::make_unique<MergeSort>();
	case sorting_algorithms::HEAP_SORT:
		DELOCUST_LOG_MATH("Heap sort specified as the sorting algorithm");
		return std::make_unique<HeapSort>();
	case sorting_algorithms::QUICK_SORT:
		DELOCUST_LOG_MATH("Quick sort specified as the sorting algorithm");
		return std::make_unique<QuickSort>();
	case sorting_algorithms::SHELL_SORT:
		DELOCUST_LOG_MATH("Shell sort specified as the sorting algorihtm");
		return std::make_unique<ShellSort>();
	case sorting_algorithms::COMB_SORT:
		DELOCUST_LOG_MATH("Comb sort specified as the sorting algorithm.");
		return std::make_unique<CombSort>();
	case sorting_algorithms::COUNTING_SORT:
		DELOCUST_LOG_MATH("Counting sort specified as the sorting algorithm");
		return std::make_unique<CountingSort>();
	case sorting_algorithms::RADIX_SORT:
		ERROR_LOG_MATH("Radix sort is not yet supported by the Game engine. Insertion sort will be used instead.");
		return std::make_unique<InsertionSort>();
	case sorting_algorithms::BUCKET_SORT:
		DELOCUST_LOG_MATH("Bucket sort specified as the sorting algorithm.");
		return std::make_unique<BucketSort>();
	default:
		EMERGENCY_LOG_MATH("Unknown sorting algorithm specified. Insertion sort will be used as a default.");
		return std::make_unique<InsertionSort>();
	}
}