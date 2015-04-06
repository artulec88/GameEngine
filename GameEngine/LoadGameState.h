#ifndef __LOAD_GAME_STATE_H__
#define __LOAD_GAME_STATE_H__

#include "Rendering\Renderer.h"
#include "Rendering\GameState.h"
#include "Rendering\IRenderable.h"
#include "Rendering\IUpdateable.h"
#include "Rendering\GameNode.h"
#include "Math\Math.h"
#include "tinythread.h"

namespace Game
{

/* TODO: Consider inheriting from Rendering::IUpdateable class */
class LoadGameState : public Rendering::GameState, public virtual Rendering::IRenderable, public virtual Rendering::IUpdateable
{
/* ==================== Static variables and functions begin ==================== */
/* ==================== Static variables and functions end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	LoadGameState();
	virtual ~LoadGameState(void);
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

	virtual void Render(Rendering::Renderer* renderer, const Rendering::GameNode& gameNode);
	
	virtual void Update(Math::Real elapsedTime, Rendering::GameNode& gameNode);
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
private:
	Math::Real m_loadingProgress;
	tthread::thread* m_loadingThread;
/* ==================== Non-static member variables end ==================== */
}; /* end class LoadGameState */

} /* end namespace Game */

#endif /* __LOAD_GAME_STATE_H__ */