#include "MenuGameState.h"
#include "StartGameCommand.h"
#include "Engine\CoreEngine.h"
#include "Engine\GameManager.h"
#include "Engine\GameCommand.h"
#include "Rendering\Shader.h"
#include "Utility\ILogger.h"
#include "Utility\IConfig.h"
#include "PlayGameState.h"
#include "LoadGameState.h"

// TODO: We include glfw3.h header to have access to GLFW_KEY_* values (basically, to be able to respond to user's input).
// It would be much better if instead we could use our own input keys and map them together in the Engine library.
// Something like Input::KeyMapping class could map keys to actions (commands) where each game state could implement its own set of key mappings.
// In the end, we don't want the Game library to depend on GLFW library at all. The GLFW should only be used in the Engine library.
#include <GLFW\glfw3.h>

using namespace Game;
using namespace Rendering;

MenuGameState::MenuGameState() :
	Engine::GameState(),
	m_mainMenuFont(GET_CONFIG_VALUE_STR("mainMenuFontTextureAtlas", "cambria.png"), GET_CONFIG_VALUE_STR("mainMenuFontMetaData", "cambria.fnt")),
	m_mainMenuFontSize(GET_CONFIG_VALUE("mainMenuFontSize", 16.0f)),
	m_mainMenuRootEntry(Engine::GameManager::GetGameManager()->GetCommand(Engine::GameCommandTypes::EMPTY), "Main menu", &m_mainMenuFont, m_mainMenuFontSize,
		Math::Vector2D(0.0f, 0.0f), 1.0f, NULL, Math::Vector2D(0.0f, 0.0f), Math::Vector3D(0.0f, 0.0f, 0.0f)),
	m_notSelectedMenuEntryColorEffect(Engine::GameManager::GetGameManager()->GetColorEffect(Rendering::Effects::STATIC, 1)),
	m_selectedMenuEntryColorEffect(Engine::GameManager::GetGameManager()->GetColorEffect(Rendering::Effects::BLINK, 1)),
	m_mousePosX(REAL_ZERO),
	m_mousePosY(REAL_ZERO),
	m_mousePicker(),
	m_currentMenuEntry(&m_mainMenuRootEntry)
#ifdef CALCULATE_GAME_STATS
	,m_classStats(STATS_STORAGE.GetClassStats("MenuGameState"))
