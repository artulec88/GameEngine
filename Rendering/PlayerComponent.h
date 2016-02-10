#ifndef __RENDERING_PLAYER_COMPONENT_H__
#define __RENDERING_PLAYER_COMPONENT_H__

#include "Rendering.h"
#include "GameComponent.h"
#include "IUpdateable.h"
#include "IInputableKeyboard.h"
#include "IInputableMouse.h"
#include "Math\Math.h"

namespace Rendering
{
	class GameNode;

	class PlayerComponent : public GameComponent, public IUpdateable, public Input::IInputableKeyboard, public Input::IInputableMouse
	{
	/* ==================== Constructors and destructors begin ==================== */
	public:
		RENDERING_API PlayerComponent(Math::Real speed);
		RENDERING_API virtual ~PlayerComponent(void);
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
		const Math::Real m_speed;
	/* ==================== Non-static member variables end ==================== */
	}; /* end class PlayerComponent */

} /* end namespace Rendering */

#endif /* __RENDERING_PLAYER_COMPONENT_H__ */