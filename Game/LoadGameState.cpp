#include "LoadGameState.h"
#include "Engine/GameManager.h"
#include "Engine/CoreEngine.h"
#include "Rendering/Shader.h"
#include "Utility/ILogger.h"
#include "PlayGameState.h"

game::LoadGameState::LoadGameState(engine::GameManager* gameManager, const std::string& inputMappingContextName) :
	GameState(inputMappingContextName),
	m_gameManager(gameManager),
	m_loadingProgress(REAL_ZERO),
	m_loadingThread(nullptr)
#ifdef PROFILING_GAME_MODULE_ENABLED
	,m_classStats(STATS_STORAGE.GetClassStats("LoadGameState"))
#endif
{
}

game::LoadGameState::~LoadGameState()
{
	SAFE_DELETE(m_loadingThread);
}

void game::LoadGameState::Entered()
{
	START_PROFILING_GAME(true, "");
	engine::CoreEngine::GetCoreEngine()->PushInputContext(m_inputMappingContextName);
	INFO_LOG_GAME("LOAD game state has been placed in the game state manager");
	NOTICE_LOG_GAME("Starting the loading thread");

	// VAOs cannot be shared across multiple window contexts, that is why we don't use additional threads anymore.
	// See http://stackoverflow.com/questions/23765371/opengl-multitreading-shared-context-and-glgenbuffers.
	m_loadingThread = new std::thread(engine::GameManager::LoadGame, engine::GameManager::GetGameManager());
	STOP_PROFILING_GAME("");
}

void game::LoadGameState::Leaving()
{
	engine::CoreEngine::GetCoreEngine()->PopInputContext();
	INFO_LOG_GAME("LOAD game state is about to be removed from the game state manager");
}

void game::LoadGameState::Obscuring()
{
	engine::CoreEngine::GetCoreEngine()->PopInputContext();
	INFO_LOG_GAME("Another game state is about to stack on top of LOAD game state");
}

void game::LoadGameState::Revealed()
{
	engine::CoreEngine::GetCoreEngine()->PushInputContext(m_inputMappingContextName);
	INFO_LOG_GAME("LOAD game state has become the topmost game state in the game state manager's stack");
}

void game::LoadGameState::Handle(engine::actions::Action action)
{
}

void game::LoadGameState::Handle(engine::states::State state)
{
}

void game::LoadGameState::Handle(engine::ranges::Range range, math::Real value)
{
}

void game::LoadGameState::Render(rendering::Renderer* renderer) const
{
	START_PROFILING_GAME(true, "");
	DELOCUST_LOG_GAME("LOAD game state rendering");
	// TODO: Rendering loading screen.
	//renderer->RenderLoadingScreen(m_gameManager->GetGuiTextShader(), m_loadingProgress);
	STOP_PROFILING_GAME("");
}

void game::LoadGameState::Update(math::Real elapsedTime)
{
	START_PROFILING_GAME(true, "");
	DELOCUST_LOG_GAME("LOAD game state updating");
	engine::GameManager* gameManager = engine::GameManager::GetGameManager();
	m_loadingProgress = gameManager->GetLoadingProgress();
	// m_loadingProgress += 0.00022f;
	if (m_loadingProgress > REAL_ONE)
	{
		m_loadingProgress = REAL_ONE;
	}

	if (gameManager->IsGameLoaded())
	{
		NOTICE_LOG_GAME("The game is loaded");
		m_loadingThread->join();
		gameManager->SetTransition(new engine::game_state_transitioning::GameStateTransition(gameManager->GetPlayGameState(), engine::game_state_transitioning::SWITCH, engine::game_state_modality::EXCLUSIVE));
	}
	STOP_PROFILING_GAME("");
}