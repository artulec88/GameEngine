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
		Math::Vector2D(0.0f, 0.0f), 1.0f, Math::Vector3D(REAL_ZERO, REAL_ZERO, REAL_ZERO), Math::Vector3D(REAL_ZERO, REAL_ZERO, REAL_ZERO), Math::Vector2D(REAL_ZERO, REAL_ZERO)),
	m_notSelectedMenuEntryColorEffect(NULL),
	m_selectedMenuEntryColorEffect(NULL),
	m_notSelectedMenuEntryOutlineColorEffect(NULL),
	m_selectedMenuEntryOutlineColorEffect(NULL),
	m_notSelectedMenuEntryOffsetEffect(NULL),
	m_selectedMenuEntryOffsetEffect(NULL),
	m_notSelectedMenuEntryCharacterWidthEffect(NULL),
	m_selectedMenuEntryCharacterWidthEffect(NULL),
	m_notSelectedMenuEntryCharacterEdgeTransitionWidthEffect(NULL),
	m_selectedMenuEntryCharacterEdgeTransitionWidthEffect(NULL),
	m_notSelectedMenuEntryBorderWidthEffect(NULL),
	m_selectedMenuEntryBorderWidthEffect(NULL),
	m_notSelectedMenuEntryBorderEdgeTransitionWidthEffect(NULL),
	m_selectedMenuEntryBorderEdgeTransitionWidthEffect(NULL),
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
		"Options", &m_mainMenuFont, m_mainMenuFontSize, Math::Vector2D(0.25f, 0.4f), 0.5f, Math::Vector3D(1.0f, 0.0f, 0.0f), Math::Vector3D(0.0f, 1.0f, 0.0f), Math::Vector2D(0.005f, 0.005f), true);
	mainMenuOptionsMenuEntry->AddChildren(new Engine::MenuEntry(Engine::GameManager::GetGameManager()->GetCommand(Engine::GameCommandTypes::EMPTY) /* TODO: Create GoTo "Sound" game command */,
		"Sound", &m_mainMenuFont, m_mainMenuFontSize, Math::Vector2D(0.25f, 0.25f), 0.5f, Math::Vector3D(1.0f, 0.0f, 0.0f), Math::Vector3D(0.0f, 1.0f, 0.0f), Math::Vector2D(0.005f, 0.005f), true));
	mainMenuOptionsMenuEntry->AddChildren(new Engine::MenuEntry(Engine::GameManager::GetGameManager()->GetCommand(Engine::GameCommandTypes::EMPTY) /* TODO: Create GoTo "Graphics" game command */,
		"Graphics", &m_mainMenuFont, m_mainMenuFontSize, Math::Vector2D(0.25f, 0.5f), 0.5f, Math::Vector3D(1.0f, 0.0f, 0.0f), Math::Vector3D(0.0f, 1.0f, 0.0f), Math::Vector2D(0.005f, 0.005f), true));
	mainMenuOptionsMenuEntry->AddChildren(new Engine::MenuEntry(Engine::GameManager::GetGameManager()->GetCommand(Engine::GameCommandTypes::EMPTY) /* TODO: Create GoTo "Controls" game command */,
		"Controls", &m_mainMenuFont, m_mainMenuFontSize, Math::Vector2D(0.25f, 0.75f), 0.5f, Math::Vector3D(1.0f, 0.0f, 0.0f), Math::Vector3D(0.0f, 1.0f, 0.0f), Math::Vector2D(0.005f, 0.005f), true));
	m_mainMenuRootEntry.AddChildren(new Engine::MenuEntry(Engine::GameManager::GetGameManager()->GetCommand(Engine::GameCommandTypes::START), "Start", &m_mainMenuFont,
		m_mainMenuFontSize, Math::Vector2D(0.25f, 0.2f), 0.5f, Math::Vector3D(1.0f, 0.0f, 0.0f), Math::Vector3D(0.0f, 1.0f, 0.0f), Math::Vector2D(0.005f, 0.005f), true));
	m_mainMenuRootEntry.AddChildren(mainMenuOptionsMenuEntry);
	m_mainMenuRootEntry.AddChildren(new Engine::MenuEntry(Engine::GameManager::GetGameManager()->GetCommand(Engine::GameCommandTypes::SHOW_INTRO), "Intro", &m_mainMenuFont,
		m_mainMenuFontSize, Math::Vector2D(0.25f, 0.6f), 0.5f, Math::Vector3D(1.0f, 0.0f, 0.0f), Math::Vector3D(0.0f, 1.0f, 0.0f), Math::Vector2D(0.005f, 0.005f), true));
	m_mainMenuRootEntry.AddChildren(new Engine::MenuEntry(Engine::GameManager::GetGameManager()->GetCommand(Engine::GameCommandTypes::QUIT), "Quit", &m_mainMenuFont,
		m_mainMenuFontSize, Math::Vector2D(0.25f, 0.8f), 0.5f, Math::Vector3D(1.0f, 0.0f, 0.0f), Math::Vector3D(0.0f, 1.0f, 0.0f), Math::Vector2D(0.005f, 0.005f), true));

	Math::Vector3D vectors3D[] = { Math::Vector3D(1.0f, 0.0f, 0.0f), Math::Vector3D(0.0f, 1.0f, 0.0f), Math::Vector3D(0.0f, 0.0f, 1.0f) };
	Math::Real colorTimes[] = { 0.0f, 1.0f, 2.0f };
	//m_notSelectedMenuEntryColorEffect = new Rendering::Effects::SmoothTransitionEffect<Math::Vector3D>(NULL, values, times, 3, 1);
	m_selectedMenuEntryColorEffect = new Engine::Effects::SmoothTransitionEffect<Math::Vector3D>(NULL, vectors3D, colorTimes, 3, false);

	Math::Real outlineDurations[] = { 1.0f, 1.0f, 2.0f };
	m_selectedMenuEntryOutlineColorEffect = new Engine::Effects::BlinkEffect<Math::Vector3D>(NULL, vectors3D, outlineDurations, 3);

	Math::Vector2D vectors2D[] = { Math::Vector2D(-0.015f, 0.015f), Math::Vector2D(0.015f, 0.015f), Math::Vector2D(0.015f, -0.015f), Math::Vector2D(-0.015f, -0.015f), Math::Vector2D(-0.015f, 0.015f) };
	Math::Real offsetTimes[] = { 0.0f, 0.75f, 1.5f, 2.25f, 3.0f };
	m_notSelectedMenuEntryOffsetEffect = new Engine::Effects::SmoothTransitionEffect<Math::Vector2D>(NULL, vectors2D, offsetTimes, 5, true);
	m_selectedMenuEntryOffsetEffect = new Engine::Effects::SmoothTransitionEffect<Math::Vector2D>(NULL, vectors2D, offsetTimes, 5, true);

	Math::Real characterWidths[] = { 0.4f, 0.45f, 0.5f, 0.55f, 0.6f };
	Math::Real characterTimes[] = { 0.0f, 0.2f, 0.4f, 0.6f, 0.8f };
	//m_notSelectedMenuEntryCharacterWidthEffect(NULL);
	m_selectedMenuEntryCharacterWidthEffect = new Engine::Effects::SmoothTransitionEffect<Math::Real>(NULL, characterWidths, characterTimes, 5, false);

	Math::Real characterEdgeTransitionWidths[] = { 0.0f, 0.1f, 0.2f, 0.3f, 0.4f };
	Math::Real characterEdgeTransitionTimes[] = { 0.0f, 1.5f, 3.0f, 4.5f, 6.0f };
	//m_notSelectedMenuEntryCharacterEdgeTransitionEffect(NULL);
	m_selectedMenuEntryCharacterEdgeTransitionWidthEffect = new Engine::Effects::SmoothTransitionEffect<Math::Real>(NULL, characterEdgeTransitionWidths, characterEdgeTransitionTimes, 5, true);
	
	Math::Real borderWidths[] = { 0.0f, 0.12f, 0.24f, 0.36f, 0.48f };
	Math::Real borderTimes[] = { 0.0f, 1.0f, 2.0f, 3.0f, 4.0f };
	//m_notSelectedMenuEntryBorderWidthEffect(NULL);
	m_selectedMenuEntryBorderWidthEffect = new Engine::Effects::SmoothTransitionEffect<Math::Real>(NULL, borderWidths, borderTimes, 5, false);

	Math::Real borderEdgeTransitionWidths[] = { 0.0f, 0.1f, 0.2f, 0.3f, 0.5f };
	Math::Real borderEdgeTransitionTimes[] = { 0.0f, 0.1f, 0.2f, 0.3f, 0.4f };
	//m_notSelectedMenuEntryBorderEdgeTransitionEffect(NULL);
	m_selectedMenuEntryBorderEdgeTransitionWidthEffect = new Engine::Effects::SmoothTransitionEffect<Math::Real>(NULL, borderEdgeTransitionWidths, borderEdgeTransitionTimes, 5, false);

	Engine::CoreEngine::GetCoreEngine()->GetAudioEngine()->LoadSoundEffect(Engine::CoreEngine::GetCoreEngine()->GetAudioDirectory() + "\\bounce.wav");

	SelectChild(0);
}

