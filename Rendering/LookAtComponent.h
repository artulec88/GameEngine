#pragma once

#include "Rendering.h"
#include "GameComponent.h"
#include "Math\Vector.h"

namespace Rendering
{

// TODO: Inherit from IUpdateable interface.
class LookAtComponent : public GameComponent
{
/* ==================== Constructors and destructors begin ==================== */
public:
	RENDERING_API LookAtComponent();
	RENDERING_API virtual ~LookAtComponent(void);
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	RENDERING_API virtual void Update(Math::Real delta);
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
protected:
	Renderer* renderer;
/* ==================== Non-static member variables end ==================== */
}; /* end class LookAtComponent */

} /* end namespace Rendering */