#ifndef __UTILITY_BUILDER_H__
#define __UTILITY_BUILDER_H__

#include "Utility.h"

#include <memory>

namespace Utility {

	template <class T>
	class Builder
	{
		/* ==================== Static variables and functions begin ==================== */
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		/// <summary> Builder constructor. </summary>
		/// <param name="object"> The object that is already created, but not yet initialized and
		/// which will be the output result of the whole building operation. </param>
		Builder(T* object) :
			m_object(object)
		{
		}
		/// <summary> Builder destructor. </summary>
		virtual ~Builder(void)
		{
		}
		/// <summary> Builder copy constructor. </summary>
		Builder(Builder& builder) = delete;
		/// <summary> Builder move constructor. </summary>
		Builder(Builder&& builder) = delete;
		/// <summary> Builder copy assignment operator. </summary>
		Builder& operator=(const Builder& builder) = delete;
		/// <summary> Builder move assignment operator. </summary>
		Builder& operator=(Builder&& builder) = delete;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		virtual void BuildPart1()
		{
		}
		virtual void BuildPart2()
		{
		}
		virtual void BuildPart3()
		{
		}

		/// <summary> Returns the properly initialized object. </summary>
		/// <returns> The already initialized object. </returns>
		T* Get()
		{
			return m_object;
		}
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	protected:
		T* m_object;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class Builder<T> */

} /* end namespace Utility */

#endif /* __UTILITY_BUILDER_H__ */