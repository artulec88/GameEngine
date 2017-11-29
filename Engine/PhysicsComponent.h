#ifndef __ENGINE_PHYSICS_COMPONENT_H__
#define __ENGINE_PHYSICS_COMPONENT_H__

#include "Engine.h"
#include "GameComponent.h"
#include "IUpdateable.h"
//#include "IInputableMouse.h"

#include "Math/Math.h"
#include "Math/Vector.h"

namespace engine
{
	class GameNode;

	class PhysicsComponent : public GameComponent, public IUpdateable
	{
		/* ==================== Static variables and functions begin ==================== */
		//static const math::Real GRAVITY; // TODO: Decide whether this value should be stored here or somewhere else.
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		ENGINE_API PhysicsComponent(math::Real movementSpeed, math::Real jumpForce);
		ENGINE_API virtual ~PhysicsComponent();
		PhysicsComponent(const PhysicsComponent& physicsComponent) = delete;
		PhysicsComponent(PhysicsComponent&& physicsComponent) = default;
		PhysicsComponent& operator=(const PhysicsComponent& physicsComponent) = delete;
		PhysicsComponent& operator=(PhysicsComponent&& physicsComponent) = default;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
		ENGINE_API void Update(math::Real deltaTime) override;
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	protected:
		math::Real m_moveSpeed;
		math::Vector3D m_jumpAcceleration;
		bool m_isJumping;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class PlayerComponent */

} /* end namespace engine */

#endif /* __ENGINE_PHYSICS_COMPONENT_H__ */