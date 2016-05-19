#include "StdAfx.h"
#include "Utility\ILogger.h"
#include "Sort.h"

using namespace Math::Sorting;

/* ==================== BubbleSort class implementation begin ==================== */
BubbleSort::BubbleSort() : ISort(Sorting::BUBBLE_SORT)
{
}

BubbleSort::~BubbleSort(void)
{
}

void BubbleSort::Sort(int* values, size_t valuesCount, SortingDirection sortingDirection /* = ASCENDING */)
{
	Sort<int>(values, valuesCount, VALUE, sortingDirection);
}

void BubbleSort::Sort(Math::Real* values, size_t valuesCount, SortingDirection sortingDirection /* = ASCENDING */)
{
	Sort<Real>(values, valuesCount, VALUE, sortingDirection);
}

void BubbleSort::Sort(Math::Vector2D* vectors, size_t vectorSize, SortingKey sortingKey /* = COMPONENT_X */, SortingDirection sortingDirection /* = ASCENDING */)
{
	Sort<Math::Vector2D>(vectors, vectorSize, sortingKey, sortingDirection);
}

void BubbleSort::Sort(Math::Vector3D* vectors, size_t vectorSize, SortingKey sortingKey /* = COMPONENT_X */, SortingDirection sortingDirection /* = ASCENDING */)
{
	Sort<Math::Vector3D>(vectors, vectorSize, sortingKey, sortingDirection);
}

void BubbleSort::Sort(Math::Vector2D* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters)
{
	Sort<Math::Vector2D>(vectors, vectorSize, sortingParameters);
}

void BubbleSort::Sort(Math::Vector3D* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters)
{
	Sort<Math::Vector3D>(vectors, vectorSize, sortingParameters);
}
/* ==================== BubbleSort class implementation end ==================== */

/* ==================== InsertionSort class implementation begin ==================== */
InsertionSort::InsertionSort() : ISort(Sorting::INSERTION_SORT)
{
}

InsertionSort::~InsertionSort(void)
{
}

void InsertionSort::Sort(int* values, size_t valuesCount, SortingDirection sortingDirection /* = ASCENDING */)
{
	Sort<int>(values, valuesCount, VALUE, sortingDirection);
}

void InsertionSort::Sort(Math::Real* values, size_t valuesCount, SortingDirection sortingDirection /* = ASCENDING */)
{
	Sort<Real>(values, valuesCount, VALUE, sortingDirection);
}

void InsertionSort::Sort(Math::Vector2D* vectors, size_t vectorSize, SortingKey sortingKey /* = COMPONENT_X */, SortingDirection sortingDirection /* = ASCENDING */)
{
	Sort<Math::Vector2D>(vectors, vectorSize, sortingKey, sortingDirection);
}

void InsertionSort::Sort(Math::Vector3D* vectors, size_t vectorSize, SortingKey sortingKey /* = COMPONENT_X */, SortingDirection sortingDirection /* = ASCENDING */)
{
	Sort<Math::Vector3D>(vectors, vectorSize, sortingKey, sortingDirection);
}

void InsertionSort::Sort(Math::Vector2D* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters)
{
	Sort<Math::Vector2D>(vectors, vectorSize, sortingParameters);
}

void InsertionSort::Sort(Math::Vector3D* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters)
{
	Sort<Math::Vector3D>(vectors, vectorSize, sortingParameters);
}
/* ==================== InsertionSort class implementation end ==================== */

/* ==================== SelectionSort class implementation begin ==================== */
SelectionSort::SelectionSort() : ISort(Sorting::SELECTION_SORT)
{
}

SelectionSort::~SelectionSort(void)
{
}

void SelectionSort::Sort(int* values, size_t valuesCount, SortingDirection sortingDirection /* = ASCENDING */)
{
	Sort<int>(values, valuesCount, VALUE, sortingDirection);
}

