#include "MenuGameState.h"
#include "QuitCommand.h"
#include "StartGameCommand.h"
#include "Engine\GameManager.h"
#include "Engine\GameCommand.h"
#include "Rendering\Shader.h"
#include "Utility\ILogger.h"
#include "PlayGameState.h"
#include "LoadGameState.h"
#include "QuitCommand.h"

// TODO: We include glfw3.h header to have access to GLFW_KEY_* values (basically, to be able to respond to user's input).
// It would be much better if instead we could use our own input keys and map them together in the Engine library.
// Something like Input::KeyMapping class could map keys to actions (commands) where each game state could implement its own set of key mappings.
// In the end, we don't want the Game library to depend on GLFW library at all. The GLFW should only be used in the Engine library.
#include <GLFW\glfw3.h>

using namespace Game;
using namespace Rendering;

MenuGameState::MenuGameState(void) :
	GameState(),
	m_quitCommand(QuitCommand(Engine::GameManager::GetGameManager())),
	m_currentMenuEntry(NULL)
#ifdef CALCULATE_GAME_STATS
	,m_classStats(STATS_STORAGE.GetClassStats("MenuGameState"))
#endif
{
	//EmptyGameCommand emptyGameCommand; // TODO: Use Flyweight pattern because EmptyGameCommand is a stateless chunk of pure behavior. There is no need to store more than one instance of this class.
	/**
	 * TODO: Make sure the new operator is performed only once. When switching state back to MenuGameState
	 * the new operations must not be called.
	 */ 
	m_currentMenuEntry = new Engine::MenuEntry(new Engine::EmptyGameCommand(), "Main menu");
	Engine::MenuEntry* optionsMenuEntry = new Engine::MenuEntry(new Engine::EmptyGameCommand(), "Options");
	optionsMenuEntry->AddChildren(new Engine::MenuEntry(new Engine::EmptyGameCommand() /* TODO: Go to "Sound" settings */, "Sound"));
	optionsMenuEntry->AddChildren(new Engine::MenuEntry(new Engine::EmptyGameCommand() /* TODO: Go to "Graphics" settings */, "Graphics"));
	optionsMenuEntry->AddChildren(new Engine::MenuEntry(new Engine::EmptyGameCommand() /* TODO: Go to "Controls" settings */, "Controls"));
	m_currentMenuEntry->AddChildren(new Engine::MenuEntry(new StartGameCommand(*Engine::GameManager::GetGameManager()), "Start"));
	m_currentMenuEntry->AddChildren(optionsMenuEntry);
	m_currentMenuEntry->AddChildren(new Engine::MenuEntry(&m_quitCommand, "Quit"));
}

MenuGameState::~MenuGameState(void)
{
	SAFE_DELETE(m_currentMenuEntry);
}

void MenuGameState::Entered()
{
	INFO_LOG("Menu game state has been placed in the game state manager");
}

void MenuGameState::Leaving()
{
	INFO_LOG("Menu game state is about to be removed from the game state manager");
}

void MenuGameState::Obscuring()
{
	INFO_LOG("Another game state is about to stack on top of menu game state");
}

void MenuGameState::Revealed()
{
	INFO_LOG("Menu game state has become the topmost game state in the game state manager's stack");
}

void MenuGameState::KeyEvent(int key, int scancode, int action, int mods)
{
	// TODO: We include glfw3.h header to have access to GLFW_KEY_* values (basically, to be able to respond to user's input).
	// It would be much better if instead we could use our own input keys and map them together in the Engine library.
	// Something like Input::KeyMapping class could map keys to actions (commands) where each game state could implement its own set of key mappings.
	// In the end, we don't want the Game library to depend on GLFW library at all. The GLFW should only be used in the Engine library.
	START_PROFILING;
	if (action != GLFW_PRESS)
	{
		return;
	}
	switch (key)
	{
	case GLFW_KEY_ESCAPE:
		if (m_currentMenuEntry->HasParent())
		{
			m_currentMenuEntry = m_currentMenuEntry->GetParent();
		}
		else
		{
				
		}
		DELOCUST_LOG("To exit the game click \"QUIT\"");
		break;
	case GLFW_KEY_UP:
		DEBUG_LOG("Selected menu entry changed from %d to %d", m_currentMenuEntry->GetSelectedMenuEntryIndex(), m_currentMenuEntry->GetSelectedMenuEntryIndex() - 1);
		m_currentMenuEntry->SelectPrevChildMenuEntry();
		break;
	case GLFW_KEY_DOWN:
		DEBUG_LOG("Selected menu entry changed from %d to %d", m_currentMenuEntry->GetSelectedMenuEntryIndex(), m_currentMenuEntry->GetSelectedMenuEntryIndex() + 1);
		m_currentMenuEntry->SelectNextChildMenuEntry();
		break;
	case GLFW_KEY_ENTER:
	{
		Engine::MenuEntry* selectedChild = m_currentMenuEntry->GetSelectedChild();
		if (selectedChild->HasChildren())
		{
			m_currentMenuEntry = selectedChild;
		}
		else
		{
			selectedChild->ExecuteCommand();
		}
		break;
	}
	default:
		DEBUG_LOG("The key %d is not supported by the menu game state", key);
		break;
	}
	STOP_PROFILING;
}

void MenuGameState::Render(Shader* shader, Renderer* renderer) const
{
	START_PROFILING;
	DELOCUST_LOG("MAIN MENU game state rendering");
	//renderer->RenderMainMenu(*m_currentMenuEntry);
	renderer->RenderMainMenu();
	STOP_PROFILING;
}
