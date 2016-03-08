#include "SaveGameCommand.h"
#include "Engine\GameManager.h"

Game::SaveGameCommand::SaveGameCommand() :
	Engine::GameCommand()
{
}


Game::SaveGameCommand::~SaveGameCommand()
{
}

void Game::SaveGameCommand::Execute() const
{
	// TODO: Game saving
	WARNING_LOG("The saving game functionality is not supported yet.");
	//Engine::GameManager::GetGameManager()->RequestGameQuit();
}