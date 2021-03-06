#ifndef __MATH_SORT_H__
#define __MATH_SORT_H__

#include "Math.h"
#include "ISort.h"
#include <vector>

namespace math {
	namespace sorting {

		/// <summary>
		/// The object representation of the bubble sort algorithm.
		/// </summary>
		/// <remarks>
		/// See https://en.wikipedia.org/wiki/Bubble_sort for details.
		/// </remarks>
		class BubbleSort : public ISort
		{
			/* ==================== Static variables and functions begin ==================== */
			/* ==================== Static variables and functions end ==================== */

			/* ==================== Constructors and destructors begin ==================== */
		public:
			/// <summary> Bubble sort object constructor. </summary>
			BubbleSort();

			/// <summary> Bubble sort object destructor. </summary>
			virtual ~BubbleSort();

			/// <summary> Bubble sort object copy constructor. </summary>
			/// <param name="bubbleSort"> The bubble sort object to copy construct from. </param>
			BubbleSort(const BubbleSort& bubbleSort) = delete;

			/// <summary> Bubble sort object move constructor. </summary>
			/// <param name="bubbleSort"> The r-value reference of the bubble sort object to move construct from. </param>
			BubbleSort(BubbleSort&& bubbleSort) = delete;

			/// <summary> Bubble sort object copy assignment operator. </summary>
			/// <param name="bubbleSort"> The bubble sort object to copy assign from. </param>
			/// <returns> The reference to the newly copy-assigned bubble sort object. </returns>
			BubbleSort& operator=(const BubbleSort& bubbleSort) = delete;

			/// <summary> Bubble sort object move assignment operator. </summary>
			/// <param name="bubbleSort"> The r-value reference of the bubble sort object to move assign from. </param>
			/// <returns> The reference to the newly move-assigned bubble sort object. </returns>
			BubbleSort& operator=(BubbleSort&& bubbleSort) = delete;
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
			void Sort(int* values, size_t valuesCount, orders::Order sortingOrder = orders::ASCENDING) override;
			void Sort(Real* values, size_t valuesCount, orders::Order sortingOrder = orders::ASCENDING) override;
			void Sort(Vector2D* vectors, size_t vectorSize, keys::Key sortingKey = keys::COMPONENT_X, orders::Order sortingOrder = orders::ASCENDING) override;
			void Sort(Vector3D* vectors, size_t vectorSize, keys::Key sortingKey = keys::COMPONENT_X, orders::Order sortingOrder = orders::ASCENDING) override;
			void Sort(Vector2D* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters) override;
			void Sort(Vector3D* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters) override;
		private:
			template <typename T>
			void Sort(T* vectors, size_t vectorSize, keys::Key sortingKey = keys::COMPONENT_X, orders::Order sortingOrder = orders::ASCENDING)
			{
				Sort(vectors, vectorSize, SortingParametersChain(sortingKey, sortingOrder));
			}

			template <typename T>
			void Sort(T* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters)
			{
				CHECK_CONDITION_RETURN_VOID_MATH(vectors != nullptr, utility::logging::EMERGENCY,
					"Cannot sort the table. The specified table is nullptr");

				bool swapOccured;
				auto n = vectorSize;
				do
				{
					swapOccured = false;
					for (size_t i = 1; i < n; ++i)
					{
						if (NeedSwapping(vectors[i - 1], vectors[i], sortingParameters))
						{
							Swap(vectors[i - 1], vectors[i]);
							swapOccured = true;
						}
					}
					--n;
				} while (swapOccured);
			}
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
			/* ==================== Non-static member variables end ==================== */
		}; /* end class BubbleSort */

		/// <summary>
		/// The object representation of the insertion sort algorithm.
		/// </summary>
		/// <remarks>
		/// See https://en.wikipedia.org/wiki/Insertion_sort for details.
		/// </remarks>
		class InsertionSort : public ISort
		{
			/* ==================== Static variables and functions begin ==================== */
			/* ==================== Static variables and functions end ==================== */

			/* ==================== Constructors and destructors begin ==================== */
		public:
			/// <summary> Insertion sort object constructor. </summary>
			InsertionSort();

			/// <summary> Insertion sort object destructor. </summary>
			virtual ~InsertionSort();

			/// <summary> Insertion sort object copy constructor. </summary>
			/// <param name="insertionSort"> The insertion sort object to copy construct from. </param>
			InsertionSort(const InsertionSort& insertionSort) = delete;

			/// <summary> Insertion sort object move constructor. </summary>
			/// <param name="insertionSort"> The r-value reference of the insertion sort object to move construct from. </param>
			InsertionSort(InsertionSort&& insertionSort) = delete;

			/// <summary> Insertion sort object copy assignment operator. </summary>
			/// <param name="insertionSort"> The insertion sort object to copy assign from. </param>
			/// <returns> The reference to the newly copy-assigned insertion sort object. </returns>
			InsertionSort& operator=(const InsertionSort& insertionSort) = delete;

			/// <summary> Insertion sort object move assignment operator. </summary>
			/// <param name="insertionSort"> The r-value reference of the insertion sort object to move assign from. </param>
			/// <returns> The reference to the newly move-assigned insertion sort object. </returns>
			InsertionSort& operator=(InsertionSort&& insertionSort) = delete;
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
			void Sort(int* values, size_t valuesCount, orders::Order sortingOrder = orders::ASCENDING) override;
			void Sort(Real* values, size_t valuesCount, orders::Order sortingOrder = orders::ASCENDING) override;
			void Sort(Vector2D* vectors, size_t vectorSize, keys::Key sortingKey = keys::COMPONENT_X, orders::Order sortingOrder = orders::ASCENDING) override;
			void Sort(Vector3D* vectors, size_t vectorSize, keys::Key sortingKey = keys::COMPONENT_X, orders::Order sortingOrder = orders::ASCENDING) override;
			void Sort(Vector2D* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters) override;
			void Sort(Vector3D* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters) override;
		private:
			template <typename T>
			void Sort(T* vectors, size_t vectorSize, keys::Key sortingKey = keys::COMPONENT_X, orders::Order sortingOrder = orders::ASCENDING)
			{
				Sort(vectors, vectorSize, SortingParametersChain(sortingKey, sortingOrder));
			}

