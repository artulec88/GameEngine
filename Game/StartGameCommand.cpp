#include "StartGameCommand.h"
#include "Def.h"
#include "LoadGameState.h"
#include "Engine\CoreEngine.h"
#include "Engine\GameStateManager.h"
#include "Engine\GameManager.h"

Game::StartGameCommand::StartGameCommand() :
	Engine::GameCommand()
{
}


Game::StartGameCommand::~StartGameCommand()
{
}

void Game::StartGameCommand::Execute(Engine::GameManager* gameManager) const
{
	DEBUG_LOG_GAME("Start game command started");
	// We load here instead of going into the LoadGameState, because of the multi-threading problems we faced. VAOs cannot be shared across different graphics context,
	// so it is not easy to switch the graphics context (from main window to the thread window), load all necessary resources and have them accessible from the main window context.
	gameManager->Load(Engine::CoreEngine::GetCoreEngine()->GetRenderer());
	gameManager->SetTransition(new Engine::GameStateTransitioning::GameStateTransition(gameManager->GetPlayGameState(), Engine::GameStateTransitioning::SWITCH, Engine::GameStateModality::EXCLUSIVE));
	//gameManager->SetTransition(new Engine::GameStateTransitioning::GameStateTransition(gameManager->GetLoadGameState(), Engine::GameStateTransitioning::SWITCH, Engine::GameStateModality::EXCLUSIVE));
}