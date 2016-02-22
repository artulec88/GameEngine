#ifndef __GAME_QUIT_COMMAND_H__
#define __GAME_QUIT_COMMAND_H__

#include "Engine\GameCommand.h"
#include "Engine\GameManager.h"

namespace Game
{

class QuitCommand : public Engine::GameCommand
{
/* ==================== Static variables and functions begin ==================== */
/* ==================== Static variables and functions end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	QuitCommand(Engine::GameManager* gameManager);
	virtual ~QuitCommand();
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	virtual void Execute() const;
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
private:
	Engine::GameManager& m_gameManager;
/* ==================== Non-static member variables end ==================== */
}; /* end class QuitCommand */

} /* end namespace Game */

#endif /* __GAME_QUIT_COMMAND_H__ */