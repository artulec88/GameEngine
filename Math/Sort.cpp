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
	Sort<Math::Vector2D>(vectors, vectorSize, sortingKey, sortingDirection);
}

void BubbleSort::Sort(Math::Vector3D* vectors, int vectorSize, SortingKey sortingKey /* = COMPONENT_X */, SortingDirection sortingDirection /* = ASCENDING */)
{
	Sort<Math::Vector3D>(vectors, vectorSize, sortingKey, sortingDirection);
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
	Sort<Math::Vector2D>(vectors, vectorSize, sortingKey, sortingDirection);
}

void InsertionSort::Sort(Math::Vector3D* vectors, int vectorSize, SortingKey sortingKey /* = COMPONENT_X */, SortingDirection sortingDirection /* = ASCENDING */)
{
	Sort<Math::Vector3D>(vectors, vectorSize, sortingKey, sortingDirection);
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
	Sort<Math::Vector2D>(vectors, vectorSize, sortingKey, sortingDirection);
}

void SelectionSort::Sort(Math::Vector3D* vectors, int vectorSize, SortingKey sortingKey /* = COMPONENT_X */, SortingDirection sortingDirection /* = ASCENDING */)
{
	Sort<Math::Vector3D>(vectors, vectorSize, sortingKey, sortingDirection);
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
	Sort<Math::Vector2D>(vectors, vectorSize, sortingKey, sortingDirection);
}

void MergeSort::Sort(Math::Vector3D* vectors, int vectorSize, SortingKey sortingKey /* = COMPONENT_X */, SortingDirection sortingDirection /* = ASCENDING */)
{
	Sort<Math::Vector3D>(vectors, vectorSize, sortingKey, sortingDirection);
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
	Sort<Math::Vector2D>(vectors, vectorSize, sortingKey, sortingDirection);
}

void HeapSort::Sort(Math::Vector3D* vectors, int vectorSize, SortingKey sortingKey /* = COMPONENT_X */, SortingDirection sortingDirection /* = ASCENDING */)
{
	Sort<Math::Vector3D>(vectors, vectorSize, sortingKey, sortingDirection);
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
	Sort<Math::Vector2D>(vectors, vectorSize, sortingKey, sortingDirection);
}

void QuickSort::Sort(Math::Vector3D* vectors, int vectorSize, SortingKey sortingKey /* = COMPONENT_X */, SortingDirection sortingDirection /* = ASCENDING */)
{
	Sort<Math::Vector3D>(vectors, vectorSize, sortingKey, sortingDirection);
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
	Sort<Math::Vector2D>(vectors, vectorSize, sortingKey, sortingDirection);
}

void ShellSort::Sort(Math::Vector3D* vectors, int vectorSize, SortingKey sortingKey /* = COMPONENT_X */, SortingDirection sortingDirection /* = ASCENDING */)
{
	Sort<Math::Vector3D>(vectors, vectorSize, sortingKey, sortingDirection);
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
	Sort<Math::Vector2D>(vectors, vectorSize, sortingKey, sortingDirection);
}

void CombSort::Sort(Math::Vector3D* vectors, int vectorSize, SortingKey sortingKey /* = COMPONENT_X */, SortingDirection sortingDirection /* = ASCENDING */)
{
	Sort<Math::Vector3D>(vectors, vectorSize, sortingKey, sortingDirection);
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
	LOG(Utility::Emergency, LOGPLACE, "Counting sort cannot be used to sort the floating point numbers");
	return;
}

void CountingSort::Sort(Math::Vector3D* vectors, int vectorSize, SortingKey sortingKey /* = COMPONENT_X */, SortingDirection sortingDirection /* = ASCENDING */)
{
	LOG(Utility::Emergency, LOGPLACE, "Counting sort cannot be used to sort the floating point numbers");
	return;
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
	LOG(Utility::Emergency, LOGPLACE, "Radix sort cannot be used to sort the floating point numbers, although there are some extensions that can. See this link: http://stereopsis.com/radix.html");
	return;
}

void RadixSort::Sort(Math::Vector3D* vectors, int vectorSize, SortingKey sortingKey /* = COMPONENT_X */, SortingDirection sortingDirection /* = ASCENDING */)
{
	LOG(Utility::Emergency, LOGPLACE, "Radix sort cannot be used to sort the floating point numbers, although there are some extensions that can. See this link: http://stereopsis.com/radix.html");
	return;
}
/* ==================== RadixSort class implementation end ==================== */

/* ==================== BucketSort class implementation begin ==================== */
BucketSort::BucketSort() : ISort(ISort::BUCKET_SORT)
{
}

BucketSort::~BucketSort(void)
{
}

void BucketSort::Sort(Math::Vector2D* vectors, int vectorSize, SortingKey sortingKey /* = COMPONENT_X */, SortingDirection sortingDirection /* = ASCENDING */)
{
	Sort<Math::Vector2D>(vectors, vectorSize, sortingKey, sortingDirection);
}

void BucketSort::Sort(Math::Vector3D* vectors, int vectorSize, SortingKey sortingKey /* = COMPONENT_X */, SortingDirection sortingDirection /* = ASCENDING */)
{
	Sort<Math::Vector3D>(vectors, vectorSize, sortingKey, sortingDirection);
}
/* ==================== BucketSort class implementation end ==================== */
