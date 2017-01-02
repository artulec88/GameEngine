#ifndef __UTILITY_BUILDER_H__
#define __UTILITY_BUILDER_H__

#include "Utility.h"
#include "BuilderDirector.h"

#include <memory>

namespace Utility {

	/// <summary>
	/// The builder of the object of type <code>T</code>.
	/// </summary>
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
		/// <param name="builder"> The builder to copy construct from. </param>
		Builder(const Builder& builder) = delete;

		/// <summary> Builder move constructor. </summary>
		/// <param name="builder"> The r-value reference of the builder to move construct from. </param>
		Builder(Builder&& builder) = delete;

		/// <summary> Builder copy assignment operator. </summary>
		/// <param name="builder"> The builder to copy assign from. </param>
		/// <returns> The newly copy-assigned builder. </returns>
		Builder& operator=(const Builder& builder) = delete;

		/// <summary> Builder move assignment operator. </summary>
		/// <param name="builder"> The r-value reference of the builder to move assign from. </param>
		/// <returns> The newly move-assigned builder. </returns>
		Builder& operator=(Builder&& builder) = delete;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	protected:
		/// <summary> Retrieves the built object. </summary>
		/// <returns> The built object of type <code>T</code>. </returns>
		virtual T Get() = 0;

		/// <summary> Builds the object. </summary>
		virtual void Build() = 0;
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	protected:
		/* ==================== Non-static member variables end ==================== */
	}; /* end class Builder<T> */

} /* end namespace Utility */

#endif /* __UTILITY_BUILDER_H__ */