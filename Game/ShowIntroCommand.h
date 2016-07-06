#ifndef __GAME_SHOW_INTRO_COMMAND_H__
#define __GAME_SHOW_INTRO_COMMAND_H__

#include "Engine\GameCommand.h"

namespace Game
{

	class ShowIntroCommand : public Engine::GameCommand
	{
	/* ==================== Static variables and functions begin ==================== */
	/* ==================== Static variables and functions end ==================== */

	/* ==================== Constructors and destructors begin ==================== */
	public:
		ShowIntroCommand();
		virtual ~ShowIntroCommand();
	/* ==================== Constructors and destructors end ==================== */

	/* ==================== Non-static member functions begin ==================== */
	public:
		virtual void Execute(Engine::GameManager* gameManager) const;
	/* ==================== Non-static member functions end ==================== */

	/* ==================== Non-static member variables begin ==================== */
	/* ==================== Non-static member variables end ==================== */
	}; /* end class ShowIntroCommand */

} /* end namespace Game */

#endif /* __GAME_SHOW_INTRO_COMMAND_H__ */