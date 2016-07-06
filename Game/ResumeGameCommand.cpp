#include "ResumeGameCommand.h"

Game::ResumeGameCommand::ResumeGameCommand() :
	Engine::GameCommand()
{
}


Game::ResumeGameCommand::~ResumeGameCommand()
{
}

void Game::ResumeGameCommand::Execute(Engine::GameManager* gameManager) const
{
	gameManager->PopState();
}