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
{
}

LoadGameState::~LoadGameState(void)
{
	SAFE_DELETE(m_loadingThread);
}

void LoadGameState::Entered()
{
	LOG(Utility::Info, LOGPLACE, "LOAD game state has been placed in the game state manager");
	LOG(Utility::Notice, LOGPLACE, "Starting the loading thread");
	m_loadingThread = new tthread::thread(Rendering::GameManager::Load, Rendering::GameManager::GetGameManager());
}

void LoadGameState::Leaving()
{
	LOG(Utility::Info, LOGPLACE, "LOAD game state is about to be removed from the game state manager");
}

void LoadGameState::Obscuring()
{
	LOG(Utility::Info, LOGPLACE, "Another game state is about to stack on top of LOAD game state");
}

void LoadGameState::Revealed()
{
	LOG(Utility::Info, LOGPLACE, "LOAD game state has become the topmost game state in the game state manager's stack");
}

void LoadGameState::Render(Rendering::Renderer* renderer, const Rendering::GameNode& gameNode)
{
	LOG(Utility::Debug, LOGPLACE, "LOAD game state rendering");
	renderer->RenderLoadingScreen(m_loadingProgress);
}

void LoadGameState::Update(Math::Real elapsedTime, Rendering::GameNode& gameNode)
{
	LOG(Utility::Debug, LOGPLACE, "LOAD game state updating");
	// m_loadingProgress = GameManager::GetGameManager()->GetLoadPercentage();
	m_loadingProgress += 0.00022f;
	if (m_loadingProgress > REAL_ONE)
	{
		m_loadingProgress = REAL_ONE;
	}

	// TODO: Get loading progress from the game manager

	if (GameManager::GetGameManager()->IsGameLoaded())
	{
		LOG(Utility::Notice, LOGPLACE, "The game is loaded");
		m_loadingThread->join();
		GameManager::GetGameManager()->SetTransition(new GameStateTransitioning::GameStateTransition(new PlayGameState(), GameStateTransitioning::SWITCH, GameStateModality::EXCLUSIVE));
	}
}