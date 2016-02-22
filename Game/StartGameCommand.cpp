#include "StartGameCommand.h"
#include "Engine\GameStateManager.h"
#include "LoadGameState.h"

Game::StartGameCommand::StartGameCommand(Engine::GameManager& gameManager) :
	m_gameManager(gameManager)
{
}


Game::StartGameCommand::~StartGameCommand()
{
}

void Game::StartGameCommand::Execute() const
{
	CRITICAL_LOG("Start game command started");
	m_gameManager.SetTransition(new Engine::GameStateTransitioning::GameStateTransition(new LoadGameState(), Engine::GameStateTransitioning::SWITCH, Engine::GameStateModality::EXCLUSIVE));
}