#ifndef __RENDERING_INPUT_IINPUTABLE_H__
#define __RENDERING_INPUT_IINPUTABLE_H__

#include "Rendering.h"
#include <vector>

namespace Rendering { namespace Input
{

/// <summary>
/// Interface for objects which can react to player's input (either by keyboard, mouse, etc.).
/// </summary>
class IInputable
{
/* ==================== Constructors and destructors begin ==================== */
public:
	/// <summary> Creates the inputable object. </summary>
	RENDERING_API IInputable()
	{
	}

	/// <summary> Destroys the inputable object. </summary>
	RENDERING_API virtual ~IInputable(void)
	{
	}
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	virtual void Input(Math::Real elapsedTime) = 0;
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
//private:
	//std::vector<Command
/* ==================== Non-static member variables end ==================== */
}; /* end class IInputable */

} /* end namespace Input */

} /* end namespace Rendering */

#endif /* __RENDERING_INPUT_IINPUTABLE_H__ */