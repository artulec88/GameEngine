#ifndef __ENGINE_LOOK_AT_COMPONENT_H__
#define __ENGINE_LOOK_AT_COMPONENT_H__

#include "Engine.h"
#include "Rendering\GameComponent.h"
#include "Rendering\IUpdateable.h"
#include "Rendering\GameNode.h"
#include "Math\Transform.h"
#include "Math\Vector.h"

namespace Engine
{

	class LookAtComponent : public Rendering::GameComponent, public Rendering::IUpdateable
	{
	/* ==================== Constructors and destructors begin ==================== */
	public:
		RENDERING_API LookAtComponent(Rendering::GameNode* gameNode);
		RENDERING_API virtual ~LookAtComponent(void);
	/* ==================== Constructors and destructors end ==================== */

	/* ==================== Non-static member functions begin ==================== */
	public:
		RENDERING_API virtual void Update(Math::Real delta);
	/* ==================== Non-static member functions end ==================== */

	/* ==================== Non-static member variables begin ==================== */
	protected:
		const Math::Transform& m_entityToLookAtTransform;
	/* ==================== Non-static member variables end ==================== */
	}; /* end class LookAtComponent */

} /* end namespace Engine */

#endif /* __ENGINE_LOOK_AT_COMPONENT_H__ */