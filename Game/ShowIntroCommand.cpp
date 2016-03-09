#include "ShowIntroCommand.h"
#include "Engine\GameManager.h"

Game::ShowIntroCommand::ShowIntroCommand() :
	Engine::GameCommand()
{
}


Game::ShowIntroCommand::~ShowIntroCommand()
{
}

void Game::ShowIntroCommand::Execute(/* Engine::GameManager& gameManager */) const
{
	DEBUG_LOG("Show intro game command started");
	Engine::GameManager* gameManager = Engine::GameManager::GetGameManager();
	// TODO: If we show the intro from the Main menu then we would prefer to use Engine::GameStateTransitioning::PUSH instead of SWITCH and then simply pop the state when we close the intro or intro simply ends.
	gameManager->SetTransition(new Engine::GameStateTransitioning::GameStateTransition(gameManager->GetIntroGameState(), Engine::GameStateTransitioning::SWITCH, Engine::GameStateModality::EXCLUSIVE));
}