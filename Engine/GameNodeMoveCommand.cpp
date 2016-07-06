#include "stdafx.h"
#include "GameNodeMoveCommand.h"


Engine::GameNodeMoveCommand::GameNodeMoveCommand(/* Math::Real movementSpeed */) :
	GameNodeCommand()
{
}


Engine::GameNodeMoveCommand::~GameNodeMoveCommand()
{
}

void Engine::GameNodeMoveCommand::Execute(GameNode* gameNode) const
{
	DEBUG_LOG_ENGINE("Move command started. Remember that executing a move command wastes precious CPU time.");
	// TODO: Perform move action
}