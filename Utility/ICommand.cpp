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
		LOG(Warning, LOGPLACE, "Trying to create new command instance when it is already present. Reallocating new command instance.");
		SAFE_DELETE(ICommand::command);
	}
	ICommand::command = new Command(argc, argv);
}

/* static */ ICommand& ICommand::GetCommand()
{
	ASSERT(ICommand::command != NULL);
	if (ICommand::command == NULL)
	{
		LOG(Error, LOGPLACE, "Command instance does not exist. Cannot read application parameters");
		exit(EXIT_FAILURE);
	}
	return *ICommand::command;
}

/* static */ void ICommand::DeleteCommand()
{
	ASSERT(ICommand::command != NULL);
	if (ICommand::command == NULL)
	{
		LOG(Warning, LOGPLACE, "Trying to delete command instance when it is already deallocated.");
		return;
	}
	LOG(Debug, LOGPLACE, "Deallocating command instance.");
	SAFE_DELETE(ICommand::command);
}

ICommand::ICommand()
{
}


ICommand::~ICommand(void)
{
}
