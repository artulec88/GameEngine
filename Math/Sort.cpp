#include "StdAfx.h"
#include "Utility\ILogger.h"
#include "Sort.h"

/* ==================== BubbleSort class implementation begin ==================== */
math::sorting::BubbleSort::BubbleSort() : ISort()
{
}

math::sorting::BubbleSort::~BubbleSort(void)
{
}

void math::sorting::BubbleSort::Sort(int* values, size_t valuesCount, orders::Order sortingOrder /* = orders::ASCENDING */)
{
	Sort<int>(values, valuesCount, keys::VALUE, sortingOrder);
}

void math::sorting::BubbleSort::Sort(Real* values, size_t valuesCount, orders::Order sortingOrder /* = orders::ASCENDING */)
{
	Sort<Real>(values, valuesCount, keys::VALUE, sortingOrder);
}

void math::sorting::BubbleSort::Sort(Vector2D* vectors, size_t vectorSize, keys::Key sortingKey /* = keys::COMPONENT_X */, orders::Order sortingOrder /* = orders::ASCENDING */)
{
	Sort<Vector2D>(vectors, vectorSize, sortingKey, sortingOrder);
}

void math::sorting::BubbleSort::Sort(Vector3D* vectors, size_t vectorSize, keys::Key sortingKey /* = keys::COMPONENT_X */, orders::Order sortingOrder /* = orders::ASCENDING */)
{
	Sort<Vector3D>(vectors, vectorSize, sortingKey, sortingOrder);
}

void math::sorting::BubbleSort::Sort(Vector2D* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters)
{
	Sort<Vector2D>(vectors, vectorSize, sortingParameters);
}

void math::sorting::BubbleSort::Sort(Vector3D* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters)
{
	Sort<Vector3D>(vectors, vectorSize, sortingParameters);
}
/* ==================== BubbleSort class implementation end ==================== */

/* ==================== InsertionSort class implementation begin ==================== */
math::sorting::InsertionSort::InsertionSort() : ISort()
{
}

math::sorting::InsertionSort::~InsertionSort(void)
{
}

void math::sorting::InsertionSort::Sort(int* values, size_t valuesCount, orders::Order sortingOrder /* = orders::ASCENDING */)
{
	Sort<int>(values, valuesCount, keys::VALUE, sortingOrder);
}

void math::sorting::InsertionSort::Sort(Real* values, size_t valuesCount, orders::Order sortingOrder /* = orders::ASCENDING */)
{
	Sort<Real>(values, valuesCount, keys::VALUE, sortingOrder);
}

void math::sorting::InsertionSort::Sort(Vector2D* vectors, size_t vectorSize, keys::Key sortingKey /* = keys::COMPONENT_X */, orders::Order sortingOrder /* = orders::ASCENDING */)
{
	Sort<Vector2D>(vectors, vectorSize, sortingKey, sortingOrder);
}

void math::sorting::InsertionSort::Sort(Vector3D* vectors, size_t vectorSize, keys::Key sortingKey /* = keys::COMPONENT_X */, orders::Order sortingOrder /* = orders::ASCENDING */)
{
	Sort<Vector3D>(vectors, vectorSize, sortingKey, sortingOrder);
}

void math::sorting::InsertionSort::Sort(Vector2D* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters)
{
	Sort<Vector2D>(vectors, vectorSize, sortingParameters);
}

void math::sorting::InsertionSort::Sort(Vector3D* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters)
{
	Sort<Vector3D>(vectors, vectorSize, sortingParameters);
}
/* ==================== InsertionSort class implementation end ==================== */

/* ==================== SelectionSort class implementation begin ==================== */
math::sorting::SelectionSort::SelectionSort() : ISort()
{
}

math::sorting::SelectionSort::~SelectionSort(void)
{
}

void math::sorting::SelectionSort::Sort(int* values, size_t valuesCount, orders::Order sortingOrder /* = orders::ASCENDING */)
{
	Sort<int>(values, valuesCount, keys::VALUE, sortingOrder);
}

void math::sorting::SelectionSort::Sort(Real* values, size_t valuesCount, orders::Order sortingOrder /* = orders::ASCENDING */)
{
	Sort<Real>(values, valuesCount, keys::VALUE, sortingOrder);
}

void math::sorting::SelectionSort::Sort(Vector2D* vectors, size_t vectorSize, keys::Key sortingKey /* = keys::COMPONENT_X */, orders::Order sortingOrder /* = orders::ASCENDING */)
{
	Sort<Vector2D>(vectors, vectorSize, sortingKey, sortingOrder);
}

void math::sorting::SelectionSort::Sort(Vector3D* vectors, size_t vectorSize, keys::Key sortingKey /* = keys::COMPONENT_X */, orders::Order sortingOrder /* = orders::ASCENDING */)
{
	Sort<Vector3D>(vectors, vectorSize, sortingKey, sortingOrder);
}

