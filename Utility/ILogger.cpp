#include "StdAfx.h"
#include "ILogger.h"
#include "Logger.h"

using namespace Utility;

/* static */ ILogger* ILogger::logger = NULL;

/* static */ const char* ILogger::LevelNames[] = 
{
	"Critical",
	"Emergency",
	"Error",
	"Warning",
	"Notice",
	"Info",
	"Debug",
	"Delocust",
	NULL
};

/* static */ ILogger& ILogger::GetLogger(/* some parameter based on which the logger implementation class is chosen */)
{
	if (ILogger::logger == NULL)
	{
		ILogger::logger = new Logger();
	}
	return *logger;
}

ILogger::ILogger(void) :
	level(Notice)
{
}


ILogger::~ILogger(void)
{
}
