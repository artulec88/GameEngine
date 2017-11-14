#include "stdafx.h"
#include "GameNodeMoveCommand.h"


engine::GameNodeMoveCommand::GameNodeMoveCommand(/* Math::Real movementSpeed */) :
	GameNodeCommand()
{
}


engine::GameNodeMoveCommand::~GameNodeMoveCommand()
{
}

void engine::GameNodeMoveCommand::Execute(GameNode* gameNode) const
{
	DEBUG_LOG_ENGINE("Move command started. Remember that executing a move command wastes precious CPU time.");
	// TODO: Perform move action
}