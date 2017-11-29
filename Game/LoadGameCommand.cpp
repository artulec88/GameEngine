#include "LoadGameCommand.h"
#include "Def.h"
#include "Engine/GameManager.h"

game::LoadGameCommand::LoadGameCommand() :
	engine::GameCommand()
{
}


game::LoadGameCommand::~LoadGameCommand()
{
}

void game::LoadGameCommand::Execute(engine::GameManager* gameManager) const
{
	// TODO: Game saving
	WARNING_LOG_GAME("The loading game functionality is not supported yet.");
	gameManager->SetTransition(new engine::game_state_transitioning::GameStateTransition(gameManager->GetIntroGameState(), engine::game_state_transitioning::SWITCH, engine::game_state_modality::EXCLUSIVE));
	//Engine::GameManager::GetGameManager()->RequestGameQuit();
}