void math::sorting::SelectionSort::Sort(Vector2D* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters)
{
	Sort<Vector2D>(vectors, vectorSize, sortingParameters);
}

void math::sorting::SelectionSort::Sort(Vector3D* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters)
{
	Sort<Vector3D>(vectors, vectorSize, sortingParameters);
}
/* ==================== SelectionSort class implementation end ==================== */

/* ==================== MergeSort class implementation begin ==================== */
math::sorting::MergeSort::MergeSort() : ISort()
{
}

math::sorting::MergeSort::~MergeSort(void)
{
}

void math::sorting::MergeSort::Sort(int* values, size_t valuesCount, orders::Order sortingOrder /* = orders::ASCENDING */)
{
	Sort<int>(values, valuesCount, keys::VALUE, sortingOrder);
}

void math::sorting::MergeSort::Sort(Real* values, size_t valuesCount, orders::Order sortingOrder /* = orders::ASCENDING */)
{
	Sort<Real>(values, valuesCount, keys::VALUE, sortingOrder);
}

void math::sorting::MergeSort::Sort(Vector2D* vectors, size_t vectorSize, keys::Key sortingKey /* = COMPONENT_X */, orders::Order sortingOrder /* = orders::ASCENDING */)
{
	Sort<Vector2D>(vectors, vectorSize, sortingKey, sortingOrder);
}

void math::sorting::MergeSort::Sort(Vector3D* vectors, size_t vectorSize, keys::Key sortingKey /* = COMPONENT_X */, orders::Order sortingOrder /* = orders::ASCENDING */)
{
	Sort<Vector3D>(vectors, vectorSize, sortingKey, sortingOrder);
}

void math::sorting::MergeSort::Sort(Vector2D* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters)
{
	Sort<Vector2D>(vectors, vectorSize, sortingParameters);
}

void math::sorting::MergeSort::Sort(Vector3D* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters)
{
	Sort<Vector3D>(vectors, vectorSize, sortingParameters);
}
/* ==================== MergeSort class implementation end ==================== */

/* ==================== HeapSort class implementation begin ==================== */
math::sorting::HeapSort::HeapSort() : ISort()
{
}

math::sorting::HeapSort::~HeapSort(void)
{
}

void math::sorting::HeapSort::Sort(int* values, size_t valuesCount, orders::Order sortingOrder /* = orders::ASCENDING */)
{
	Sort<int>(values, valuesCount, keys::VALUE, sortingOrder);
}

void math::sorting::HeapSort::Sort(Real* values, size_t valuesCount, orders::Order sortingOrder /* = orders::ASCENDING */)
{
	Sort<Real>(values, valuesCount, keys::VALUE, sortingOrder);
}

void math::sorting::HeapSort::Sort(Vector2D* vectors, size_t vectorSize, keys::Key sortingKey /* = keys::COMPONENT_X */, orders::Order sortingOrder /* = orders::ASCENDING */)
{
	Sort<Vector2D>(vectors, vectorSize, sortingKey, sortingOrder);
}

void math::sorting::HeapSort::Sort(Vector3D* vectors, size_t vectorSize, keys::Key sortingKey /* = keys::COMPONENT_X */, orders::Order sortingOrder /* = orders::ASCENDING */)
{
	Sort<Vector3D>(vectors, vectorSize, sortingKey, sortingOrder);
}

void math::sorting::HeapSort::Sort(Vector2D* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters)
{
	Sort<Vector2D>(vectors, vectorSize, sortingParameters);
}

void math::sorting::HeapSort::Sort(Vector3D* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters)
{
	Sort<Vector3D>(vectors, vectorSize, sortingParameters);
}
/* ==================== HeapSort class implementation end ==================== */

/* ==================== QuickSort class implementation begin ==================== */
math::sorting::QuickSort::QuickSort() : ISort()
{
}

math::sorting::QuickSort::~QuickSort(void)
{
}

void math::sorting::QuickSort::Sort(int* values, size_t valuesCount, orders::Order sortingOrder /* = orders::ASCENDING */)
{
	Sort<int>(values, valuesCount, keys::VALUE, sortingOrder);
}

void math::sorting::QuickSort::Sort(Real* values, size_t valuesCount, orders::Order sortingOrder /* = orders::ASCENDING */)
{
	Sort<Real>(values, valuesCount, keys::VALUE, sortingOrder);
}

void math::sorting::QuickSort::Sort(Vector2D* vectors, size_t vectorSize, keys::Key sortingKey /* = keys::COMPONENT_X */, orders::Order sortingOrder /* = orders::ASCENDING */)
{
	Sort<Vector2D>(vectors, vectorSize, sortingKey, sortingOrder);
}

