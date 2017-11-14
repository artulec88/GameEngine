#include "ResumeGameCommand.h"

Game::ResumeGameCommand::ResumeGameCommand() :
	engine::GameCommand()
{
}


Game::ResumeGameCommand::~ResumeGameCommand()
{
}

void Game::ResumeGameCommand::Execute(engine::GameManager* gameManager) const
{
	gameManager->PopState();
}