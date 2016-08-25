#include "PlayMenuGameState.h"
#include "Def.h"
#include "Engine\GameManager.h"
#include "Engine\GameCommand.h"
#include "Engine\CoreEngine.h"
#include "Rendering\Shader.h"
#include "Utility\ILogger.h"
#include "Utility\IConfig.h"
#include "PlayGameState.h"
#include "ResumeGameCommand.h"

Game::PlayMenuGameState::PlayMenuGameState(Engine::GameManager* gameManager, const std::string& inputMappingContextName, const Rendering::Text::Font* playMainMenuFont, Math::Real playMainMenuFontSize) :
	Engine::GameState(inputMappingContextName),
	m_gameManager(gameManager),
	m_playMainMenuRootEntry("Play main menu", playMainMenuFont, playMainMenuFontSize, NULL, Math::Vector2D(0.0f, 0.0f), 1.0f,
		Rendering::Color(Rendering::ColorNames::BLACK), Rendering::Color(Rendering::ColorNames::BLACK), Math::Vector2D(REAL_ZERO, REAL_ZERO)),
	//m_notSelectedMenuEntryColorEffect(Engine::GameManager::GetGameManager()->GetVec3DEffect(Rendering::Effects::STATIC, 2)),
	//m_selectedMenuEntryColorEffect(Engine::GameManager::GetGameManager()->GetVec3DEffect(Rendering::Effects::BLINK, 2)),
	//m_notSelectedMenuEntryOutlineColorEffect(Engine::GameManager::GetGameManager()->GetVec3DEffect(Rendering::Effects::STATIC, 1)),
	//m_selectedMenuEntryOutlineColorEffect(Engine::GameManager::GetGameManager()->GetVec3DEffect(Rendering::Effects::SMOOTH, 2)),
	//m_notSelectedMenuEntryOffsetEffect(Engine::GameManager::GetGameManager()->GetVec2DEffect(Rendering::Effects::STATIC, 1)),
	//m_selectedMenuEntryOffsetEffect(Engine::GameManager::GetGameManager()->GetVec2DEffect(Rendering::Effects::SMOOTH, 2)),
	m_mousePosX(REAL_ZERO),
	m_mousePosY(REAL_ZERO),
	m_currentMenuEntry(NULL)
#ifdef PROFILING_GAME_MODULE_ENABLED
	,m_classStats(STATS_STORAGE.GetClassStats("PlayMenuGameState"))
