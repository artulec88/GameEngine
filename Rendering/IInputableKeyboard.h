#ifndef __RENDERING_IINPUTABLE_KEYBOARD_H__
#define __RENDERING_IINPUTABLE_KEYBOARD_H__

#include "Rendering.h"
//#include "IInputable.h"

namespace Rendering { namespace Input
{

/// <summary>
/// Interface for objects which can react to player's keyboard input
/// </summary>
class IInputableKeyboard // : public virtual IInputable
{
/* ==================== Constructors and destructors begin ==================== */
public:
	/// <summary> Creates the keyboard inputable object. </summary>
	RENDERING_API IInputableKeyboard()
	{
	}

	/// <summary> Destroys the keyboard inputable object. </summary>
	RENDERING_API virtual ~IInputableKeyboard(void)
	{
	}
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	RENDERING_API virtual void KeyEvent(int key, int scancode, int action, int mods) = 0;
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
/* ==================== Non-static member variables end ==================== */
}; /* end class IInputableKeyboard */

} /* end namespace Input */

} /* end namespace Rendering */

#endif /* __RENDERING_IINPUTABLE_KEYBOARD_H__ */