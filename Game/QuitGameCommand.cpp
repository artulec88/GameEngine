#include "QuitGameCommand.h"
#include "Engine\GameManager.h"

Game::QuitGameCommand::QuitGameCommand() :
	Engine::GameCommand()
{
}


Game::QuitGameCommand::~QuitGameCommand()
{
}

void Game::QuitGameCommand::Execute() const
{
	Engine::GameManager::GetGameManager()->RequestGameQuit();
}