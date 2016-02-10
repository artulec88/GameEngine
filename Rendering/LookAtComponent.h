#pragma once

#include "Rendering.h"
#include "GameComponent.h"
#include "IUpdateable.h"
#include "Transform.h"
#include "Math\Vector.h"

namespace Rendering
{

	class GameNode;

// TODO: Inherit from IUpdateable interface.
class LookAtComponent : public GameComponent, public IUpdateable
{
/* ==================== Constructors and destructors begin ==================== */
public:
	RENDERING_API LookAtComponent(GameNode* gameNode);
	RENDERING_API virtual ~LookAtComponent(void);
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	RENDERING_API virtual void Update(Math::Real delta);
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
protected:
	const Transform& m_entityToLookAtTransform;
	Renderer* renderer;
/* ==================== Non-static member variables end ==================== */
}; /* end class LookAtComponent */

} /* end namespace Rendering */