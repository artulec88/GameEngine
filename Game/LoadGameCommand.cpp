#include "LoadGameCommand.h"
#include "Def.h"
#include "Engine\GameManager.h"

Game::LoadGameCommand::LoadGameCommand() :
	Engine::GameCommand()
{
}


Game::LoadGameCommand::~LoadGameCommand()
{
}

void Game::LoadGameCommand::Execute() const
{
	// TODO: Game saving
	WARNING_LOG_GAME("The loading game functionality is not supported yet.");
	Engine::GameManager* gameManager = Engine::GameManager::GetGameManager();
	gameManager->SetTransition(new Engine::GameStateTransitioning::GameStateTransition(gameManager->GetIntroGameState(), Engine::GameStateTransitioning::SWITCH, Engine::GameStateModality::EXCLUSIVE));
	//Engine::GameManager::GetGameManager()->RequestGameQuit();
}