void SelectionSort::Sort(Math::Real* values, size_t valuesCount, SortingDirection sortingDirection /* = ASCENDING */)
{
	Sort<Real>(values, valuesCount, VALUE, sortingDirection);
}

void SelectionSort::Sort(Math::Vector2D* vectors, size_t vectorSize, SortingKey sortingKey /* = COMPONENT_X */, SortingDirection sortingDirection /* = ASCENDING */)
{
	Sort<Math::Vector2D>(vectors, vectorSize, sortingKey, sortingDirection);
}

void SelectionSort::Sort(Math::Vector3D* vectors, size_t vectorSize, SortingKey sortingKey /* = COMPONENT_X */, SortingDirection sortingDirection /* = ASCENDING */)
{
	Sort<Math::Vector3D>(vectors, vectorSize, sortingKey, sortingDirection);
}

void SelectionSort::Sort(Math::Vector2D* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters)
{
	Sort<Math::Vector2D>(vectors, vectorSize, sortingParameters);
}

void SelectionSort::Sort(Math::Vector3D* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters)
{
	Sort<Math::Vector3D>(vectors, vectorSize, sortingParameters);
}
/* ==================== SelectionSort class implementation end ==================== */

/* ==================== MergeSort class implementation begin ==================== */
MergeSort::MergeSort() : ISort(Sorting::MERGE_SORT)
{
}

MergeSort::~MergeSort(void)
{
}

void MergeSort::Sort(int* values, size_t valuesCount, SortingDirection sortingDirection /* = ASCENDING */)
{
	Sort<int>(values, valuesCount, VALUE, sortingDirection);
}

void MergeSort::Sort(Math::Real* values, size_t valuesCount, SortingDirection sortingDirection /* = ASCENDING */)
{
	Sort<Real>(values, valuesCount, VALUE, sortingDirection);
}

void MergeSort::Sort(Math::Vector2D* vectors, size_t vectorSize, SortingKey sortingKey /* = COMPONENT_X */, SortingDirection sortingDirection /* = ASCENDING */)
{
	Sort<Math::Vector2D>(vectors, vectorSize, sortingKey, sortingDirection);
}

void MergeSort::Sort(Math::Vector3D* vectors, size_t vectorSize, SortingKey sortingKey /* = COMPONENT_X */, SortingDirection sortingDirection /* = ASCENDING */)
{
	Sort<Math::Vector3D>(vectors, vectorSize, sortingKey, sortingDirection);
}

void MergeSort::Sort(Math::Vector2D* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters)
{
	Sort<Math::Vector2D>(vectors, vectorSize, sortingParameters);
}

void MergeSort::Sort(Math::Vector3D* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters)
{
	Sort<Math::Vector3D>(vectors, vectorSize, sortingParameters);
}
/* ==================== MergeSort class implementation end ==================== */

/* ==================== HeapSort class implementation begin ==================== */
HeapSort::HeapSort() : ISort(Sorting::HEAP_SORT)
{
}

HeapSort::~HeapSort(void)
{
}

void HeapSort::Sort(int* values, size_t valuesCount, SortingDirection sortingDirection /* = ASCENDING */)
{
	Sort<int>(values, valuesCount, VALUE, sortingDirection);
}

void HeapSort::Sort(Math::Real* values, size_t valuesCount, SortingDirection sortingDirection /* = ASCENDING */)
{
	Sort<Real>(values, valuesCount, VALUE, sortingDirection);
}

void HeapSort::Sort(Math::Vector2D* vectors, size_t vectorSize, SortingKey sortingKey /* = COMPONENT_X */, SortingDirection sortingDirection /* = ASCENDING */)
{
	Sort<Math::Vector2D>(vectors, vectorSize, sortingKey, sortingDirection);
}

void HeapSort::Sort(Math::Vector3D* vectors, size_t vectorSize, SortingKey sortingKey /* = COMPONENT_X */, SortingDirection sortingDirection /* = ASCENDING */)
{
	Sort<Math::Vector3D>(vectors, vectorSize, sortingKey, sortingDirection);
}

