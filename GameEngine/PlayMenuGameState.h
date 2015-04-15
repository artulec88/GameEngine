#ifndef __PLAY_MENU_GAME_STATE_H__
#define __PLAY_MENU_GAME_STATE_H__

#include "Rendering\Renderer.h"
#include "Rendering\GameState.h"
#include "Rendering\IInputable.h"
#include "Rendering\IRenderable.h"
#include "Math\Math.h"
#include "Rendering\GameNode.h"
#include "Rendering\MenuEntry.h"

#include <vector>

namespace Game
{

class PlayMenuGameState : public Rendering::GameState, public virtual Rendering::IInputable, public virtual Rendering::IRenderable
{
/* ==================== Static variables and functions begin ==================== */
/* ==================== Static variables and functions end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	PlayMenuGameState();
	virtual ~PlayMenuGameState(void);
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	/**
	 * Called after the game state has been placed in the game state manager
	 */
	virtual void Entered();

	/**
	 * Called right before the game state is removed from the game state manager
	 */
	virtual void Leaving();

	/**
	 * Called right before another game state is stacked on top of this one
	 */
	virtual void Obscuring();

	/**
	 * Called after the game state has become the topmost game state on the stack again
	 */
	virtual void Revealed();

	virtual void KeyEvent(int key, int scancode, int action, int mods);
	virtual void Input(Math::Real elapsedTime);
	virtual void Render(Rendering::Renderer* renderer);
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
private:
	Rendering::MenuEntry* m_currentMenuEntry;
/* ==================== Non-static member variables end ==================== */
}; /* end class PlayMenuGameState */

} /* end namespace Game */

#endif /* __PLAY_MENU_GAME_STATE_H__ */