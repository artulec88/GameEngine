#include "LoadGameState.h"
#include "Engine\GameManager.h"
#include "Engine\CoreEngine.h"
#include "Rendering\Shader.h"
#include "Utility\ILogger.h"
#include "PlayGameState.h"

#include "tinythread.h"

using namespace Game;
using namespace Rendering;

LoadGameState::LoadGameState(void) :
	Engine::GameState(),
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
	START_PROFILING;
	INFO_LOG("LOAD game state has been placed in the game state manager");
	NOTICE_LOG("Starting the loading thread");
	m_loadingThread = new tthread::thread(Engine::GameManager::Load, Engine::GameManager::GetGameManager());
	STOP_PROFILING;
}

void LoadGameState::Leaving()
{
	INFO_LOG("LOAD game state is about to be removed from the game state manager");
}

void LoadGameState::Obscuring()
{
	INFO_LOG("Another game state is about to stack on top of LOAD game state");
}

void LoadGameState::Revealed()
{
	INFO_LOG("LOAD game state has become the topmost game state in the game state manager's stack");
}

void LoadGameState::Render(const Rendering::Shader* shader, Rendering::Renderer* renderer) const
{
	START_PROFILING;
	DELOCUST_LOG("LOAD game state rendering");
	renderer->RenderLoadingScreen(m_loadingProgress);
	STOP_PROFILING;
}

void LoadGameState::Update(Math::Real elapsedTime)
{
	START_PROFILING;
	DELOCUST_LOG("LOAD game state updating");
	Engine::GameManager* gameManager = Engine::GameManager::GetGameManager();
	m_loadingProgress = gameManager->GetLoadingProgress();
	// m_loadingProgress += 0.00022f;
	if (m_loadingProgress > REAL_ONE)
	{
		m_loadingProgress = REAL_ONE;
	}

	if (gameManager->IsGameLoaded())
	{
		NOTICE_LOG("The game is loaded");
		m_loadingThread->join();
		gameManager->SetTransition(new Engine::GameStateTransitioning::GameStateTransition(gameManager->GetPlayGameState(), Engine::GameStateTransitioning::SWITCH, Engine::GameStateModality::EXCLUSIVE));
	}
	STOP_PROFILING;
}