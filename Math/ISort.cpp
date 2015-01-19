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
	case ISort::HEAP_SORT: LOG(Utility::Error, LOGPLACE, "Heap sort is not yet supported by the GameEngine. Bubble sort will be used instead."); sortObject = new BubbleSort(); break;
	case ISort::QUICK_SORT: LOG(Utility::Error, LOGPLACE, "Quick sort is not yet supported by the GameEngine. Bubble sort will be used instead."); sortObject = new BubbleSort(); break;
	case ISort::SHELL_SORT: LOG(Utility::Error, LOGPLACE, "Shell sort is not yet supported by the GameEngine. Bubble sort will be used instead."); sortObject = new BubbleSort(); break;
	case ISort::COMB_SORT: LOG(Utility::Error, LOGPLACE, "Comb sort is not yet supported by the GameEngine. Bubble sort will be used instead."); sortObject = new BubbleSort(); break;
	case ISort::COUNTING_SORT: LOG(Utility::Error, LOGPLACE, "Counting sort is not yet supported by the GameEngine. Bubble sort will be used instead."); sortObject = new BubbleSort(); break;
	case ISort::RADIX_SORT: LOG(Utility::Error, LOGPLACE, "Radix sort is not yet supported by the GameEngine. Bubble sort will be used instead."); sortObject = new BubbleSort(); break;
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