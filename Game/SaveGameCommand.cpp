#include "SaveGameCommand.h"
#include "Def.h"
#include "Engine\GameManager.h"

Game::SaveGameCommand::SaveGameCommand() :
	Engine::GameCommand()
{
}


Game::SaveGameCommand::~SaveGameCommand()
{
}

void Game::SaveGameCommand::Execute(Engine::GameManager* gameManager) const
{
	// TODO: Game saving
	WARNING_LOG_GAME("The saving game functionality is not supported yet.");
	//Engine::GameManager::GetGameManager()->RequestGameQuit();
}