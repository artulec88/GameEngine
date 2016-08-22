#include "LoadGameState.h"
#include "Def.h"
#include "Engine\GameManager.h"
#include "Engine\CoreEngine.h"
#include "Rendering\Shader.h"
#include "Utility\ILogger.h"
#include "PlayGameState.h"

#include "tinythread.h"

using namespace Game;
using namespace Rendering;

LoadGameState::LoadGameState(Engine::GameManager* gameManager, const std::string& inputMappingContextName) :
	Engine::GameState(inputMappingContextName),
	m_gameManager(gameManager),
	m_loadingProgress(REAL_ZERO),
	m_loadingThread(NULL)
#ifdef CALCULATE_GAME_STATS
	,m_classStats(STATS_STORAGE.GetClassStats("LoadGameState"))
#endif
{
}

LoadGameState::~LoadGameState(void)
{
	SAFE_DELETE(m_loadingThread);
}

void LoadGameState::Entered()
{
	START_PROFILING("");
	Engine::CoreEngine::GetCoreEngine()->PushInputContext(m_inputMappingContextName);
	INFO_LOG_GAME("LOAD game state has been placed in the game state manager");
	NOTICE_LOG_GAME("Starting the loading thread");

	// VAOs cannot be shared across multiple window contexts, that is why we don't use additional threads anymore.
	// See http://stackoverflow.com/questions/23765371/opengl-multitreading-shared-context-and-glgenbuffers.
	m_loadingThread = new tthread::thread(Engine::GameManager::Load, Engine::GameManager::GetGameManager());
	STOP_PROFILING("");
}

void LoadGameState::Leaving()
{
	Engine::CoreEngine::GetCoreEngine()->PopInputContext();
	INFO_LOG_GAME("LOAD game state is about to be removed from the game state manager");
}

void LoadGameState::Obscuring()
{
	Engine::CoreEngine::GetCoreEngine()->PopInputContext();
	INFO_LOG_GAME("Another game state is about to stack on top of LOAD game state");
}

void LoadGameState::Revealed()
{
	Engine::CoreEngine::GetCoreEngine()->PushInputContext(m_inputMappingContextName);
	INFO_LOG_GAME("LOAD game state has become the topmost game state in the game state manager's stack");
}

void LoadGameState::Handle(Engine::Actions::Action action)
{
}

void LoadGameState::Handle(Engine::States::State state)
{
}

void LoadGameState::Handle(Engine::Ranges::Range range, Math::Real value)
{
}

void LoadGameState::Render(Rendering::Renderer* renderer) const
{
	START_PROFILING("");
	DELOCUST_LOG_GAME("LOAD game state rendering");
	// TODO: Rendering loading screen.
	//renderer->RenderLoadingScreen(m_gameManager->GetGuiTextShader(), m_loadingProgress);
	STOP_PROFILING("");
}

void LoadGameState::Update(Math::Real elapsedTime)
{
	START_PROFILING("");
	DELOCUST_LOG_GAME("LOAD game state updating");
	Engine::GameManager* gameManager = Engine::GameManager::GetGameManager();
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
		gameManager->SetTransition(new Engine::GameStateTransitioning::GameStateTransition(gameManager->GetPlayGameState(), Engine::GameStateTransitioning::SWITCH, Engine::GameStateModality::EXCLUSIVE));
	}
	STOP_PROFILING("");
}