			template <typename T>
			void Sort(T* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters)
			{
				CHECK_CONDITION_RETURN_VOID_MATH(vectors != nullptr, utility::logging::EMERGENCY,
					"Cannot sort the table using the insertion sort algorithm. The specified table is nullptr");

				for (auto i = 1; i < vectorSize; ++i)
				{
					T key(vectors[i]);
					auto j = i - 1;
					while (j >= 0 && NeedSwapping(vectors[j], key, sortingParameters))
					{
						vectors[j + 1] = vectors[j];
						vectors[j] = key;
						--j;
					}
				}
			}
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
			/* ==================== Non-static member variables end ==================== */
		}; /* end class InsertionSort */

		/// <summary>
		/// The object representation of the selection sort algorithm.
		/// </summary>
		/// <remarks>
		/// See https://en.wikipedia.org/wiki/Selection_sort for details.
		/// </remarks>
		class SelectionSort : public ISort
		{
			/* ==================== Static variables and functions begin ==================== */
			/* ==================== Static variables and functions end ==================== */

			/* ==================== Constructors and destructors begin ==================== */
		public:
			/// <summary> Selection sort object constructor. </summary>
			SelectionSort();

			/// <summary> Selection sort object destructor. </summary>
			virtual ~SelectionSort();

			/// <summary> Selection sort object copy constructor. </summary>
			/// <param name="selectionSort"> The selection sort object to copy construct from. </param>
			SelectionSort(const SelectionSort& selectionSort) = delete;

			/// <summary> Selection sort object move constructor. </summary>
			/// <param name="selectionSort"> The r-value reference of the selection sort object to move construct from. </param>
			SelectionSort(SelectionSort&& selectionSort) = delete;

			/// <summary> Selection sort object copy assignment operator. </summary>
			/// <param name="selectionSort"> The selection sort object to copy assign from. </param>
			/// <returns> The reference to the newly copy-assigned selection sort object. </returns>
			SelectionSort& operator=(const SelectionSort& selectionSort) = delete;

			/// <summary> Selection sort object move assignment operator. </summary>
			/// <param name="selectionSort"> The r-value reference of the selection sort object to move assign from. </param>
			/// <returns> The reference to the newly move-assigned selection sort object. </returns>
			SelectionSort& operator=(SelectionSort&& selectionSort) = delete;
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
			void Sort(int* values, size_t valuesCount, orders::Order sortingOrder = orders::ASCENDING) override;
			void Sort(Real* values, size_t valuesCount, orders::Order sortingOrder = orders::ASCENDING) override;
			void Sort(Vector2D* vectors, size_t vectorSize, keys::Key sortingKey = keys::COMPONENT_X, orders::Order sortingOrder = orders::ASCENDING) override;
			void Sort(Vector3D* vectors, size_t vectorSize, keys::Key sortingKey = keys::COMPONENT_X, orders::Order sortingOrder = orders::ASCENDING) override;
			void Sort(Vector2D* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters) override;
			void Sort(Vector3D* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters) override;
		private:
			template <typename T>
			void Sort(T* vectors, size_t vectorSize, keys::Key sortingKey = keys::COMPONENT_X, orders::Order sortingOrder = orders::ASCENDING)
			{
				Sort(vectors, vectorSize, SortingParametersChain(sortingKey, sortingOrder));
			}

			template <typename T>
			void Sort(T* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters)
			{
				CHECK_CONDITION_RETURN_VOID_MATH(vectors != nullptr, utility::logging::EMERGENCY,
					"Cannot sort the table. The specified table is nullptr");
				CHECK_CONDITION_RETURN_VOID_ALWAYS_MATH(vectorSize >= 2, utility::logging::DEBUG,
					"Selection sort operation ignored. Only one element to sort.");

				for (auto k = 0; k < vectorSize - 1; ++k)
				{
					auto indexToSwap = k + 1;
					T vecToSwap = vectors[indexToSwap];
					for (auto i = k + 2; i < vectorSize; ++i)
					{
						if (NeedSwapping(vecToSwap, vectors[i], sortingParameters))
						{
							indexToSwap = i;
							vecToSwap = vectors[indexToSwap];
						}
					}
					if (NeedSwapping(vectors[k], vectors[indexToSwap], sortingParameters))
					{
						Swap(vectors[k], vectors[indexToSwap]);
					}
				}
			}
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
			/* ==================== Non-static member variables end ==================== */
		}; /* end class SelectionSort */

		/// <summary>
		/// The object representation of the merge sort algorithm.
		/// </summary>
		/// <remarks>
		/// See https://en.wikipedia.org/wiki/Merge_sort for details.
		/// </remarks>
		class MergeSort : public ISort
		{
			/* ==================== Static variables and functions begin ==================== */
			/* ==================== Static variables and functions end ==================== */

			/* ==================== Constructors and destructors begin ==================== */
		public:
			/// <summary> Merge sort object constructor. </summary>
			MergeSort();

			/// <summary> Merge sort object destructor. </summary>
			virtual ~MergeSort();

			/// <summary> Merge sort object copy constructor. </summary>
			/// <param name="mergeSort"> The merge sort object to copy construct from. </param>
			MergeSort(const MergeSort& mergeSort) = delete;

			/// <summary> Merge sort object move constructor. </summary>
			/// <param name="mergeSort"> The r-value reference of the merge sort object to move construct from. </param>
			MergeSort(MergeSort&& mergeSort) = delete;

			/// <summary> Merge sort object copy assignment operator. </summary>
			/// <param name="mergeSort"> The merge sort object to copy assign from. </param>
			/// <returns> The reference to the newly copy-assigned merge sort object. </returns>
			MergeSort& operator=(const MergeSort& mergeSort) = delete;

			/// <summary> Merge sort object move assignment operator. </summary>
			/// <param name="mergeSort"> The r-value reference of the merge sort object to move assign from. </param>
			/// <returns> The reference to the newly move-assigned merge sort object. </returns>
			MergeSort& operator=(MergeSort&& mergeSort) = delete;
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
			void Sort(int* values, size_t valuesCount, orders::Order sortingOrder = orders::ASCENDING) override;
			void Sort(Real* values, size_t valuesCount, orders::Order sortingOrder = orders::ASCENDING) override;
			void Sort(Vector2D* vectors, size_t vectorSize, keys::Key sortingKey = keys::COMPONENT_X, orders::Order sortingOrder = orders::ASCENDING) override;
			void Sort(Vector3D* vectors, size_t vectorSize, keys::Key sortingKey = keys::COMPONENT_X, orders::Order sortingOrder = orders::ASCENDING) override;
			void Sort(Vector2D* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters) override;
			void Sort(Vector3D* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters) override;
		private:
			template <typename T>
			void Sort(T* vectors, size_t vectorSize, keys::Key sortingKey = keys::COMPONENT_X, orders::Order sortingOrder = orders::ASCENDING)
			{
				Sort(vectors, vectorSize, SortingParametersChain(sortingKey, sortingOrder));
			}

