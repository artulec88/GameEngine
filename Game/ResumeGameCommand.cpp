#include "ResumeGameCommand.h"

Game::ResumeGameCommand::ResumeGameCommand(Engine::GameManager& gameManager) :
	m_gameManager(gameManager)
{
}


Game::ResumeGameCommand::~ResumeGameCommand()
{
}

void Game::ResumeGameCommand::Execute() const
{
	m_gameManager.PopState();
}