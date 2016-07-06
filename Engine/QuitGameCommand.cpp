#include "stdafx.h"
#include "QuitGameCommand.h"
#include "GameManager.h"

Engine::QuitGameCommand::QuitGameCommand() :
	GameCommand()
{
}


Engine::QuitGameCommand::~QuitGameCommand()
{
}

void Engine::QuitGameCommand::Execute(GameManager* gameManager) const
{
	gameManager->RequestGameQuit();
}