			template <typename T>
			void Sort(T* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters)
			{
				CHECK_CONDITION_RETURN_VOID_MATH(vectors != nullptr, utility::logging::EMERGENCY,
					"Cannot sort the table. The specified table is nullptr");
				std::vector<T> auxiliaryTable(vectorSize);
				Sort<T>(vectors, 0, vectorSize - 1, sortingParameters, auxiliaryTable.data());
			}

			template <typename T>
			void Sort(T* values, size_t left, size_t right, const SortingParametersChain& sortingParameters, T* auxiliaryTable)
			{
				if (left >= right)
				{
					return;
				}
				auto pivot = (left + right) / 2;
				Sort(values, left, pivot, sortingParameters, auxiliaryTable);
				Sort(values, pivot + 1, right, sortingParameters, auxiliaryTable);
				Merge(values, left, pivot, right, sortingParameters, auxiliaryTable);
			}

			template <typename T>
			void Merge(T* values, size_t left, size_t pivot, size_t right, const SortingParametersChain& sortingParameters, T* auxiliaryTable)
			{
				auto j = pivot + 1 - left;
				auto q = left;
				size_t i;
				//Vector2D* t = new Vector2D [right - left + 1];
				size_t k = 0;
				for (i = left; i <= right; ++i, ++k)
				{
					auxiliaryTable[k] = values[i]; // copying the data to auxiliary table
				}
				i = 0;
				while (i <= pivot - left && j <= right - left) // transferring the data with sorting from auxiliary tables to the result table
				{
					if (NeedSwapping(auxiliaryTable[j], auxiliaryTable[i], sortingParameters))
					{
						values[q] = auxiliaryTable[i++];
					}
					else
					{
						values[q] = auxiliaryTable[j++];
					}
					++q;
				}
				while (i <= pivot - left) // transferring the not-yet-copied data from the first set when the second one is already empty
				{
					values[q] = auxiliaryTable[i];
					++q;
					++i;
				}
				//delete [] t;
			}
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
			/* ==================== Non-static member variables end ==================== */
		}; /* end class MergeSort */

		/// <summary>
		/// The object representation of the heap sort algorithm.
		/// </summary>
		/// <remarks>
		/// See https://en.wikipedia.org/wiki/Heap_sort for details.
		/// </remarks>
		class HeapSort : public ISort
		{
			/* ==================== Static variables and functions begin ==================== */
			/* ==================== Static variables and functions end ==================== */

			/* ==================== Constructors and destructors begin ==================== */
		public:
			/// <summary> Heap sort object constructor. </summary>
			HeapSort();

			/// <summary> Heap sort object destructor. </summary>
			virtual ~HeapSort();

			/// <summary> Heap sort object copy constructor. </summary>
			/// <param name="heapSort"> The heap sort object to copy construct from. </param>
			HeapSort(const HeapSort& heapSort) = delete;

			/// <summary> Heap sort object move constructor. </summary>
			/// <param name="heapSort"> The r-value reference of the heap sort object to move construct from. </param>
			HeapSort(HeapSort&& heapSort) = delete;

			/// <summary> Heap sort object copy assignment operator. </summary>
			/// <param name="heapSort"> The heap sort object to copy assign from. </param>
			/// <returns> The reference to the newly copy-assigned heap sort object. </returns>
			HeapSort& operator=(const HeapSort& heapSort) = delete;

			/// <summary> Heap sort object move assignment operator. </summary>
			/// <param name="heapSort"> The r-value reference of the heap sort object to move assign from. </param>
			/// <returns> The reference to the newly move-assigned heap sort object. </returns>
			HeapSort& operator=(HeapSort&& heapSort) = delete;
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
			void Sort(int* values, size_t valuesCount, orders::Order sortingOrder = orders::ASCENDING) override;
			void Sort(Real* values, size_t valuesCount, orders::Order sortingOrder = orders::ASCENDING) override;
			void Sort(Vector2D* vectors, size_t vectorSize, keys::Key sortingKey = keys::COMPONENT_X, orders::Order sortingOrder = orders::ASCENDING) override;
			void Sort(Vector3D* vectors, size_t vectorSize, keys::Key sortingKey = keys::COMPONENT_X, orders::Order sortingOrder = orders::ASCENDING) override;
			void Sort(Vector2D* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters) override;
			void Sort(Vector3D* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters) override;
		private:
			template <typename T>
			void Sort(T* vectors, size_t vectorSize, keys::Key sortingKey, orders::Order sortingOrder)
			{
				Sort(vectors, vectorSize, SortingParametersChain(sortingKey, sortingOrder));
			}

			template <typename T>
			void Sort(T* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters)
			{
				auto j = vectorSize - 1;
				auto heapHeight = 0;
				while (j > 1)
				{
					j /= 2; // specifying the height of the heap based on the number of elements
					++heapHeight;
				}
				for (j = 0; j <= heapHeight; ++j)
				{
					auto i = vectorSize - 1;
					while (i > 0)
					{
						auto k = i / 2; // the heap is started being constructed here in the input vector
						if (NeedSwapping(vectors[i], vectors[k], sortingParameters)) // checking the condition of heap existance in the input vector
						{
							Swap(vectors[i], vectors[k]);
						}
						--i;
					}
				}
				//CRITICAL_LOG_MATH("Heap sort after heap is built:");
				//for (size_t index = 0; index < vectorSize; ++index)
				//{
				//	ERROR_LOG_MATH("vectors[", index, "] = ", vectors[index], ".");
				//}
				for (j = vectorSize - 1; j > 0; --j)
				{
					//EMERGENCY_LOG_MATH("j = ", j, ".");
					size_t i = 0;
					auto swap = true;
					while (swap)
					{
						if (2 * i + 1 <= j && NeedSwapping(vectors[2 * i + 1], vectors[2 * i], sortingParameters))
						{
							if (NeedSwapping(vectors[2 * i + 1], vectors[i], sortingParameters))
							{
								//ERROR_LOG_MATH("1) Swapping elements at positions: ", i, " and ", 2 * i + 1, ".");
								Swap(vectors[i], vectors[2 * i + 1]);
								i = 2 * i + 1;
							}
							else
							{
								swap = false;
							}
						}
						else if (2 * i <= j && NeedSwapping(vectors[2 * i], vectors[i], sortingParameters))
						{
							//ERROR_LOG_MATH("2) Swapping elements at positions: ", i, " and ", 2 * i, ".");
							Swap(vectors[i], vectors[2 * i]);
							i = 2 * i;
						}
						else
						{
							swap = false;
						}
					}
					//ERROR_LOG_MATH("3) Swapping elements at positions: ", 0, " and ", j, ".");
					Swap(vectors[0], vectors[j]);
				}
			}
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
			/* ==================== Non-static member variables end ==================== */
		}; /* end class HeapSort */

