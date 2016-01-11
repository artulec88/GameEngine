#include "QuitCommand.h"



Game::QuitCommand::QuitCommand(Rendering::GameManager* gameManager) :
	m_gameManager(*gameManager)
{
}


Game::QuitCommand::~QuitCommand()
{
}

void Game::QuitCommand::Execute() const
{
	m_gameManager.RequestGameQuit();
}