#include "ResumeGameCommand.h"

game::ResumeGameCommand::ResumeGameCommand() :
	engine::GameCommand()
{
}


game::ResumeGameCommand::~ResumeGameCommand()
{
}

void game::ResumeGameCommand::Execute(engine::GameManager* gameManager) const
{
	gameManager->PopState();
}