#endif
{
	/**
	* TODO: Calculating the proper locations for the menu entries and updating these locations whenever the window is resized.
	*/
	Engine::MenuEntry* playMenuOptionsMenuEntry = new Engine::CompositeMenuEntry("Options", playMainMenuFont, playMainMenuFontSize, NULL, Math::Vector2D(0.25f, 0.7f), 0.5f,
		Rendering::Color(1.0f, 0.0f, 0.0f), Rendering::Color(0.0f, 1.0f, 0.0f), Math::Vector2D(0.005f, 0.005f), true);
	playMenuOptionsMenuEntry->AddChild(new Engine::CompositeMenuEntry("Sound", playMainMenuFont, playMainMenuFontSize, NULL, Math::Vector2D(0.25f, 0.25f), 0.5f,
		Rendering::Color(1.0f, 0.0f, 0.0f), Rendering::Color(0.0f, 1.0f, 0.0f), Math::Vector2D(0.005f, 0.005f), true));
	playMenuOptionsMenuEntry->AddChild(new Engine::CompositeMenuEntry("Graphics", playMainMenuFont, playMainMenuFontSize, NULL, Math::Vector2D(0.25f, 0.5f), 0.5f,
		Rendering::Color(1.0f, 0.0f, 0.0f), Rendering::Color(0.0f, 1.0f, 0.0f), Math::Vector2D(0.005f, 0.005f), true));
	playMenuOptionsMenuEntry->AddChild(new Engine::CompositeMenuEntry("Controls", playMainMenuFont, playMainMenuFontSize, NULL, Math::Vector2D(0.25f, 0.75f), 0.5f,
		Rendering::Color(1.0f, 0.0f, 0.0f), Rendering::Color(0.0f, 1.0f, 0.0f), Math::Vector2D(0.005f, 0.005f), true));
	m_playMainMenuRootEntry.AddChild(playMenuOptionsMenuEntry);
	m_playMainMenuRootEntry.AddChild(new Engine::ActionMenuEntry(Engine::GameManager::GetGameManager()->GetCommand(Engine::Actions::RESUME_GAME), "Resume", playMainMenuFont,
		playMainMenuFontSize, NULL, Math::Vector2D(0.25f, 0.1f), 0.5f, Rendering::Color(1.0f, 0.0f, 0.0f), Rendering::Color(0.0f, 1.0f, 0.0f), Math::Vector2D(0.005f, 0.005f), true));
	m_playMainMenuRootEntry.AddChild(new Engine::ActionMenuEntry(Engine::GameManager::GetGameManager()->GetCommand(Engine::Actions::SAVE_GAME), "Save",playMainMenuFont,
		playMainMenuFontSize, NULL, Math::Vector2D(0.25f, 0.3f), 0.5f, Rendering::Color(1.0f, 0.0f, 0.0f), Rendering::Color(0.0f, 1.0f, 0.0f), Math::Vector2D(0.005f, 0.005f), true));
	m_playMainMenuRootEntry.AddChild(new Engine::ActionMenuEntry(Engine::GameManager::GetGameManager()->GetCommand(Engine::Actions::LOAD_GAME), "Load",playMainMenuFont,
		playMainMenuFontSize, NULL, Math::Vector2D(0.25f, 0.5f), 0.5f, Rendering::Color(1.0f, 0.0f, 0.0f), Rendering::Color(0.0f, 1.0f, 0.0f), Math::Vector2D(0.005f, 0.005f), true));
	m_playMainMenuRootEntry.AddChild(new Engine::ActionMenuEntry(Engine::GameManager::GetGameManager()->GetCommand(Engine::Actions::QUIT_GAME), "Quit", playMainMenuFont,
		playMainMenuFontSize, NULL, Math::Vector2D(0.25f, 0.9f), 0.5f, Rendering::Color(1.0f, 0.0f, 0.0f), Rendering::Color(0.0f, 1.0f, 0.0f), Math::Vector2D(0.005f, 0.005f), true));

	m_currentMenuEntry = m_playMainMenuRootEntry.SelectChild(0);
}

Game::PlayMenuGameState::~PlayMenuGameState(void)
{
}

void Game::PlayMenuGameState::Entered()
{
	Engine::CoreEngine::GetCoreEngine()->PushInputContext(m_inputMappingContextName);
	INFO_LOG_GAME("PlayMenu game state has been placed in the game state manager");
}

void Game::PlayMenuGameState::Leaving()
{
	Engine::CoreEngine::GetCoreEngine()->PopInputContext();
	INFO_LOG_GAME("PlayMenu game state is about to be removed from the game state manager");
}

void Game::PlayMenuGameState::Obscuring()
{
	Engine::CoreEngine::GetCoreEngine()->PopInputContext();
	INFO_LOG_GAME("Another game state is about to stack on top of PlayMenu game state");
}

void Game::PlayMenuGameState::Revealed()
{
	Engine::CoreEngine::GetCoreEngine()->PushInputContext(m_inputMappingContextName);
	INFO_LOG_GAME("PlayMenu game state has become the topmost game state in the game state manager's stack");
}

void Game::PlayMenuGameState::Handle(Engine::Actions::Action action)
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
		if (m_currentMenuEntry->HasParent())
		{
			m_currentMenuEntry = m_currentMenuEntry->GetParent();
		}
		else
		{
			Engine::GameManager::GetGameManager()->PopState();
		}
		break;
	default:
		DEBUG_LOG_GAME("The action ", action, " is not supported by the MenuGameState");
		break;
	}
}

void Game::PlayMenuGameState::Handle(Engine::States::State state)
{
}

void Game::PlayMenuGameState::Handle(Engine::Ranges::Range range, Math::Real value)
{
}

