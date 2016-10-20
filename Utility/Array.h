#ifndef __UTILITY_CONTAINER_ARRAY_H__
#define __UTILITY_CONTAINER_ARRAY_H__

#include "Utility.h"

namespace Utility {
	namespace Container {

		/// <summary>
		/// The array template class encapsulates contiguous storage of objects.
		/// The template parameter class need only implement the default constructor, the copy constructor, and the copy assignment operator.
		/// </summary>
		template <class T, unsigned int SIZE>
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
			/// <param name="arr"> Array to copy construct from. </summary>
			Array(const Array& arr) = default;

			/// <summary> Array move constructor. </summary>
			/// <param name="arr"> Array to move construct from. </summary>
			Array(Array&& arr) = default;

			/// <summary> Array copy assignment operator. </summary>
			/// <param name="arr"> Array to copy assign from. </summary>
			Array& operator=(const Array& arr) = default;

			/// <summary> Array move assignment operator. </summary>
			/// <param name="arr"> Array to move assign from. </summary>
			Array& operator=(Array&& arr) = default;
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
		public:
			unsigned int GetSize() const
			{
				return static_cast<unsigned int>(m_data.size());
			}
			T* GetData() { return m_data.data(); }
			const T* GetData() const { return m_data.data(); }
			T& operator[](unsigned int i);
			const T& operator[](unsigned int i) const;

			/// <summary>
			/// The method allows to insert a new element at any index larger than Q (where Q is the size of the array).
			/// After the insertion, the size of the array is increased so that the array is just large enough to include the new data element.
			/// If the input index is already in the current range of valid indices, no resizing is performed and the current element at that index is overwritten.
			/// </summary>
			void SetElement(unsigned int i, const T& dataElement);
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
		private:
			std::array<T, SIZE> m_data;
			/* ==================== Non-static member variables end ==================== */
		}; /* end class Array */

	} /* end namespace Container */

} /* end namespace Utility */

#endif /* __UTILITY_CONTAINER_ARRAY_H__ */