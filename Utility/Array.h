#ifndef __UTILITY_CONTAINER_ARRAY_H__
#define __UTILITY_CONTAINER_ARRAY_H__

#include "Utility.h"

namespace Utility {
	namespace Container {

		/// <summary>
		/// The array template class encapsulates dynamically resizable, contiguous storage of objects.
		/// The template parameter class need only implement the default constructor, the copy constructor, and the assignment operator.
		/// </summary>
		template <class T>
		class Array
		{
			/* ==================== Constructors and destructors begin ==================== */
		public:
			/// <summary>
			/// The array constructor allows you to specify the initial quantity of elements in the array (<paramref name="initialSize"/>).
			/// The <paramref name="growSizeByValue"/> represents the number of elements to grow by when the array size is dynamically increased.
			/// The initial size is also the maximum quantity allowed, but dynamic resizing can cause the two numbers to differ.
			/// </summary>
			explicit Array(int initialSize = 1, int growSizeByValue = 1);
			~Array();
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
		public:
			int GetSize() const;
			T* GetArray();
			const T* GetArray() const;
			T& operator[](int i);
			const T& operator[](int i) const;

			/// <summary>
			/// The method inserts the given <paramref name="dataElement"/> to the array at index Q (where Q is the size of the array). After the insertion, Q is incremented by 1.
			/// </summary>
			void Append(const T& dataElement);

			/// <summary>
			/// The method allows to insert a new element at any index larger than Q (where Q is the size of the array).
			/// After the insertion, the size of the array is increased so that the array is just large enough to include the new data element.
			/// If the input index is already in the current range of valid indices, no resizing is performed and the current element at that index is overwritten.
			/// </summary>
			void SetElement(int i, const T& dataElement);

			/// <summary>
			/// Deletes the <code>i</code>-th element by shifting the elements at larger indices to fill the vacant slots.
			/// That is, the element at index <code>i+1</code> is copied to the location of <code>i</code>, the element at index <code>i+2</code> is copied to the location <code>i+1</code>, and so forth.
			/// Assuming Q is the current size of the array the element at index (Q-1) is copied to the location (Q-2).
			/// Because construction and destruction can have side effects, the default constructor is used to generate a dummy object that is stored at location (Q-1),
			/// even though the current size of the array will be decremented so as not to include the vacated slot. For example, if the template parameter class is a graphics engine <code>Object</code>
			/// that holds onto other objects or has dynamically allocated memory, this last step allows the object to free up its resources.
			/// </summary>	
			void Remove(int i);

			/// <summary>
			/// Sets all objects in the valid index range to default constructed objects, and then sets the current size to 0.
			/// </summary>
			void RemoveAll();

			/// <summary>
			/// If the given <paramref name="maxSize"/> is zero, the array is deallocated and the quantity set to zero.
			/// If the given <paramref name="maxSize"/> is equal to the current maximum size, nothing needs to be done and the function just returns.
			/// If the given <paramref name="maxSize"/> is different than current maximum size, the array is reallocated.
			/// In the event the array grows, the <paramref name="copy"/> parameter specified whether or not the old array items should be copied to the new array.
			/// </summary>
			void SetMaxSize(int maxSize, bool copy);
			int GetMaxSize() const;
			void SetGrowSizeByValue(int growSizeByValue);
			int GetGrowSizeByValue() const;
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
		private:
			T* m_data;
			int m_size;
			int m_maxSize;
			int m_growSizeByValue;
			// TODO: Compiler warning C4820: '4' bytes padding added after data member 'm_growSizeByValue'.
			/* ==================== Non-static member variables end ==================== */
		}; /* end class Array */

		//#ifndef IN_
		/* extern */ template class UTILITY_API Array<int>;
		/* extern */ template class UTILITY_API Array<float>;
		//#endif

	} /* end namespace Container */

} /* end namespace Utility */

#endif /* __UTILITY_CONTAINER_ARRAY_H__ */