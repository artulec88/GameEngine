#include "StdAfx.h"
#include "Utility\ILogger.h"
#include "Sort.h"

/* ==================== BubbleSort class implementation begin ==================== */
Math::Sorting::BubbleSort::BubbleSort() : ISort()
{
}

Math::Sorting::BubbleSort::~BubbleSort(void)
{
}

void Math::Sorting::BubbleSort::Sort(int* values, size_t valuesCount, Orders::Order sortingOrder /* = Orders::ASCENDING */)
{
	Sort<int>(values, valuesCount, Keys::VALUE, sortingOrder);
}

void Math::Sorting::BubbleSort::Sort(Math::Real* values, size_t valuesCount, Orders::Order sortingOrder /* = Orders::ASCENDING */)
{
	Sort<Real>(values, valuesCount, Keys::VALUE, sortingOrder);
}

void Math::Sorting::BubbleSort::Sort(Math::Vector2D* vectors, size_t vectorSize, Keys::Key sortingKey /* = Keys::COMPONENT_X */, Orders::Order sortingOrder /* = Orders::ASCENDING */)
{
	Sort<Math::Vector2D>(vectors, vectorSize, sortingKey, sortingOrder);
}

void Math::Sorting::BubbleSort::Sort(Math::Vector3D* vectors, size_t vectorSize, Keys::Key sortingKey /* = Keys::COMPONENT_X */, Orders::Order sortingOrder /* = Orders::ASCENDING */)
{
	Sort<Math::Vector3D>(vectors, vectorSize, sortingKey, sortingOrder);
}

void Math::Sorting::BubbleSort::Sort(Math::Vector2D* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters)
{
	Sort<Math::Vector2D>(vectors, vectorSize, sortingParameters);
}

void Math::Sorting::BubbleSort::Sort(Math::Vector3D* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters)
{
	Sort<Math::Vector3D>(vectors, vectorSize, sortingParameters);
}
/* ==================== BubbleSort class implementation end ==================== */

/* ==================== InsertionSort class implementation begin ==================== */
Math::Sorting::InsertionSort::InsertionSort() : ISort()
{
}

Math::Sorting::InsertionSort::~InsertionSort(void)
{
}

void Math::Sorting::InsertionSort::Sort(int* values, size_t valuesCount, Orders::Order sortingOrder /* = Orders::ASCENDING */)
{
	Sort<int>(values, valuesCount, Keys::VALUE, sortingOrder);
}

void Math::Sorting::InsertionSort::Sort(Math::Real* values, size_t valuesCount, Orders::Order sortingOrder /* = Orders::ASCENDING */)
{
	Sort<Real>(values, valuesCount, Keys::VALUE, sortingOrder);
}

void Math::Sorting::InsertionSort::Sort(Math::Vector2D* vectors, size_t vectorSize, Keys::Key sortingKey /* = Keys::COMPONENT_X */, Orders::Order sortingOrder /* = Orders::ASCENDING */)
{
	Sort<Math::Vector2D>(vectors, vectorSize, sortingKey, sortingOrder);
}

void Math::Sorting::InsertionSort::Sort(Math::Vector3D* vectors, size_t vectorSize, Keys::Key sortingKey /* = Keys::COMPONENT_X */, Orders::Order sortingOrder /* = Orders::ASCENDING */)
{
	Sort<Math::Vector3D>(vectors, vectorSize, sortingKey, sortingOrder);
}

void Math::Sorting::InsertionSort::Sort(Math::Vector2D* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters)
{
	Sort<Math::Vector2D>(vectors, vectorSize, sortingParameters);
}

void Math::Sorting::InsertionSort::Sort(Math::Vector3D* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters)
{
	Sort<Math::Vector3D>(vectors, vectorSize, sortingParameters);
}
/* ==================== InsertionSort class implementation end ==================== */

/* ==================== SelectionSort class implementation begin ==================== */
Math::Sorting::SelectionSort::SelectionSort() : ISort()
{
}

Math::Sorting::SelectionSort::~SelectionSort(void)
{
}

