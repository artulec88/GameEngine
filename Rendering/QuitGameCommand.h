#ifndef __CORE_QUIT_GAME_COMMAND_H__
#define __CORE_QUIT_GAME_COMMAND_H__

#include "Rendering.h"
#include "GameCommand.h"

namespace Core
{

	class GameManager;

	/// <summary>
	/// A class representing a "Quit game" command.
	/// </summary>
	class QuitGameCommand : public GameCommand
	{
	/* ==================== Constructors and destructors begin ==================== */
	public:
		RENDERING_API QuitGameCommand(const GameManager& gameManager);
		RENDERING_API virtual ~QuitGameCommand();
	/* ==================== Constructors and destructors end ==================== */

	/* ==================== Non-static member functions begin ==================== */
	public:
		RENDERING_API virtual void Execute() const;
	/* ==================== Non-static member functions end ==================== */

	/* ==================== Non-static member variables begin ==================== */
	private:
		const GameManager& m_gameManager;
	/* ==================== Non-static member variables end ==================== */
	}; /* end class QuitGameCommand */

} /* end namespace Core */

#endif /* __CORE_QUIT_GAME_COMMAND_H__ */