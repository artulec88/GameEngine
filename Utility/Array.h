#ifndef UTILITY_CONTAINER_ARRAY_H
#define UTILITY_CONTAINER_ARRAY_H

#include "Utility.h"

namespace utility {
	namespace container {

		/// <summary>
		/// The array template class encapsulates contiguous storage of objects.
		/// The template parameter class need only implement the default constructor, the copy constructor, and the copy assignment operator.
		/// </summary>
		template <class T, unsigned int Size>
		class Array
		{
			/* ==================== Constructors and destructors begin ==================== */
		public:
			/// <summary>
			/// The array constructor.
			/// </summary>
			explicit Array() :
				m_data() // TODO: Is it necessary to do that?
			{
			}

			/// <summary> Array destructor. </summary>
			~Array()
			{
			}

			/// <summary> Array copy constructor. </summary>
			/// <param name="arr"> Array to copy construct from. </param>
			Array(const Array& arr) = default;

			/// <summary> Array move constructor. </summary>
			/// <param name="arr"> Array to move construct from. </param>
			Array(Array&& arr) = default;

			/// <summary> Array copy assignment operator. </summary>
			/// <param name="arr"> Array to copy assign from. </param>
			/// <returns> The reference to the newly copy-assigned array object. </returns>
			Array& operator=(const Array& arr) = default;

			/// <summary> Array move assignment operator. </summary>
			/// <param name="arr"> Array to move assign from. </param>
			/// <returns> The reference to the newly move-assigned array object. </returns>
			Array& operator=(Array&& arr) = default;
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
			/// <summary> Calculates and returns the size of the array. </summary>
			/// <returns> The size of the array. </returns>
			unsigned int GetSize() const
			{
				return static_cast<unsigned int>(m_data.size());
			}

			T* GetData() { return m_data.data(); }
			const T* GetData() const { return m_data.data(); }
			T& operator[](const unsigned int i);
			const T& operator[](const unsigned int i) const;

			/// <summary>
			/// The method allows to insert a new element at any index larger than Q (where Q is the size of the array).
			/// After the insertion, the size of the array is increased so that the array is just large enough to include the new data element.
			/// If the input index is already in the current range of valid indices, no resizing is performed and the current element at that index is overwritten.
			/// </summary>
			void SetElement(unsigned int i, const T& dataElement);
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
		private:
			std::array<T, Size> m_data;
			/* ==================== Non-static member variables end ==================== */
		}; /* end class Array */

	} /* end namespace Container */

} /* end namespace Utility */

#endif /* UTILITY_CONTAINER_ARRAY_H */