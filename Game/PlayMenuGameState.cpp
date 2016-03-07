#include "PlayMenuGameState.h"
#include "Engine\GameManager.h"
#include "Engine\GameCommand.h"
#include "Rendering\Shader.h"
#include "Utility\ILogger.h"
#include "PlayGameState.h"
#include "QuitCommand.h"
#include "ResumeGameCommand.h"

// TODO: We include glfw3.h header to have access to GLFW_KEY_* values (basically, to be able to respond to user's input).
// It would be much better if instead we could use our own input keys and map them together in the Engine library.
// Something like Input::KeyMapping class could map keys to actions (commands) where each game state could implement its own set of key mappings.
// In the end, we don't want the Game library to depend on GLFW library at all. The GLFW should only be used in the Engine library.
#include <GLFW\glfw3.h>

using namespace Game;

PlayMenuGameState::PlayMenuGameState(void) :
	Engine::GameState(),
	m_quitCommand(QuitCommand(Engine::GameManager::GetGameManager())),
	m_currentMenuEntry(NULL)
#ifdef CALCULATE_GAME_STATS
	,m_classStats(STATS_STORAGE.GetClassStats("PlayMenuGameState"))
#endif
{
	//Rendering::EmptyGameCommand emptyGameCommand; // TODO: Use Flyweight pattern because EmptyGameCommand is a stateless chunk of pure behavior. There is no need to store more than one instance of this class.

	/**
	 * TODO: Make sure the new operator is performed only once. When switching state back to MenuGameState
	 * the new operations must not be called.
	 */ 
	m_currentMenuEntry = new Engine::MenuEntry(new Engine::EmptyGameCommand(), "Main menu", Math::Vector2D(0.0f, 0.0f));
	Engine::MenuEntry* optionsMenuEntry = new Engine::MenuEntry(new Engine::EmptyGameCommand(), "Options", Math::Vector2D(450.0f, 450.0f));
	optionsMenuEntry->AddChildren(new Engine::MenuEntry(new Engine::EmptyGameCommand() /* TODO: Go to "Sound" settings */, "Sound", Math::Vector2D(450.0f, 550.0f)));
	optionsMenuEntry->AddChildren(new Engine::MenuEntry(new Engine::EmptyGameCommand() /* TODO: Go to "Graphics" settings */, "Graphics", Math::Vector2D(450.0f, 450.0f)));
	optionsMenuEntry->AddChildren(new Engine::MenuEntry(new Engine::EmptyGameCommand() /* TODO: Go to "Controls" settings */, "Controls", Math::Vector2D(450.0f, 350.0f)));
	m_currentMenuEntry->AddChildren(new Engine::MenuEntry(new ResumeGameCommand(*Engine::GameManager::GetGameManager()), "Resume", Math::Vector2D(450.0f, 550.0f)));
	m_currentMenuEntry->AddChildren(optionsMenuEntry);
	m_currentMenuEntry->AddChildren(new Engine::MenuEntry(&m_quitCommand, "Quit", Math::Vector2D(450.0f, 350.0f)));
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
	// TODO: We include glfw3.h header to have access to GLFW_KEY_* values (basically, to be able to respond to user's input).
	// It would be much better if instead we could use our own input keys and map them together in the Engine library.
	// Something like Input::KeyMapping class could map keys to actions (commands) where each game state could implement its own set of key mappings.
	// In the end, we don't want the Game library to depend on GLFW library at all. The GLFW should only be used in the Engine library.
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
		if (m_currentMenuEntry->GetSelectedChild()->HasChildren())
		{
			m_currentMenuEntry = m_currentMenuEntry->GetSelectedChild();
		}
		else
		{
			m_currentMenuEntry->GetSelectedChild()->ExecuteCommand();
		}
		break;
	default:
		DEBUG_LOG("The key %d is not supported by the menu game state", key);
		break;
	}
}

void PlayMenuGameState::Render(const Rendering::Shader* shader, Rendering::Renderer* renderer) const
{
	DEBUG_LOG("PLAY MAIN MENU game state rendering");
	int menuEntryChildrenCount = m_currentMenuEntry->GetChildrenCount();
	for (int i = 0; i < menuEntryChildrenCount; ++i)
	{
		renderer->RenderString(m_currentMenuEntry->GetChildScreenPosition(i).GetX(), m_currentMenuEntry->GetChildScreenPosition(i).GetY(), m_currentMenuEntry->GetChildText(i),
			m_currentMenuEntry->GetChildFontSize(i),
			m_currentMenuEntry->IsChildMenuEntrySelected(i) ? Engine::MenuEntry::GetSelectedMenuEntryTextColor() : Engine::MenuEntry::GetNotSelectedMenuEntryTextColor());
	}
}