void Math::Sorting::SelectionSort::Sort(int* values, size_t valuesCount, Orders::Order sortingOrder /* = Orders::ASCENDING */)
{
	Sort<int>(values, valuesCount, Keys::VALUE, sortingOrder);
}

void Math::Sorting::SelectionSort::Sort(Math::Real* values, size_t valuesCount, Orders::Order sortingOrder /* = Orders::ASCENDING */)
{
	Sort<Real>(values, valuesCount, Keys::VALUE, sortingOrder);
}

void Math::Sorting::SelectionSort::Sort(Math::Vector2D* vectors, size_t vectorSize, Keys::Key sortingKey /* = Keys::COMPONENT_X */, Orders::Order sortingOrder /* = Orders::ASCENDING */)
{
	Sort<Math::Vector2D>(vectors, vectorSize, sortingKey, sortingOrder);
}

void Math::Sorting::SelectionSort::Sort(Math::Vector3D* vectors, size_t vectorSize, Keys::Key sortingKey /* = Keys::COMPONENT_X */, Orders::Order sortingOrder /* = Orders::ASCENDING */)
{
	Sort<Math::Vector3D>(vectors, vectorSize, sortingKey, sortingOrder);
}

void Math::Sorting::SelectionSort::Sort(Math::Vector2D* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters)
{
	Sort<Math::Vector2D>(vectors, vectorSize, sortingParameters);
}

void Math::Sorting::SelectionSort::Sort(Math::Vector3D* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters)
{
	Sort<Math::Vector3D>(vectors, vectorSize, sortingParameters);
}
/* ==================== SelectionSort class implementation end ==================== */

/* ==================== MergeSort class implementation begin ==================== */
Math::Sorting::MergeSort::MergeSort() : ISort()
{
}

Math::Sorting::MergeSort::~MergeSort(void)
{
}

void Math::Sorting::MergeSort::Sort(int* values, size_t valuesCount, Orders::Order sortingOrder /* = Orders::ASCENDING */)
{
	Sort<int>(values, valuesCount, Keys::VALUE, sortingOrder);
}

void Math::Sorting::MergeSort::Sort(Math::Real* values, size_t valuesCount, Orders::Order sortingOrder /* = Orders::ASCENDING */)
{
	Sort<Real>(values, valuesCount, Keys::VALUE, sortingOrder);
}

void Math::Sorting::MergeSort::Sort(Math::Vector2D* vectors, size_t vectorSize, Keys::Key sortingKey /* = COMPONENT_X */, Orders::Order sortingOrder /* = Orders::ASCENDING */)
{
	Sort<Math::Vector2D>(vectors, vectorSize, sortingKey, sortingOrder);
}

void Math::Sorting::MergeSort::Sort(Math::Vector3D* vectors, size_t vectorSize, Keys::Key sortingKey /* = COMPONENT_X */, Orders::Order sortingOrder /* = Orders::ASCENDING */)
{
	Sort<Math::Vector3D>(vectors, vectorSize, sortingKey, sortingOrder);
}

void Math::Sorting::MergeSort::Sort(Math::Vector2D* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters)
{
	Sort<Math::Vector2D>(vectors, vectorSize, sortingParameters);
}

void Math::Sorting::MergeSort::Sort(Math::Vector3D* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters)
{
	Sort<Math::Vector3D>(vectors, vectorSize, sortingParameters);
}
/* ==================== MergeSort class implementation end ==================== */

/* ==================== HeapSort class implementation begin ==================== */
Math::Sorting::HeapSort::HeapSort() : ISort()
{
}

Math::Sorting::HeapSort::~HeapSort(void)
{
}

void Math::Sorting::HeapSort::Sort(int* values, size_t valuesCount, Orders::Order sortingOrder /* = Orders::ASCENDING */)
{
	Sort<int>(values, valuesCount, Keys::VALUE, sortingOrder);
}

void Math::Sorting::HeapSort::Sort(Math::Real* values, size_t valuesCount, Orders::Order sortingOrder /* = Orders::ASCENDING */)
{
	Sort<Real>(values, valuesCount, Keys::VALUE, sortingOrder);
}

