#include "MenuGameState.h"
#include "Utility\ILogger.h"

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
	m_currentMenuEntry = new Rendering::MenuEntry("Main menu");
	Rendering::MenuEntry* optionsMenuEntry = new Rendering::MenuEntry("Options");
	optionsMenuEntry->AddChildren(new Rendering::MenuEntry("Sound"));
	optionsMenuEntry->AddChildren(new Rendering::MenuEntry("Graphics"));
	optionsMenuEntry->AddChildren(new Rendering::MenuEntry("Controls"));
	m_currentMenuEntry->AddChildren(new Rendering::MenuEntry("Start"));
	m_currentMenuEntry->AddChildren(optionsMenuEntry);
	m_currentMenuEntry->AddChildren(new Rendering::MenuEntry("Exit"));

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
	switch (key)
	{
	case GLFW_KEY_ESCAPE:
		if (action == GLFW_PRESS)
		{
			if (m_currentMenuEntry->HasParent())
			{
				m_currentMenuEntry = m_currentMenuEntry->GetParent();
			}
			else
			{
				// TODO: Switch to InGameState state
			}
		}
		//LOG(Utility::Info, LOGPLACE, "To exit the game click \"QUIT\"");
		break;
	case GLFW_KEY_UP:
		//LOG(Utility::Info, LOGPLACE, "Selected menu entry changed from %d to %d",
		//	m_currentMenuEntry->GetSelectedMenuEntryIndex(),
		//	m_currentMenuEntry->GetSelectedMenuEntryIndex() - 1);
		if (action == GLFW_PRESS)
		{
			m_currentMenuEntry->SelectPrevChildMenuEntry();
		}
		break;
	case GLFW_KEY_DOWN:
		//LOG(Utility::Info, LOGPLACE, "Selected menu entry changed from %d to %d",
		//	m_currentMenuEntry->GetSelectedMenuEntryIndex(),
		//	m_currentMenuEntry->GetSelectedMenuEntryIndex() + 1);
		if (action == GLFW_PRESS)
		{
			m_currentMenuEntry->SelectNextChildMenuEntry();
		}
		break;
	case GLFW_KEY_ENTER:
	{
		Rendering::MenuEntry* selectedChild = m_currentMenuEntry->GetSelectedChild();
		if ((action == GLFW_PRESS) && selectedChild->HasChildren())
		{
			m_currentMenuEntry = selectedChild;
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
