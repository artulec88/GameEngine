#ifndef __GAME_RESUME_GAME_COMMAND_H__
#define __GAME_RESUME_GAME_COMMAND_H__

#include "Engine/GameCommand.h"
#include "Engine/GameManager.h"

namespace game
{

	class ResumeGameCommand : public engine::GameCommand
	{
	/* ==================== Static variables and functions begin ==================== */
	/* ==================== Static variables and functions end ==================== */

	/* ==================== Constructors and destructors begin ==================== */
	public:
		ResumeGameCommand();
		virtual ~ResumeGameCommand();
	/* ==================== Constructors and destructors end ==================== */

	/* ==================== Non-static member functions begin ==================== */
	public:
		virtual void Execute(engine::GameManager* gameManager) const;
	/* ==================== Non-static member functions end ==================== */

	/* ==================== Non-static member variables begin ==================== */
	/* ==================== Non-static member variables end ==================== */
	}; /* end class ResumeGameCommand */

} /* end namespace Game */

#endif /* __GAME_RESUME_GAME_COMMAND_H__ */