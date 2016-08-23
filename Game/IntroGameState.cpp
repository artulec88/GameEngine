#include "IntroGameState.h"
#include "Def.h"
#include "Engine\GameManager.h"
#include "Engine\CoreEngine.h"
#include "Utility\ILogger.h"
#include "MenuGameState.h"

Game::IntroGameState::IntroGameState(Engine::GameManager* gameManager, const std::string& inputMappingContextName) :
	Engine::GameState(inputMappingContextName),
	m_gameManager(gameManager)
#ifdef CALCULATE_GAME_STATS
	,m_classStats(STATS_STORAGE.GetClassStats("IntroGameState"))
#endif
{
}

Game::IntroGameState::~IntroGameState(void)
{
}

void Game::IntroGameState::Entered()
{
	Engine::CoreEngine::GetCoreEngine()->PushInputContext(m_inputMappingContextName);
	INFO_LOG_GAME("Intro game state has been placed in the game state manager");
}

void Game::IntroGameState::Leaving()
{
	Engine::CoreEngine::GetCoreEngine()->PopInputContext();
	INFO_LOG_GAME("Intro game state is about to be removed from the game state manager");
}

void Game::IntroGameState::Obscuring()
{
	Engine::CoreEngine::GetCoreEngine()->PopInputContext();
	INFO_LOG_GAME("Another game state is about to stack on top of intro game state");
}

void Game::IntroGameState::Revealed()
{
	Engine::CoreEngine::GetCoreEngine()->PushInputContext(m_inputMappingContextName);
	INFO_LOG_GAME("Intro game state has become the topmost game state in the game state manager's stack");
}

void Game::IntroGameState::Handle(Engine::Actions::Action action)
{
	Engine::GameManager* gameManager = Engine::GameManager::GetGameManager();
	switch (action)
	{
	case Engine::Actions::QUIT_INTRO:
		gameManager->SetTransition(new Engine::GameStateTransitioning::GameStateTransition(gameManager->GetMainMenuGameState(), Engine::GameStateTransitioning::SWITCH, Engine::GameStateModality::EXCLUSIVE));
		break;
	default:
		INFO_LOG_GAME("To skip the intro you have to double-click ESC button");
	}
}

void Game::IntroGameState::Handle(Engine::States::State state)
{
}

void Game::IntroGameState::Handle(Engine::Ranges::Range range, Math::Real value)
{
}

void Game::IntroGameState::Render(Rendering::Renderer* renderer) const
{
	START_PROFILING_GAME("");
	DEBUG_LOG_GAME("INTRO game state rendering");
	renderer->BindAsRenderTarget();
	renderer->ClearScreen(/* TODO: specify menu game state clear screen color */);
	//renderer->RenderText(m_gameManager->GetGuiTextShader(), 450, 450, "Intro (click ESC)");
	STOP_PROFILING_GAME("");
}
