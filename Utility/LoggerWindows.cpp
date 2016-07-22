#include "stdafx.h"
#include "LoggerWindows.h"
//#include "Command.h"
#include <string>
#include "Time.h"
	
Utility::Logging::LoggerWindows::LoggerWindows(const char* fileName /* = NULL */) :
	ILogger(fileName),
	m_modified(false),
	m_indentDepth(0),
	m_dateTimeFormat("%H:%M:%S")
{
	m_console = GetStdHandle(STD_OUTPUT_HANDLE); // For use of SetConsoleTextAttribute()
	ReadConsoleColorsFromConfigFile();
}

Utility::Logging::LoggerWindows::~LoggerWindows()
{
}

void Utility::Logging::LoggerWindows::Fill(const std::string& strLevel /* = EmptyString */, LogLevel defaultLogLevel /* = NOTICE */)
{
	//std::string str = line.Get<string>("--log", "");
	if (strLevel == "")
	{
		DELOCUST_LOG_UTILITY("Setting new logging level from \"", LOGGING_LEVEL_NAMES[static_cast<int>(GetLevel())], "\" to \"", LOGGING_LEVEL_NAMES[static_cast<int>(defaultLogLevel)], "\".");
		SetLevel(defaultLogLevel);
		return;
	}
	int i = 0;
	for (std::array<std::string, COUNT>::const_iterator levelNameItr = LOGGING_LEVEL_NAMES.begin(); levelNameItr != LOGGING_LEVEL_NAMES.end(); ++levelNameItr, ++i)
	{
		if ((*levelNameItr) == strLevel)
		{
			SetLevel((LogLevel)i);
			return;
		}
	}
	SetLevel(defaultLogLevel);
}

void Utility::Logging::LoggerWindows::ResetConsoleColor() const
{
	SetConsoleTextAttribute(m_console, 15); // set color back to normal
}

void Utility::Logging::LoggerWindows::ReadConsoleColorsFromConfigFile()
{
	// TODO: Not yet implemented
	// TODO: Store console colors for each logging level from configuration file
}

void Utility::Logging::LoggerWindows::SetConsoleColor(LogLevel level) const
{
	switch (level)
	{
	case CRITICAL:
		//SetConsoleTextAttribute(console, FOREGROUND_RED | FOREGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE );
		SetConsoleTextAttribute(m_console, FOREGROUND_RED | FOREGROUND_INTENSITY);
		break;
	case EMERGENCY:
		SetConsoleTextAttribute(m_console, FOREGROUND_RED);
		break;
	case ERR:
		SetConsoleTextAttribute(m_console, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		break;
	case WARNING:
		SetConsoleTextAttribute(m_console, FOREGROUND_RED | FOREGROUND_GREEN);
		break;
	case NOTICE:
		SetConsoleTextAttribute(m_console, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		break;
	case INFO:
		SetConsoleTextAttribute(m_console, FOREGROUND_GREEN);
		break;
	case DEBUG:
	case DELOCUST:
		SetConsoleTextAttribute(m_console, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		break;
	default:
		ERROR_LOG_UTILITY("Incorrect logging level set: ", level);
		break;
	}
}