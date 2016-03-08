#ifndef __GAME_QUIT_GAME_COMMAND_H__
#define __GAME_QUIT_GAME_COMMAND_H__

#include "Engine\GameCommand.h"

namespace Game
{

	class QuitGameCommand : public Engine::GameCommand
	{
	/* ==================== Static variables and functions begin ==================== */
	/* ==================== Static variables and functions end ==================== */

	/* ==================== Constructors and destructors begin ==================== */
	public:
		QuitGameCommand();
		virtual ~QuitGameCommand();
	/* ==================== Constructors and destructors end ==================== */

	/* ==================== Non-static member functions begin ==================== */
	public:
		virtual void Execute(/*const GameManager& gameManager*/) const;
	/* ==================== Non-static member functions end ==================== */

	/* ==================== Non-static member variables begin ==================== */
	/* ==================== Non-static member variables end ==================== */
	}; /* end class QuitGameCommand */

} /* end namespace Game */

#endif /* __GAME_QUIT_GAME_COMMAND_H__ */