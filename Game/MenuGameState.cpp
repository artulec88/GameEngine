#include "MenuGameState.h"
#include "StartGameCommand.h"
#include "Engine\CoreEngine.h"
#include "Engine\GameManager.h"
#include "Engine\GameCommand.h"
#include "Rendering\Shader.h"
#include "Utility\ILogger.h"
#include "PlayGameState.h"
#include "LoadGameState.h"

// TODO: We include glfw3.h header to have access to GLFW_KEY_* values (basically, to be able to respond to user's input).
// It would be much better if instead we could use our own input keys and map them together in the Engine library.
// Something like Input::KeyMapping class could map keys to actions (commands) where each game state could implement its own set of key mappings.
// In the end, we don't want the Game library to depend on GLFW library at all. The GLFW should only be used in the Engine library.
#include <GLFW\glfw3.h>

using namespace Game;
using namespace Rendering;

MenuGameState::MenuGameState(Engine::MenuEntry* mainMenuRootEntry) :
	Engine::GameState(),
	m_mousePosX(REAL_ZERO),
	m_mousePosY(REAL_ZERO),
	m_mousePicker(),
	m_currentMenuEntry(mainMenuRootEntry)
#ifdef CALCULATE_GAME_STATS
	,m_classStats(STATS_STORAGE.GetClassStats("MenuGameState"))
#endif
{
}

MenuGameState::~MenuGameState(void)
{
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
		ChooseCurrentMenuEntry();
		break;
	}
	default:
		DEBUG_LOG("The key %d is not supported by the menu game state", key);
		break;
	}
	STOP_PROFILING;
}

void MenuGameState::ChooseCurrentMenuEntry()
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
}

void MenuGameState::Render(const Rendering::Shader* shader, Rendering::Renderer* renderer) const
{
	START_PROFILING;
	DELOCUST_LOG("MAIN MENU game state rendering");

	renderer->BindAsRenderTarget();
	renderer->ClearScreen(/* TODO: specify menu game state clear screen color */);
	const int menuEntryChildrenCount = m_currentMenuEntry->GetChildrenCount();
	for (int i = 0; i < menuEntryChildrenCount; ++i)
	{
		renderer->RenderText(m_currentMenuEntry->GetChildScreenPosition(i).GetX(), m_currentMenuEntry->GetChildScreenPosition(i).GetY(), m_currentMenuEntry->GetChildText(i),
			m_currentMenuEntry->GetChildFontSize(i),
			m_currentMenuEntry->IsChildMenuEntrySelected(i) ? Engine::MenuEntry::GetSelectedMenuEntryTextColor() : Engine::MenuEntry::GetNotSelectedMenuEntryTextColor());
	}
	STOP_PROFILING;
}

void MenuGameState::MouseButtonEvent(int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		if (m_currentMenuEntry->GetSelectedChild()->DoesMouseHoverOver(m_mousePosX, m_mousePosY))
		{
			ChooseCurrentMenuEntry();
		}
		else
		{
			//CRITICAL_LOG("Does not hover (%.2f, %.2f)", m_mousePosX, m_mousePosY);
		}
	}
}

void MenuGameState::MousePosEvent(double xPos, double yPos)
{
	//const Rendering::CameraBase& currentCamera = Engine::CoreEngine::GetCoreEngine()->GetRenderer()->GetCurrentCamera();
	//m_mousePicker.CalculateCurrentRay(xPos, yPos, currentCamera.GetProjection(), currentCamera.GetViewMatrix());

	m_mousePosX = static_cast<Math::Real>(xPos);
	m_mousePosY = static_cast<Math::Real>(yPos);

	const int menuEntryChildrenCount = m_currentMenuEntry->GetChildrenCount();
	//CRITICAL_LOG("Menu mouse position event (%.2f, %.2f)", m_mousePosX, m_mousePosY);
	for (int i = 0; i < menuEntryChildrenCount; ++i)
	{
		if (m_currentMenuEntry->DoesMouseHoverOverChild(i, m_mousePosX, m_mousePosY))
		{
			//CRITICAL_LOG("Menu entry \"%s\" selected", m_currentMenuEntry->GetChildText(i).c_str());
			m_currentMenuEntry->SelectChildMenuEntry(i);
		}
	}
}

void MenuGameState::ScrollEvent(double xOffset, double yOffset)
{
}
