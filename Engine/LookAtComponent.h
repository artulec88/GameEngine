#ifndef __ENGINE_LOOK_AT_COMPONENT_H__
#define __ENGINE_LOOK_AT_COMPONENT_H__

#include "Engine.h"
#include "GameComponent.h"
#include "IUpdateable.h"
#include "GameNode.h"

#include "Math\Transform.h"
#include "Math\Vector.h"

namespace engine
{

	class LookAtComponent : public GameComponent, public IUpdateable
	{
		/* ==================== Constructors and destructors begin ==================== */
	public:
		ENGINE_API LookAtComponent(GameNode* gameNode);
		ENGINE_API virtual ~LookAtComponent(void);
		LookAtComponent(const LookAtComponent& lookAtComponent) = delete;
		LookAtComponent(LookAtComponent&& lookAtComponent) = default;
		LookAtComponent& operator=(const LookAtComponent& lookAtComponent) = delete;
		LookAtComponent& operator=(LookAtComponent&& lookAtComponent) = default;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		virtual void Update(Math::Real delta);
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	protected:
		const Math::Transform& m_entityToLookAtTransform;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class LookAtComponent */

} /* end namespace Engine */

#endif /* __ENGINE_LOOK_AT_COMPONENT_H__ */