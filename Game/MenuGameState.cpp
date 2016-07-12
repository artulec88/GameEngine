#include "MenuGameState.h"
#include "Def.h"
#include "StartGameCommand.h"
#include "Engine\CoreEngine.h"
#include "Engine\GameManager.h"
#include "Engine\GameCommand.h"
#include "Rendering\Shader.h"
#include "Utility\ILogger.h"
#include "Utility\IConfig.h"
#include "PlayGameState.h"
#include "LoadGameState.h"

using namespace Game;
using namespace Rendering;

MenuGameState::MenuGameState(const std::string& inputMappingContextName) :
	Engine::GameState(inputMappingContextName),
	m_mainMenuFont(GET_CONFIG_VALUE_STR_GAME("mainMenuFontTextureAtlas", "cambria.png"), GET_CONFIG_VALUE_STR_GAME("mainMenuFontMetaData", "cambria.fnt")),
	m_mainMenuFontSize(GET_CONFIG_VALUE_GAME("mainMenuFontSize", 16.0f)),
	m_mainMenuRootEntry("Main menu", &m_mainMenuFont, m_mainMenuFontSize,
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
	m_mousePosChanged(false),
	m_mousePicker(),
	m_currentMenuEntry(NULL)
	//m_quitGameCommand()
#ifdef CALCULATE_GAME_STATS
	, m_classStats(STATS_STORAGE.GetClassStats("MenuGameState"))
#endif
{
	/**
	* TODO: Calculating the proper locations for the menu entries and updating these locations whenever the window is resized.
	*/
	Engine::MenuEntry* mainMenuOptionsMenuEntry = new Engine::CompositeMenuEntry("Options", &m_mainMenuFont, m_mainMenuFontSize, Math::Vector2D(0.25f, 0.4f), 0.5f,
		Math::Vector3D(1.0f, 0.0f, 0.0f), Math::Vector3D(0.0f, 1.0f, 0.0f), Math::Vector2D(0.005f, 0.005f), true);
	mainMenuOptionsMenuEntry->AddChild(new Engine::CompositeMenuEntry("Sound", &m_mainMenuFont, m_mainMenuFontSize, Math::Vector2D(0.25f, 0.25f), 0.5f,
		Math::Vector3D(1.0f, 0.0f, 0.0f), Math::Vector3D(0.0f, 1.0f, 0.0f), Math::Vector2D(0.005f, 0.005f), true));
	mainMenuOptionsMenuEntry->AddChild(new Engine::CompositeMenuEntry("Graphics", &m_mainMenuFont, m_mainMenuFontSize, Math::Vector2D(0.25f, 0.5f), 0.5f,
		Math::Vector3D(1.0f, 0.0f, 0.0f), Math::Vector3D(0.0f, 1.0f, 0.0f), Math::Vector2D(0.005f, 0.005f), true));
	mainMenuOptionsMenuEntry->AddChild(new Engine::CompositeMenuEntry("Controls", &m_mainMenuFont, m_mainMenuFontSize, Math::Vector2D(0.25f, 0.75f), 0.5f,
		Math::Vector3D(1.0f, 0.0f, 0.0f), Math::Vector3D(0.0f, 1.0f, 0.0f), Math::Vector2D(0.005f, 0.005f), true));
	m_mainMenuRootEntry.AddChild(new Engine::ActionMenuEntry(Engine::GameManager::GetGameManager()->GetCommand(Engine::Actions::START_GAME), "Start", &m_mainMenuFont,
		m_mainMenuFontSize, Math::Vector2D(0.25f, 0.2f), 0.5f, Math::Vector3D(1.0f, 0.0f, 0.0f), Math::Vector3D(0.0f, 1.0f, 0.0f), Math::Vector2D(0.005f, 0.005f), true));
	m_mainMenuRootEntry.AddChild(mainMenuOptionsMenuEntry);
	m_mainMenuRootEntry.AddChild(new Engine::ActionMenuEntry(Engine::GameManager::GetGameManager()->GetCommand(Engine::Actions::SHOW_INTRO), "Intro", &m_mainMenuFont,
		m_mainMenuFontSize, Math::Vector2D(0.25f, 0.6f), 0.5f, Math::Vector3D(1.0f, 0.0f, 0.0f), Math::Vector3D(0.0f, 1.0f, 0.0f), Math::Vector2D(0.005f, 0.005f), true));
	m_mainMenuRootEntry.AddChild(new Engine::ActionMenuEntry(Engine::GameManager::GetGameManager()->GetCommand(Engine::Actions::QUIT_GAME), "Quit", &m_mainMenuFont,
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

	Engine::CoreEngine::GetCoreEngine()->GetAudioEngine().LoadSoundEffect(Engine::CoreEngine::GetCoreEngine()->GetAudioDirectory() + "\\bounce.wav");

	DeselectAll();
	m_currentMenuEntry = m_mainMenuRootEntry.SelectChild(0); // TODO: Two assignments to the same variable. Fix that!
	//m_currentMenuEntry = &m_mainMenuRootEntry;

	//m_inputMapping.PushContext("MenuGameStateContext");
	//m_inputMapping.RegisterCallback(MenuGameStateInputCallback, 0);
	//Engine::CoreEngine::GetCoreEngine()->RegisterHandlerForAction(Engine::Input::Actions::ACTION_QUIT_GAME, m_quitGameCommand);
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
	Engine::CoreEngine::GetCoreEngine()->PushInputContext(m_inputMappingContextName);
	INFO_LOG_GAME("Menu game state has been placed in the game state manager");
}

void MenuGameState::Leaving()
{
	Engine::CoreEngine::GetCoreEngine()->PopInputContext();
	INFO_LOG_GAME("Menu game state is about to be removed from the game state manager");
}

void MenuGameState::Obscuring()
{
	Engine::CoreEngine::GetCoreEngine()->PopInputContext();
	INFO_LOG_GAME("Another game state is about to stack on top of menu game state");
}

void MenuGameState::Revealed()
{
	Engine::CoreEngine::GetCoreEngine()->PushInputContext(m_inputMappingContextName);
	INFO_LOG_GAME("Menu game state has become the topmost game state in the game state manager's stack");
}

void MenuGameState::Handle(Engine::Actions::Action action)
{
	switch (action)
	{
	case Engine::Actions::SELECT_PREVIOUS_MENU_ENTRY:
		m_currentMenuEntry = m_currentMenuEntry->GetParent()->SelectPrevChild(); // TODO: Is it possible that GetParent() == NULL?
		break;
	case Engine::Actions::SELECT_NEXT_MENU_ENTRY:
		m_currentMenuEntry = m_currentMenuEntry->GetParent()->SelectNextChild(); // TODO: Is it possible that GetParent() == NULL?
		break;
	case Engine::Actions::CHOOSE_CURRENT_MENU_ENTRY:
		m_currentMenuEntry = m_currentMenuEntry->Execute();
		break;
	case Engine::Actions::RETURN_TO_PARENT_MENU_ENTRY:
		if (m_currentMenuEntry->GetParent()->HasParent())
		{
			m_currentMenuEntry = m_currentMenuEntry->GetParent();
		}
		else
		{
			// TODO: Select the "Quit" menu entry?
		}
		break;
	default:
		DEBUG_LOG_GAME("The action %d is not supported by the MenuGameState", action);
		break;
	}
}

void MenuGameState::Handle(Engine::States::State state)
{
	//DELOCUST_LOG_GAME("Handling the state %d", state);
	switch (state)
	{
	case Engine::States::MOUSE_KEY_LEFT_PRESSED:
		if (m_currentMenuEntry->DoesMouseHoverOver(m_mousePosX, m_mousePosY))
		{
			CRITICAL_LOG_GAME("Handling the state %d", state);
			m_currentMenuEntry = m_currentMenuEntry->Execute();
		}
		break;
	default:
		DEBUG_LOG_GAME("The state %d is not supported by the MenuGameState", state);
		break;
	}
}

void MenuGameState::Handle(Engine::Ranges::Range range, Math::Real value)
{
	switch (range)
	{
	case Engine::Ranges::AXIS_X:
		m_mousePosX = value;
		break;
	case Engine::Ranges::AXIS_Y:
		m_mousePosY = value;
		break;
	default:
		DEBUG_LOG_GAME("The range %d is not supported by the MenuGameState", range);
		break;
	}
	SelectChild(m_currentMenuEntry->GetParent()->GetSelectedChildIndex(m_mousePosX, m_mousePosY));
}

void MenuGameState::DeselectAll()
{
	START_PROFILING;
	// This function is supposed to apply the "non-selected" effects to all non-selected menu entries. However Effect class can only handle one attribute at a time.
	// It is not possible to modify many attributes using one Effect class instance. As a result only the last non-selected menu entry will have "non-selected" effects applied to it.
	//for (int i = 0; i < m_currentMenuEntry->GetChildrenCount(); ++i)
	//{
	//	m_currentMenuEntry->SelectChildMenuEntry(i, false);
	//	//m_currentMenuEntry->GetSelectedChild()->ApplyBorderEdgeTransitionWidthEffect(m_notSelectedMenuEntryBorderEdgeTransitionWidthEffect);
	//	//m_currentMenuEntry->GetSelectedChild()->ApplyBorderWidthEffect(m_notSelectedMenuEntryBorderWidthEffect);
	//	//m_currentMenuEntry->GetSelectedChild()->ApplyCharacterEdgeTransitionWidthEffect(m_notSelectedMenuEntryCharacterEdgeTransitionWidthEffect);
	//	//m_currentMenuEntry->GetSelectedChild()->ApplyCharacterWidthEffect(m_notSelectedMenuEntryCharacterWidthEffect);
	//	//m_currentMenuEntry->GetSelectedChild()->ApplyColorEffect(m_notSelectedMenuEntryColorEffect);
	//	m_currentMenuEntry->GetSelectedChild()->ApplyOffsetEffect(m_notSelectedMenuEntryOffsetEffect);
	//	//m_currentMenuEntry->GetSelectedChild()->ApplyOutlineColorEffect(m_notSelectedMenuEntryOutlineColorEffect);
	//}
	STOP_PROFILING;
}

void MenuGameState::SelectChild(size_t childIndex)
{
	CHECK_CONDITION_RETURN_VOID_ALWAYS_GAME(m_currentMenuEntry->GetParent()->GetSelectedChildIndex() != childIndex, Utility::Logging::DEBUG, "Trying to select the child which is already selected (%d).", childIndex);
	////m_currentMenuEntry->GetSelectedChild()->ApplyOffsetEffect(m_notSelectedMenuEntryOffsetEffect);
	m_currentMenuEntry = m_currentMenuEntry->GetParent()->SelectChild(childIndex);
	//m_currentMenuEntry->SelectChildMenuEntry(childIndex);
	////m_currentMenuEntry->GetSelectedChild()->ApplyColorEffect(m_selectedMenuEntryColorEffect);
	////m_currentMenuEntry->GetSelectedChild()->ApplyOutlineColorEffect(m_selectedMenuEntryOutlineColorEffect);
	////m_currentMenuEntry->GetSelectedChild()->ApplyOffsetEffect(m_selectedMenuEntryOffsetEffect);
	////m_currentMenuEntry->GetSelectedChild()->ApplyCharacterWidthEffect(m_selectedMenuEntryCharacterWidthEffect);
	////m_currentMenuEntry->GetSelectedChild()->ApplyCharacterEdgeTransitionWidthEffect(m_selectedMenuEntryCharacterEdgeTransitionWidthEffect);
	////m_currentMenuEntry->GetSelectedChild()->ApplyBorderWidthEffect(m_selectedMenuEntryBorderWidthEffect);
	////m_currentMenuEntry->GetSelectedChild()->ApplyBorderEdgeTransitionWidthEffect(m_selectedMenuEntryBorderEdgeTransitionWidthEffect);
	Engine::CoreEngine::GetCoreEngine()->GetAudioEngine().PlaySoundEffect(Engine::CoreEngine::GetCoreEngine()->GetAudioDirectory() + "\\bounce.wav", 1.0f, 1.0f);
	DEBUG_LOG_GAME("Selected menu entry changed to %d", childIndex);
}

void MenuGameState::Render(const Rendering::Shader* shader, Rendering::Renderer* renderer) const
{
	START_PROFILING;
	DELOCUST_LOG_GAME("MAIN MENU game state rendering");

	renderer->BindAsRenderTarget();
	renderer->ClearScreen(/* TODO: specify menu game state clear screen color */);
	m_currentMenuEntry->GetParent()->RenderAll(renderer);

	STOP_PROFILING;
}

//void MenuGameState::MouseButtonEvent(int button, int action, int mods)
//{
//	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
//	{
//		if (m_currentMenuEntry->GetSelectedChild()->DoesMouseHoverOver(m_mousePosX, m_mousePosY))
//		{
//			m_currentMenuEntry = m_currentMenuEntry->Execute();
//		}
//		else
//		{
//			//CRITICAL_LOG_GAME("Does not hover (%.2f, %.2f)", m_mousePosX, m_mousePosY);
//		}
//	}
//}

//void MenuGameState::MousePosEvent(double xPos, double yPos)
//{
//	//const Rendering::CameraBase& currentCamera = Engine::CoreEngine::GetCoreEngine()->GetRenderer()->GetCurrentCamera();
//	//m_mousePicker.CalculateCurrentRay(xPos, yPos, currentCamera.GetProjection(), currentCamera.GetViewMatrix());
//
//	m_mousePosX = static_cast<Math::Real>(xPos);
//	m_mousePosY = static_cast<Math::Real>(yPos);
//
//	const int menuEntryChildrenCount = m_currentMenuEntry->GetChildrenCount();
//	//EMERGENCY_LOG_GAME("Menu mouse position event (%.2f, %.2f)", m_mousePosX, m_mousePosY);
//	for (int i = 0; i < menuEntryChildrenCount; ++i)
//	{
//		if (m_currentMenuEntry->DoesMouseHoverOverChild(i, m_mousePosX, m_mousePosY))
//		{
//			if (m_currentMenuEntry->GetSelectedMenuEntryIndex() != i)
//			{
//				INFO_LOG_GAME("Menu entry \"%s\" selected", m_currentMenuEntry->GetChildGuiText(i).GetText().c_str());
//			}
//			SelectChild(i);
//			break;
//		}
//	}
//}

//void MenuGameState::ScrollEvent(double xOffset, double yOffset)
//{
//}

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