#include "ShowIntroCommand.h"
#include "Def.h"
#include "Engine/GameManager.h"

Game::ShowIntroCommand::ShowIntroCommand() :
	engine::GameCommand()
{
}


Game::ShowIntroCommand::~ShowIntroCommand()
{
}

void Game::ShowIntroCommand::Execute(engine::GameManager* gameManager) const
{
	DEBUG_LOG_GAME("Show intro game command started");
	// TODO: If we show the intro from the Main menu then we would prefer to use Engine::GameStateTransitioning::PUSH instead of SWITCH and then simply pop the state when we close the intro or intro simply ends.
	gameManager->SetTransition(new engine::game_state_transitioning::GameStateTransition(gameManager->GetIntroGameState(), engine::game_state_transitioning::SWITCH, engine::game_state_modality::EXCLUSIVE));
}