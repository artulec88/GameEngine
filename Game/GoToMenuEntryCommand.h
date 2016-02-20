#ifndef __GAME_GO_TO_MENU_ENTRY_COMMAND_H__
#define __GAME_GO_TO_MENU_ENTRY_COMMAND_H__

#include "Engine\GameCommand.h"

namespace Game
{

/// <summary>
/// A class representing an action of going into the submenu of the currently selected menu entry.
/// </summary>
/// <remarks>
/// This class is a stateless representation of pure behavior so having more than one instance of that class wastes memory since all instances are equivalent.
/// The Flyweight pattern addresses that.
/// </remarks>
class GoToMenuEntryCommand : public Engine::GameCommand
{
/* ==================== Static variables and functions begin ==================== */
/* ==================== Static variables and functions end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	GoToMenuEntryCommand();
	virtual ~GoToMenuEntryCommand();
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	virtual void Execute() const;
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
private:
/* ==================== Non-static member variables end ==================== */
}; /* end class GoToMenuEntryCommand */

} /* end namespace Game */

#endif /* __GAME_GO_TO_MENU_ENTRY_COMMAND_H__ */