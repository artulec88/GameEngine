#include "IntroGameState.h"
#include "Def.h"
#include "Engine/GameManager.h"
#include "Engine/CoreEngine.h"
#include "Utility/ILogger.h"
#include "MenuGameState.h"

Game::IntroGameState::IntroGameState(engine::GameManager* gameManager, const std::string& inputMappingContextName) :
	engine::GameState(inputMappingContextName),
	m_gameManager(gameManager)
#ifdef PROFILING_GAME_MODULE_ENABLED
	,m_classStats(STATS_STORAGE.GetClassStats("IntroGameState"))
#endif
{
}

Game::IntroGameState::~IntroGameState()
{
}

void Game::IntroGameState::Entered()
{
	engine::CoreEngine::GetCoreEngine()->PushInputContext(m_inputMappingContextName);
	INFO_LOG_GAME("Intro game state has been placed in the game state manager");
}

void Game::IntroGameState::Leaving()
{
	engine::CoreEngine::GetCoreEngine()->PopInputContext();
	INFO_LOG_GAME("Intro game state is about to be removed from the game state manager");
}

void Game::IntroGameState::Obscuring()
{
	engine::CoreEngine::GetCoreEngine()->PopInputContext();
	INFO_LOG_GAME("Another game state is about to stack on top of intro game state");
}

void Game::IntroGameState::Revealed()
{
	engine::CoreEngine::GetCoreEngine()->PushInputContext(m_inputMappingContextName);
	INFO_LOG_GAME("Intro game state has become the topmost game state in the game state manager's stack");
}

void Game::IntroGameState::Handle(engine::Actions::Action action)
{
	engine::GameManager* gameManager = engine::GameManager::GetGameManager();
	switch (action)
	{
	case engine::Actions::QUIT_INTRO:
		gameManager->SetTransition(new engine::GameStateTransitioning::GameStateTransition(gameManager->GetMainMenuGameState(), engine::GameStateTransitioning::SWITCH, engine::GameStateModality::EXCLUSIVE));
		break;
	default:
		INFO_LOG_GAME("To skip the intro you have to double-click ESC button");
	}
}

void Game::IntroGameState::Handle(engine::States::State state)
{
}

void Game::IntroGameState::Handle(engine::Ranges::Range range, math::Real value)
{
}

void Game::IntroGameState::Render(rendering::Renderer* renderer) const
{
	START_PROFILING_GAME(true, "");
	DEBUG_LOG_GAME("INTRO game state rendering");
	renderer->BindAsRenderTarget();
	renderer->ClearScreen(/* TODO: specify menu game state clear screen color */);
	//renderer->RenderText(m_gameManager->GetGuiTextShader(), 450, 450, "Intro (click ESC)");
	STOP_PROFILING_GAME("");
}
