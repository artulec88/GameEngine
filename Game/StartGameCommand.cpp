#include "StartGameCommand.h"
#include "Def.h"
#include "LoadGameState.h"
#include "Engine\CoreEngine.h"
#include "Engine\GameStateManager.h"
#include "Engine/GameManager.h"

game::StartGameCommand::StartGameCommand() :
	engine::GameCommand()
{
}


game::StartGameCommand::~StartGameCommand()
{
}

void game::StartGameCommand::Execute(engine::GameManager* gameManager) const
{
	DEBUG_LOG_GAME("Start game command started");
	// We load here instead of going into the LoadGameState, because of the multi-threading problems we faced. VAOs cannot be shared across different graphics context,
	// so it is not easy to switch the graphics context (from main window to the thread window), load all necessary resources and have them accessible from the main window context.
	gameManager->Load();
	gameManager->SetTransition(new engine::game_state_transitioning::GameStateTransition(gameManager->GetPlayGameState(), engine::game_state_transitioning::SWITCH, engine::game_state_modality::EXCLUSIVE));
	//gameManager->SetTransition(new Engine::GameStateTransitioning::GameStateTransition(gameManager->GetLoadGameState(), Engine::GameStateTransitioning::SWITCH, Engine::GameStateModality::EXCLUSIVE));
}