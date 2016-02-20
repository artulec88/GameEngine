#include "stdafx.h"
#include "QuitGameCommand.h"
#include "GameManager.h"

Core::QuitGameCommand::QuitGameCommand(const GameManager& gameManager) :
	Core::GameCommand(),
	m_gameManager(*GameManager::GetGameManager())
{
}

Core::QuitGameCommand::~QuitGameCommand()
{
}

void Core::QuitGameCommand::Execute() const
{
}