		/// <summary>
		/// The object representation of the quick sort algorithm.
		/// </summary>
		/// <remarks>
		/// See https://en.wikipedia.org/wiki/Quick_sort for details.
		/// </remarks>
		class QuickSort : public ISort
		{
			/* ==================== Static variables and functions begin ==================== */
			/* ==================== Static variables and functions end ==================== */

			/* ==================== Constructors and destructors begin ==================== */
		public:
			/// <summary> Quick sort object constructor. </summary>
			QuickSort();

			/// <summary> Quick sort object destructor. </summary>
			virtual ~QuickSort();

			/// <summary> Quick sort object copy constructor. </summary>
			/// <param name="quickSort"> The quick sort object to copy construct from. </param>
			QuickSort(const QuickSort& quickSort) = delete;

			/// <summary> Quick sort object move constructor. </summary>
			/// <param name="quickSort"> The r-value reference of the quick sort object to move construct from. </param>
			QuickSort(QuickSort&& quickSort) = delete;

			/// <summary> Quick sort object copy assignment operator. </summary>
			/// <param name="quickSort"> The quick sort object to copy assign from. </param>
			/// <returns> The reference to the newly copy-assigned quick sort object. </returns>
			QuickSort& operator=(const QuickSort& quickSort) = delete;

			/// <summary> Quick sort object move assignment operator. </summary>
			/// <param name="quickSort"> The r-value reference of the quick sort object to move assign from. </param>
			/// <returns> The reference to the newly move-assigned quick sort object. </returns>
			QuickSort& operator=(QuickSort&& quickSort) = delete;
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
			void Sort(int* values, size_t valuesCount, orders::Order sortingOrder = orders::ASCENDING) override;
			void Sort(Real* values, size_t valuesCount, orders::Order sortingOrder = orders::ASCENDING) override;
			void Sort(Vector2D* vectors, size_t vectorSize, keys::Key sortingKey = keys::COMPONENT_X, orders::Order sortingOrder = orders::ASCENDING) override;
			void Sort(Vector3D* vectors, size_t vectorSize, keys::Key sortingKey = keys::COMPONENT_X, orders::Order sortingOrder = orders::ASCENDING) override;
			void Sort(Vector2D* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters) override;
			void Sort(Vector3D* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters) override;
		private:
			template <typename T>
			void Sort(T* vectors, size_t vectorSize, keys::Key sortingKey, orders::Order sortingOrder)
			{
				Sort(vectors, vectorSize, SortingParametersChain(sortingKey, sortingOrder));
			}

			template <typename T>
			void Sort(T* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters)
			{
				CHECK_CONDITION_RETURN_VOID_MATH(vectors != nullptr, utility::logging::EMERGENCY,
					"Cannot sort the table. The specified table is nullptr");
				Sort<T>(vectors, 0, vectorSize - 1, sortingParameters);
			}

			template <typename T>
			void Sort(T* vectors, size_t left, size_t right, const SortingParametersChain& sortingParameters)
			{
				DELOCUST_LOG_MATH("Quick sort started for the range of indices: [", left, "; ", right, "].");
				CHECK_CONDITION_RETURN_VOID_ALWAYS_MATH(left < right, utility::logging::WARNING,
					"Calling quick sort method with incorrect values for left and right indices (", left, ", ", right, " respectively).");

				const auto pivotIndex = left;
				T pivot = vectors[pivotIndex];
				Swap(vectors[pivotIndex], vectors[right]);

				auto index = left;
				for (auto i = left; i < right; ++i)
				{
					if (NeedSwapping(pivot, vectors[i], sortingParameters))
					{
						Swap(vectors[i], vectors[index]);
						++index;
					}
				}
				Swap(vectors[index], vectors[right]);

				if (index > left + 1) // "if (index - 1 > left)" could fail when index == 0, because of size_t not handling negative values correctly.
				{
					Sort(vectors, left, index - 1, sortingParameters);
				}
				if (right > index + 1)
				{
					Sort(vectors, index + 1, right, sortingParameters);
				}
			}
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
			/* ==================== Non-static member variables end ==================== */
		}; /* end class QuickSort */

		/// <summary>
		/// The object representation of the shell sort algorithm.
		/// </summary>
		/// <remarks>
		/// See https://en.wikipedia.org/wiki/Shell_sort for details.
		/// </remarks>
		class ShellSort : public ISort
		{
			/* ==================== Static variables and functions begin ==================== */
			/* ==================== Static variables and functions end ==================== */

			/* ==================== Constructors and destructors begin ==================== */
		public:
			/// <summary> Shell sort object constructor. </summary>
			ShellSort();

			/// <summary> Shell sort object destructor. </summary>
			virtual ~ShellSort();

			/// <summary> Shell sort object copy constructor. </summary>
			/// <param name="quickSort"> The shell sort object to copy construct from. </param>
			ShellSort(const ShellSort& shellSort) = delete;

			/// <summary> Shell sort object move constructor. </summary>
			/// <param name="quickSort"> The r-value reference of the shell sort object to move construct from. </param>
			ShellSort(ShellSort&& shellSort) = delete;

			/// <summary> Shell sort object copy assignment operator. </summary>
			/// <param name="quickSort"> The shell sort object to copy assign from. </param>
			/// <returns> The reference to the newly copy-assigned shell sort object. </returns>
			ShellSort& operator=(const ShellSort& shellSort) = delete;

