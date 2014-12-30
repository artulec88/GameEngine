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
	console = GetStdHandle(STD_OUTPUT_HANDLE); // For use of SetConsoleTextAttribute()
	ReadConsoleColorsFromConfigFile();
}


ILogger::~ILogger(void)
{
	SetConsoleTextAttribute(console, 15); // set color back to normal
}

void ILogger::ReadConsoleColorsFromConfigFile()
{
	// TODO: Not yet implemented
	// TODO: Store console colors for each logging level from configuration file
}

void ILogger::SetConsoleColor(LogLevel level) const
{
	switch (level)
	{
	case Critical:
		//SetConsoleTextAttribute(console, FOREGROUND_RED | FOREGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE );
		SetConsoleTextAttribute(console, FOREGROUND_RED | FOREGROUND_INTENSITY );
		break;
	case Emergency:
		SetConsoleTextAttribute(console, FOREGROUND_RED );
		break;
	case Error:
		SetConsoleTextAttribute(console, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY );
		break;
	case Warning:
		SetConsoleTextAttribute(console, FOREGROUND_RED | FOREGROUND_GREEN );
		break;
	case Notice:
		SetConsoleTextAttribute(console, FOREGROUND_GREEN | FOREGROUND_INTENSITY );
		break;
	case Info:
		SetConsoleTextAttribute(console, FOREGROUND_GREEN );
		break;
	case Debug:
	case Delocust:
	case DevNull:
		SetConsoleTextAttribute(console, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE );
		break;
	default:
		LOG(Error, LOGPLACE, "Incorrect logging level set");
		break;
	}
}