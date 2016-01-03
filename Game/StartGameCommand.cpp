#include "StartGameCommand.h"
#include "Rendering\GameStateManager.h"
#include "LoadGameState.h"

Game::StartGameCommand::StartGameCommand(Rendering::GameManager& gameManager) :
	m_gameManager(gameManager)
{
}


Game::StartGameCommand::~StartGameCommand()
{
}

void Game::StartGameCommand::Execute() const
{
	CRITICAL_LOG("Start game command started");
	m_gameManager.SetTransition(new Rendering::GameStateTransitioning::GameStateTransition(new LoadGameState(), Rendering::GameStateTransitioning::SWITCH, Rendering::GameStateModality::EXCLUSIVE));
}