void Math::Sorting::HeapSort::Sort(Math::Vector2D* vectors, size_t vectorSize, Keys::Key sortingKey /* = Keys::COMPONENT_X */, Orders::Order sortingOrder /* = Orders::ASCENDING */)
{
	Sort<Math::Vector2D>(vectors, vectorSize, sortingKey, sortingOrder);
}

void Math::Sorting::HeapSort::Sort(Math::Vector3D* vectors, size_t vectorSize, Keys::Key sortingKey /* = Keys::COMPONENT_X */, Orders::Order sortingOrder /* = Orders::ASCENDING */)
{
	Sort<Math::Vector3D>(vectors, vectorSize, sortingKey, sortingOrder);
}

void Math::Sorting::HeapSort::Sort(Math::Vector2D* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters)
{
	Sort<Math::Vector2D>(vectors, vectorSize, sortingParameters);
}

void Math::Sorting::HeapSort::Sort(Math::Vector3D* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters)
{
	Sort<Math::Vector3D>(vectors, vectorSize, sortingParameters);
}
/* ==================== HeapSort class implementation end ==================== */

/* ==================== QuickSort class implementation begin ==================== */
Math::Sorting::QuickSort::QuickSort() : ISort()
{
}

Math::Sorting::QuickSort::~QuickSort(void)
{
}

void Math::Sorting::QuickSort::Sort(int* values, size_t valuesCount, Orders::Order sortingOrder /* = Orders::ASCENDING */)
{
	Sort<int>(values, valuesCount, Keys::VALUE, sortingOrder);
}

void Math::Sorting::QuickSort::Sort(Math::Real* values, size_t valuesCount, Orders::Order sortingOrder /* = Orders::ASCENDING */)
{
	Sort<Real>(values, valuesCount, Keys::VALUE, sortingOrder);
}

void Math::Sorting::QuickSort::Sort(Math::Vector2D* vectors, size_t vectorSize, Keys::Key sortingKey /* = Keys::COMPONENT_X */, Orders::Order sortingOrder /* = Orders::ASCENDING */)
{
	Sort<Math::Vector2D>(vectors, vectorSize, sortingKey, sortingOrder);
}

void Math::Sorting::QuickSort::Sort(Math::Vector3D* vectors, size_t vectorSize, Keys::Key sortingKey /* = Keys::COMPONENT_X */, Orders::Order sortingOrder /* = Orders::ASCENDING */)
{
	Sort<Math::Vector3D>(vectors, vectorSize, sortingKey, sortingOrder);
}

void Math::Sorting::QuickSort::Sort(Math::Vector2D* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters)
{
	Sort<Math::Vector2D>(vectors, vectorSize, sortingParameters);
}

void Math::Sorting::QuickSort::Sort(Math::Vector3D* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters)
{
	Sort<Math::Vector3D>(vectors, vectorSize, sortingParameters);
}
/* ==================== QuickSort class implementation end ==================== */

/* ==================== ShellSort class implementation begin ==================== */
Math::Sorting::ShellSort::ShellSort() : ISort()
{
}

Math::Sorting::ShellSort::~ShellSort(void)
{
}

void Math::Sorting::ShellSort::Sort(int* values, size_t valuesCount, Orders::Order sortingOrder /* = Orders::ASCENDING */)
{
	Sort<int>(values, valuesCount, Keys::VALUE, sortingOrder);
}

void Math::Sorting::ShellSort::Sort(Math::Real* values, size_t valuesCount, Orders::Order sortingOrder /* = Orders::ASCENDING */)
{
	Sort<Real>(values, valuesCount, Keys::VALUE, sortingOrder);
}

void Math::Sorting::ShellSort::Sort(Math::Vector2D* vectors, size_t vectorSize, Keys::Key sortingKey /* = Keys::COMPONENT_X */, Orders::Order sortingOrder /* = Orders::ASCENDING */)
{
	Sort<Math::Vector2D>(vectors, vectorSize, sortingKey, sortingOrder);
}

void Math::Sorting::ShellSort::Sort(Math::Vector3D* vectors, size_t vectorSize, Keys::Key sortingKey /* = Keys::COMPONENT_X */, Orders::Order sortingOrder /* = Orders::ASCENDING */)
{
	Sort<Math::Vector3D>(vectors, vectorSize, sortingKey, sortingOrder);
}

