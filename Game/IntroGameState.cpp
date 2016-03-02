#include "IntroGameState.h"
#include "Engine\GameManager.h"
#include "Utility\ILogger.h"
#include "MenuGameState.h"

// TODO: We include glfw3.h header to have access to GLFW_KEY_* values (basically, to be able to respond to user's input).
// It would be much better if instead we could use our own input keys and map them together in the Engine library.
// Something like Input::KeyMapping class could map keys to actions (commands) where each game state could implement its own set of key mappings.
// In the end, we don't want the Game library to depend on GLFW library at all. The GLFW should only be used in the Engine library.
#include <GLFW\glfw3.h>

using namespace Game;
using namespace Rendering;

IntroGameState::IntroGameState(void) :
	GameState()
#ifdef CALCULATE_GAME_STATS
	,m_classStats(STATS_STORAGE.GetClassStats("IntroGameState"))
#endif
{
}

IntroGameState::~IntroGameState(void)
{
}

void IntroGameState::Entered()
{
	INFO_LOG("Intro game state has been placed in the game state manager");
}

void IntroGameState::Leaving()
{
	INFO_LOG("Intro game state is about to be removed from the game state manager");
}

void IntroGameState::Obscuring()
{
	INFO_LOG("Another game state is about to stack on top of intro game state");
}

void IntroGameState::Revealed()
{
	INFO_LOG("Intro game state has become the topmost game state in the game state manager's stack");
}

void IntroGameState::KeyEvent(int key, int scancode, int action, int mods)
{
	// TODO: We include glfw3.h header to have access to GLFW_KEY_* values (basically, to be able to respond to user's input).
	// It would be much better if instead we could use our own input keys and map them together in the Engine library.
	// Something like Input::KeyMapping class could map keys to actions (commands) where each game state could implement its own set of key mappings.
	// In the end, we don't want the Game library to depend on GLFW library at all. The GLFW should only be used in the Engine library.
	START_PROFILING;
	switch (key)
	{
	case GLFW_KEY_ESCAPE:
		if (action == GLFW_REPEAT)
		{
			Engine::GameManager::GetGameManager()->SetTransition(new Engine::GameStateTransitioning::GameStateTransition(new MenuGameState(), Engine::GameStateTransitioning::SWITCH, Engine::GameStateModality::EXCLUSIVE));
			break;
		}
	default:
		INFO_LOG("To skip the intro you have to double-click ESC button");
		break;
	}
	STOP_PROFILING;
}

void IntroGameState::Input(Math::Real elapsedTime)
{
	START_PROFILING;
	DEBUG_LOG("INTRO game state input processing");
	STOP_PROFILING;
}

void IntroGameState::Render(const Rendering::Shader* shader, Rendering::Renderer* renderer) const
{
	START_PROFILING;
	DEBUG_LOG("INTRO game state rendering");
	STOP_PROFILING;
}