			/// <summary> Shell sort object move assignment operator. </summary>
			/// <param name="quickSort"> The r-value reference of the shell sort object to move assign from. </param>
			/// <returns> The reference to the newly move-assigned shell sort object. </returns>
			ShellSort& operator=(ShellSort&& shellSort) = delete;
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
			void Sort(int* values, size_t valuesCount, orders::Order sortingOrder = orders::ASCENDING) override;
			void Sort(Real* values, size_t valuesCount, orders::Order sortingOrder = orders::ASCENDING) override;
			void Sort(Vector2D* vectors, size_t vectorSize, keys::Key sortingKey = keys::COMPONENT_X, orders::Order sortingOrder = orders::ASCENDING) override;
			void Sort(Vector3D* vectors, size_t vectorSize, keys::Key sortingKey = keys::COMPONENT_X, orders::Order sortingOrder = orders::ASCENDING) override;
			void Sort(Vector2D* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters) override;
			void Sort(Vector3D* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters) override;
		private:
			template <typename T>
			void Sort(T* vectors, size_t vectorSize, keys::Key sortingKey, orders::Order sortingOrder)
			{
				Sort(vectors, vectorSize, SortingParametersChain(sortingKey, sortingOrder));
			}

			template <typename T>
			void Sort(T* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters)
			{
				CHECK_CONDITION_RETURN_VOID_MATH(vectors != nullptr, utility::logging::EMERGENCY,
					"Cannot sort the table using the insertion sort algorithm. The specified table is nullptr");
				const auto quarterVectorSize = vectorSize / 4;
				const auto firstConstIndex = 1;
				const auto lastConstIndex = 701;
				const auto recursiveIndexStep = 2.25f;
				int constIndices[] = { firstConstIndex, 4, 10, 23, 57, 132, 301, lastConstIndex };
				std::vector<size_t> indices(constIndices, constIndices + sizeof constIndices / sizeof(int));
				if (indices.back() >= quarterVectorSize)
				{
					do
					{
						indices.pop_back();
					} while (indices.back() >= quarterVectorSize);
				}
				else
				{
					auto nextIndex = Floor(static_cast<Real>(lastConstIndex) * recursiveIndexStep);
					while (nextIndex < quarterVectorSize)
					{
						indices.push_back(nextIndex);
						nextIndex = Floor(static_cast<Real>(nextIndex) * recursiveIndexStep);
					}
				}
				CHECK_CONDITION_MATH(indices.back() < QUARTER_VECTOR_SIZE, utility::logging::ERR,
					"Incorrect calculation of the initial gap value for the shell sort algorithm. The vector size = ", vectorSize, " and the gap = ", indices.back());
				CHECK_CONDITION_MATH(indices.front() == FIRST_CONST_INDEX, utility::logging::ERR,
					"Incorrect calculation of the last gap value for the shell sort algorithm. The last gap value must be equal to ", FIRST_CONST_INDEX, ", but is equal to ", indices.front());

				while (!indices.empty())
				{
					auto gap = indices.back();
					for (auto i = gap; i < vectorSize; ++i)
					{
						T temp(vectors[i]);
						auto j = i;
						for (; j >= gap && NeedSwapping(vectors[j - gap], temp, sortingParameters); j -= gap)
						{
							vectors[j] = vectors[j - gap];
						}
						vectors[j] = temp;
					}
					//DELOCUST_LOG_MATH("Popping back from the indices collection.");
					indices.pop_back();
				}
			}
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
			/* ==================== Non-static member variables end ==================== */
		}; /* end class ShellSort */

		/// <summary>
		/// The object representation of the comb sort algorithm.
		/// </summary>
		/// <remarks>
		/// See https://en.wikipedia.org/wiki/Comb_sort for details.
		/// </remarks>
		class CombSort : public ISort
		{
			/* ==================== Static variables and functions begin ==================== */
			/* ==================== Static variables and functions end ==================== */

			/* ==================== Constructors and destructors begin ==================== */
		public:
			/// <summary> Comb sort object constructor. </summary>
			CombSort();

			/// <summary> Comb sort object destructor. </summary>
			virtual ~CombSort();

			/// <summary> Comb sort object copy constructor. </summary>
			/// <param name="combSort"> The comb sort object to copy construct from. </param>
			CombSort(const CombSort& combSort) = delete;

			/// <summary> Comb sort object move constructor. </summary>
			/// <param name="combSort"> The r-value reference of the comb sort object to move construct from. </param>
			CombSort(CombSort&& combSort) = delete;

			/// <summary> Comb sort object copy assignment operator. </summary>
			/// <param name="combSort"> The comb sort object to copy assign from. </param>
			/// <returns> The reference to the newly copy-assigned comb sort object. </returns>
			CombSort& operator=(const CombSort& combSort) = delete;

			/// <summary> Comb sort object move assignment operator. </summary>
			/// <param name="combSort"> The r-value reference of the comb sort object to move assign from. </param>
			/// <returns> The reference to the newly move-assigned comb sort object. </returns>
			CombSort& operator=(CombSort&& combSort) = delete;
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
			void Sort(int* values, size_t valuesCount, orders::Order sortingOrder = orders::ASCENDING) override;
			void Sort(Real* values, size_t valuesCount, orders::Order sortingOrder = orders::ASCENDING) override;
			void Sort(Vector2D* vectors, size_t vectorSize, keys::Key sortingKey = keys::COMPONENT_X, orders::Order sortingOrder = orders::ASCENDING) override;
			void Sort(Vector3D* vectors, size_t vectorSize, keys::Key sortingKey = keys::COMPONENT_X, orders::Order sortingOrder = orders::ASCENDING) override;
			void Sort(Vector2D* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters) override;
			void Sort(Vector3D* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters) override;
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
		private:
			template <typename T>
			void Sort(T* vectors, size_t vectorSize, keys::Key sortingKey, orders::Order sortingOrder)
			{
				Sort(vectors, vectorSize, SortingParametersChain(sortingKey, sortingOrder));
			}

			template <typename T>
			void Sort(T* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters)
			{
				CHECK_CONDITION_RETURN_VOID_MATH(vectors != nullptr, utility::logging::EMERGENCY,
					"Cannot sort the table. The specified table is nullptr");

				auto gap = vectorSize;

				// TODO: replace with some predefined const from the configuration file.
				// This value is set by the rule of thumb (see http://en.wikipedia.org/wiki/Comb_sort)
				constexpr auto shrink = 1.3f;

				auto swapOccured = false;
				while (gap != 1 || swapOccured)
				{
					gap = static_cast<int>(gap / shrink);
					if (gap < 1)
					{
						gap = 1;
					}

					swapOccured = false;
					for (auto i = 0; i + gap < vectorSize; ++i)
					{
						if (NeedSwapping(vectors[i], vectors[i + gap], sortingParameters))
						{
							Swap(vectors[i], vectors[i + gap]);
							swapOccured = true;
						}
					}
				}
			}
			/* ==================== Non-static member variables end ==================== */
		}; /* end class CombSort */