void HeapSort::Sort(Math::Vector2D* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters)
{
	Sort<Math::Vector2D>(vectors, vectorSize, sortingParameters);
}

void HeapSort::Sort(Math::Vector3D* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters)
{
	Sort<Math::Vector3D>(vectors, vectorSize, sortingParameters);
}
/* ==================== HeapSort class implementation end ==================== */

/* ==================== QuickSort class implementation begin ==================== */
QuickSort::QuickSort() : ISort(Sorting::QUICK_SORT)
{
}

QuickSort::~QuickSort(void)
{
}

void QuickSort::Sort(int* values, size_t valuesCount, SortingDirection sortingDirection /* = ASCENDING */)
{
	Sort<int>(values, valuesCount, VALUE, sortingDirection);
}

void QuickSort::Sort(Math::Real* values, size_t valuesCount, SortingDirection sortingDirection /* = ASCENDING */)
{
	Sort<Real>(values, valuesCount, VALUE, sortingDirection);
}

void QuickSort::Sort(Math::Vector2D* vectors, size_t vectorSize, SortingKey sortingKey /* = COMPONENT_X */, SortingDirection sortingDirection /* = ASCENDING */)
{
	Sort<Math::Vector2D>(vectors, vectorSize, sortingKey, sortingDirection);
}

void QuickSort::Sort(Math::Vector3D* vectors, size_t vectorSize, SortingKey sortingKey /* = COMPONENT_X */, SortingDirection sortingDirection /* = ASCENDING */)
{
	Sort<Math::Vector3D>(vectors, vectorSize, sortingKey, sortingDirection);
}

void QuickSort::Sort(Math::Vector2D* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters)
{
	Sort<Math::Vector2D>(vectors, vectorSize, sortingParameters);
}

void QuickSort::Sort(Math::Vector3D* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters)
{
	Sort<Math::Vector3D>(vectors, vectorSize, sortingParameters);
}
/* ==================== QuickSort class implementation end ==================== */

/* ==================== ShellSort class implementation begin ==================== */
ShellSort::ShellSort() : ISort(Sorting::SHELL_SORT)
{
}

ShellSort::~ShellSort(void)
{
}

void ShellSort::Sort(int* values, size_t valuesCount, SortingDirection sortingDirection /* = ASCENDING */)
{
	Sort<int>(values, valuesCount, VALUE, sortingDirection);
}

void ShellSort::Sort(Math::Real* values, size_t valuesCount, SortingDirection sortingDirection /* = ASCENDING */)
{
	Sort<Real>(values, valuesCount, VALUE, sortingDirection);
}

void ShellSort::Sort(Math::Vector2D* vectors, size_t vectorSize, SortingKey sortingKey /* = COMPONENT_X */, SortingDirection sortingDirection /* = ASCENDING */)
{
	Sort<Math::Vector2D>(vectors, vectorSize, sortingKey, sortingDirection);
}

void ShellSort::Sort(Math::Vector3D* vectors, size_t vectorSize, SortingKey sortingKey /* = COMPONENT_X */, SortingDirection sortingDirection /* = ASCENDING */)
{
	Sort<Math::Vector3D>(vectors, vectorSize, sortingKey, sortingDirection);
}

void ShellSort::Sort(Math::Vector2D* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters)
{
	Sort<Math::Vector2D>(vectors, vectorSize, sortingParameters);
}

void ShellSort::Sort(Math::Vector3D* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters)
{
	Sort<Math::Vector3D>(vectors, vectorSize, sortingParameters);
}
/* ==================== ShellSort class implementation end ==================== */

/* ==================== CombSort class implementation begin ==================== */
CombSort::CombSort() : ISort(Sorting::COMB_SORT)
{
}

CombSort::~CombSort(void)
{
}

void CombSort::Sort(int* values, size_t valuesCount, SortingDirection sortingDirection /* = ASCENDING */)
{
	Sort<int>(values, valuesCount, VALUE, sortingDirection);
}

