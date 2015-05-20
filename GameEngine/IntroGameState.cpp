#include "IntroGameState.h"
#include "Rendering\GameManager.h"
#include "Utility\ILogger.h"
#include "MenuGameState.h"

using namespace Game;
using namespace Rendering;

IntroGameState::IntroGameState(void) :
	GameState()
#ifdef CALCULATE_STATS
	,m_classStats(STATS_STORAGE.GetClassStats("IntroGameState"))
#endif
{
}

IntroGameState::~IntroGameState(void)
{
}

void IntroGameState::Entered()
{
	LOG(Utility::Info, LOGPLACE, "Intro game state has been placed in the game state manager");
}

void IntroGameState::Leaving()
{
	LOG(Utility::Info, LOGPLACE, "Intro game state is about to be removed from the game state manager");
}

void IntroGameState::Obscuring()
{
	LOG(Utility::Info, LOGPLACE, "Another game state is about to stack on top of intro game state");
}

void IntroGameState::Revealed()
{
	LOG(Utility::Info, LOGPLACE, "Intro game state has become the topmost game state in the game state manager's stack");
}

void IntroGameState::KeyEvent(int key, int scancode, int action, int mods)
{
	START_PROFILING;
	switch (key)
	{
	case GLFW_KEY_ESCAPE:
		if (action == GLFW_REPEAT)
		{
			GameManager::GetGameManager()->SetTransition(new GameStateTransitioning::GameStateTransition(new MenuGameState(), GameStateTransitioning::SWITCH, GameStateModality::EXCLUSIVE));
			break;
		}
	default:
		LOG(Utility::Info, LOGPLACE, "To skip the intro you have to double-click ESC button");
		break;
	}
	STOP_PROFILING;
}

void IntroGameState::Input(Math::Real elapsedTime)
{
	START_PROFILING;
	LOG(Utility::Debug, LOGPLACE, "INTRO game state input processing");
	STOP_PROFILING;
}

void IntroGameState::Render(Renderer* renderer)
{
	START_PROFILING;
	LOG(Utility::Debug, LOGPLACE, "INTRO game state rendering");
	STOP_PROFILING;
}
