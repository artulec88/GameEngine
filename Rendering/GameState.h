#ifndef __GAME_STATE_H__
#define __GAME_STATE_H__

#include "Rendering.h"
//#include "IInputable.h"
//#include "IRenderable.h"
//#include "IUpdateable.h"

namespace Rendering
{

class RENDERING_API GameState
{
/* ==================== Constructors and destructors begin ==================== */
public:
	GameState()
	{
	}
	
	virtual ~GameState(void)
	{
	}
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	/**
	 * Called after the game state has been placed in the game state manager
	 */
	virtual void Entered() = 0;

	/**
	 * Called right before the game state is removed from the game state manager
	 */
	virtual void Leaving() = 0;

	/**
	 * Called right before another game state is stacked on top of this one
	 */
	virtual void Obscuring() = 0;

	/**
	 * Called after the game state has become the topmost game state on the stack again
	 */
	virtual void Revealed() = 0;
	
	virtual bool IsInGameTimeCalculationEnabled() const { return false; }
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
private:
/* ==================== Non-static member variables end ==================== */
}; /* end class GameState */

} /* end namespace Rendering */

#endif /* __GAME_STATE_H__ */
