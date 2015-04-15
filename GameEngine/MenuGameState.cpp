#include "MenuGameState.h"
#include "Rendering\GameManager.h"
#include "Utility\ILogger.h"
#include "PlayGameState.h"
#include "LoadGameState.h"

using namespace Game;
using namespace Rendering;

MenuGameState::MenuGameState(void) :
	GameState(),
	m_currentMenuEntry(NULL)
{
	/**
	 * TODO: Make sure the new operator is performed only once. When switching state back to MenuGameState
	 * the new operations must not be called.
	 */ 
	m_currentMenuEntry = new MenuEntry(Actions::UNDEFINED, "Main menu");
	MenuEntry* optionsMenuEntry = new MenuEntry(Actions::OTHER, "Options");
	optionsMenuEntry->AddChildren(new MenuEntry(Actions::OTHER, "Sound"));
	optionsMenuEntry->AddChildren(new MenuEntry(Actions::OTHER, "Graphics"));
	optionsMenuEntry->AddChildren(new MenuEntry(Actions::OTHER, "Controls"));
	m_currentMenuEntry->AddChildren(new MenuEntry(Actions::START_RESUME, "Start"));
	m_currentMenuEntry->AddChildren(optionsMenuEntry);
	m_currentMenuEntry->AddChildren(new MenuEntry(Actions::QUIT, "Quit"));
}

MenuGameState::~MenuGameState(void)
{
	SAFE_DELETE(m_currentMenuEntry);
}

void MenuGameState::Entered()
{
	LOG(Utility::Info, LOGPLACE, "Menu game state has been placed in the game state manager");
}

void MenuGameState::Leaving()
{
	LOG(Utility::Info, LOGPLACE, "Menu game state is about to be removed from the game state manager");
}

void MenuGameState::Obscuring()
{
	LOG(Utility::Info, LOGPLACE, "Another game state is about to stack on top of menu game state");
}

void MenuGameState::Revealed()
{
	LOG(Utility::Info, LOGPLACE, "Menu game state has become the topmost game state in the game state manager's stack");
}

void MenuGameState::KeyEvent(int key, int scancode, int action, int mods)
{
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
		//LOG(Utility::Info, LOGPLACE, "To exit the game click \"QUIT\"");
		break;
	case GLFW_KEY_UP:
		//LOG(Utility::Info, LOGPLACE, "Selected menu entry changed from %d to %d",
		//	m_currentMenuEntry->GetSelectedMenuEntryIndex(),
		//	m_currentMenuEntry->GetSelectedMenuEntryIndex() - 1);
		m_currentMenuEntry->SelectPrevChildMenuEntry();
		break;
	case GLFW_KEY_DOWN:
		//LOG(Utility::Info, LOGPLACE, "Selected menu entry changed from %d to %d",
		//	m_currentMenuEntry->GetSelectedMenuEntryIndex(),
		//	m_currentMenuEntry->GetSelectedMenuEntryIndex() + 1);
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
			// TODO: Both "Start" and "Exit" have no children, but the ENTER action should be handled in a different way for them.
			Actions::ActionID selectedMenuEntryAction = selectedChild->GetAction();
			switch (selectedMenuEntryAction)
			{
			case Actions::START_RESUME:
			{
				GameManager::GetGameManager()->SetTransition(new GameStateTransitioning::GameStateTransition(new LoadGameState(), GameStateTransitioning::SWITCH, GameStateModality::EXCLUSIVE));
				break;
			}
			case Actions::QUIT:
				GameManager::GetGameManager()->RequestGameQuit();
				break;
			}
		}
		break;
	}
	default:
		LOG(Utility::Debug, LOGPLACE, "The key %d is not supported by the menu game state", key);
		break;
	}
}

void MenuGameState::Input(Math::Real elapsedTime)
{
	LOG(Utility::Debug, LOGPLACE, "MAIN MENU game state input processing");
}

void MenuGameState::Render(Renderer* renderer)
{
	LOG(Utility::Debug, LOGPLACE, "MAIN MENU game state rendering");
	renderer->RenderMainMenu(*m_currentMenuEntry);
}
