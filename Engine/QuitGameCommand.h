#ifndef __ENGINE_QUIT_GAME_COMMAND_H__
#define __ENGINE_QUIT_GAME_COMMAND_H__

#include "GameCommand.h"

namespace engine
{

	class QuitGameCommand : public GameCommand
	{
	/* ==================== Static variables and functions begin ==================== */
	/* ==================== Static variables and functions end ==================== */

	/* ==================== Constructors and destructors begin ==================== */
	public:
		ENGINE_API QuitGameCommand();
		ENGINE_API virtual ~QuitGameCommand();
	/* ==================== Constructors and destructors end ==================== */

	/* ==================== Non-static member functions begin ==================== */
		ENGINE_API void Execute(GameManager* gameManager) const override;
	/* ==================== Non-static member functions end ==================== */

	/* ==================== Non-static member variables begin ==================== */
	/* ==================== Non-static member variables end ==================== */
	}; /* end class QuitGameCommand */

} /* end namespace engine */

#endif /* __ENGINE_QUIT_GAME_COMMAND_H__ */