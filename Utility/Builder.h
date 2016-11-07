#ifndef __UTILITY_BUILDER_H__
#define __UTILITY_BUILDER_H__

#include "Utility.h"
#include "BuilderDirector.h"

#include <memory>

namespace Utility {

	template <class T>
	class Builder
	{
		friend class BuilderDirector<T>;
		/* ==================== Static variables and functions begin ==================== */
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		/// <summary> Builder constructor. </summary>
		explicit Builder()
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
	protected:
		virtual T Get() = 0;
		virtual void BuildParts() = 0;
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	protected:
		/* ==================== Non-static member variables end ==================== */
	}; /* end class Builder<T> */

} /* end namespace Utility */

#endif /* __UTILITY_BUILDER_H__ */