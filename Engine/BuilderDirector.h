#ifndef __ENGINE_BUILDER_DIRECTOR_H__
#define __ENGINE_BUILDER_DIRECTOR_H__

#include "Engine.h"
#include "Builder.h"

namespace Engine
{

	class BuilderDirector
	{
	/* ==================== Static variables and functions begin ==================== */
	/* ==================== Static variables and functions end ==================== */

	/* ==================== Constructors and destructors begin ==================== */
	public:
		ENGINE_API BuilderDirector(Builder& builder);
		ENGINE_API virtual ~BuilderDirector(void);
	/* ==================== Constructors and destructors end ==================== */

	/* ==================== Non-static member functions begin ==================== */
	public:
		ENGINE_API virtual void Construct() const;
	/* ==================== Non-static member functions end ==================== */

	/* ==================== Non-static member variables begin ==================== */
	private:
		Builder& m_builder;
	/* ==================== Non-static member variables end ==================== */
	}; /* end class BuilderDirector */

} /* end namespace Engine */

#endif // __ENGINE_BUILDER_DIRECTOR_H__
