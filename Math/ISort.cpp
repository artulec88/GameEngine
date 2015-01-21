#include "stdafx.h"
#include "ISort.h"
#include "Sort.h"

using namespace Math;

/* static */ ISort* ISort::sortObject = NULL;
/* static */ ISort& ISort::GetSortingObject(SortingMethod sortingMethod)
{
	if ((ISort::sortObject != NULL) && (ISort::sortObject->GetSortingMethod() == sortingMethod))
	{
		return *ISort::sortObject;
	}

	SAFE_DELETE(ISort::sortObject);
	switch (sortingMethod)
	{
	case ISort::BUBBLE_SORT: LOG(Utility::Notice, LOGPLACE, "Bubble sort specified as the sorting algorithm."); sortObject = new BubbleSort(); break;
	case ISort::INSERTION_SORT: LOG(Utility::Notice, LOGPLACE, "Insertion sort specified as the sorting algorithm"); sortObject = new InsertionSort(); break;
	case ISort::SELECTION_SORT: LOG(Utility::Notice, LOGPLACE, "Selection sort specified as the sorting algorithm."); sortObject = new SelectionSort(); break;
	case ISort::MERGE_SORT: LOG(Utility::Notice, LOGPLACE, "Merge sort specified as the sorting algorithm."); sortObject = new MergeSort(); break;
	case ISort::HEAP_SORT: LOG(Utility::Notice, LOGPLACE, "Heap sort specified as the sorting algorithm."); sortObject = new HeapSort(); break;
	case ISort::QUICK_SORT: LOG(Utility::Notice, LOGPLACE, "Quick sort specified as the sorting algorithm"); sortObject = new QuickSort(); break;
	case ISort::SHELL_SORT: LOG(Utility::Error, LOGPLACE, "Shell sort is not yet supported by the Game engine. Insertion sort will be used instead."); sortObject = new InsertionSort(); break;
	case ISort::COMB_SORT: LOG(Utility::Error, LOGPLACE, "Comb sort is not yet supported by the Game engine. Insertion sort will be used instead."); sortObject = new InsertionSort(); break;
	case ISort::COUNTING_SORT: LOG(Utility::Error, LOGPLACE, "Counting sort is not yet supported by the Game engine. Insertion sort will be used instead."); sortObject = new InsertionSort(); break;
	case ISort::RADIX_SORT: LOG(Utility::Error, LOGPLACE, "Radix sort is not yet supported by the Game engine. Insertion sort will be used instead."); sortObject = new InsertionSort(); break;
	case ISort::BUCKET_SORT: LOG(Utility::Error, LOGPLACE, "Bucket sort is not yet supported by the Game engine. Insertion sort will be used instead."); sortObject = new InsertionSort(); break;
	default: LOG(Utility::Emergency, LOGPLACE, "Unknown sorting algorithm specified. Insertion sort will be used as a default."); sortObject = new InsertionSort(); break;
	}

	return *sortObject;
}

ISort::ISort(SortingMethod sortingMethod) :
	sortingMethod(sortingMethod)
{
}

ISort::~ISort()
{
}