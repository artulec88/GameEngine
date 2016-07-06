#include "StdAfx.h"
#include "ICommandLineMapper.h"
#include "CommandLineMapper.h"
#include "ILogger.h"

/* static */ std::unique_ptr<Utility::ICommandLineMapper> Utility::ICommandLineMapper::CreateCommandLineMapper(int argc, char* argv[])
{
	// Implicit move operation into the variable that stores the result.
	return std::make_unique<CommandLineMapper>(argc, argv);
}

Utility::ICommandLineMapper::ICommandLineMapper()
{
	DELOCUST_LOG_UTILITY("ICommandLine constructor");
}


Utility::ICommandLineMapper::~ICommandLineMapper()
{
	DELOCUST_LOG_UTILITY("ICommandLine destructor");
}