		/// <summary>
		/// The object representation of the counting sort algorithm. Counting sort is only applicable for sorting integer values.
		/// </summary>
		/// <remarks>
		/// See https://en.wikipedia.org/wiki/Counting_sort for details.
		/// </remarks>
		class CountingSort : public ISort
		{
			/* ==================== Static variables and functions begin ==================== */
			/* ==================== Static variables and functions end ==================== */

			/* ==================== Constructors and destructors begin ==================== */
		public:
			/// <summary> Counting sort object constructor. </summary>
			CountingSort();

			/// <summary> Counting sort object destructor. </summary>
			virtual ~CountingSort();

			/// <summary> Counting sort object copy constructor. </summary>
			/// <param name="countingSort"> The counting sort object to copy construct from. </param>
			CountingSort(const CountingSort& countingSort) = delete;

			/// <summary> Counting sort object move constructor. </summary>
			/// <param name="countingSort"> The r-value reference of the counting sort object to move construct from. </param>
			CountingSort(CountingSort&& countingSort) = delete;

			/// <summary> Counting sort object copy assignment operator. </summary>
			/// <param name="countingSort"> The counting sort object to copy assign from. </param>
			/// <returns> The reference to the newly copy-assigned counting sort object. </returns>
			CountingSort& operator=(const CountingSort& countingSort) = delete;

			/// <summary> Counting sort object move assignment operator. </summary>
			/// <param name="countingSort"> The r-value reference of the counting sort object to move assign from. </param>
			/// <returns> The reference to the newly move-assigned counting sort object. </returns>
			CountingSort& operator=(CountingSort&& countingSort) = delete;
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
			void Sort(int* values, size_t valuesCount, orders::Order sortingOrder = orders::ASCENDING) override;
			void Sort(Real* values, size_t valuesCount, orders::Order sortingOrder = orders::ASCENDING) override;
			void Sort(Vector2D* vectors, size_t vectorSize, keys::Key sortingKey = keys::COMPONENT_X, orders::Order sortingOrder = orders::ASCENDING) override;
			void Sort(Vector3D* vectors, size_t vectorSize, keys::Key sortingKey = keys::COMPONENT_X, orders::Order sortingOrder = orders::ASCENDING) override;
			void Sort(Vector2D* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters) override;
			void Sort(Vector3D* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters) override;
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
			/* ==================== Non-static member variables end ==================== */
		}; /* end class CountingSort */

		/// <summary>
		/// The object representation of the radix sort algorithm. Radix sort is only applicable for sorting integer values.
		/// </summary>
		/// <remarks>
		/// See https://en.wikipedia.org/wiki/Radix_sort for details.
		/// </remarks>
		class RadixSort : public ISort
		{
			/* ==================== Static variables and functions begin ==================== */
			/* ==================== Static variables and functions end ==================== */

			/* ==================== Constructors and destructors begin ==================== */
		public:
			/// <summary> Radix sort object constructor. </summary>
			RadixSort();

			/// <summary> Radix sort object destructor. </summary>
			virtual ~RadixSort();

			/// <summary> Radix sort object copy constructor. </summary>
			/// <param name="radixSort"> The radix sort object to copy construct from. </param>
			RadixSort(const RadixSort& radixSort) = delete;

			/// <summary> Radix sort object move constructor. </summary>
			/// <param name="radixSort"> The r-value reference of the radix sort object to move construct from. </param>
			RadixSort(RadixSort&& radixSort) = delete;

			/// <summary> Radix sort object copy assignment operator. </summary>
			/// <param name="radixSort"> The radix sort object to copy assign from. </param>
			/// <returns> The reference to the newly copy-assigned radix sort object. </returns>
			RadixSort& operator=(const RadixSort& radixSort) = delete;

			/// <summary> Radix sort object move assignment operator. </summary>
			/// <param name="radixSort"> The r-value reference of the radix sort object to move assign from. </param>
			/// <returns> The reference to the newly move-assigned radix sort object. </returns>
			RadixSort& operator=(RadixSort&& radixSort) = delete;
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
			void Sort(int* values, size_t valuesCount, orders::Order sortingOrder = orders::ASCENDING) override;
			void Sort(Real* values, size_t valuesCount, orders::Order sortingOrder = orders::ASCENDING) override;
			void Sort(Vector2D* vectors, size_t vectorSize, keys::Key sortingKey = keys::COMPONENT_X, orders::Order sortingOrder = orders::ASCENDING) override;
			void Sort(Vector3D* vectors, size_t vectorSize, keys::Key sortingKey = keys::COMPONENT_X, orders::Order sortingOrder = orders::ASCENDING) override;
			void Sort(Vector2D* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters) override;
			void Sort(Vector3D* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters) override;
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
			/* ==================== Non-static member variables end ==================== */
		}; /* end class RadixSort */

		/// <summary>
		/// The object representation of the bucket sort algorithm.
		/// </summary>
		/// <remarks>
		/// See https://en.wikipedia.org/wiki/Bucket_sort for details.
		/// </remarks>
		class BucketSort : public ISort
		{
			template <typename T>
			class Bucket
			{
			public:
				Bucket() : m_lowBound(REAL_MIN), m_highBound(REAL_MAX)
				{
				}
				Bucket(Real lowBound, Real highBound) : m_lowBound(lowBound), m_highBound(highBound)
				{
				}
				~Bucket()
				{
				}

				/// <summary> Bucket copy constructor. </summary>
				/// <param name="bucket"> Bucket to copy construct from. </param>
				Bucket(const Bucket& bucket) = delete;

				/// <summary> Bucket move constructor. </summary>
				/// <param name="bucket"> Bucket to move construct from. </param>
				Bucket(Bucket&& bucket) = default;

				/// <summary> Bucket copy assignment operator. </summary>
				/// <param name="bucket"> Bucket to copy assign from. </param>
				/// <returns> The reference to the newly copy-assigned bucket. </returns>
				Bucket& operator=(const Bucket& bucket) = delete;

				/// <summary> Bucket move assignment operator. </summary>
				/// <param name="bucket"> Bucket to move assign from. </param>
				/// <returns> The reference to the newly move-assigned bucket. </returns>
				Bucket& operator=(Bucket&& bucket) = delete;


