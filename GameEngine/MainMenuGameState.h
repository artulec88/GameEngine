#ifndef __MAIN_MENU_GAME_STATE_H__
#define __MAIN_MENU_GAME_STATE_H__

#include "Rendering\Renderer.h"
#include "Rendering\GameState.h"
#include "Rendering\IInputable.h"
#include "Rendering\IRenderable.h"
#include "Math\Math.h"
#include "Rendering\GameNode.h"

namespace GameNamespace
{

class MainMenuGameState : public Rendering::GameState, public virtual Rendering::IInputable, public virtual Rendering::IRenderable
{
/* ==================== Constructors and destructors begin ==================== */
public:
	MainMenuGameState();
	virtual ~MainMenuGameState(void);
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

	virtual void Input(Math::Real elapsedTime, Rendering::GameNode& gameNode);
	virtual void Render(Rendering::Renderer* renderer, const Rendering::GameNode& gameNode);
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
private:
/* ==================== Non-static member variables end ==================== */
}; /* end class MainMenuGameState */

} /* end namespace GameNamespace */

#endif /* __MAIN_MENU_GAME_STATE_H__ */