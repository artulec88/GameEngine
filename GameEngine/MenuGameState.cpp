#include "MenuGameState.h"
#include "Rendering\GameManager.h"
#include "Utility\ILogger.h"
#include "InGameState.h"

using namespace Game;

/* static */ Rendering::MenuEntry MenuGameState::s_mainMenuEntry;

/* static */ void MenuGameState::InitializeMainMenu()
{
	//s_mainMenuEntry.AddChildren(new Rendering::MenuEntry("Start"));
	//s_mainMenuEntry.AddChildren(new Rendering::MenuEntry("Options"));
	//s_mainMenuEntry.AddChildren(new Rendering::MenuEntry("Exit"));
}

MenuGameState::MenuGameState(void) :
	Rendering::GameState(),
	m_currentMenuEntry(NULL)
{
	m_currentMenuEntry = new Rendering::MenuEntry(Rendering::Actions::UNDEFINED, "Main menu");
	Rendering::MenuEntry* optionsMenuEntry = new Rendering::MenuEntry(Rendering::Actions::OTHER, "Options");
	optionsMenuEntry->AddChildren(new Rendering::MenuEntry(Rendering::Actions::OTHER, "Sound"));
	optionsMenuEntry->AddChildren(new Rendering::MenuEntry(Rendering::Actions::OTHER, "Graphics"));
	optionsMenuEntry->AddChildren(new Rendering::MenuEntry(Rendering::Actions::OTHER, "Controls"));
	m_currentMenuEntry->AddChildren(new Rendering::MenuEntry(Rendering::Actions::START, "Start"));
	m_currentMenuEntry->AddChildren(optionsMenuEntry);
	m_currentMenuEntry->AddChildren(new Rendering::MenuEntry(Rendering::Actions::QUIT, "Quit"));

	s_mainMenuEntry = *m_currentMenuEntry;
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
		Rendering::MenuEntry* selectedChild = m_currentMenuEntry->GetSelectedChild();
		if (selectedChild->HasChildren())
		{
			m_currentMenuEntry = selectedChild;
		}
		else
		{
			// TODO: Both "Start" and "Exit" have no children, but the ENTER action should be handled in a different way for them.
			Rendering::Actions::ActionID selectedMenuEntryAction = selectedChild->GetAction();
			switch (selectedMenuEntryAction)
			{
			case Rendering::Actions::START:
				Rendering::GameManager::GetGameManager()->SwitchState(new InGameState());
				break;
			case Rendering::Actions::QUIT:
				Rendering::GameManager::GetGameManager()->RequestGameQuit();
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

void MenuGameState::Input(Math::Real elapsedTime, Rendering::GameNode& gameNode)
{
	LOG(Utility::Debug, LOGPLACE, "MAIN MENU game state input processing");
}

void MenuGameState::Render(Rendering::Renderer* renderer, const Rendering::GameNode& gameNode)
{
	LOG(Utility::Debug, LOGPLACE, "MAIN MENU game state rendering");
	renderer->RenderMainMenu(*m_currentMenuEntry);
}