void Math::Sorting::ShellSort::Sort(Math::Vector2D* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters)
{
	Sort<Math::Vector2D>(vectors, vectorSize, sortingParameters);
}

void Math::Sorting::ShellSort::Sort(Math::Vector3D* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters)
{
	Sort<Math::Vector3D>(vectors, vectorSize, sortingParameters);
}
/* ==================== ShellSort class implementation end ==================== */

/* ==================== CombSort class implementation begin ==================== */
Math::Sorting::CombSort::CombSort() : ISort()
{
}

Math::Sorting::CombSort::~CombSort(void)
{
}

void Math::Sorting::CombSort::Sort(int* values, size_t valuesCount, Orders::Order sortingOrder /* = Orders::ASCENDING */)
{
	Sort<int>(values, valuesCount, Keys::VALUE, sortingOrder);
}

void Math::Sorting::CombSort::Sort(Math::Real* values, size_t valuesCount, Orders::Order sortingOrder /* = Orders::ASCENDING */)
{
	Sort<Real>(values, valuesCount, Keys::VALUE, sortingOrder);
}

void Math::Sorting::CombSort::Sort(Math::Vector2D* vectors, size_t vectorSize, Keys::Key sortingKey /* = Keys::COMPONENT_X */, Orders::Order sortingOrder /* = Orders::ASCENDING */)
{
	Sort<Math::Vector2D>(vectors, vectorSize, sortingKey, sortingOrder);
}

void Math::Sorting::CombSort::Sort(Math::Vector3D* vectors, size_t vectorSize, Keys::Key sortingKey /* = Keys::COMPONENT_X */, Orders::Order sortingOrder /* = Orders::ASCENDING */)
{
	Sort<Math::Vector3D>(vectors, vectorSize, sortingKey, sortingOrder);
}

void Math::Sorting::CombSort::Sort(Math::Vector2D* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters)
{
	Sort<Math::Vector2D>(vectors, vectorSize, sortingParameters);
}

void Math::Sorting::CombSort::Sort(Math::Vector3D* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters)
{
	Sort<Math::Vector3D>(vectors, vectorSize, sortingParameters);
}
/* ==================== CombSort class implementation end ==================== */

/* ==================== CountingSort class implementation begin ==================== */
Math::Sorting::CountingSort::CountingSort() : ISort()
{
}

Math::Sorting::CountingSort::~CountingSort(void)
{
}

void Math::Sorting::CountingSort::Sort(int* values, size_t valuesCount, Orders::Order sortingOrder /* = Orders::ASCENDING */)
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

void Math::Sorting::CountingSort::Sort(Math::Real* values, size_t valuesCount, Orders::Order sortingOrder /* = Orders::ASCENDING */)
{
	//Sort<Math::Real>(values, valuesCount, VALUE, sortingOrder);
	CRITICAL_LOG_MATH("Counting sort cannot be used to sort the floating point numbers");
	return;
}

void Math::Sorting::CountingSort::Sort(Math::Vector2D* vectors, size_t vectorSize, Keys::Key sortingKey /* = Keys::COMPONENT_X */, Orders::Order sortingOrder /* = Orders::ASCENDING */)
{
	CRITICAL_LOG_MATH("Counting sort cannot be used to sort the floating point numbers");
	return;
}

void Math::Sorting::CountingSort::Sort(Math::Vector3D* vectors, size_t vectorSize, Keys::Key sortingKey /* = Keys::COMPONENT_X */, Orders::Order sortingOrder /* = Orders::ASCENDING */)
{
	CRITICAL_LOG_MATH("Counting sort cannot be used to sort the floating point numbers");
	return;
}

void Math::Sorting::CountingSort::Sort(Math::Vector2D* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters)
{
	CRITICAL_LOG_MATH("Counting sort cannot be used to sort the floating point numbers");
	return;
}

void Math::Sorting::CountingSort::Sort(Math::Vector3D* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters)
{
	CRITICAL_LOG_MATH("Counting sort cannot be used to sort the floating point numbers");
	return;
}
/* ==================== CountingSort class implementation end ==================== */

