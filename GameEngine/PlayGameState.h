#ifndef __PLAY_GAME_STATE_H__
#define __PLAY_GAME_STATE_H__

#include "Rendering\Renderer.h"
#include "Rendering\GameState.h"
#include "Rendering\IInputableKeyboard.h"
#include "Rendering\IInputableMouse.h"
#include "Rendering\IRenderable.h"
#include "Rendering\IUpdateable.h"
#include "Math\Math.h"
#include "GameStats.h"
#ifdef CALCULATE_GAME_STATS
#include "Math\IStatisticsStorage.h"
#endif

namespace Game
{

class PlayGameState : public Rendering::GameState, public virtual Rendering::Input::IInputableKeyboard,
	public virtual Rendering::Input::IInputableMouse, public virtual Rendering::IRenderable, public virtual Rendering::IUpdateable
{
/* ==================== Constructors and destructors begin ==================== */
public:
	PlayGameState();
	virtual ~PlayGameState(void);
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
	
	bool IsInGameTimeCalculationEnabled() const { return true; }

	virtual void MouseButtonEvent(int button, int action, int mods);
	virtual void MousePosEvent(double xPos, double yPos);
	virtual void KeyEvent(int key, int scancode, int action, int mods);
	virtual void Input(Math::Real elapsedTime);
	virtual void Render(Rendering::Renderer* renderer);
	virtual void Update(Math::Real elapsedTime);
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
private:
	//Rendering::GameNode& m_rootGameNode;
	bool m_isMouseLocked;
#ifdef CALCULATE_GAME_STATS
	mutable Math::Statistics::ClassStats& m_classStats;
#endif
/* ==================== Non-static member variables end ==================== */
}; /* end class PlayGameState */

} /* end namespace Game */

#endif /* __PLAY_GAME_STATE_H__ */
