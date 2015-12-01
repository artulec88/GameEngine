#pragma once

#include "Rendering.h"
#include "GameComponent.h"
#include "Math\Vector.h"

namespace Rendering
{

// TODO: Inherit from IUpdateable interface.
class RENDERING_API LookAtComponent : public GameComponent
{
/* ==================== Constructors and destructors begin ==================== */
public:
	LookAtComponent();
	virtual ~LookAtComponent(void);
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	virtual void Update(Math::Real delta);
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
protected:
	Renderer* renderer;
/* ==================== Non-static member variables end ==================== */
}; /* end class LookAtComponent */

} /* end namespace Rendering */