#include "LoadGameState.h"
#include "Rendering\GameManager.h"
#include "Rendering\CoreEngine.h"
#include "Utility\ILogger.h"
#include "PlayGameState.h"

#include "tinythread.h"

using namespace Game;
using namespace Rendering;

LoadGameState::LoadGameState(void) :
	Rendering::GameState(),
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
	m_loadingThread = new tthread::thread(Rendering::GameManager::Load, Rendering::GameManager::GetGameManager());
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

void LoadGameState::Render(Rendering::Renderer* renderer)
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
	m_loadingProgress = GameManager::GetGameManager()->GetLoadingProgress();
	// m_loadingProgress += 0.00022f;
	if (m_loadingProgress > REAL_ONE)
	{
		m_loadingProgress = REAL_ONE;
	}

	if (GameManager::GetGameManager()->IsGameLoaded())
	{
		NOTICE_LOG("The game is loaded");
		m_loadingThread->join();
		GameManager::GetGameManager()->SetTransition(new GameStateTransitioning::GameStateTransition(new PlayGameState(), GameStateTransitioning::SWITCH, GameStateModality::EXCLUSIVE));
	}
	STOP_PROFILING;
}