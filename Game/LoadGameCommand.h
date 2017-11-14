#ifndef __GAME_LOAD_GAME_COMMAND_H__
#define __GAME_LOAD_GAME_COMMAND_H__

#include "Engine\GameCommand.h"

namespace Game
{
	/// <summary>
	/// The game command responsible for loading the previously saved game.
	/// </summary>
	class LoadGameCommand : public engine::GameCommand
	{
	/* ==================== Static variables and functions begin ==================== */
	/* ==================== Static variables and functions end ==================== */

	/* ==================== Constructors and destructors begin ==================== */
	public:
		LoadGameCommand();
		virtual ~LoadGameCommand();
	/* ==================== Constructors and destructors end ==================== */

	/* ==================== Non-static member functions begin ==================== */
	public:
		virtual void Execute(engine::GameManager* gameManager) const;
	/* ==================== Non-static member functions end ==================== */

	/* ==================== Non-static member variables begin ==================== */
	/* ==================== Non-static member variables end ==================== */
	}; /* end class LoadGameCommand */

} /* end namespace Game */

#endif /* __GAME_LOAD_GAME_COMMAND_H__ */