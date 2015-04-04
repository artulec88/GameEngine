#include "LoadGameState.h"
#include "Rendering\GameManager.h"
#include "Utility\ILogger.h"
#include "PlayGameState.h"

using namespace Game;

LoadGameState::LoadGameState(void) :
	Rendering::GameState(),
	m_loadingProgress(REAL_ZERO)
{
}

LoadGameState::~LoadGameState(void)
{
}

void LoadGameState::Entered()
{
	LOG(Utility::Info, LOGPLACE, "LOAD game state has been placed in the game state manager");
	Rendering::GameManager* gameManager = Rendering::GameManager::GetGameManager();
	if (!gameManager->IsGameLoaded())
	{
		gameManager->Load();
		gameManager->InitializeTweakBars();
	}
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
	m_loadingProgress += 0.001f * elapsedTime;
}