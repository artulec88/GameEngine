#include "MenuGameState.h"
#include "QuitCommand.h"
#include "StartGameCommand.h"
#include "Engine\CoreEngine.h"
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
	m_mousePosX(REAL_ZERO),
	m_mousePosY(REAL_ZERO),
	m_mousePicker(),
	m_quitCommand(QuitCommand(Engine::GameManager::GetGameManager())),
	m_currentMenuEntry(NULL)
#ifdef CALCULATE_GAME_STATS
	,m_classStats(STATS_STORAGE.GetClassStats("MenuGameState"))
#endif
{
	//EmptyGameCommand emptyGameCommand; // TODO: Use Flyweight pattern because EmptyGameCommand is a stateless chunk of pure behavior. There is no need to store more than one instance of this class.
	/**
	 * TODO: Make sure the new operator is performed only once. When switching state back to MenuGameState
	 * the new operations must not be called. I think the CoreEngine itself can contain two main menu entries one for the MenuGameState and one for the PlayMenuGameState.
	 * Each one would be a root in the hierarchy of menu entries. The constructor of MenuGameState and PlayMenuGameState would have one parameter pointing to the instances contained in the CoreEngine class.
	 * This way we wouldn't have to create menus each time we go to menuGameState or PlayMenuGameState.
	 *
	 * TODO 2: Another thing to do is calculating the proper locations for the menu entries and updating these locations whenever the window is resized.
	 */
	m_currentMenuEntry = new Engine::MenuEntry(new Engine::EmptyGameCommand(), "Main menu", Math::Vector2D(0.0f, 0.0f));
	Engine::MenuEntry* optionsMenuEntry = new Engine::MenuEntry(new Engine::EmptyGameCommand(), "Options", Math::Vector2D(450.0f, 450.0f));
	optionsMenuEntry->AddChildren(new Engine::MenuEntry(new Engine::EmptyGameCommand() /* TODO: Go to "Sound" settings */, "Sound", Math::Vector2D(450.0f, 350.0f)));
	optionsMenuEntry->AddChildren(new Engine::MenuEntry(new Engine::EmptyGameCommand() /* TODO: Go to "Graphics" settings */, "Graphics", Math::Vector2D(450.0f, 450.0f)));
	optionsMenuEntry->AddChildren(new Engine::MenuEntry(new Engine::EmptyGameCommand() /* TODO: Go to "Controls" settings */, "Controls", Math::Vector2D(450.0f, 550.0f)));
	m_currentMenuEntry->AddChildren(new Engine::MenuEntry(new StartGameCommand(*Engine::GameManager::GetGameManager()), "Start", Math::Vector2D(450.0f, 350.0f)));
	m_currentMenuEntry->AddChildren(optionsMenuEntry);
	m_currentMenuEntry->AddChildren(new Engine::MenuEntry(&m_quitCommand, "Quit", Math::Vector2D(450.0f, 550.0f)));
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
		// TODO: The same is handled in the MouseButtonEvent function, so the two should be moved into one function.
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
		renderer->RenderString(m_currentMenuEntry->GetChildScreenPosition(i).GetX(), m_currentMenuEntry->GetChildScreenPosition(i).GetY(), m_currentMenuEntry->GetChildText(i),
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
