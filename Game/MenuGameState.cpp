#include "MenuGameState.h"
#include "QuitCommand.h"
#include "StartGameCommand.h"
#include "Rendering\GameManager.h"
#include "Rendering\GameCommand.h"
#include "Rendering\Shader.h"
#include "Utility\ILogger.h"
#include "PlayGameState.h"
#include "LoadGameState.h"
#include "QuitCommand.h"

using namespace Game;
using namespace Rendering;

MenuGameState::MenuGameState(void) :
	GameState(),
	m_quitCommand(QuitCommand(Core::GameManager::GetGameManager())),
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
	m_currentMenuEntry = new MenuEntry(new Core::EmptyGameCommand(), "Main menu");
	MenuEntry* optionsMenuEntry = new MenuEntry(new Core::EmptyGameCommand(), "Options");
	optionsMenuEntry->AddChildren(new MenuEntry(new Core::EmptyGameCommand() /* TODO: Go to "Sound" settings */, "Sound"));
	optionsMenuEntry->AddChildren(new MenuEntry(new Core::EmptyGameCommand() /* TODO: Go to "Graphics" settings */, "Graphics"));
	optionsMenuEntry->AddChildren(new MenuEntry(new Core::EmptyGameCommand() /* TODO: Go to "Controls" settings */, "Controls"));
	m_currentMenuEntry->AddChildren(new MenuEntry(new StartGameCommand(*Core::GameManager::GetGameManager()), "Start"));
	m_currentMenuEntry->AddChildren(optionsMenuEntry);
	m_currentMenuEntry->AddChildren(new MenuEntry(&m_quitCommand, "Quit"));
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
		MenuEntry* selectedChild = m_currentMenuEntry->GetSelectedChild();
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
	renderer->RenderMainMenu(*m_currentMenuEntry);
	STOP_PROFILING;
}