				bool IsEmpty() const { return m_vectors.empty(); }
				Real GetLowBound() const { return m_lowBound; }
				Real GetHighBound() const { return m_highBound; }
				void SetLowBound(Real lowBound) { m_lowBound = lowBound; }
				void SetHighBound(Real highBound) { m_highBound = highBound; }
				void PushVector(const T& vector) { m_vectors.push_back(vector); }
				T* GetVectors() { return &m_vectors[0]; }
				const T& GetVector(int i) const { return m_vectors[i]; }
				size_t GetVectorsSize() const { return m_vectors.size(); }
			private:
				std::vector<T> m_vectors;
				Real m_lowBound; // TODO: Remove this member variable in the future. It is not used anywhere
				Real m_highBound; // TODO: Remove this member variable in the future. It is not used anywhere.
			};
			/* ==================== Static variables and functions begin ==================== */
			/* ==================== Static variables and functions end ==================== */

			/* ==================== Constructors and destructors begin ==================== */
		public:
			/// <summary> Bucket sort object constructor. </summary>
			BucketSort();

			/// <summary> Bucket sort object destructor. </summary>
			virtual ~BucketSort();

			/// <summary> Bucket sort object copy constructor. </summary>
			/// <param name="bucketSort"> The bucket sort object to copy construct from. </param>
			BucketSort(const BucketSort& bucketSort) = delete;

			/// <summary> Bucket sort object move constructor. </summary>
			/// <param name="bucketSort"> The r-value reference of the bucket sort object to move construct from. </param>
			BucketSort(BucketSort&& bucketSort) = delete;

			/// <summary> Bucket sort object copy assignment operator. </summary>
			/// <param name="bucketSort"> The bucket sort object to copy assign from. </param>
			/// <returns> The reference to the newly copy-assigned bucket sort object. </returns>
			BucketSort& operator=(const BucketSort& bucketSort) = delete;

			/// <summary> Bucket sort object move assignment operator. </summary>
			/// <param name="bucketSort"> The r-value reference of the bucket sort object to move assign from. </param>
			/// <returns> The reference to the newly move-assigned bucket sort object. </returns>
			BucketSort& operator=(BucketSort&& bucketSort) = delete;
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
			void Sort(int* values, size_t valuesCount, orders::Order sortingOrder = orders::ASCENDING) override;
			void Sort(Real* values, size_t valuesCount, orders::Order sortingOrder = orders::ASCENDING) override;
			void Sort(Vector2D* vectors, size_t vectorSize, keys::Key sortingKey = keys::COMPONENT_X, orders::Order sortingOrder = orders::ASCENDING) override;
			void Sort(Vector3D* vectors, size_t vectorSize, keys::Key sortingKey = keys::COMPONENT_X, orders::Order sortingOrder = orders::ASCENDING) override;
			void Sort(Vector2D* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters) override;
			void Sort(Vector3D* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters) override;
		private:
			void FindMinMax(Vector2D* vectors, size_t vectorSize, keys::Key sortingKey, Real& minValue, Real& maxValue) const
			{
				CHECK_CONDITION_RETURN_VOID_MATH(vectors != nullptr, utility::logging::EMERGENCY,
					"Cannot sort the table. The specified table is nullptr");

				minValue = REAL_MAX;
				maxValue = REAL_MIN;
				for (auto i = 0; i < vectorSize; ++i)
				{
					auto value = CollectValueByKey(vectors[i], sortingKey);
					if (minValue > value)
					{
						minValue = value;
					}
					else if (maxValue < value)
					{
						maxValue = value;
					}
				}
				CHECK_CONDITION_MATH(!(maxValue < minValue), utility::logging::ERR, "Incorrect values for min and max keys. The minimum = ", minValue, " and the maximum = ", maxValue);
			}

			void FindMinMax(Vector3D* vectors, size_t vectorSize, keys::Key sortingKey, Real& minValue, Real& maxValue) const
			{
				CHECK_CONDITION_RETURN_VOID_MATH(vectors != nullptr, utility::logging::EMERGENCY,
					"Cannot sort the table. The specified table is nullptr");

				minValue = REAL_MAX;
				maxValue = REAL_MIN;
				for (auto i = 0; i < vectorSize; ++i)
				{
					auto value = CollectValueByKey(vectors[i], sortingKey);
					if (minValue > value)
					{
						minValue = value;
					}
					else if (maxValue < value)
					{
						maxValue = value;
					}
				}
				CHECK_CONDITION_MATH(!(maxValue < minValue), utility::logging::ERR, "Incorrect values for min and max keys. The minimum = ", minValue, " and the maximum = ", maxValue);
			}

