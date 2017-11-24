#include "SaveGameCommand.h"
#include "Def.h"
#include "Engine/GameManager.h"

Game::SaveGameCommand::SaveGameCommand() :
	engine::GameCommand()
{
}


Game::SaveGameCommand::~SaveGameCommand()
{
}

void Game::SaveGameCommand::Execute(engine::GameManager* gameManager) const
{
	// TODO: Game saving
	WARNING_LOG_GAME("The saving game functionality is not supported yet.");
	//Engine::GameManager::GetGameManager()->RequestGameQuit();
}