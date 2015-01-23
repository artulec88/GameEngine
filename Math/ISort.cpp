#include "stdafx.h"
#include "ISort.h"
#include "Sort.h"

using namespace Math;

/* static */ ISort* ISort::GetSortingObject(SortingMethod sortingMethod)
{
	switch (sortingMethod)
	{
	case ISort::BUBBLE_SORT: LOG(Utility::Notice, LOGPLACE, "Bubble sort specified as the sorting algorithm"); return new BubbleSort();
	case ISort::INSERTION_SORT: LOG(Utility::Notice, LOGPLACE, "Insertion sort specified as the sorting algorithm"); return new InsertionSort();
	case ISort::SELECTION_SORT: LOG(Utility::Notice, LOGPLACE, "Selection sort specified as the sorting algorithm"); return new SelectionSort();
	case ISort::MERGE_SORT: LOG(Utility::Notice, LOGPLACE, "Merge sort specified as the sorting algorithm"); return new MergeSort();
	case ISort::HEAP_SORT: LOG(Utility::Notice, LOGPLACE, "Heap sort specified as the sorting algorithm"); return new HeapSort();
	case ISort::QUICK_SORT: LOG(Utility::Notice, LOGPLACE, "Quick sort specified as the sorting algorithm"); return new QuickSort();
	case ISort::SHELL_SORT: LOG(Utility::Notice, LOGPLACE, "Shell sort specified as the sorting algorihtm"); return new ShellSort();
	case ISort::COMB_SORT: LOG(Utility::Notice, LOGPLACE, "Comb sort specified as the sorting algorithm."); return new CombSort();
	case ISort::COUNTING_SORT: LOG(Utility::Error, LOGPLACE, "Counting sort is not yet supported by the Game engine. Insertion sort will be used instead."); return new InsertionSort();
	case ISort::RADIX_SORT: LOG(Utility::Error, LOGPLACE, "Radix sort is not yet supported by the Game engine. Insertion sort will be used instead."); return new InsertionSort();
	case ISort::BUCKET_SORT: LOG(Utility::Notice, LOGPLACE, "Bucket sort specified as the sorting algorithm."); return new BucketSort();
	default: LOG(Utility::Emergency, LOGPLACE, "Unknown sorting algorithm specified. Insertion sort will be used as a default."); return new InsertionSort();
	}
}

ISort::ISort(SortingMethod sortingMethod) :
	sortingMethod(sortingMethod)
{
}

ISort::~ISort()
{
}