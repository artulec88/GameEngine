#ifndef __PLAY_MENU_GAME_STATE_H__
#define __PLAY_MENU_GAME_STATE_H__

#include "Rendering\Renderer.h"
#include "Engine\GameState.h"
#include "Engine\IInputableKeyboard.h"
#include "Engine\IInputableMouse.h"
#include "Engine\IRenderable.h"
#include "Math\Math.h"
#include "Engine\GameNode.h"
#include "Engine\MenuEntry.h"
#include "Engine\GameCommand.h"

#include <vector>

#include "GameStats.h"
#ifdef CALCULATE_GAME_STATS
#include "Math\IStatisticsStorage.h"
#endif

namespace Game
{

class Rendering::Shader;

class PlayMenuGameState : public Engine::GameState, public virtual Engine::Input::IInputableKeyboard, public virtual Engine::IRenderable, public virtual Engine::Input::IInputableMouse
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
	virtual void Render(const Rendering::Shader* shader, Rendering::Renderer* renderer) const;

	virtual void MouseButtonEvent(int button, int action, int mods);
	virtual void MousePosEvent(double xPos, double yPos);
	virtual void ScrollEvent(double xOffset, double yOffset);

private:
	void ChooseCurrentMenuEntry();
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
private:
	Rendering::Text::Font m_playMainMenuFont;
	Math::Real m_playMainMenuFontSize;
	Engine::MenuEntry m_playMainMenuRootEntry;
	//Rendering::Effects::Effect<Math::Vector3D>* m_notSelectedMenuEntryColorEffect;
	//Rendering::Effects::Effect<Math::Vector3D>* m_selectedMenuEntryColorEffect;
	//Rendering::Effects::Effect<Math::Vector3D>* m_notSelectedMenuEntryOutlineColorEffect;
	//Rendering::Effects::Effect<Math::Vector3D>* m_selectedMenuEntryOutlineColorEffect;
	//Rendering::Effects::Effect<Math::Vector2D>* m_notSelectedMenuEntryOffsetEffect;
	//Rendering::Effects::Effect<Math::Vector2D>* m_selectedMenuEntryOffsetEffect;
	Math::Real m_mousePosX, m_mousePosY;
	Engine::MenuEntry* m_currentMenuEntry;
#ifdef CALCULATE_GAME_STATS
	Math::Statistics::ClassStats& m_classStats;
#endif
/* ==================== Non-static member variables end ==================== */
}; /* end class PlayMenuGameState */

} /* end namespace Game */

#endif /* __PLAY_MENU_GAME_STATE_H__ */