void math::sorting::QuickSort::Sort(Vector3D* vectors, size_t vectorSize, keys::Key sortingKey /* = keys::COMPONENT_X */, orders::Order sortingOrder /* = orders::ASCENDING */)
{
	Sort<Vector3D>(vectors, vectorSize, sortingKey, sortingOrder);
}

void math::sorting::QuickSort::Sort(Vector2D* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters)
{
	Sort<Vector2D>(vectors, vectorSize, sortingParameters);
}

void math::sorting::QuickSort::Sort(Vector3D* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters)
{
	Sort<Vector3D>(vectors, vectorSize, sortingParameters);
}
/* ==================== QuickSort class implementation end ==================== */

/* ==================== ShellSort class implementation begin ==================== */
math::sorting::ShellSort::ShellSort() : ISort()
{
}

math::sorting::ShellSort::~ShellSort(void)
{
}

void math::sorting::ShellSort::Sort(int* values, size_t valuesCount, orders::Order sortingOrder /* = orders::ASCENDING */)
{
	Sort<int>(values, valuesCount, keys::VALUE, sortingOrder);
}

void math::sorting::ShellSort::Sort(Real* values, size_t valuesCount, orders::Order sortingOrder /* = orders::ASCENDING */)
{
	Sort<Real>(values, valuesCount, keys::VALUE, sortingOrder);
}

void math::sorting::ShellSort::Sort(Vector2D* vectors, size_t vectorSize, keys::Key sortingKey /* = keys::COMPONENT_X */, orders::Order sortingOrder /* = orders::ASCENDING */)
{
	Sort<Vector2D>(vectors, vectorSize, sortingKey, sortingOrder);
}

void math::sorting::ShellSort::Sort(Vector3D* vectors, size_t vectorSize, keys::Key sortingKey /* = keys::COMPONENT_X */, orders::Order sortingOrder /* = orders::ASCENDING */)
{
	Sort<Vector3D>(vectors, vectorSize, sortingKey, sortingOrder);
}

void math::sorting::ShellSort::Sort(Vector2D* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters)
{
	Sort<Vector2D>(vectors, vectorSize, sortingParameters);
}

void math::sorting::ShellSort::Sort(Vector3D* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters)
{
	Sort<Vector3D>(vectors, vectorSize, sortingParameters);
}
/* ==================== ShellSort class implementation end ==================== */

/* ==================== CombSort class implementation begin ==================== */
math::sorting::CombSort::CombSort() : ISort()
{
}

math::sorting::CombSort::~CombSort(void)
{
}

void math::sorting::CombSort::Sort(int* values, size_t valuesCount, orders::Order sortingOrder /* = orders::ASCENDING */)
{
	Sort<int>(values, valuesCount, keys::VALUE, sortingOrder);
}

void math::sorting::CombSort::Sort(Real* values, size_t valuesCount, orders::Order sortingOrder /* = orders::ASCENDING */)
{
	Sort<Real>(values, valuesCount, keys::VALUE, sortingOrder);
}

void math::sorting::CombSort::Sort(Vector2D* vectors, size_t vectorSize, keys::Key sortingKey /* = keys::COMPONENT_X */, orders::Order sortingOrder /* = orders::ASCENDING */)
{
	Sort<Vector2D>(vectors, vectorSize, sortingKey, sortingOrder);
}

void math::sorting::CombSort::Sort(Vector3D* vectors, size_t vectorSize, keys::Key sortingKey /* = keys::COMPONENT_X */, orders::Order sortingOrder /* = orders::ASCENDING */)
{
	Sort<Vector3D>(vectors, vectorSize, sortingKey, sortingOrder);
}

void math::sorting::CombSort::Sort(Vector2D* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters)
{
	Sort<Vector2D>(vectors, vectorSize, sortingParameters);
}

void math::sorting::CombSort::Sort(Vector3D* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters)
{
	Sort<Vector3D>(vectors, vectorSize, sortingParameters);
}
/* ==================== CombSort class implementation end ==================== */

/* ==================== CountingSort class implementation begin ==================== */
math::sorting::CountingSort::CountingSort() : ISort()
{
}

math::sorting::CountingSort::~CountingSort(void)
{
}

void math::sorting::CountingSort::Sort(int* values, size_t valuesCount, orders::Order sortingOrder /* = orders::ASCENDING */)
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

	std::vector<int> helpTab1(valuesCount);
	std::vector<int> helpTab2(range + 1);
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
}

void math::sorting::CountingSort::Sort(Real* values, size_t valuesCount, orders::Order sortingOrder /* = orders::ASCENDING */)
{
	//Sort<Real>(values, valuesCount, VALUE, sortingOrder);
	CRITICAL_LOG_MATH("Counting sort cannot be used to sort the floating point numbers");
	return;
}

