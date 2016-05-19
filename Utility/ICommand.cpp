#include "StdAfx.h"
#include "ICommand.h"
#include "Command.h"
#include "ILogger.h"

/* static */ std::unique_ptr<Utility::ICommand> Utility::ICommand::CreateCommand(int argc, char* argv[])
{
	// Implicit move operation into the variable that stores the result.
	return std::make_unique<Command>(argc, argv);
}

Utility::ICommand::ICommand(int argc, char* argv[])
{
	DELOCUST_LOG("ICommand constructor");
}


Utility::ICommand::~ICommand()
{
	DELOCUST_LOG("ICommand destructor");
}
