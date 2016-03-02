#ifndef __INTRO_GAME_STATE_H__
#define __INTRO_GAME_STATE_H__

#include "Rendering\Renderer.h"
#include "Engine\GameState.h"
#include "Engine\IInputableKeyboard.h"
#include "Engine\IRenderable.h"
//#include "Rendering\GameCommand.h"
#include "Math\Math.h"
#include "Engine\GameNode.h"
#include "GameStats.h"
#ifdef CALCULATE_GAME_STATS
#include "Math\IStatisticsStorage.h"
#endif

namespace Game
{

class IntroGameState : public Engine::GameState, public virtual Engine::Input::IInputableKeyboard, public virtual Engine::IRenderable
{
/* ==================== Constructors and destructors begin ==================== */
public:
	IntroGameState();
	virtual ~IntroGameState(void);
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
	virtual void Render(const Rendering::Shader* shader, Rendering::Renderer* renderer) const;
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
private:
	//Rendering::GameCommand* quitIntroCommand;
#ifdef CALCULATE_GAME_STATS
	Math::Statistics::ClassStats& m_classStats;
#endif
/* ==================== Non-static member variables end ==================== */
}; /* end class IntroGameState */

} /* end namespace Game */

#endif /* __INTRO_GAME_STATE_H__ */