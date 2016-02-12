#ifndef __RENDERING_PLAYER_COMPONENT_H__
#define __RENDERING_PLAYER_COMPONENT_H__

#include "Rendering.h"
#include "GameComponent.h"
#include "IUpdateable.h"
#include "IInputableKeyboard.h"
#include "IInputableMouse.h"
#include "Math\Math.h"
#include "Math\Angle.h"

namespace Rendering
{
	class GameNode;

	class MoveComponent : public GameComponent, public IUpdateable, public Input::IInputableKeyboard, public Input::IInputableMouse
	{
	/* ==================== Static variables and functions begin ==================== */
	private:
		static const Math::Real GRAVITY;
	/* ==================== Static variables and functions end ==================== */

	/* ==================== Constructors and destructors begin ==================== */
	public:
		RENDERING_API MoveComponent(Math::Real movementSpeed, const Math::Angle& rotationSpeed, Math::Real strafeSpeed, Math::Real jumpSpeed);
		RENDERING_API virtual ~MoveComponent(void);
	/* ==================== Constructors and destructors end ==================== */

	/* ==================== Non-static member functions begin ==================== */
	public:
		RENDERING_API virtual void Update(Math::Real deltaTime);
		RENDERING_API virtual void KeyEvent(int key, int scancode, int action, int mods);
		RENDERING_API virtual void MouseButtonEvent(int button, int action, int mods);
		RENDERING_API virtual void MousePosEvent(double xPos, double yPos);
	/* ==================== Non-static member functions end ==================== */

	/* ==================== Non-static member variables begin ==================== */
	protected:
		const Math::Real m_movementSpeed;
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

} /* end namespace Rendering */

#endif /* __RENDERING_PLAYER_COMPONENT_H__ */