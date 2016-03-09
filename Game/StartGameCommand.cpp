#include "StartGameCommand.h"
#include "LoadGameState.h"
#include "Engine\GameStateManager.h"
#include "Engine\GameManager.h"

Game::StartGameCommand::StartGameCommand() :
	Engine::GameCommand()
{
}


Game::StartGameCommand::~StartGameCommand()
{
}

void Game::StartGameCommand::Execute(/* Engine::GameManager& gameManager */) const
{
	DEBUG_LOG("Start game command started");
	Engine::GameManager* gameManager = Engine::GameManager::GetGameManager();
	gameManager->SetTransition(new Engine::GameStateTransitioning::GameStateTransition(gameManager->GetLoadGameState(), Engine::GameStateTransitioning::SWITCH, Engine::GameStateModality::EXCLUSIVE));
}