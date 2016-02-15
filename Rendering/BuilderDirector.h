#ifndef __RENDERING_BUILDER_DIRECTOR_H__
#define __RENDERING_BUILDER_DIRECTOR_H__

#include "Rendering.h"
#include "Builder.h"

namespace Rendering
{

	class BuilderDirector
	{
	/* ==================== Static variables and functions begin ==================== */
	/* ==================== Static variables and functions end ==================== */

	/* ==================== Constructors and destructors begin ==================== */
	public:
		RENDERING_API BuilderDirector(Builder& builder);
		RENDERING_API virtual ~BuilderDirector(void);
	/* ==================== Constructors and destructors end ==================== */

	/* ==================== Non-static member functions begin ==================== */
	public:
		RENDERING_API virtual void Construct() const;
	/* ==================== Non-static member functions end ==================== */

	/* ==================== Non-static member variables begin ==================== */
	private:
		Builder& m_builder;
	/* ==================== Non-static member variables end ==================== */
	}; /* end class BuilderDirector */

} /* end namespace Rendering */

#endif // __RENDERING_BUILDER_DIRECTOR_H__
