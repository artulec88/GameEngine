#ifndef __GAME_START_GAME_COMMAND_H__
#define __GAME_START_GAME_COMMAND_H__

#include "Rendering\GameCommand.h"
#include "Rendering\GameManager.h"

namespace Game
{

	class StartGameCommand : public Rendering::GameCommand
	{
		/* ==================== Static variables and functions begin ==================== */
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		StartGameCommand(Rendering::GameManager& gameManager);
		virtual ~StartGameCommand();
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		virtual void Execute() const;
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	private:
		Rendering::GameManager& m_gameManager;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class StartGameCommand */

} /* end namespace Game */

#endif /* __GAME_START_GAME_COMMAND_H__ */