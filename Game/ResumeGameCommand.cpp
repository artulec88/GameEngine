#include "ResumeGameCommand.h"

Game::ResumeGameCommand::ResumeGameCommand() :
	Engine::GameCommand()
{
}


Game::ResumeGameCommand::~ResumeGameCommand()
{
}

void Game::ResumeGameCommand::Execute() const
{
	Engine::GameManager::GetGameManager()->PopState();
}