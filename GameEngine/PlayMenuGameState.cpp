#include "PlayMenuGameState.h"
#include "Rendering\GameManager.h"
#include "Utility\ILogger.h"
#include "PlayGameState.h"

using namespace Game;

PlayMenuGameState::PlayMenuGameState(void) :
	Rendering::GameState(),
	m_currentMenuEntry(NULL)
#ifdef CALCULATE_GAME_STATS
	,m_classStats(STATS_STORAGE.GetClassStats("PlayMenuGameState"))
#endif
{
	/**
	 * TODO: Make sure the new operator is performed only once. When switching state back to MenuGameState
	 * the new operations must not be called.
	 */ 
	m_currentMenuEntry = new Rendering::MenuEntry(Rendering::MenuActions::UNDEFINED, "Main menu");
	Rendering::MenuEntry* optionsMenuEntry = new Rendering::MenuEntry(Rendering::MenuActions::OTHER, "Options");
	optionsMenuEntry->AddChildren(new Rendering::MenuEntry(Rendering::MenuActions::OTHER, "Sound"));
	optionsMenuEntry->AddChildren(new Rendering::MenuEntry(Rendering::MenuActions::OTHER, "Graphics"));
	optionsMenuEntry->AddChildren(new Rendering::MenuEntry(Rendering::MenuActions::OTHER, "Controls"));
	m_currentMenuEntry->AddChildren(new Rendering::MenuEntry(Rendering::MenuActions::START_RESUME, "Resume"));
	m_currentMenuEntry->AddChildren(optionsMenuEntry);
	m_currentMenuEntry->AddChildren(new Rendering::MenuEntry(Rendering::MenuActions::QUIT, "Quit"));
}

PlayMenuGameState::~PlayMenuGameState(void)
{
	SAFE_DELETE(m_currentMenuEntry);
}

void PlayMenuGameState::Entered()
{
	INFO_LOG("PlayMenu game state has been placed in the game state manager");
}

void PlayMenuGameState::Leaving()
{
	INFO_LOG("PlayMenu game state is about to be removed from the game state manager");
}

void PlayMenuGameState::Obscuring()
{
	INFO_LOG("Another game state is about to stack on top of PlayMenu game state");
}

void PlayMenuGameState::Revealed()
{
	INFO_LOG("PlayMenu game state has become the topmost game state in the game state manager's stack");
}

void PlayMenuGameState::KeyEvent(int key, int scancode, int action, int mods)
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
		//INFO_LOG("To exit the game click \"QUIT\"");
		break;
	case GLFW_KEY_UP:
		//INFO_LOG("Selected menu entry changed from %d to %d",
		//	m_currentMenuEntry->GetSelectedMenuEntryIndex(),
		//	m_currentMenuEntry->GetSelectedMenuEntryIndex() - 1);
		m_currentMenuEntry->SelectPrevChildMenuEntry();
		break;
	case GLFW_KEY_DOWN:
		//INFO_LOG("Selected menu entry changed from %d to %d",
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
			Rendering::MenuActions::MenuActionID selectedMenuEntryAction = selectedChild->GetMenuActionID();
			switch (selectedMenuEntryAction)
			{
			case Rendering::MenuActions::START_RESUME:
				//ASSERT(Rendering::GameManager::GetGameManager()->HasMoreThanOneState)
				Rendering::GameManager::GetGameManager()->PopState();
				break;
			case Rendering::MenuActions::QUIT:
				Rendering::GameManager::GetGameManager()->RequestGameQuit();
				break;
			}
		}
		break;
	}
	default:
		DEBUG_LOG("The key %d is not supported by the menu game state", key);
		break;
	}
}

void PlayMenuGameState::Input(Math::Real elapsedTime)
{
	DEBUG_LOG("PLAY MAIN MENU game state input processing");
}

void PlayMenuGameState::Render(Rendering::Renderer* renderer)
{
	DEBUG_LOG("PLAY MAIN MENU game state rendering");
	renderer->RenderMainMenu(*m_currentMenuEntry);
}
