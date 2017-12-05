#ifndef __UTILITY_BUILDER_DIRECTOR_H__
#define __UTILITY_BUILDER_DIRECTOR_H__

#include "Utility.h"

namespace utility
{
	template <class T>
	class Builder;

	template <class T>
	class BuilderDirector
	{
		/* ==================== Static variables and functions begin ==================== */
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		/// <summary> Builder director constructor. </summary>
		/// <param name="builder">
		/// The specific builder for constructing the objects of type <code>T</code>.
		/// </param>
		explicit BuilderDirector(Builder<T>* builder) :
			m_builder(builder)
		{
		}
		/// <summary> Builder director destructor. </summary>
		~BuilderDirector()
		{
		}
		
		/// <summary> Builder director copy constructor. </summary>
		/// <param name="builderDirector"> The reference to builder director to copy construct from. </param>
		BuilderDirector(const BuilderDirector& builderDirector) = delete;

		/// <summary> Builder director move constructor. </summary>
		/// <param name="builderDirector"> The r-value reference to builder director to move construct from. </param>
		BuilderDirector(BuilderDirector&& builderDirector) = delete;

		/// <summary> Builder director copy assignment operator. </summary>
		/// <param name="builderDirector"> The reference to builder director to copy assign from. </param>
		/// <returns> The reference to the newly copy-assigned builder director. </returns>
		BuilderDirector& operator=(const BuilderDirector& builderDirector) = delete;

		/// <summary> Builder director move assignment operator. </summary>
		/// <param name="builderDirector"> The r-value reference to builder director to move assign from. </param>
		/// <returns> The reference to the newly move-assigned builder director. </returns>
		BuilderDirector& operator=(BuilderDirector&& builderDirector) = delete;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
		/// <summary> Sets the builder to be used by the director. </summary>
		/// <param name="builder">
		/// The specific builder for constructing the objects of type <code>T</code>.
		/// </param>
		void SetBuilder(Builder<T>* builder)
		{
			m_builder = builder;
		}

		/// <summary> Builds the object using the current builder and returns it. </summary>
		/// <returns> The object of type <code>T</code> built using the current builder. </returns>
		T Construct() const
		{
			return m_builder->Build();
		}
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	private:
		Builder<T>* m_builder;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class BuilderDirector<T> */

} /* end namespace Utility */

#endif // __UTILITY_BUILDER_DIRECTOR_H__
