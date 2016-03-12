#ifndef __ENGINE_PHYSICS_COMPONENT_H__
#define __ENGINE_PHYSICS_COMPONENT_H__

#include "Engine.h"
#include "GameComponent.h"
#include "IUpdateable.h"
#include "IInputableKeyboard.h"
#include "IInputableMouse.h"

#include "Math\Math.h"
#include "Math\Angle.h"

#include "Physics\PhysicsObject.h"

#include <GLFW\glfw3.h> // TODO: Just so that we can check the key bindings. Remove in the future.

namespace Engine
{
	class GameNode;

	class PhysicsComponent : public GameComponent, public Input::IInputableKeyboard
	{
	/* ==================== Static variables and functions begin ==================== */
	private:
		//static const Math::Real GRAVITY;
	/* ==================== Static variables and functions end ==================== */

	/* ==================== Constructors and destructors begin ==================== */
	public:
		ENGINE_API PhysicsComponent(Physics::PhysicsObject* physicsObject);
		ENGINE_API virtual ~PhysicsComponent(void);
	/* ==================== Constructors and destructors end ==================== */

	/* ==================== Non-static member functions begin ==================== */
	public:
		ENGINE_API virtual void Update(Math::Real deltaTime);
		ENGINE_API virtual void KeyEvent(int key, int scancode, int action, int mods);
	/* ==================== Non-static member functions end ==================== */

	/* ==================== Non-static member variables begin ==================== */
	protected:
		/// <summary>
		/// Specifies the ratio between running speed and the standard walking speed.
		/// </summary>
		/// <remarks>
		/// The value should be greater or equal to 1.0.
		/// </remarks>
		Physics::PhysicsObject* m_physicsObject;
	/* ==================== Non-static member variables end ==================== */
	}; /* end class PlayerComponent */

} /* end namespace Engine */

#endif /* __ENGINE_PHYSICS_COMPONENT_H__ */