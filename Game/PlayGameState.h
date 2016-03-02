#ifndef __PLAY_GAME_STATE_H__
#define __PLAY_GAME_STATE_H__

#include "GameStats.h"

#include "Engine\GameManager.h"
#include "Engine\GameState.h"
#include "Engine\IInputableKeyboard.h"
#include "Engine\IInputableMouse.h"
#include "Engine\IRenderable.h"
#include "Engine\IUpdateable.h"

#include "Rendering\Shader.h"
#include "Rendering\Renderer.h"

#include "Math\Math.h"
#ifdef CALCULATE_GAME_STATS
#include "Math\IStatisticsStorage.h"
#endif

namespace Game
{

class PlayGameState : public Engine::GameState, public virtual Engine::Input::IInputableKeyboard,
	public virtual Engine::Input::IInputableMouse, public virtual Engine::IRenderable, public virtual Engine::IUpdateable
{
/* ==================== Constructors and destructors begin ==================== */
public:
	PlayGameState(Engine::GameManager* gameManager);
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
	virtual void ScrollEvent(double xOffset, double yOffset);
	virtual void KeyEvent(int key, int scancode, int action, int mods);
	virtual void Render(const Rendering::Shader* shader, Rendering::Renderer* renderer) const;
	virtual void Update(Math::Real elapsedTime);
private:
	void RenderSceneWithAmbientLight(Rendering::Renderer* renderer) const;
	void RenderSceneWithPointLights(Rendering::Renderer* renderer) const;
	void RenderSceneWithDirectionalAndSpotLights(Rendering::Renderer* renderer) const;
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
private:
	//Rendering::GameNode& m_rootGameNode;
	bool m_isMouseLocked;
	Engine::GameManager* m_gameManager;
#ifdef CALCULATE_GAME_STATS
	Math::Statistics::ClassStats& m_classStats;
#endif
/* ==================== Non-static member variables end ==================== */
}; /* end class PlayGameState */

} /* end namespace Game */

#endif /* __PLAY_GAME_STATE_H__ */