void CombSort::Sort(Math::Real* values, size_t valuesCount, SortingDirection sortingDirection /* = ASCENDING */)
{
	Sort<Real>(values, valuesCount, VALUE, sortingDirection);
}

void CombSort::Sort(Math::Vector2D* vectors, size_t vectorSize, SortingKey sortingKey /* = COMPONENT_X */, SortingDirection sortingDirection /* = ASCENDING */)
{
	Sort<Math::Vector2D>(vectors, vectorSize, sortingKey, sortingDirection);
}

void CombSort::Sort(Math::Vector3D* vectors, size_t vectorSize, SortingKey sortingKey /* = COMPONENT_X */, SortingDirection sortingDirection /* = ASCENDING */)
{
	Sort<Math::Vector3D>(vectors, vectorSize, sortingKey, sortingDirection);
}

void CombSort::Sort(Math::Vector2D* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters)
{
	Sort<Math::Vector2D>(vectors, vectorSize, sortingParameters);
}

void CombSort::Sort(Math::Vector3D* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters)
{
	Sort<Math::Vector3D>(vectors, vectorSize, sortingParameters);
}
/* ==================== CombSort class implementation end ==================== */

/* ==================== CountingSort class implementation begin ==================== */
CountingSort::CountingSort() : ISort(Sorting::COUNTING_SORT)
{
}

CountingSort::~CountingSort(void)
{
}

void CountingSort::Sort(int* values, size_t valuesCount, SortingDirection sortingDirection /* = ASCENDING */)
{
	/* ==================== Calculating range of data begin ==================== */
	int min = INT_MAX;
	int max = INT_MIN;
	for (int i = 0; i < valuesCount; ++i)
	{
		if (values[i] < min)
		{
			min = values[i];
		}
		else if (values[i] > max)
		{
			max = values[i];
		}
	}
	int range = max - min;
	/* ==================== Calculating range of data end ==================== */

	int* helpTab1 = new int[valuesCount];
	int* helpTab2 = new int[range + 1];
	for (int i = 0; i <= range; ++i)
		helpTab2[i] = 0;
	for (int i = 0; i < valuesCount; ++i)
		++helpTab2[values[i]];
	for (int i = 1; i <= range; ++i)
		helpTab2[i] += helpTab2[i - 1];
	for (int i = 0; i < valuesCount; ++i)
	{
		helpTab1[helpTab2[values[i]] - 1] = values[i];
		--helpTab2[values[i]];
	}
	for (int i = 0; i < valuesCount; ++i)
		values[i] = helpTab1[i];
	SAFE_DELETE_JUST_TABLE(helpTab1);
	SAFE_DELETE_JUST_TABLE(helpTab2);
}

void CountingSort::Sort(Math::Real* values, size_t valuesCount, SortingDirection sortingDirection /* = ASCENDING */)
{
	//Sort<Math::Real>(values, valuesCount, VALUE, sortingDirection);
	CRITICAL_LOG_MATH("Counting sort cannot be used to sort the floating point numbers");
	return;
}

void CountingSort::Sort(Math::Vector2D* vectors, size_t vectorSize, SortingKey sortingKey /* = COMPONENT_X */, SortingDirection sortingDirection /* = ASCENDING */)
{
	CRITICAL_LOG_MATH("Counting sort cannot be used to sort the floating point numbers");
	return;
}

void CountingSort::Sort(Math::Vector3D* vectors, size_t vectorSize, SortingKey sortingKey /* = COMPONENT_X */, SortingDirection sortingDirection /* = ASCENDING */)
{
	CRITICAL_LOG_MATH("Counting sort cannot be used to sort the floating point numbers");
	return;
}

void CountingSort::Sort(Math::Vector2D* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters)
{
	CRITICAL_LOG_MATH("Counting sort cannot be used to sort the floating point numbers");
	return;
}

