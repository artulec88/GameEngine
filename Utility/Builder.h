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
		Builder()
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

		T* Get() { return m_object; }
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	protected:
		//std::unique_ptr<T> m_object;
		T* m_object;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class Builder<T> */

} /* end namespace Utility */

#endif /* __UTILITY_BUILDER_H__ */