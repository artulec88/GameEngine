#include "LoadGameCommand.h"
#include "Def.h"
#include "Engine\GameManager.h"

Game::LoadGameCommand::LoadGameCommand() :
	engine::GameCommand()
{
}


Game::LoadGameCommand::~LoadGameCommand()
{
}

void Game::LoadGameCommand::Execute(engine::GameManager* gameManager) const
{
	// TODO: Game saving
	WARNING_LOG_GAME("The loading game functionality is not supported yet.");
	gameManager->SetTransition(new engine::GameStateTransitioning::GameStateTransition(gameManager->GetIntroGameState(), engine::GameStateTransitioning::SWITCH, engine::GameStateModality::EXCLUSIVE));
	//Engine::GameManager::GetGameManager()->RequestGameQuit();
}