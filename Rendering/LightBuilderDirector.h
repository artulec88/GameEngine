#ifndef __RENDERING_LIGHT_BUILDER_DIRECTOR_H__
#define __RENDERING_LIGHT_BUILDER_DIRECTOR_H__

#include "Rendering.h"
#include "LightBuilder.h"

namespace Rendering
{

class LightBuilderDirector
{
/* ==================== Static variables and functions begin ==================== */
/* ==================== Static variables and functions end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	RENDERING_API LightBuilderDirector(LightBuilder& lightBuilder);
	RENDERING_API virtual ~LightBuilderDirector(void);
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	RENDERING_API void Construct();
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
private:
	LightBuilder& m_lightBuilder;
/* ==================== Non-static member variables end ==================== */
}; /* end class LightBuilderDirector */

} /* end namespace Rendering */

#endif // __RENDERING_LIGHT_BUILDER_DIRECTOR_H__