void math::sorting::CountingSort::Sort(Vector2D* vectors, size_t vectorSize, keys::Key sortingKey /* = keys::COMPONENT_X */, orders::Order sortingOrder /* = orders::ASCENDING */)
{
	CRITICAL_LOG_MATH("Counting sort cannot be used to sort the floating point numbers");
	return;
}

void math::sorting::CountingSort::Sort(Vector3D* vectors, size_t vectorSize, keys::Key sortingKey /* = keys::COMPONENT_X */, orders::Order sortingOrder /* = orders::ASCENDING */)
{
	CRITICAL_LOG_MATH("Counting sort cannot be used to sort the floating point numbers");
	return;
}

void math::sorting::CountingSort::Sort(Vector2D* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters)
{
	CRITICAL_LOG_MATH("Counting sort cannot be used to sort the floating point numbers");
	return;
}

void math::sorting::CountingSort::Sort(Vector3D* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters)
{
	CRITICAL_LOG_MATH("Counting sort cannot be used to sort the floating point numbers");
	return;
}
/* ==================== CountingSort class implementation end ==================== */

/* ==================== RadixSort class implementation begin ==================== */
math::sorting::RadixSort::RadixSort() : ISort()
{
}

math::sorting::RadixSort::~RadixSort(void)
{
}

void math::sorting::RadixSort::Sort(int* values, size_t valuesCount, orders::Order sortingOrder /* = orders::ASCENDING */)
{
	// TODO: Create implementation of radix sort (see http://en.wikipedia.org/wiki/Radix_sort)
}

void math::sorting::RadixSort::Sort(Real* values, size_t valuesCount, orders::Order sortingOrder /* = orders::ASCENDING */)
{
	EMERGENCY_LOG_MATH("Radix sort cannot be used to sort the floating point numbers, although there are some extensions that can. See this link: http://stereopsis.com/radix.html");
	return;
}

void math::sorting::RadixSort::Sort(Vector2D* vectors, size_t vectorSize, keys::Key sortingKey /* = keys::COMPONENT_X */, orders::Order sortingOrder /* = orders::ASCENDING */)
{
	EMERGENCY_LOG_MATH("Radix sort cannot be used to sort the floating point numbers, although there are some extensions that can. See this link: http://stereopsis.com/radix.html");
	return;
}

void math::sorting::RadixSort::Sort(Vector3D* vectors, size_t vectorSize, keys::Key sortingKey /* = keys::COMPONENT_X */, orders::Order sortingOrder /* = orders::ASCENDING */)
{
	EMERGENCY_LOG_MATH("Radix sort cannot be used to sort the floating point numbers, although there are some extensions that can. See this link: http://stereopsis.com/radix.html");
	return;
}

void math::sorting::RadixSort::Sort(Vector2D* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters)
{
	EMERGENCY_LOG_MATH("Radix sort cannot be used to sort the floating point numbers, although there are some extensions that can. See this link: http://stereopsis.com/radix.html");
	return;
}

void math::sorting::RadixSort::Sort(Vector3D* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters)
{
	EMERGENCY_LOG_MATH("Radix sort cannot be used to sort the floating point numbers, although there are some extensions that can. See this link: http://stereopsis.com/radix.html");
	return;
}
/* ==================== RadixSort class implementation end ==================== */

/* ==================== BucketSort class implementation begin ==================== */
math::sorting::BucketSort::BucketSort() : ISort()
{
}

math::sorting::BucketSort::~BucketSort(void)
{
}

void math::sorting::BucketSort::Sort(int* values, size_t valuesCount, orders::Order sortingOrder /* = orders::ASCENDING */)
{
	//Sort<int>(values, valuesCount, VALUE, sortingOrder);
}

void math::sorting::BucketSort::Sort(Real* values, size_t valuesCount, orders::Order sortingOrder /* = orders::ASCENDING */)
{
	//Sort<Real>(values, valuesCount, VALUE, sortingOrder);
}

void math::sorting::BucketSort::Sort(Vector2D* vectors, size_t vectorSize, keys::Key sortingKey /* = keys::COMPONENT_X */, orders::Order sortingOrder /* = orders::ASCENDING */)
{
	Sort<Vector2D>(vectors, vectorSize, sortingKey, sortingOrder);
}

void math::sorting::BucketSort::Sort(Vector3D* vectors, size_t vectorSize, keys::Key sortingKey /* = keys::COMPONENT_X */, orders::Order sortingOrder /* = orders::ASCENDING */)
{
	Sort<Vector3D>(vectors, vectorSize, sortingKey, sortingOrder);
}

void math::sorting::BucketSort::Sort(Vector2D* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters)
{
	Sort<Vector2D>(vectors, vectorSize, sortingParameters);
}

void math::sorting::BucketSort::Sort(Vector3D* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters)
{
	Sort<Vector3D>(vectors, vectorSize, sortingParameters);
}
/* ==================== BucketSort class implementation end ==================== */
