#ifndef __ENGINE_IINPUTABLE_KEYBOARD_H__
#define __ENGINE_IINPUTABLE_KEYBOARD_H__

#include "Engine.h"

namespace Engine { namespace Input
{

/// <summary>
/// Interface for objects which can react to player's keyboard input
/// </summary>
class IInputableKeyboard
{
/* ==================== Constructors and destructors begin ==================== */
public:
	/// <summary> Creates the keyboard inputable object. </summary>
	ENGINE_API IInputableKeyboard()
	{
	}

	/// <summary> Destroys the keyboard inputable object. </summary>
	ENGINE_API virtual ~IInputableKeyboard(void)
	{
	}
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	ENGINE_API virtual void KeyEvent(int key, int scancode, int action, int mods) = 0;
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
/* ==================== Non-static member variables end ==================== */
}; /* end class IInputableKeyboard */

} /* end namespace Input */

} /* end namespace Engine */

#endif /* __ENGINE_IINPUTABLE_KEYBOARD_H__ */