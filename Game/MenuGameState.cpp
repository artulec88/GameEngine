#include "MenuGameState.h"
#include "Def.h"
#include "PlayGameState.h"
#include "LoadGameState.h"
#include "StartGameCommand.h"

#include "Engine\CoreEngine.h"
#include "Engine\GameManager.h"
#include "Engine\GameCommand.h"

#include "Rendering\Shader.h"

#include "Math\Effect_impl.h"

#include "Utility\ILogger.h"
#include "Utility\IConfig.h"

Game::MenuGameState::MenuGameState(Engine::GameManager* gameManager, const std::string& inputMappingContextName, const Rendering::Text::Font* mainMenuFont, Math::Real mainMenuFontSize) :
	Engine::GameState(inputMappingContextName),
	m_gameManager(gameManager),
	m_mainMenuRootEntry("Main menu", mainMenuFont, mainMenuFontSize, NULL,
		Math::Vector2D(0.0f, 0.0f), 1.0f, Rendering::Color(Rendering::ColorNames::BLACK), Rendering::Color(Rendering::ColorNames::BLACK), Math::Vector2D(REAL_ZERO, REAL_ZERO)),
	m_notSelectedMenuEntryColorEffect(std::make_unique<Math::Effects::NoEffect<Rendering::Color>>(Rendering::Color(GET_CONFIG_VALUE_GAME("mainMenuNotSelectedEntryColorRed", 1.0f),
		GET_CONFIG_VALUE_GAME("mainMenuNotSelectedEntryColorGreen", 1.0f), GET_CONFIG_VALUE_GAME("mainMenuNotSelectedEntryColorBlue", 1.0f),
		GET_CONFIG_VALUE_GAME("mainMenuNotSelectedEntryColorAlpha", 1.0f)))),
	m_selectedMenuEntryColorEffect(std::make_unique<Math::Effects::SmoothTransitionEffect<Rendering::Color>>(
		std::vector<Rendering::Color>{
			Rendering::Color(GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryColorRed_1", 1.0f), GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryColorGreen_1", 0.0f), GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryColorBlue_1", 0.0f)),
			Rendering::Color(GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryColorRed_2", 0.0f), GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryColorGreen_2", 1.0f), GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryColorBlue_2", 0.0f)),
			Rendering::Color(GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryColorRed_3", 0.0f), GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryColorGreen_3", 0.0f), GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryColorBlue_3", 1.0f)) }.data(),
		std::vector<Math::Real>{ GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryColorTime_1", 0.0f), GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryColorTime_2", 1.0f), GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryColorTime_3", 2.0f) }.data(), 3, false)),
	m_notSelectedMenuEntryOutlineColorEffect(std::make_unique<Math::Effects::NoEffect<Rendering::Color>>(Rendering::Color(GET_CONFIG_VALUE_GAME("mainMenuNotSelectedEntryOutlineColorRed", 1.0f),
		GET_CONFIG_VALUE_GAME("mainMenuNotSelectedEntryOutlineColorGreen", 1.0f), GET_CONFIG_VALUE_GAME("mainMenuNotSelectedEntryOutlineColorBlue", 1.0f),
		GET_CONFIG_VALUE_GAME("mainMenuNotSelectedEntryOutlineColorAlpha", 1.0f)))),
		m_selectedMenuEntryOutlineColorEffect(std::make_unique<Math::Effects::BlinkEffect<Rendering::Color>>(
			std::vector<Rendering::Color>{
				Rendering::Color(GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryOutlineColorRed_1", 1.0f), GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryOutlineColorGreen_1", 0.0f), GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryOutlineColorBlue_1", 0.0f)),
				Rendering::Color(GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryOutlineColorRed_2", 0.0f), GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryOutlineColorGreen_2", 1.0f), GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryOutlineColorBlue_2", 0.0f)),
				Rendering::Color(GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryOutlineColorRed_3", 0.0f), GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryOutlineColorGreen_3", 0.0f), GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryOutlineColorBlue_3", 1.0f)) }.data(),
			std::vector<Math::Real>{ GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryOutlineColorDuration_1", 1.0f), GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryOutlineColorDuration_2", 1.0f), GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryOutlineColorDuration_3", 2.0f) }.data(), 3)),
		m_notSelectedMenuEntryOffsetEffect(std::make_unique<Math::Effects::NoEffect<Math::Vector2D>>(Math::Vector2D(GET_CONFIG_VALUE_GAME("mainMenuNotSelectedEntryOffsetX", 0.0f), GET_CONFIG_VALUE_GAME("mainMenuNotSelectedEntryOffsetY", 0.0f)))),
			m_selectedMenuEntryOffsetEffect(std::make_unique<Math::Effects::SmoothTransitionEffect<Math::Vector2D>>(
				std::vector<Math::Vector2D>{
					Math::Vector2D(GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryOffsetX_1", -0.015f), GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryOffsetY_1", 0.015f)),
					Math::Vector2D(GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryOffsetX_2", 0.015f), GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryOffsetY_2", 0.015f)),
					Math::Vector2D(GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryOffsetX_3", 0.015f), GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryOffsetY_3", -0.015f)),
					Math::Vector2D(GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryOffsetX_4", -0.015f), GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryOffsetY_3", -0.015f)),
					Math::Vector2D(GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryOffsetX_5", -0.015f), GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryOffsetY_5", 0.015f)) }.data(),
				std::vector<Math::Real>{
						GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryOffsetTime_1", 0.0f), GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryOffsetTime_2", 0.75f),
						GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryOffsetTime_3", 1.5f), GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryOffsetTime_4", 2.25f),
						GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryOffsetTime_5", 3.0f)}.data(), 5, true)),
			m_notSelectedMenuEntryCharacterWidthEffect(std::make_unique<Math::Effects::NoEffect<Math::Real>>(GET_CONFIG_VALUE_GAME("mainMenuNotSelectedEntryCharacterWidth", 0.5f))),
			m_selectedMenuEntryCharacterWidthEffect(std::make_unique<Math::Effects::SmoothTransitionEffect<Math::Real>>(
				std::vector<Math::Real>{
					GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryCharacterWidth_1", 0.4f), GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryCharacterWidth_2", 0.45f),
					GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryCharacterWidth_3", 0.5f), GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryCharacterWidth_4", 0.55f),
					GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryCharacterWidth_5", 0.6f)}.data(),
				std::vector<Math::Real>{
						GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryCharacterWidthTime_1", 0.0f),
						GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryCharacterWidthTime_2", 0.2f),
						GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryCharacterWidthTime_3", 0.4f),
						GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryCharacterWidthTime_4", 0.6f),
						GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryCharacterWidthTime_5", 0.8f) }.data(), 5, false)),
			m_notSelectedMenuEntryCharacterEdgeTransitionWidthEffect(std::make_unique<Math::Effects::NoEffect<Math::Real>>(GET_CONFIG_VALUE_GAME("mainMenuNotSelectedEntryCharacterEdgeTransitionWidth", 0.1f))),
			m_selectedMenuEntryCharacterEdgeTransitionWidthEffect(std::make_unique<Math::Effects::SmoothTransitionEffect<Math::Real>>(
				std::vector<Math::Real>{
					GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryCharacterEdgeTransitionWidth_1", 0.0f),
					GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryCharacterEdgeTransitionWidth_2", 0.1f),
					GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryCharacterEdgeTransitionWidth_3", 0.2f),
					GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryCharacterEdgeTransitionWidth_4", 0.3f),
					GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryCharacterEdgeTransitionWidth_5", 0.4f) }.data(),
				std::vector<Math::Real>{
					GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryCharacterEdgeTransitionWidthTime_1", 0.0f),
					GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryCharacterEdgeTransitionWidthTime_2", 1.5f),
					GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryCharacterEdgeTransitionWidthTime_3", 3.0f),
					GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryCharacterEdgeTransitionWidthTime_4", 4.5f),
					GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryCharacterEdgeTransitionWidthTime_5", 6.0f) }.data(), 5, true)),
			m_notSelectedMenuEntryBorderWidthEffect(std::make_unique<Math::Effects::NoEffect<Math::Real>>(GET_CONFIG_VALUE_GAME("mainMenuNotSelectedEntryBorderWidth", 0.4f))),
			m_selectedMenuEntryBorderWidthEffect(std::make_unique<Math::Effects::SmoothTransitionEffect<Math::Real>>(
				std::vector<Math::Real>{
					GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryBorderWidth_1", 0.0f),
					GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryBorderWidth_2", 0.12f),
					GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryBorderWidth_3", 0.24f),
					GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryBorderWidth_4", 0.36f),
					GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryBorderWidth_5", 0.48f) }.data(),
				std::vector<Math::Real>{
					GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryBorderWidthTime_1", 0.0f),
					GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryBorderWidthTime_2", 1.0f),
					GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryBorderWidthTime_3", 2.0f),
					GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryBorderWidthTime_4", 3.0f),
					GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryBorderWidthTime_5", 4.0f) }.data(), 5, false)),
			m_notSelectedMenuEntryBorderEdgeTransitionWidthEffect(std::make_unique<Math::Effects::NoEffect<Math::Real>>(GET_CONFIG_VALUE_GAME("mainMenuNotSelectedEntryBorderEdgeTransitionWidth", 0.1f))),
			m_selectedMenuEntryBorderEdgeTransitionWidthEffect(std::make_unique<Math::Effects::SmoothTransitionEffect<Math::Real>>(
				std::vector<Math::Real>{
					GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryBorderEdgeTransitionWidth_1", 0.0f),
					GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryBorderEdgeTransitionWidth_2", 0.1f),
					GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryBorderEdgeTransitionWidth_3", 0.2f),
					GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryBorderEdgeTransitionWidth_4", 0.3f),
					GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryBorderEdgeTransitionWidth_5", 0.4f) }.data(),
				std::vector<Math::Real>{
					GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryBorderEdgeTransitionWidthTime_1", 0.0f),
					GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryBorderEdgeTransitionWidthTime_2", 0.1f),
					GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryBorderEdgeTransitionWidthTime_3", 0.2f),
					GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryBorderEdgeTransitionWidthTime_4", 0.3f),
					GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryBorderEdgeTransitionWidthTime_5", 0.4f) }.data(), 5, false)),
			m_mousePosX(REAL_ZERO),
			m_mousePosY(REAL_ZERO),
			m_mousePosChanged(false),
			m_mousePicker(),
			m_currentMenuEntry(NULL)
#ifdef PROFILING_GAME_MODULE_ENABLED
			, m_classStats(STATS_STORAGE.GetClassStats("MenuGameState"))
#endif
{
	/**
	* TODO: Calculating the proper locations for the menu entries and updating these locations whenever the window is resized.
	*/
	Engine::MenuEntry* mainMenuLoadMenuEntry = new Engine::CompositeMenuEntry("Load", mainMenuFont, mainMenuFontSize, NULL, Math::Vector2D(0.25f, 0.34f), 0.5f,
		Rendering::Color(Rendering::ColorNames::RED), Rendering::Color(Rendering::ColorNames::GREEN), Math::Vector2D(0.005f, 0.005f), true);
	// TODO: Populate "Load" menu entry with children.
	Engine::MenuEntry* mainMenuOptionsMenuEntry = new Engine::CompositeMenuEntry("Options", mainMenuFont, mainMenuFontSize, NULL, Math::Vector2D(0.25f, 0.51f), 0.5f,
		Rendering::Color(Rendering::ColorNames::RED), Rendering::Color(Rendering::ColorNames::GREEN), Math::Vector2D(0.005f, 0.005f), true);
	mainMenuOptionsMenuEntry->AddChild(new Engine::CompositeMenuEntry("Sound", mainMenuFont, mainMenuFontSize, NULL, Math::Vector2D(0.25f, 0.2f), 0.5f,
		Rendering::Color(Rendering::ColorNames::RED), Rendering::Color(Rendering::ColorNames::GREEN), Math::Vector2D(0.005f, 0.005f), true));
	mainMenuOptionsMenuEntry->AddChild(new Engine::CompositeMenuEntry("Graphics", mainMenuFont, mainMenuFontSize, NULL, Math::Vector2D(0.25f, 0.4f), 0.5f,
		Rendering::Color(Rendering::ColorNames::RED), Rendering::Color(Rendering::ColorNames::GREEN), Math::Vector2D(0.005f, 0.005f), true));
	mainMenuOptionsMenuEntry->AddChild(new Engine::CompositeMenuEntry("Controls", mainMenuFont, mainMenuFontSize, NULL, Math::Vector2D(0.25f, 0.6f), 0.5f,
		Rendering::Color(Rendering::ColorNames::RED), Rendering::Color(Rendering::ColorNames::GREEN), Math::Vector2D(0.005f, 0.005f), true));
	mainMenuOptionsMenuEntry->AddChild(new Engine::ActionMenuEntry(Engine::Actions::RETURN_TO_PARENT_MENU_ENTRY, "Back", mainMenuFont,
		mainMenuFontSize, NULL, Math::Vector2D(0.25f, 0.8f), 0.5f, Rendering::Color(Rendering::ColorNames::RED), Rendering::Color(Rendering::ColorNames::GREEN), Math::Vector2D(0.005f, 0.005f), true));
	m_mainMenuRootEntry.AddChild(new Engine::ActionMenuEntry(Engine::Actions::START_GAME, "New game", mainMenuFont,
		mainMenuFontSize, NULL /*m_gameManager->GetTextureFactory().GetTexture("check-297273_960_720.png")*/, Math::Vector2D(0.25f, 0.17f), 0.5f, Rendering::Color(Rendering::ColorNames::RED), Rendering::Color(Rendering::ColorNames::GREEN), Math::Vector2D(0.005f, 0.005f), true));
	m_mainMenuRootEntry.AddChild(mainMenuLoadMenuEntry);
	m_mainMenuRootEntry.AddChild(mainMenuOptionsMenuEntry);
	m_mainMenuRootEntry.AddChild(new Engine::ActionMenuEntry(Engine::Actions::SHOW_INTRO, "Intro", mainMenuFont,
		mainMenuFontSize, NULL, Math::Vector2D(0.25f, 0.68f), 0.5f, Rendering::Color(Rendering::ColorNames::RED), Rendering::Color(Rendering::ColorNames::GREEN), Math::Vector2D(0.005f, 0.005f), true));
	m_mainMenuRootEntry.AddChild(new Engine::ActionMenuEntry(Engine::Actions::QUIT_GAME, "Quit", mainMenuFont,
		mainMenuFontSize, NULL, Math::Vector2D(0.25f, 0.85f), 0.5f, Rendering::Color(Rendering::ColorNames::RED), Rendering::Color(Rendering::ColorNames::GREEN), Math::Vector2D(0.005f, 0.005f), true));

	Engine::CoreEngine::GetCoreEngine()->GetAudioEngine().LoadSoundEffect(Engine::CoreEngine::GetCoreEngine()->GetAudioDirectory() + "\\bounce.wav");

	DeselectAll();
	m_currentMenuEntry = m_mainMenuRootEntry.SelectChild(0); // TODO: Two assignments to the same variable. Fix that!
	//m_currentMenuEntry = &m_mainMenuRootEntry;

	//m_inputMapping.PushContext("MenuGameStateContext");
	//m_inputMapping.RegisterCallback(MenuGameStateInputCallback, 0);
	//Engine::CoreEngine::GetCoreEngine()->RegisterHandlerForAction(Engine::Input::Actions::ACTION_QUIT_GAME, m_quitGameCommand);
}

Game::MenuGameState::~MenuGameState(void)
{
}

void Game::MenuGameState::Entered()
{
	Engine::CoreEngine::GetCoreEngine()->PushInputContext(m_inputMappingContextName);
	INFO_LOG_GAME("Menu game state has been placed in the game state manager");
}

void Game::MenuGameState::Leaving()
{
	Engine::CoreEngine::GetCoreEngine()->PopInputContext();
	INFO_LOG_GAME("Menu game state is about to be removed from the game state manager");
}

void Game::MenuGameState::Obscuring()
{
	Engine::CoreEngine::GetCoreEngine()->PopInputContext();
	INFO_LOG_GAME("Another game state is about to stack on top of menu game state");
}

void Game::MenuGameState::Revealed()
{
	Engine::CoreEngine::GetCoreEngine()->PushInputContext(m_inputMappingContextName);
	INFO_LOG_GAME("Menu game state has become the topmost game state in the game state manager's stack");
}

void Game::MenuGameState::Handle(Engine::Actions::Action action)
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
		m_currentMenuEntry->Dispatch();
		break;
	case Engine::Actions::GO_TO_CHILD_MENU_ENTRY:
		m_currentMenuEntry = m_currentMenuEntry->GoTo();
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
		DEBUG_LOG_GAME("The action ", action, " is not supported by the MenuGameState");
		break;
	}
}

void Game::MenuGameState::Handle(Engine::States::State state)
{
	//DELOCUST_LOG_GAME("Handling the state ", state);
	switch (state)
	{
	case Engine::States::MOUSE_KEY_LEFT_PRESSED:
		if (m_currentMenuEntry->DoesMouseHoverOver(m_mousePosX, m_mousePosY))
		{
			m_currentMenuEntry->Dispatch();
		}
		break;
	default:
		DEBUG_LOG_GAME("The state ", state, " is not supported by the MenuGameState");
		break;
	}
}

void Game::MenuGameState::Handle(Engine::Ranges::Range range, Math::Real value)
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
		DEBUG_LOG_GAME("The range ", range, " is not supported by the MenuGameState");
		break;
	}
	SelectChild(m_currentMenuEntry->GetParent()->GetSelectedChildIndex(m_mousePosX, m_mousePosY));
}

void Game::MenuGameState::DeselectAll()
{
	START_PROFILING_GAME(true, "");
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
	STOP_PROFILING_GAME("");
}

void Game::MenuGameState::SelectChild(size_t childIndex)
{
	CHECK_CONDITION_RETURN_VOID_ALWAYS_GAME(m_currentMenuEntry->GetParent()->GetSelectedChildIndex() != childIndex, Utility::Logging::DELOCUST,
		"Trying to select the child which is already selected (", childIndex, ").");
	m_currentMenuEntry = m_currentMenuEntry->GetParent()->SelectChild(childIndex);
	Engine::CoreEngine::GetCoreEngine()->GetAudioEngine().PlaySoundEffect(Engine::CoreEngine::GetCoreEngine()->GetAudioDirectory() + "\\bounce.wav", 1.0f, 1.0f);
	ERROR_LOG_GAME("Selected menu entry changed to ", childIndex);
}

void Game::MenuGameState::Render(Rendering::Renderer* renderer) const
{
	START_PROFILING_GAME(true, "");
	DELOCUST_LOG_GAME("MAIN MENU game state rendering");

	renderer->BindAsRenderTarget();
	renderer->ClearScreen(/* TODO: specify menu game state clear screen color */);
	m_currentMenuEntry->GetParent()->RenderAll(renderer, m_gameManager->GetShaderFactory().GetShader(Engine::ShaderTypes::GUI));
	//renderer->FinalizeRenderScene();

	STOP_PROFILING_GAME("");
}

//void Game::MenuGameState::MouseButtonEvent(int button, int action, int mods)
//{
//	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
//	{
//		if (m_currentMenuEntry->GetSelectedChild()->DoesMouseHoverOver(m_mousePosX, m_mousePosY))
//		{
//			m_currentMenuEntry = m_currentMenuEntry->Execute();
//		}
//		else
//		{
//			//CRITICAL_LOG_GAME("Does not hover (", m_mousePosX, ", ", m_mousePosY, ")");
//		}
//	}
//}

//void Game::MenuGameState::MousePosEvent(double xPos, double yPos)
//{
//	//const Rendering::Camera& currentCamera = Engine::CoreEngine::GetCoreEngine()->GetRenderer()->GetCurrentCamera();
//	//m_mousePicker.CalculateCurrentRay(xPos, yPos, currentCamera.GetProjection(), currentCamera.GetViewMatrix());
//
//	m_mousePosX = static_cast<Math::Real>(xPos);
//	m_mousePosY = static_cast<Math::Real>(yPos);
//
//	const int menuEntryChildrenCount = m_currentMenuEntry->GetChildrenCount();
//	//EMERGENCY_LOG_GAME("Menu mouse position event (", m_mousePosX, ", ", m_mousePosY, ")");
//	for (int i = 0; i < menuEntryChildrenCount; ++i)
//	{
//		if (m_currentMenuEntry->DoesMouseHoverOverChild(i, m_mousePosX, m_mousePosY))
//		{
//			if (m_currentMenuEntry->GetSelectedMenuEntryIndex() != i)
//			{
//				INFO_LOG_GAME("Menu entry \"", m_currentMenuEntry->GetChildGuiText(i).GetText(), "\" selected");
//			}
//			SelectChild(i);
//			break;
//		}
//	}
//}

//void MenuGameState::ScrollEvent(double xOffset, double yOffset)
//{
//}

void Game::MenuGameState::Update(Math::Real deltaTime)
{
	//m_selectedMenuEntryColorEffect->Update(m_currentMenuEntry->GetSelectedChild()->GetGuiText(), deltaTime);
	if (m_notSelectedMenuEntryColorEffect != NULL)
	{
		m_notSelectedMenuEntryColorEffect->Update(deltaTime);
		m_currentMenuEntry->GetParent()->ApplyColorEffectToAll(*m_notSelectedMenuEntryColorEffect);
	}
	if (m_selectedMenuEntryColorEffect != NULL)
	{
		m_selectedMenuEntryColorEffect->Update(deltaTime);
		m_currentMenuEntry->ApplyColorEffect(*m_selectedMenuEntryColorEffect);
	}
	if (m_notSelectedMenuEntryOutlineColorEffect != NULL)
	{
		m_notSelectedMenuEntryOutlineColorEffect->Update(deltaTime);
		m_currentMenuEntry->GetParent()->ApplyOutlineColorEffectToAll(*m_notSelectedMenuEntryOutlineColorEffect);
	}
	if (m_selectedMenuEntryOutlineColorEffect != NULL)
	{
		m_selectedMenuEntryOutlineColorEffect->Update(deltaTime);
		m_currentMenuEntry->ApplyOutlineColorEffect(*m_selectedMenuEntryOutlineColorEffect);
	}
	if (m_notSelectedMenuEntryOffsetEffect != NULL)
	{
		m_notSelectedMenuEntryOffsetEffect->Update(deltaTime);
		m_currentMenuEntry->GetParent()->ApplyOffsetEffectToAll(*m_notSelectedMenuEntryOffsetEffect);
	}
	if (m_selectedMenuEntryOffsetEffect != NULL)
	{
		m_selectedMenuEntryOffsetEffect->Update(deltaTime);
		m_currentMenuEntry->ApplyOffsetEffect(*m_selectedMenuEntryOffsetEffect);
	}
	if (m_notSelectedMenuEntryCharacterWidthEffect != NULL)
	{
		m_notSelectedMenuEntryCharacterWidthEffect->Update(deltaTime);
		m_currentMenuEntry->GetParent()->ApplyCharacterWidthEffectToAll(*m_notSelectedMenuEntryCharacterWidthEffect);
	}
	if (m_selectedMenuEntryCharacterWidthEffect != NULL)
	{
		m_selectedMenuEntryCharacterWidthEffect->Update(deltaTime);
		m_currentMenuEntry->ApplyCharacterWidthEffect(*m_selectedMenuEntryCharacterWidthEffect);
	}
	if (m_notSelectedMenuEntryCharacterEdgeTransitionWidthEffect != NULL)
	{
		m_notSelectedMenuEntryCharacterEdgeTransitionWidthEffect->Update(deltaTime);
		m_currentMenuEntry->GetParent()->ApplyCharacterEdgeTransitionWidthEffectToAll(*m_notSelectedMenuEntryCharacterEdgeTransitionWidthEffect);
	}
	if (m_selectedMenuEntryCharacterEdgeTransitionWidthEffect != NULL)
	{
		m_selectedMenuEntryCharacterEdgeTransitionWidthEffect->Update(deltaTime);
		m_currentMenuEntry->ApplyCharacterEdgeTransitionWidthEffect(*m_selectedMenuEntryCharacterEdgeTransitionWidthEffect);
	}
	if (m_notSelectedMenuEntryBorderWidthEffect != NULL)
	{
		m_notSelectedMenuEntryBorderWidthEffect->Update(deltaTime);
		m_currentMenuEntry->GetParent()->ApplyBorderWidthEffectToAll(*m_notSelectedMenuEntryBorderWidthEffect);
	}
	if (m_selectedMenuEntryBorderWidthEffect != NULL)
	{
		m_selectedMenuEntryBorderWidthEffect->Update(deltaTime);
		m_currentMenuEntry->ApplyBorderWidthEffect(*m_selectedMenuEntryBorderWidthEffect);
	}
	if (m_notSelectedMenuEntryBorderEdgeTransitionWidthEffect != NULL)
	{
		m_notSelectedMenuEntryBorderEdgeTransitionWidthEffect->Update(deltaTime);
		m_currentMenuEntry->GetParent()->ApplyBorderEdgeTransitionWidthEffectToAll(*m_notSelectedMenuEntryBorderEdgeTransitionWidthEffect);
	}
	if (m_selectedMenuEntryBorderEdgeTransitionWidthEffect != NULL)
	{
		m_selectedMenuEntryBorderEdgeTransitionWidthEffect->Update(deltaTime);
		m_currentMenuEntry->ApplyBorderEdgeTransitionWidthEffect(*m_selectedMenuEntryBorderEdgeTransitionWidthEffect);
	}
}