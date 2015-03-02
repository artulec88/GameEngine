#ifndef __IINPUTABLE_H__
#define __IINPUTABLE_H__

#include "Rendering.h"
#include "GameNode.h"
#include "Math\Math.h"

namespace Rendering
{
/// <summary>
/// Interface for objects which can react to player's input
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
	virtual void Input(Math::Real elapsedTime, GameNode& gameNode) = 0;
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
/* ==================== Non-static member variables end ==================== */
}; /* end class IInputable */

} /* end namespace Rendering */

#endif /* __IINPUTABLE_H__ */