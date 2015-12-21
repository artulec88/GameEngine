#ifndef __RENDERING_GAME_COMMAND_H__
#define __RENDERING_GAME_COMMAND_H__

namespace Rendering
{

/// <summary>
/// A class representing a single game action (e.g. jump, strafe, move).
/// </summary>
class GameCommand
{
/* ==================== Constructors and destructors begin ==================== */
public:
	GameCommand();
	virtual ~GameCommand();
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	virtual void Execute() = 0;
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
/* ==================== Non-static member variables end ==================== */
}; /* end class GameCommand */

} /* end namespace Rendering */

#endif /* __RENDERING_GAME_COMMAND_H__ */