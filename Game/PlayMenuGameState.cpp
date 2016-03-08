#include "PlayMenuGameState.h"
#include "Engine\GameManager.h"
#include "Engine\GameCommand.h"
#include "Rendering\Shader.h"
#include "Utility\ILogger.h"
#include "PlayGameState.h"
#include "ResumeGameCommand.h"

// TODO: We include glfw3.h header to have access to GLFW_KEY_* values (basically, to be able to respond to user's input).
// It would be much better if instead we could use our own input keys and map them together in the Engine library.
// Something like Input::KeyMapping class could map keys to actions (commands) where each game state could implement its own set of key mappings.
// In the end, we don't want the Game library to depend on GLFW library at all. The GLFW should only be used in the Engine library.
#include <GLFW\glfw3.h>

using namespace Game;

PlayMenuGameState::PlayMenuGameState(Engine::MenuEntry* playMainMenuRootEntry) :
	Engine::GameState(),
	m_currentMenuEntry(playMainMenuRootEntry)
#ifdef CALCULATE_GAME_STATS
	,m_classStats(STATS_STORAGE.GetClassStats("PlayMenuGameState"))
#endif
{
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
