#ifndef __ENGINE_QUIT_GAME_COMMAND_H__
#define __ENGINE_QUIT_GAME_COMMAND_H__

#include "Engine.h"
#include "GameCommand.h"

namespace Engine
{

	class GameManager;

	/// <summary>
	/// A class representing a "Quit game" command.
	/// </summary>
	class QuitGameCommand : public GameCommand
	{
	/* ==================== Constructors and destructors begin ==================== */
	public:
		//ENGINE_API QuitGameCommand(const GameManager& gameManager);
		ENGINE_API QuitGameCommand();
		ENGINE_API virtual ~QuitGameCommand();
	/* ==================== Constructors and destructors end ==================== */

	/* ==================== Non-static member functions begin ==================== */
	public:
		ENGINE_API virtual void Execute() const;
	/* ==================== Non-static member functions end ==================== */

	/* ==================== Non-static member variables begin ==================== */
	private:
		//const GameManager& m_gameManager;
	/* ==================== Non-static member variables end ==================== */
	}; /* end class QuitGameCommand */

} /* end namespace Engine */

#endif /* __ENGINE_QUIT_GAME_COMMAND_H__ */