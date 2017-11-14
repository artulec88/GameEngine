#ifndef __ENGINE_PLAYER_COMPONENT_H__
#define __ENGINE_PLAYER_COMPONENT_H__

#include "Engine.h"
#include "GameComponent.h"
#include "IUpdateable.h"
#include "IInputableMouse.h"

#include "Math\Math.h"
#include "Math\Angle.h"

#include "Physics\PhysicsObject.h"

namespace engine
{
	class GameNode;

	// TODO: This class will probably be replaced with the PhysicsComponent (after all, if some object can move it means it is a physical object).
	class MoveComponent : public GameComponent, public IUpdateable, public Input::IInputableMouse
	{
	/* ==================== Static variables and functions begin ==================== */
	/* ==================== Static variables and functions end ==================== */

	/* ==================== Constructors and destructors begin ==================== */
	public:
		ENGINE_API MoveComponent(Physics::PhysicsObject* physicsObject, Math::Real movementSpeed, Math::Real runSpeedFactor, Math::Angle& rotationSpeed, Math::Real strafeSpeed, Math::Real jumpSpeed);
		ENGINE_API virtual ~MoveComponent(void);
		MoveComponent(const MoveComponent& moveComponent) = delete;
		MoveComponent(MoveComponent&& moveComponent) = default;
		MoveComponent& operator=(const MoveComponent& moveComponent) = delete;
		MoveComponent& operator=(MoveComponent&& moveComponent) = default;
	/* ==================== Constructors and destructors end ==================== */

	/* ==================== Non-static member functions begin ==================== */
	public:
		ENGINE_API virtual void Update(Math::Real deltaTime);
		ENGINE_API virtual void MouseButtonEvent(int button, int action, int mods);
		ENGINE_API virtual void MousePosEvent(double xPos, double yPos);
		ENGINE_API virtual void ScrollEvent(double xOffset, double yOffset);
	/* ==================== Non-static member functions end ==================== */

	/* ==================== Non-static member variables begin ==================== */
	protected:
		Physics::PhysicsObject* m_physicsObject;
		const Math::Real m_movementSpeed;
		/// <summary>
		/// Specifies the ratio between running speed and the standard walking speed.
		/// </summary>
		/// <remarks>
		/// The value should be greater or equal to 1.0.
		/// </remarks>
		const Math::Real m_runSpeedFactor;
		const Math::Angle m_rotationSpeed;
		const Math::Real m_strafeSpeed;
		const Math::Real m_jumpSpeed;

		Math::Real m_currentMovementSpeed;
		Math::Angle m_currentRotationSpeed;
		Math::Real m_currentStrafeSpeed;
		Math::Real m_currentJumpSpeed;
		bool m_isJumping;
	/* ==================== Non-static member variables end ==================== */
	}; /* end class PlayerComponent */

} /* end namespace Engine */

#endif /* __ENGINE_PLAYER_COMPONENT_H__ */