MenuGameState::~MenuGameState(void)
{
	SAFE_DELETE(m_notSelectedMenuEntryColorEffect);
	SAFE_DELETE(m_selectedMenuEntryColorEffect);
	SAFE_DELETE(m_notSelectedMenuEntryOutlineColorEffect);
	SAFE_DELETE(m_selectedMenuEntryOutlineColorEffect);
	SAFE_DELETE(m_notSelectedMenuEntryOffsetEffect);
	SAFE_DELETE(m_selectedMenuEntryOffsetEffect);
	SAFE_DELETE(m_notSelectedMenuEntryCharacterWidthEffect);
	SAFE_DELETE(m_selectedMenuEntryCharacterWidthEffect);
	SAFE_DELETE(m_notSelectedMenuEntryCharacterEdgeTransitionWidthEffect);
	SAFE_DELETE(m_selectedMenuEntryCharacterEdgeTransitionWidthEffect);
	SAFE_DELETE(m_notSelectedMenuEntryBorderWidthEffect);
	SAFE_DELETE(m_selectedMenuEntryBorderWidthEffect);
	SAFE_DELETE(m_notSelectedMenuEntryBorderEdgeTransitionWidthEffect);
	SAFE_DELETE(m_selectedMenuEntryBorderEdgeTransitionWidthEffect);
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
	if (action == GLFW_RELEASE)
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
		SelectChild(m_currentMenuEntry->GetSelectedMenuEntryIndex() - 1);
		break;
	case GLFW_KEY_DOWN:
		SelectChild(m_currentMenuEntry->GetSelectedMenuEntryIndex() + 1);
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

void MenuGameState::SelectChild(int childIndex)
{
	DEBUG_LOG("Selected menu entry changed from %d to %d", m_currentMenuEntry->GetSelectedMenuEntryIndex(), childIndex);
	CHECK_CONDITION_RETURN_VOID_ALWAYS(m_currentMenuEntry->GetSelectedMenuEntryIndex() != childIndex, Utility::Debug, "Trying to select the child which is already selected (%d).", childIndex);
	m_currentMenuEntry->GetSelectedChild()->ApplyOffsetEffect(m_notSelectedMenuEntryOffsetEffect);
	m_currentMenuEntry->SelectChildMenuEntry(childIndex);
	m_mainMenuRootEntry.GetSelectedChild()->ApplyColorEffect(m_selectedMenuEntryColorEffect);
	m_mainMenuRootEntry.GetSelectedChild()->ApplyOutlineColorEffect(m_selectedMenuEntryOutlineColorEffect);
	m_mainMenuRootEntry.GetSelectedChild()->ApplyOffsetEffect(m_selectedMenuEntryOffsetEffect);
	m_mainMenuRootEntry.GetSelectedChild()->ApplyCharacterWidthEffect(m_selectedMenuEntryCharacterWidthEffect);
	m_mainMenuRootEntry.GetSelectedChild()->ApplyCharacterEdgeTransitionWidthEffect(m_selectedMenuEntryCharacterEdgeTransitionWidthEffect);
	m_mainMenuRootEntry.GetSelectedChild()->ApplyBorderWidthEffect(m_selectedMenuEntryBorderWidthEffect);
	m_mainMenuRootEntry.GetSelectedChild()->ApplyBorderEdgeTransitionWidthEffect(m_selectedMenuEntryBorderEdgeTransitionWidthEffect);
	Engine::CoreEngine::GetCoreEngine()->GetAudioEngine()->PlaySoundEffect(Engine::CoreEngine::GetCoreEngine()->GetAudioDirectory() + "\\bounce.wav", 0.9f, 1.0f, 0.4f, 1.0f);
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
			SelectChild(i);
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
	if (m_notSelectedMenuEntryColorEffect != NULL) { m_notSelectedMenuEntryColorEffect->Update(deltaTime); }
	if (m_selectedMenuEntryColorEffect != NULL) { m_selectedMenuEntryColorEffect->Update(deltaTime); }
	if (m_notSelectedMenuEntryOutlineColorEffect != NULL) { m_notSelectedMenuEntryOutlineColorEffect->Update(deltaTime); }
	if (m_selectedMenuEntryOutlineColorEffect != NULL) { m_selectedMenuEntryOutlineColorEffect->Update(deltaTime); }
	if (m_notSelectedMenuEntryOffsetEffect != NULL) { m_notSelectedMenuEntryOffsetEffect->Update(deltaTime); }
	if (m_selectedMenuEntryOffsetEffect != NULL) { m_selectedMenuEntryOffsetEffect->Update(deltaTime); }
	if (m_notSelectedMenuEntryCharacterWidthEffect != NULL) { m_notSelectedMenuEntryCharacterWidthEffect->Update(deltaTime); }
	if (m_selectedMenuEntryCharacterWidthEffect != NULL) { m_selectedMenuEntryCharacterWidthEffect->Update(deltaTime); }
	if (m_notSelectedMenuEntryCharacterEdgeTransitionWidthEffect != NULL) { m_notSelectedMenuEntryCharacterEdgeTransitionWidthEffect->Update(deltaTime); }
	if (m_selectedMenuEntryCharacterEdgeTransitionWidthEffect != NULL) { m_selectedMenuEntryCharacterEdgeTransitionWidthEffect->Update(deltaTime); }
	if (m_notSelectedMenuEntryBorderWidthEffect != NULL) { m_notSelectedMenuEntryBorderWidthEffect->Update(deltaTime); }
	if (m_selectedMenuEntryBorderWidthEffect != NULL) { m_selectedMenuEntryBorderWidthEffect->Update(deltaTime); }
	if (m_notSelectedMenuEntryBorderEdgeTransitionWidthEffect != NULL) { m_notSelectedMenuEntryBorderEdgeTransitionWidthEffect->Update(deltaTime); }
	if (m_selectedMenuEntryBorderEdgeTransitionWidthEffect != NULL) { m_selectedMenuEntryBorderEdgeTransitionWidthEffect->Update(deltaTime); }
}