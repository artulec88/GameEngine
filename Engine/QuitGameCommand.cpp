#include "stdafx.h"
#include "QuitGameCommand.h"
#include "GameManager.h"

engine::QuitGameCommand::QuitGameCommand() :
	GameCommand()
{
}


engine::QuitGameCommand::~QuitGameCommand()
{
}

void engine::QuitGameCommand::Execute(GameManager* gameManager) const
{
	gameManager->RequestGameQuit();
}