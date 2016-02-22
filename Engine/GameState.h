#ifndef __ENGINE_GAME_STATE_H__
#define __ENGINE_GAME_STATE_H__

#include "Engine.h"
//#include "IInputable.h"
//#include "IRenderable.h"
//#include "IUpdateable.h"

namespace Engine
{

///<summary>
/// The base class for each game state. It provides functionality for game states lifecycles.
/// </summary>
class GameState
{
/* ==================== Constructors and destructors begin ==================== */
public:
	ENGINE_API GameState()
	{
	}
	
	ENGINE_API virtual ~GameState(void)
	{
	}
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	/// <summary>The function invoked after the current game state has been placed in the game state manager.</summary>
	ENGINE_API virtual void Entered() = 0;

	/// <summary>The function invoked before the current game state is removed game state manager.</summary>
	ENGINE_API virtual void Leaving() = 0;

	/// <summary>The function invoked right before another game state is stacked on top of the current one.</summary>
	ENGINE_API virtual void Obscuring() = 0;

	/// <summary>The function invoked right after the game state has become the topmost game state on the stack again.</summary>
	ENGINE_API virtual void Revealed() = 0;
	
	/// <summary>The function indicating whether the current game state wants in-game time calculation to be performed or not.</summary>
	ENGINE_API virtual bool IsInGameTimeCalculationEnabled() const { return false; }
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
private:
/* ==================== Non-static member variables end ==================== */
}; /* end class GameState */

} /* end namespace Engine */

#endif /* __ENGINE_GAME_STATE_H__ */