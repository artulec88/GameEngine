#ifndef __RENDERING_GAME_COMMAND_H__
#define __RENDERING_GAME_COMMAND_H__

#include "Rendering.h"
#include "Utility\ILogger.h"

namespace Rendering
{

/// <summary>
/// A class representing a single game action (e.g. jump, strafe, move).
/// </summary>
class GameCommand
{
/* ==================== Constructors and destructors begin ==================== */
public:
	RENDERING_API GameCommand();
	RENDERING_API virtual ~GameCommand();
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	RENDERING_API virtual void Execute() const = 0;
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
/* ==================== Non-static member variables end ==================== */
}; /* end class GameCommand */


/// <summary>
/// A class representing a "Do-nothing" action.
/// </summary>
class EmptyGameCommand : public GameCommand
{
/* ==================== Constructors and destructors begin ==================== */
public:
	RENDERING_API EmptyGameCommand() :
		GameCommand()
	{
	}
	RENDERING_API virtual ~EmptyGameCommand()
	{
	}
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	RENDERING_API virtual void Execute() const
	{
		DEBUG_LOG("Do-nothing command started. Remember that executing a DO-NOTHING command wastes precious CPU time.");
		// Do nothing
	}
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
/* ==================== Non-static member variables end ==================== */
}; /* end class EmptyGameCommand */

} /* end namespace Rendering */

#endif /* __RENDERING_GAME_COMMAND_H__ */