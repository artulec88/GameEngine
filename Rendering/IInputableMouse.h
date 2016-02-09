#ifndef __RENDERING_INPUT_IINPUTABLE_MOUSE_H__
#define __RENDERING_INPUT_IINPUTABLE_MOUSE_H__

#include "Rendering.h"
#include "IInputable.h"

namespace Rendering { namespace Input
{
/// <summary>
/// Interface for objects which can react to player's mouse input
/// </summary>
class IInputableMouse : public virtual IInputable
{
/* ==================== Constructors and destructors begin ==================== */
public:
	/// <summary> Creates the mouse inputable object. </summary>
	RENDERING_API IInputableMouse();

	/// <summary> Destroys the mouse inputable object. </summary>
	RENDERING_API virtual ~IInputableMouse(void);
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	RENDERING_API virtual void MouseButtonEvent(int button, int action, int mods) = 0;
	RENDERING_API virtual void MousePosEvent(double xPos, double yPos) = 0;
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
/* ==================== Non-static member variables end ==================== */
}; /* end class IInputableMouse */

} /* end namespace Input */

} /* end namespace Rendering */

#endif /* __RENDERING_INPUT_IINPUTABLE_MOUSE_H__ */