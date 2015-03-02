#ifndef __GAME_STATE_H__
#define __GAME_STATE_H__

#include "Rendering.h"
#include "IRenderable.h"
#include "IInputable.h"

namespace Rendering
{

class GameState
{
/* ==================== Constructors and destructors begin ==================== */
public:
	RENDERING_API GameState();
	RENDERING_API virtual ~GameState(void);
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
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
private:
/* ==================== Non-static member variables end ==================== */
}; /* end class GameState */

class IntroGameState : public GameState, public virtual IInputable, public virtual IRenderable
{
/* ==================== Constructors and destructors begin ==================== */
public:
	RENDERING_API IntroGameState() {};
	RENDERING_API virtual ~IntroGameState(void) {};
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	/**
	 * Called after the game state has been placed in the game state manager
	 */
	virtual void Entered() {};

	/**
	 * Called right before the game state is removed from the game state manager
	 */
	virtual void Leaving() {};

	/**
	 * Called right before another game state is stacked on top of this one
	 */
	virtual void Obscuring() {};

	/**
	 * Called after the game state has become the topmost game state on the stack again
	 */
	virtual void Revealed() {};

	virtual void Input(Math::Real elapsedTime, GameNode& gameNode);
	virtual void Render(Renderer* renderer, const GameNode& gameNode);
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
private:
/* ==================== Non-static member variables end ==================== */
}; /* end class IntroGameState */

class MainMenuGameState : public GameState, public virtual IInputable, public virtual IRenderable
{
/* ==================== Constructors and destructors begin ==================== */
public:
	RENDERING_API MainMenuGameState()
	{
	}
	
	RENDERING_API virtual ~MainMenuGameState(void)
	{
	}
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	/**
	 * Called after the game state has been placed in the game state manager
	 */
	virtual void Entered()
	{
	}

	/**
	 * Called right before the game state is removed from the game state manager
	 */
	virtual void Leaving()
	{
	}

	/**
	 * Called right before another game state is stacked on top of this one
	 */
	virtual void Obscuring()
	{
	}

	/**
	 * Called after the game state has become the topmost game state on the stack again
	 */
	virtual void Revealed()
	{
	}

	virtual void Input(Math::Real elapsedTime, GameNode& gameNode);
	virtual void Render(Renderer* renderer, const GameNode& gameNode);
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
private:
/* ==================== Non-static member variables end ==================== */
}; /* end class MainMenuGameState */

} /* end namespace Rendering */

#endif /* __GAME_STATE_H__ */