#include "IntroGameState.h"
#include "Def.h"
#include "Engine/GameManager.h"
#include "Engine/CoreEngine.h"
#include "Utility/ILogger.h"
#include "MenuGameState.h"

game::IntroGameState::IntroGameState(engine::GameManager* gameManager, const std::string& inputMappingContextName) :
	GameState(inputMappingContextName),
	m_gameManager(gameManager)
#ifdef PROFILING_GAME_MODULE_ENABLED
	,m_classStats(STATS_STORAGE.GetClassStats("IntroGameState"))
#endif
{
}

game::IntroGameState::~IntroGameState()
{
}

void game::IntroGameState::Entered()
{
	engine::CoreEngine::GetCoreEngine()->PushInputContext(m_inputMappingContextName);
	INFO_LOG_GAME("Intro game state has been placed in the game state manager");
}

void game::IntroGameState::Leaving()
{
	engine::CoreEngine::GetCoreEngine()->PopInputContext();
	INFO_LOG_GAME("Intro game state is about to be removed from the game state manager");
}

void game::IntroGameState::Obscuring()
{
	engine::CoreEngine::GetCoreEngine()->PopInputContext();
	INFO_LOG_GAME("Another game state is about to stack on top of intro game state");
}

void game::IntroGameState::Revealed()
{
	engine::CoreEngine::GetCoreEngine()->PushInputContext(m_inputMappingContextName);
	INFO_LOG_GAME("Intro game state has become the topmost game state in the game state manager's stack");
}

void game::IntroGameState::Handle(engine::actions::Action action)
{
	const auto gameManager = engine::GameManager::GetGameManager();
	switch (action)
	{
	case engine::actions::QUIT_INTRO:
		gameManager->SetTransition(new engine::game_state_transitioning::GameStateTransition(gameManager->GetMainMenuGameState(), engine::game_state_transitioning::SWITCH, engine::game_state_modality::EXCLUSIVE));
		break;
	default:
		INFO_LOG_GAME("To skip the intro you have to double-click ESC button");
	}
}

void game::IntroGameState::Handle(engine::states::State state)
{
}

void game::IntroGameState::Handle(engine::ranges::Range range, math::Real value)
{
}

void game::IntroGameState::Render(rendering::Renderer* renderer) const
{
	START_PROFILING_GAME(true, "");
	DEBUG_LOG_GAME("INTRO game state rendering");
	renderer->BindAsRenderTarget();
	renderer->ClearScreen(/* TODO: specify menu game state clear screen color */);
	//renderer->RenderText(m_gameManager->GetGuiTextShader(), 450, 450, "Intro (click ESC)");
	STOP_PROFILING_GAME("");
}
