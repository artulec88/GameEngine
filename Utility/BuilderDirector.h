#ifndef __UTILITY_BUILDER_DIRECTOR_H__
#define __UTILITY_BUILDER_DIRECTOR_H__

#include "Utility.h"
#include "Builder.h"

namespace Utility
{
	template <class T>
	class BuilderDirector
	{
		/* ==================== Static variables and functions begin ==================== */
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		/// <summary> Builder director constructor. </summary>
		/// <param name="builder"> The specific builder for constructing the object. </param>
		BuilderDirector(Builder<T>& builder) :
			m_builder(builder)
		{
		}
		/// <summary> Builder director destructor. </summary>
		~BuilderDirector(void)
		{
		}
		/// <summary> Builder director copy constructor. </summary>
		BuilderDirector(const BuilderDirector& builderDirector) = delete;
		/// <summary> Builder director move constructor. </summary>
		BuilderDirector(BuilderDirector&& builderDirector) = delete;
		/// <summary> Builder director copy assignment operator. </summary>
		BuilderDirector& operator=(const BuilderDirector& builderDirector) = delete;
		/// <summary> Builder director move assignment operator. </summary>
		BuilderDirector& operator=(BuilderDirector&& builderDirector) = delete;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		void Construct() const
		{
			m_builder.BuildPart1();
			m_builder.BuildPart2();
			m_builder.BuildPart3();
			m_builder.Get();
		}
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	private:
		Builder<T>& m_builder;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class BuilderDirector */

} /* end namespace Engine */

#endif // __UTILITY_BUILDER_DIRECTOR_H__