/* ==================== RadixSort class implementation begin ==================== */
Math::Sorting::RadixSort::RadixSort() : ISort()
{
}

Math::Sorting::RadixSort::~RadixSort(void)
{
}

void Math::Sorting::RadixSort::Sort(int* values, size_t valuesCount, Orders::Order sortingOrder /* = Orders::ASCENDING */)
{
	// TODO: Create implementation of radix sort (see http://en.wikipedia.org/wiki/Radix_sort)
}

void Math::Sorting::RadixSort::Sort(Math::Real* values, size_t valuesCount, Orders::Order sortingOrder /* = Orders::ASCENDING */)
{
	EMERGENCY_LOG_MATH("Radix sort cannot be used to sort the floating point numbers, although there are some extensions that can. See this link: http://stereopsis.com/radix.html");
	return;
}

void Math::Sorting::RadixSort::Sort(Math::Vector2D* vectors, size_t vectorSize, Keys::Key sortingKey /* = Keys::COMPONENT_X */, Orders::Order sortingOrder /* = Orders::ASCENDING */)
{
	EMERGENCY_LOG_MATH("Radix sort cannot be used to sort the floating point numbers, although there are some extensions that can. See this link: http://stereopsis.com/radix.html");
	return;
}

void Math::Sorting::RadixSort::Sort(Math::Vector3D* vectors, size_t vectorSize, Keys::Key sortingKey /* = Keys::COMPONENT_X */, Orders::Order sortingOrder /* = Orders::ASCENDING */)
{
	EMERGENCY_LOG_MATH("Radix sort cannot be used to sort the floating point numbers, although there are some extensions that can. See this link: http://stereopsis.com/radix.html");
	return;
}

void Math::Sorting::RadixSort::Sort(Math::Vector2D* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters)
{
	EMERGENCY_LOG_MATH("Radix sort cannot be used to sort the floating point numbers, although there are some extensions that can. See this link: http://stereopsis.com/radix.html");
	return;
}

void Math::Sorting::RadixSort::Sort(Math::Vector3D* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters)
{
	EMERGENCY_LOG_MATH("Radix sort cannot be used to sort the floating point numbers, although there are some extensions that can. See this link: http://stereopsis.com/radix.html");
	return;
}
/* ==================== RadixSort class implementation end ==================== */

/* ==================== BucketSort class implementation begin ==================== */
Math::Sorting::BucketSort::BucketSort() : ISort()
{
}

Math::Sorting::BucketSort::~BucketSort(void)
{
}

void Math::Sorting::BucketSort::Sort(int* values, size_t valuesCount, Orders::Order sortingOrder /* = Orders::ASCENDING */)
{
	//Sort<int>(values, valuesCount, VALUE, sortingOrder);
}

void Math::Sorting::BucketSort::Sort(Math::Real* values, size_t valuesCount, Orders::Order sortingOrder /* = Orders::ASCENDING */)
{
	//Sort<Real>(values, valuesCount, VALUE, sortingOrder);
}

void Math::Sorting::BucketSort::Sort(Math::Vector2D* vectors, size_t vectorSize, Keys::Key sortingKey /* = Keys::COMPONENT_X */, Orders::Order sortingOrder /* = Orders::ASCENDING */)
{
	Sort<Math::Vector2D>(vectors, vectorSize, sortingKey, sortingOrder);
}

void Math::Sorting::BucketSort::Sort(Math::Vector3D* vectors, size_t vectorSize, Keys::Key sortingKey /* = Keys::COMPONENT_X */, Orders::Order sortingOrder /* = Orders::ASCENDING */)
{
	Sort<Math::Vector3D>(vectors, vectorSize, sortingKey, sortingOrder);
}

void Math::Sorting::BucketSort::Sort(Math::Vector2D* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters)
{
	Sort<Math::Vector2D>(vectors, vectorSize, sortingParameters);
}

void Math::Sorting::BucketSort::Sort(Math::Vector3D* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters)
{
	Sort<Math::Vector3D>(vectors, vectorSize, sortingParameters);
}
/* ==================== BucketSort class implementation end ==================== */