#endif
{
	/**
	* TODO: Calculating the proper locations for the menu entries and updating these locations whenever the window is resized.
	*/
	Engine::MenuEntry* mainMenuOptionsMenuEntry = new Engine::MenuEntry(Engine::GameManager::GetGameManager()->GetCommand(Engine::GameCommandTypes::EMPTY) /* TODO: Create GoTo "Options" game command */,
		"Options", &m_mainMenuFont, m_mainMenuFontSize, Math::Vector2D(0.25f, 0.4f), 0.5f, m_notSelectedMenuEntryColorEffect, Math::Vector2D(0.0f, 0.0f), Math::Vector3D(0.0f, 1.0f, 0.0f), true);
	mainMenuOptionsMenuEntry->AddChildren(new Engine::MenuEntry(Engine::GameManager::GetGameManager()->GetCommand(Engine::GameCommandTypes::EMPTY) /* TODO: Create GoTo "Sound" game command */,
		"Sound", &m_mainMenuFont, m_mainMenuFontSize, Math::Vector2D(0.25f, 0.25f), 0.5f, m_notSelectedMenuEntryColorEffect, Math::Vector2D(0.0f, 0.0f), Math::Vector3D(0.0f, 0.0f, 1.0f), true));
	mainMenuOptionsMenuEntry->AddChildren(new Engine::MenuEntry(Engine::GameManager::GetGameManager()->GetCommand(Engine::GameCommandTypes::EMPTY) /* TODO: Create GoTo "Graphics" game command */,
		"Graphics", &m_mainMenuFont, m_mainMenuFontSize, Math::Vector2D(0.25f, 0.5f), 0.5f, m_notSelectedMenuEntryColorEffect, Math::Vector2D(0.0f, 0.0f), Math::Vector3D(1.0f, 1.0f, 1.0f), true));
	mainMenuOptionsMenuEntry->AddChildren(new Engine::MenuEntry(Engine::GameManager::GetGameManager()->GetCommand(Engine::GameCommandTypes::EMPTY) /* TODO: Create GoTo "Controls" game command */,
		"Controls", &m_mainMenuFont, m_mainMenuFontSize, Math::Vector2D(0.25f, 0.75f), 0.5f, m_notSelectedMenuEntryColorEffect, Math::Vector2D(0.0f, 0.0f), Math::Vector3D(0.0f, 1.0f, 1.0f), true));
	m_mainMenuRootEntry.AddChildren(new Engine::MenuEntry(Engine::GameManager::GetGameManager()->GetCommand(Engine::GameCommandTypes::START), "Start", &m_mainMenuFont,
		m_mainMenuFontSize, Math::Vector2D(0.25f, 0.2f), 0.5f, m_selectedMenuEntryColorEffect, Math::Vector2D(0.0f, 0.0f), Math::Vector3D(0.0f, 1.0f, 0.0f), true));
	m_mainMenuRootEntry.AddChildren(mainMenuOptionsMenuEntry);
	m_mainMenuRootEntry.AddChildren(new Engine::MenuEntry(Engine::GameManager::GetGameManager()->GetCommand(Engine::GameCommandTypes::SHOW_INTRO), "Intro", &m_mainMenuFont,
		m_mainMenuFontSize, Math::Vector2D(0.25f, 0.6f), 0.5f, m_notSelectedMenuEntryColorEffect, Math::Vector2D(0.006f, 0.006f), Math::Vector3D(0.0f, 1.0f, 0.0f), true, 0.5f, 0.2f, 0.6f, 0.1f));
	m_mainMenuRootEntry.AddChildren(new Engine::MenuEntry(Engine::GameManager::GetGameManager()->GetCommand(Engine::GameCommandTypes::QUIT), "Quit", &m_mainMenuFont,
		m_mainMenuFontSize, Math::Vector2D(0.25f, 0.8f), 0.5f, m_notSelectedMenuEntryColorEffect, Math::Vector2D(0.006f, 0.006f), Math::Vector3D(1.0f, 1.0f, 1.0f), true));
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
			// TODO: Select the "Quit" menu entry?
		}
		DELOCUST_LOG("To exit the game click \"QUIT\"");
		break;
	case GLFW_KEY_UP:
		DEBUG_LOG("Selected menu entry changed from %d to %d", m_currentMenuEntry->GetSelectedMenuEntryIndex(), m_currentMenuEntry->GetSelectedMenuEntryIndex() - 1);
		m_currentMenuEntry->SelectChildMenuEntry(m_currentMenuEntry->GetSelectedMenuEntryIndex() - 1, m_notSelectedMenuEntryColorEffect, m_selectedMenuEntryColorEffect);
		break;
	case GLFW_KEY_DOWN:
		DEBUG_LOG("Selected menu entry changed from %d to %d", m_currentMenuEntry->GetSelectedMenuEntryIndex(), m_currentMenuEntry->GetSelectedMenuEntryIndex() + 1);
		m_currentMenuEntry->SelectChildMenuEntry(m_currentMenuEntry->GetSelectedMenuEntryIndex() + 1, m_notSelectedMenuEntryColorEffect, m_selectedMenuEntryColorEffect);
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
		renderer->RenderText(m_currentMenuEntry->GetChildGuiText(i));
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
	//EMERGENCY_LOG("Menu mouse position event (%.2f, %.2f)", m_mousePosX, m_mousePosY);
	for (int i = 0; i < menuEntryChildrenCount; ++i)
	{
		if (m_currentMenuEntry->DoesMouseHoverOverChild(i, m_mousePosX, m_mousePosY))
		{
			if (m_currentMenuEntry->GetSelectedMenuEntryIndex() != i)
			{
				INFO_LOG("Menu entry \"%s\" selected", m_currentMenuEntry->GetChildGuiText(i).GetText().c_str());
			}
			m_currentMenuEntry->SelectChildMenuEntry(i, m_notSelectedMenuEntryColorEffect, m_selectedMenuEntryColorEffect);
			break;
		}
	}
}

void MenuGameState::ScrollEvent(double xOffset, double yOffset)
{
}

void MenuGameState::Update(Math::Real deltaTime)
{
	//m_selectedMenuEntryColorEffect->Update(m_currentMenuEntry->GetSelectedChild()->GetGuiText(), deltaTime);
	for (int i = 0; i < m_currentMenuEntry->GetChildrenCount(); ++i)
	{
		m_currentMenuEntry->GetChildGuiText(i).Update(deltaTime);
	}
}