void Game::PlayMenuGameState::DeselectAll()
{
	START_PROFILING_GAME(true, "");
	//// This function is supposed to apply the "non-selected" effects to all non-selected menu entries. However Effect class can only handle one attribute at a time.
	//// It is not possible to modify many attributes using one Effect class instance. As a result only the last non-selected menu entry will have "non-selected" effects applied to it.
	//for (int i = 0; i < m_currentMenuEntry->GetChildrenCount(); ++i)
	//{
	//	m_currentMenuEntry->SelectChildMenuEntry(i, false);
	//	//m_currentMenuEntry->GetSelectedChild()->ApplyBorderEdgeTransitionWidthEffect(m_notSelectedMenuEntryBorderEdgeTransitionWidthEffect);
	//	//m_currentMenuEntry->GetSelectedChild()->ApplyBorderWidthEffect(m_notSelectedMenuEntryBorderWidthEffect);
	//	//m_currentMenuEntry->GetSelectedChild()->ApplyCharacterEdgeTransitionWidthEffect(m_notSelectedMenuEntryCharacterEdgeTransitionWidthEffect);
	//	//m_currentMenuEntry->GetSelectedChild()->ApplyCharacterWidthEffect(m_notSelectedMenuEntryCharacterWidthEffect);
	//	//m_currentMenuEntry->GetSelectedChild()->ApplyColorEffect(m_notSelectedMenuEntryColorEffect);
	//	//m_currentMenuEntry->GetSelectedChild()->ApplyOffsetEffect(m_notSelectedMenuEntryOffsetEffect);
	//	//m_currentMenuEntry->GetSelectedChild()->ApplyOutlineColorEffect(m_notSelectedMenuEntryOutlineColorEffect);
	//}
	STOP_PROFILING_GAME("");
}

void Game::PlayMenuGameState::SelectChild(int childIndex)
{
	//CHECK_CONDITION_RETURN_VOID_ALWAYS_GAME(m_currentMenuEntry->GetSelectedMenuEntryIndex() != childIndex, Utility::DEBUG, "Trying to select the child which is already selected (", childIndex, ").");
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
	DEBUG_LOG_GAME("Selected menu entry changed to ", childIndex);
}

void Game::PlayMenuGameState::Render(Rendering::Renderer* renderer) const
{
	START_PROFILING_GAME(true, "");
	DELOCUST_LOG_GAME("PLAY MAIN MENU game state rendering");

	renderer->BindAsRenderTarget();
	renderer->ClearScreen(/* TODO: specify menu game state clear screen color */);
	m_currentMenuEntry->Render(renderer, m_gameManager->GetGuiShader());
	STOP_PROFILING_GAME("");
}

//void Game::PlayMenuGameState::MouseButtonEvent(int button, int action, int mods)
//{
//	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
//	{
//		if (m_currentMenuEntry->GetSelectedChild()->DoesMouseHoverOver(m_mousePosX, m_mousePosY))
//		{
//			ChooseCurrentMenuEntry();
//		}
//		else
//		{
//			//CRITICAL_LOG_GAME("Does not hover (", m_mousePosX, ", ", m_mousePosY, ")");
//		}
//	}
//}

//void Game::PlayMenuGameState::MousePosEvent(double xPos, double yPos)
//{
//	//const Rendering::Camera& currentCamera = Engine::CoreEngine::GetCoreEngine()->GetRenderer()->GetCurrentCamera();
//	//m_mousePicker.CalculateCurrentRay(xPos, yPos, currentCamera.GetProjection(), currentCamera.GetViewMatrix());
//
//	m_mousePosX = static_cast<Math::Real>(xPos);
//	m_mousePosY = static_cast<Math::Real>(yPos);
//
//	const int menuEntryChildrenCount = m_currentMenuEntry->GetChildrenCount();
//	//CRITICAL_LOG_GAME("Menu mouse position event (", m_mousePosX, ", ", m_mousePosY, ")");
//	for (int i = 0; i < menuEntryChildrenCount; ++i)
//	{
//		if (m_currentMenuEntry->DoesMouseHoverOverChild(i, m_mousePosX, m_mousePosY))
//		{
//			//CRITICAL_LOG_GAME("Menu entry \"", m_currentMenuEntry->GetChildText(i), "\" selected");
//			m_currentMenuEntry->SelectChildMenuEntry(i);
//		}
//	}
//}

//void Game::PlayMenuGameState::ScrollEvent(double xOffset, double yOffset)
//{
//}
