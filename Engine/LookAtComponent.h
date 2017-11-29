#ifndef __ENGINE_LOOK_AT_COMPONENT_H__
#define __ENGINE_LOOK_AT_COMPONENT_H__

#include "Engine.h"
#include "GameComponent.h"
#include "IUpdateable.h"
#include "GameNode.h"

#include "Math/Transform.h"

namespace engine
{

	class LookAtComponent : public GameComponent, public IUpdateable
	{
		/* ==================== Constructors and destructors begin ==================== */
	public:
		ENGINE_API explicit LookAtComponent(GameNode* gameNode);
		ENGINE_API virtual ~LookAtComponent();
		LookAtComponent(const LookAtComponent& lookAtComponent) = delete;
		LookAtComponent(LookAtComponent&& lookAtComponent) = default;
		LookAtComponent& operator=(const LookAtComponent& lookAtComponent) = delete;
		LookAtComponent& operator=(LookAtComponent&& lookAtComponent) = default;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
		void Update(math::Real delta) override;
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	protected:
		const math::Transform& m_entityToLookAtTransform;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class LookAtComponent */

} /* end namespace engine */

#endif /* __ENGINE_LOOK_AT_COMPONENT_H__ */