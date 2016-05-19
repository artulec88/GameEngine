#include "PlayMenuGameState.h"
#include "Def.h"
#include "Engine\GameManager.h"
#include "Engine\GameCommand.h"
#include "Rendering\Shader.h"
#include "Utility\ILogger.h"
#include "Utility\IConfig.h"
#include "PlayGameState.h"
#include "ResumeGameCommand.h"

// TODO: We include glfw3.h header to have access to GLFW_KEY_* values (basically, to be able to respond to user's input).
// It would be much better if instead we could use our own input keys and map them together in the Engine library.
// Something like Input::KeyMapping class could map keys to actions (commands) where each game state could implement its own set of key mappings.
// In the end, we don't want the Game library to depend on GLFW library at all. The GLFW should only be used in the Engine library.
#include <GLFW\glfw3.h>

using namespace Game;

PlayMenuGameState::PlayMenuGameState() :
	Engine::GameState(),
	m_playMainMenuFont(GET_CONFIG_VALUE_STR_GAME("mainMenuFontTextureAtlas", "cambria.png"), GET_CONFIG_VALUE_STR_GAME("mainMenuFontMetaData", "cambria.fnt")),
	m_playMainMenuFontSize(GET_CONFIG_VALUE_GAME("mainMenuFontSize", 16.0f)),
	m_playMainMenuRootEntry(Engine::GameManager::GetGameManager()->GetCommand(Engine::GameCommandTypes::EMPTY), "Play main menu", &m_playMainMenuFont,
		m_playMainMenuFontSize, Math::Vector2D(0.0f, 0.0f), 1.0f, Math::Vector3D(REAL_ZERO, REAL_ZERO, REAL_ZERO), Math::Vector3D(REAL_ZERO, REAL_ZERO, REAL_ZERO), Math::Vector2D(REAL_ZERO, REAL_ZERO)),
	//m_notSelectedMenuEntryColorEffect(Engine::GameManager::GetGameManager()->GetVec3DEffect(Rendering::Effects::STATIC, 2)),
	//m_selectedMenuEntryColorEffect(Engine::GameManager::GetGameManager()->GetVec3DEffect(Rendering::Effects::BLINK, 2)),
	//m_notSelectedMenuEntryOutlineColorEffect(Engine::GameManager::GetGameManager()->GetVec3DEffect(Rendering::Effects::STATIC, 1)),
	//m_selectedMenuEntryOutlineColorEffect(Engine::GameManager::GetGameManager()->GetVec3DEffect(Rendering::Effects::SMOOTH, 2)),
	//m_notSelectedMenuEntryOffsetEffect(Engine::GameManager::GetGameManager()->GetVec2DEffect(Rendering::Effects::STATIC, 1)),
	//m_selectedMenuEntryOffsetEffect(Engine::GameManager::GetGameManager()->GetVec2DEffect(Rendering::Effects::SMOOTH, 2)),
	m_mousePosX(REAL_ZERO),
	m_mousePosY(REAL_ZERO),
	m_currentMenuEntry(&m_playMainMenuRootEntry)
#ifdef CALCULATE_GAME_STATS
	,m_classStats(STATS_STORAGE.GetClassStats("PlayMenuGameState"))
