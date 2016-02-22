#ifndef __RENDERING_INPUT_IINPUTABLE_MOUSE_H__
#define __RENDERING_INPUT_IINPUTABLE_MOUSE_H__

#include "Rendering.h"
//#include "IInputable.h"

namespace Rendering { namespace Input
{
/// <summary>
/// Interface for objects which can react to player's mouse input
/// </summary>
class IInputableMouse // : public virtual IInputable
{
/* ==================== Constructors and destructors begin ==================== */
public:
	/// <summary> Creates the mouse inputable object. </summary>
	RENDERING_API IInputableMouse()
	{
	}

	/// <summary> Destroys the mouse inputable object. </summary>
	RENDERING_API virtual ~IInputableMouse(void)
	{
	}
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	/// <summary>
	/// The function handling the mouse button callback.
	/// </summary>
	/// <param name="button">The mouse button that was pressed or released. See http://www.glfw.org/docs/latest/group__buttons.html for details.</param>
	/// <param name="action">One of GLFW_PRESS or GLFW_RELEASE.</param>
	/// <param name="mods">Bit field describing which modifier keys were held down. See http://www.glfw.org/docs/latest/group__mods.html for details.</param>
	RENDERING_API virtual void MouseButtonEvent(int button, int action, int mods) = 0;

	/// <summary>
	/// The function handling the mouse position callback.
	/// </summary>
	/// <param name="xPos">The new X-coordinate, in screen coordinates, of the cursor.</param>
	/// <param name="yPos">The new Y-coordinate, in screen coordinates, of the cursor.</param>
	RENDERING_API virtual void MousePosEvent(double xPos, double yPos) = 0;

	/// <summary>
	/// The function handling the scroll callback.
	/// </summary>
	/// <param name="xOffset">The scroll offset along the X-axis.</param>
	/// <param name="yOffset">The scroll offset along the Y-axis.</param>
	RENDERING_API virtual void ScrollEvent(double xOffset, double yOffset) = 0;
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
/* ==================== Non-static member variables end ==================== */
}; /* end class IInputableMouse */

} /* end namespace Input */

} /* end namespace Rendering */

#endif /* __RENDERING_INPUT_IINPUTABLE_MOUSE_H__ */