			template <typename T>
			void Sort(T* vectors, size_t vectorSize, keys::Key sortingKey, orders::Order sortingOrder)
			{
				Real minValue, maxValue;
				FindMinMax(vectors, vectorSize, sortingKey, minValue, maxValue);
				//DEBUG_LOG_MATH("minValue = ", minValue, "; maxValue = ", maxValue);

				//const int NUMBER_OF_BUCKETS = ((vectorSize / 20) < 2) ? 2 : vectorSize / 20; // estimated by myself
				const auto bucketsCount = static_cast<int>(sqrt(static_cast<Real>(vectorSize)));
				const auto bucketWidth = static_cast<Real>((maxValue - minValue) / bucketsCount);
				std::vector<Bucket<T>> buckets;
				buckets.reserve(bucketsCount);
				auto bucketLowBound = minValue;
				for (auto i = 0; i < bucketsCount; ++i)
				{
					buckets.emplace_back(bucketLowBound, bucketLowBound + bucketWidth);
					//DEBUG_LOG_MATH("Bucket[", i, "] takes range [", bucketLowBound, "; ", bucketLowBound + bucketWidth, ")");
					bucketLowBound += bucketWidth;
				}

				for (auto i = 0; i < vectorSize; ++i)
				{
					Real value = CollectValueByKey(vectors[i], sortingKey);
					//DEBUG_LOG_MATH("vectors[", i, "] = ", vectors[i].ToString(), " and the value = ", value);

					// Calculate the index of the bucket to which we will add the vector
					auto bucketIndex = static_cast<int>(bucketsCount * ((value - minValue) / (maxValue - minValue))); // TODO: Is it possible for minValue == maxValue? If so, then we have a division by 0 problem.
					if (bucketIndex == bucketsCount)
					{
						--bucketIndex;
					}

					CHECK_CONDITION_MATH((bucketIndex >= 0) && (bucketIndex < NUMBER_OF_BUCKETS), utility::logging::CRITICAL, "Miscalculated bucket index. Bucket index must be within range [0; ", NUMBER_OF_BUCKETS, "), but is equal to ", bucketIndex);
					buckets[bucketIndex].PushVector(vectors[i]);
				}
				auto insertionSorter = GetSortingObject(sorting_algorithms::INSERTION_SORT);
				for (auto i = 0; i < bucketsCount; ++i)
				{
					//for (int j = 0; j < buckets[i].GetVectorsSize(); ++j)
					//{
					//	DEBUG_LOG_MATH("buckets[", i, "][", j, "] = ", buckets[i].GetVector(j).ToString());
					//}
					if (!buckets[i].IsEmpty())
					{
						insertionSorter->Sort(buckets[i].GetVectors(), buckets[i].GetVectorsSize(), sortingKey, sortingOrder);
					}
				}

				//for (int i = 0; i < NUMBER_OF_BUCKETS; ++i)
				//{
				//	for (int j = 0; j < buckets[i].GetVectorsSize(); ++j)
				//	{
				//		DEBUG_LOG_MATH("buckets[", i, "][", j, "] = ", buckets[i].GetVector(j).ToString());
				//	}
				//}

				auto index = 0;
				switch (sortingOrder)
				{
				case orders::ASCENDING:
					for (auto i = 0; i < bucketsCount; ++i)
					{
						for (auto j = 0; j < buckets[i].GetVectorsSize(); ++j, ++index)
						{
							vectors[index] = buckets[i].GetVector(j);
						}
					}
					break;
				case orders::DESCENDING:
					for (auto i = bucketsCount - 1; i >= 0; --i)
					{
						for (auto j = 0; j < buckets[i].GetVectorsSize(); ++j, ++index)
						{
							vectors[index] = buckets[i].GetVector(j);
						}
					}
					break;
				default:
					ERROR_LOG_MATH("Unknown sorting direction value (", sortingOrder, "). Only ASCENDING (", orders::ASCENDING,
						") and DESCENDING (", orders::DESCENDING, ") are acceptable.");
					break;
				}
			}

			template <typename T>
			void Sort(T* vectors, size_t vectorSize, const SortingParametersChain& sortingParameters)
			{
				Real minValue, maxValue;
				FindMinMax(vectors, vectorSize, sortingParameters.GetKey(), minValue, maxValue);
				//DEBUG_LOG_MATH("minValue = ", minValue, "; maxValue = ", maxValue);

				//const int NUMBER_OF_BUCKETS = ((vectorSize / 20) < 2) ? 2 : vectorSize / 20; // estimated by myself
				const auto bucketsCount = static_cast<int>(sqrt(static_cast<Real>(vectorSize)));
				const auto bucketWidth = static_cast<Real>((maxValue - minValue) / bucketsCount);
				std::vector<Bucket<T>> buckets;
				buckets.reserve(bucketsCount);
				auto bucketLowBound = minValue;
				for (auto i = 0; i < bucketsCount; ++i)
				{
					buckets.emplace_back(bucketLowBound, bucketLowBound + bucketWidth);
					//DEBUG_LOG_MATH("Bucket[", i, "] takes range [", bucketLowBound, "; ", bucketLowBound + bucketWidth, ")");
					bucketLowBound += bucketWidth;
				}

				for (auto i = 0; i < vectorSize; ++i)
				{
					auto value = CollectValueByKey(vectors[i], sortingParameters.GetKey());
					//DEBUG_LOG_MATH("vectors[", i, "] = ", vectors[i].ToString(), " and the value = ", value);

					// Calculate the index of the bucket to which we will add the vector
					auto bucketIndex = static_cast<int>(bucketsCount * ((value - minValue) / (maxValue - minValue))); // TODO: Is it possible for minValue == maxValue? If so, then we have a division by 0 problem.
					if (bucketIndex == bucketsCount)
					{
						--bucketIndex;
					}

					CHECK_CONDITION_MATH((bucketIndex >= 0) && (bucketIndex < NUMBER_OF_BUCKETS), utility::logging::ERR, "Miscalculated bucket index. Bucket index must be within range [0; ", NUMBER_OF_BUCKETS, "), but is equal to ", bucketIndex);
					buckets[bucketIndex].PushVector(vectors[i]);
				}
				auto insertionSorter = GetSortingObject(sorting_algorithms::INSERTION_SORT);
				for (auto i = 0; i < bucketsCount; ++i)
				{
					//for (int j = 0; j < buckets[i].GetVectorsSize(); ++j)
					//{
					//	DEBUG_LOG_MATH("buckets[", i, "][", j, "] = ", buckets[i].GetVector(j).ToString());
					//}
					if (!buckets[i].IsEmpty())
					{
						insertionSorter->Sort(buckets[i].GetVectors(), buckets[i].GetVectorsSize(), sortingParameters);
					}
				}

				//for (int i = 0; i < NUMBER_OF_BUCKETS; ++i)
				//{
				//	for (int j = 0; j < buckets[i].GetVectorsSize(); ++j)
				//	{
				//		DEBUG_LOG_MATH("buckets[", i, "][", j, "] = ", buckets[i].GetVector(j).ToString());
				//	}
				//}

				auto index = 0;
				switch (sortingParameters.GetOrder())
				{
				case orders::ASCENDING:
					for (auto i = 0; i < bucketsCount; ++i)
					{
						for (auto j = 0; j < buckets[i].GetVectorsSize(); ++j, ++index)
						{
							vectors[index] = buckets[i].GetVector(j);
						}
					}
					break;
				case orders::DESCENDING:
					for (auto i = bucketsCount - 1; i >= 0; --i)
					{
						for (auto j = 0; j < buckets[i].GetVectorsSize(); ++j, ++index)
						{
							vectors[index] = buckets[i].GetVector(j);
						}
					}
					break;
				default:
					ERROR_LOG_MATH("Unknown sorting order value (", sortingParameters.GetOrder(), "). Only ASCENDING (",
						orders::ASCENDING, ") and DESCENDING (", orders::DESCENDING, ") are acceptable.");
					break;
				}
			}
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
			/* ==================== Non-static member variables end ==================== */
		}; /* end class BucketSort */

	} /* end namespace Sorting */

} /* end namespace Math */

#endif /* __MATH_SORT_H__ */