#endif
{
	/**
	* TODO: Calculating the proper locations for the menu entries and updating these locations whenever the window is resized.
	*/
	Engine::MenuEntry* playMenuOptionsMenuEntry = new Engine::MenuEntry(Engine::GameManager::GetGameManager()->GetCommand(Engine::GameCommandTypes::EMPTY) /* TODO: Create GoTo "Options" game command */,
		"Options", &m_playMainMenuFont, m_playMainMenuFontSize, Math::Vector2D(0.25f, 0.7f), 0.5f, Math::Vector3D(1.0f, 0.0f, 0.0f), Math::Vector3D(0.0f, 1.0f, 0.0f), Math::Vector2D(0.005f, 0.005f), true);
	playMenuOptionsMenuEntry->AddChildren(new Engine::MenuEntry(Engine::GameManager::GetGameManager()->GetCommand(Engine::GameCommandTypes::EMPTY) /* TODO: Create GoTo "Sound" game command */,
		"Sound", &m_playMainMenuFont, m_playMainMenuFontSize, Math::Vector2D(0.25f, 0.25f), 0.5f, Math::Vector3D(1.0f, 0.0f, 0.0f), Math::Vector3D(0.0f, 1.0f, 0.0f), Math::Vector2D(0.005f, 0.005f), true));
	playMenuOptionsMenuEntry->AddChildren(new Engine::MenuEntry(Engine::GameManager::GetGameManager()->GetCommand(Engine::GameCommandTypes::EMPTY) /* TODO: Create GoTo "Graphics" game command */,
		"Graphics", &m_playMainMenuFont, m_playMainMenuFontSize, Math::Vector2D(0.25f, 0.5f), 0.5f, Math::Vector3D(1.0f, 0.0f, 0.0f), Math::Vector3D(0.0f, 1.0f, 0.0f), Math::Vector2D(0.005f, 0.005f), true));
	playMenuOptionsMenuEntry->AddChildren(new Engine::MenuEntry(Engine::GameManager::GetGameManager()->GetCommand(Engine::GameCommandTypes::EMPTY) /* TODO: Create GoTo "Controls" game command */,
		"Controls", &m_playMainMenuFont, m_playMainMenuFontSize, Math::Vector2D(0.25f, 0.75f), 0.5f, Math::Vector3D(1.0f, 0.0f, 0.0f), Math::Vector3D(0.0f, 1.0f, 0.0f), Math::Vector2D(0.005f, 0.005f), true));
	m_playMainMenuRootEntry.AddChildren(playMenuOptionsMenuEntry);
	m_playMainMenuRootEntry.AddChildren(new Engine::MenuEntry(Engine::GameManager::GetGameManager()->GetCommand(Engine::GameCommandTypes::RESUME), "Resume", &m_playMainMenuFont,
		m_playMainMenuFontSize, Math::Vector2D(0.25f, 0.1f), 0.5f, Math::Vector3D(1.0f, 0.0f, 0.0f), Math::Vector3D(0.0f, 1.0f, 0.0f), Math::Vector2D(0.005f, 0.005f), true));
	m_playMainMenuRootEntry.AddChildren(new Engine::MenuEntry(Engine::GameManager::GetGameManager()->GetCommand(Engine::GameCommandTypes::SAVE), "Save", &m_playMainMenuFont,
		m_playMainMenuFontSize, Math::Vector2D(0.25f, 0.3f), 0.5f, Math::Vector3D(1.0f, 0.0f, 0.0f), Math::Vector3D(0.0f, 1.0f, 0.0f), Math::Vector2D(0.005f, 0.005f), true));
	m_playMainMenuRootEntry.AddChildren(new Engine::MenuEntry(Engine::GameManager::GetGameManager()->GetCommand(Engine::GameCommandTypes::LOAD), "Load", &m_playMainMenuFont,
		m_playMainMenuFontSize, Math::Vector2D(0.25f, 0.5f), 0.5f, Math::Vector3D(1.0f, 0.0f, 0.0f), Math::Vector3D(0.0f, 1.0f, 0.0f), Math::Vector2D(0.005f, 0.005f), true));
	m_playMainMenuRootEntry.AddChildren(new Engine::MenuEntry(Engine::GameManager::GetGameManager()->GetCommand(Engine::GameCommandTypes::QUIT), "Quit", &m_playMainMenuFont,
		m_playMainMenuFontSize, Math::Vector2D(0.25f, 0.9f), 0.5f, Math::Vector3D(1.0f, 0.0f, 0.0f), Math::Vector3D(0.0f, 1.0f, 0.0f), Math::Vector2D(0.005f, 0.005f), true));
}

PlayMenuGameState::~PlayMenuGameState(void)
{
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
			Engine::GameManager::GetGameManager()->PopState();
		}
		//INFO_LOG("To exit the game click \"QUIT\"");
		break;
	case GLFW_KEY_UP:
		DEBUG_LOG("Selected menu entry changed from %d to %d", m_currentMenuEntry->GetSelectedMenuEntryIndex(), m_currentMenuEntry->GetSelectedMenuEntryIndex() - 1);
		m_currentMenuEntry->SelectChildMenuEntry(m_currentMenuEntry->GetSelectedMenuEntryIndex() - 1);
		break;
	case GLFW_KEY_DOWN:
		DEBUG_LOG("Selected menu entry changed from %d to %d", m_currentMenuEntry->GetSelectedMenuEntryIndex(), m_currentMenuEntry->GetSelectedMenuEntryIndex() + 1);
		m_currentMenuEntry->SelectChildMenuEntry(m_currentMenuEntry->GetSelectedMenuEntryIndex() + 1);
		break;
	case GLFW_KEY_ENTER:
	{
		ChooseCurrentMenuEntry();
		break;
	}
	default:
		DEBUG_LOG("The key %d is not supported by the play menu game state", key);
		break;
	}
	STOP_PROFILING;
}

void PlayMenuGameState::ChooseCurrentMenuEntry()
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

void PlayMenuGameState::Render(const Rendering::Shader* shader, Rendering::Renderer* renderer) const
{
	START_PROFILING;
	DELOCUST_LOG("PLAY MAIN MENU game state rendering");

	renderer->BindAsRenderTarget();
	renderer->ClearScreen(/* TODO: specify menu game state clear screen color */);
	const int menuEntryChildrenCount = m_currentMenuEntry->GetChildrenCount();
	for (int i = 0; i < menuEntryChildrenCount; ++i)
	{
		renderer->RenderText(m_currentMenuEntry->GetChildGuiText(i));
	}
	STOP_PROFILING;
}

void PlayMenuGameState::MouseButtonEvent(int button, int action, int mods)
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

void PlayMenuGameState::MousePosEvent(double xPos, double yPos)
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

void PlayMenuGameState::ScrollEvent(double xOffset, double yOffset)
{
}
