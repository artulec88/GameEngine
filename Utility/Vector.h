#ifndef __UTILITY_CONTAINER_VECTOR_H__
#define __UTILITY_CONTAINER_VECTOR_H__

#include "Utility.h"

namespace utility {
	namespace container {

		/// <summary>
		/// The vector template class encapsulates dynamically resizable, contiguous storage of objects.
		/// The template parameter class need only implement the default constructor, the copy constructor, and the assignment operator.
		/// </summary>
		template <class T>
		class Vector
		{
			/* ==================== Constructors and destructors begin ==================== */
		public:
			/// <summary>
			/// The vector constructor allows you to specify the initial quantity of elements in the container (<paramref name="initialSize"/>).
			/// The <paramref name="growSizeByValue"/> represents the number of elements to grow by when the vector size is dynamically increased.
			/// The initial size is also the maximum quantity allowed, but dynamic resizing can cause the two numbers to differ.
			/// </summary>
			explicit Vector(const unsigned int initialSize = 1, const unsigned int growSizeByValue = 1) :
				m_data(nullptr),
				m_size(initialSize),
				m_maxSize(initialSize),
				m_growSizeByValue(growSizeByValue)
			{
				m_data.reset(new T[initialSize]);
			}

			/// <summary> Vector destructor. </summary>
			~Vector()
			{
			}

			/// <summary> Vector copy constructor. </summary>
			/// <param name="vector"> Vector to copy construct from. </param>
			Vector(const Vector& vector) = delete;

			/// <summary> Vector move constructor. </summary>
			/// <param name="vector"> Vector to move construct from. </param>
			Vector(Vector&& vector) = default;

			/// <summary> Vector copy assignment operator. </summary>
			/// <param name="vector"> Vector to copy assign from. </param>
			/// <returns> The reference to the newly copy-assigned vector. </returns>
			Vector& operator=(const Vector& vector) = delete;

			/// <summary> Vector move assignment operator. </summary>
			/// <param name="vector"> Vector to move assign from. </param>
			/// <returns> The reference to the newly move-assigned vector. </returns>
			Vector& operator=(Vector&& vector) = default;
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
			T* GetData() { return &m_data[0]; }
			const T* GetData() const { return &m_data[0]; }
			T& operator[](unsigned int i);
			const T& operator[](unsigned int i) const;

			/// <summary>
			/// The method inserts the given <paramref name="dataElement"/> to the array at index Q (where Q is the size of the array). After the insertion, Q is incremented by 1.
			/// </summary>
			void Append(const T& dataElement);

			/// <summary>
			/// The method allows to insert a new element at any index larger than Q (where Q is the size of the array).
			/// After the insertion, the size of the array is increased so that the array is just large enough to include the new data element.
			/// If the input index is already in the current range of valid indices, no resizing is performed and the current element at that index is overwritten.
			/// </summary>
			void SetElement(unsigned int i, const T& dataElement);

			/// <summary>
			/// Deletes the <code>i</code>-th element by shifting the elements at larger indices to fill the vacant slots.
			/// That is, the element at index <code>i+1</code> is copied to the location of <code>i</code>, the element at index <code>i+2</code> is copied to the location <code>i+1</code>, and so forth.
			/// Assuming Q is the current size of the array the element at index (Q-1) is copied to the location (Q-2).
			/// Because construction and destruction can have side effects, the default constructor is used to generate a dummy object that is stored at location (Q-1),
			/// even though the current size of the array will be decremented so as not to include the vacated slot. For example, if the template parameter class is a graphics engine <code>Object</code>
			/// that holds onto other objects or has dynamically allocated memory, this last step allows the object to free up its resources.
			/// </summary>	
			void Remove(unsigned int i);

			/// <summary>
			/// Sets all objects in the valid index range to default constructed objects, and then sets the current size to 0.
			/// </summary>
			void RemoveAll();

			unsigned int GetSize() const { return m_size; }
			unsigned int GetMaxSize() const { return m_maxSize; }
			/// <summary>
			/// If the given <paramref name="maxSize"/> is zero, the array is deallocated and the quantity set to zero.
			/// If the given <paramref name="maxSize"/> is equal to the current maximum size, nothing needs to be done and the function just returns.
			/// If the given <paramref name="maxSize"/> is different than current maximum size, the array is reallocated.
			/// In the event the array grows, the <paramref name="copy"/> parameter specified whether or not the old array items should be copied to the new array.
			/// </summary>
			void SetMaxSize(const unsigned int maxSize, bool copy);
			unsigned int GetGrowSizeByValue() const { return m_growSizeByValue; }
			void SetGrowSizeByValue(const unsigned int growSizeByValue) { m_growSizeByValue = growSizeByValue; }
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
		private:
			std::unique_ptr<T[]> m_data;
			unsigned int m_size;
			unsigned int m_maxSize;
			unsigned int m_growSizeByValue;
			// TODO: Compiler warning C4820: '4' bytes padding added after data member 'm_growSizeByValue'.
			/* ==================== Non-static member variables end ==================== */
		}; /* end class Vector<T> */

	} /* end namespace Container */

} /* end namespace Utility */

#endif /* __UTILITY_CONTAINER_VECTOR_H__ */