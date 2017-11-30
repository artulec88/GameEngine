#include "ResumeGameCommand.h"
#include "Engine/GameManager.h"

game::ResumeGameCommand::ResumeGameCommand() :
	GameCommand()
{
}


game::ResumeGameCommand::~ResumeGameCommand()
{
}

void game::ResumeGameCommand::Execute(engine::GameManager* gameManager) const
{
	gameManager->PopState();
}