void CountingSort::Sort(Math::Vector3D* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters)
{
	CRITICAL_LOG_MATH("Counting sort cannot be used to sort the floating point numbers");
	return;
}
/* ==================== CountingSort class implementation end ==================== */

/* ==================== RadixSort class implementation begin ==================== */
RadixSort::RadixSort() : ISort(Sorting::RADIX_SORT)
{
}

RadixSort::~RadixSort(void)
{
}

void RadixSort::Sort(int* values, size_t valuesCount, SortingDirection sortingDirection /* = ASCENDING */)
{
	// TODO: Create implementation of radix sort (see http://en.wikipedia.org/wiki/Radix_sort)
}

void RadixSort::Sort(Math::Real* values, size_t valuesCount, SortingDirection sortingDirection /* = ASCENDING */)
{
	EMERGENCY_LOG_MATH("Radix sort cannot be used to sort the floating point numbers, although there are some extensions that can. See this link: http://stereopsis.com/radix.html");
	return;
}

void RadixSort::Sort(Math::Vector2D* vectors, size_t vectorSize, SortingKey sortingKey /* = COMPONENT_X */, SortingDirection sortingDirection /* = ASCENDING */)
{
	EMERGENCY_LOG_MATH("Radix sort cannot be used to sort the floating point numbers, although there are some extensions that can. See this link: http://stereopsis.com/radix.html");
	return;
}

void RadixSort::Sort(Math::Vector3D* vectors, size_t vectorSize, SortingKey sortingKey /* = COMPONENT_X */, SortingDirection sortingDirection /* = ASCENDING */)
{
	EMERGENCY_LOG_MATH("Radix sort cannot be used to sort the floating point numbers, although there are some extensions that can. See this link: http://stereopsis.com/radix.html");
	return;
}

void RadixSort::Sort(Math::Vector2D* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters)
{
	EMERGENCY_LOG_MATH("Radix sort cannot be used to sort the floating point numbers, although there are some extensions that can. See this link: http://stereopsis.com/radix.html");
	return;
}

void RadixSort::Sort(Math::Vector3D* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters)
{
	EMERGENCY_LOG_MATH("Radix sort cannot be used to sort the floating point numbers, although there are some extensions that can. See this link: http://stereopsis.com/radix.html");
	return;
}
/* ==================== RadixSort class implementation end ==================== */

/* ==================== BucketSort class implementation begin ==================== */
BucketSort::BucketSort() : ISort(Sorting::BUCKET_SORT)
{
}

BucketSort::~BucketSort(void)
{
}

void BucketSort::Sort(int* values, size_t valuesCount, SortingDirection sortingDirection /* = ASCENDING */)
{
	//Sort<int>(values, valuesCount, VALUE, sortingDirection);
}

void BucketSort::Sort(Math::Real* values, size_t valuesCount, SortingDirection sortingDirection /* = ASCENDING */)
{
	//Sort<Real>(values, valuesCount, VALUE, sortingDirection);
}

void BucketSort::Sort(Math::Vector2D* vectors, size_t vectorSize, SortingKey sortingKey /* = COMPONENT_X */, SortingDirection sortingDirection /* = ASCENDING */)
{
	Sort<Math::Vector2D>(vectors, vectorSize, sortingKey, sortingDirection);
}

void BucketSort::Sort(Math::Vector3D* vectors, size_t vectorSize, SortingKey sortingKey /* = COMPONENT_X */, SortingDirection sortingDirection /* = ASCENDING */)
{
	Sort<Math::Vector3D>(vectors, vectorSize, sortingKey, sortingDirection);
}

void BucketSort::Sort(Math::Vector2D* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters)
{
	Sort<Math::Vector2D>(vectors, vectorSize, sortingParameters);
}

void BucketSort::Sort(Math::Vector3D* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters)
{
	Sort<Math::Vector3D>(vectors, vectorSize, sortingParameters);
}
/* ==================== BucketSort class implementation end ==================== */
