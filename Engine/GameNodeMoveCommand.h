#ifndef __ENGINE_GAME_NODE_MOVE_COMMAND_H__
#define __ENGINE_GAME_NODE_MOVE_COMMAND_H__

#include "Engine.h"
#include "GameNodeCommand.h"

namespace engine
{

	/// <summary>
	/// A class allowing a game node to move.
	/// </summary>
	class GameNodeMoveCommand : public GameNodeCommand
	{
		/* ==================== Constructors and destructors begin ==================== */
	public:
		ENGINE_API GameNodeMoveCommand(/* math::Real movementSpeed */);
		ENGINE_API virtual ~GameNodeMoveCommand();
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
		ENGINE_API void Execute(GameNode* gameNode) const override;
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
		/* ==================== Non-static member variables end ==================== */
	}; /* end class GameNodeMoveCommand */

} /* end namespace engine */

#endif /* __ENGINE_GAME_NODE_MOVE_COMMAND_H__ */