#include "StdAfx.h"
#include "ICommand.h"
#include "Command.h"
#include "ILogger.h"

using namespace Utility;

/* static */ ICommand* ICommand::command = NULL;

/* static */ void ICommand::SetCommand(int argc, char* argv[])
{
	ASSERT(ICommand::command == NULL);
	if (ICommand::command != NULL)
	{
		WARNING_LOG("Trying to create new command instance when it is already present. Reallocating new command instance.");
		SAFE_DELETE(ICommand::command);
	}
	ICommand::command = new Command(argc, argv);
}

/* static */ ICommand& ICommand::GetCommand()
{
	CHECK_CONDITION_EXIT(ICommand::command != NULL, Utility::Error, "Command instance does not exist. Cannot read application parameters.");
	return *ICommand::command;
}

/* static */ void ICommand::DeleteCommand()
{
	CHECK_CONDITION_RETURN(ICommand::command != NULL, Utility::Warning, "Trying to delete command instance when it is already deallocated.");
	DEBUG_LOG("Deallocating command instance.");
	SAFE_DELETE(ICommand::command);
}

ICommand::ICommand()
{
}


ICommand::~ICommand(void)
{
}
