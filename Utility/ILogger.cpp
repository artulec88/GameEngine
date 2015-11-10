#include "StdAfx.h"
#include "ILogger.h"
#include "Logger.h"
#include <iostream>

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
	m_level(Notice)
{
	m_console = GetStdHandle(STD_OUTPUT_HANDLE); // For use of SetConsoleTextAttribute()
	ReadConsoleColorsFromConfigFile();
}


ILogger::~ILogger(void)
{
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
		SetConsoleTextAttribute(m_console, FOREGROUND_RED | FOREGROUND_INTENSITY );
		break;
	case Emergency:
		SetConsoleTextAttribute(m_console, FOREGROUND_RED );
		break;
	case Error:
		SetConsoleTextAttribute(m_console, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY );
		break;
	case Warning:
		SetConsoleTextAttribute(m_console, FOREGROUND_RED | FOREGROUND_GREEN );
		break;
	case Notice:
		SetConsoleTextAttribute(m_console, FOREGROUND_GREEN | FOREGROUND_INTENSITY );
		break;
	case Info:
		SetConsoleTextAttribute(m_console, FOREGROUND_GREEN );
		break;
	case Debug:
	case Delocust:
	case DevNull:
		SetConsoleTextAttribute(m_console, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE );
		break;
	default:
		ERROR_LOG("Incorrect logging level set");
		break;
	}
}

void ILogger::ResetConsoleColor() const
{
	SetConsoleTextAttribute(m_console, 15); // set color back to normal
}

void ILogger::SetLevel(LogLevel level)
{
	//mutex.Lock();
	//if ((!m_modified) || (level > this->m_level))
	//{
	//	std::cout << "Logging level set to " << LevelNames[level] << std::endl;
	//	this->m_level = level;
	//}
	//m_modified = true;
	//mutex.Unlock();
	std::cout << "Logging level set to " << LevelNames[level] << std::endl;
	m_level = level;
}