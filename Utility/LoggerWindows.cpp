#include "stdafx.h"
#include "LoggerWindows.h"
//#include "Command.h"
#include <string>
#include "Time.h"
	
utility::logging::LoggerWindows::LoggerWindows(const char* fileName /* = nullptr */) :
	ILogger(fileName),
	m_indentDepth(0),
	m_dateTimeFormat("%H:%M:%S"),
	m_console(),
	m_modified(false)
{
	m_console = GetStdHandle(STD_OUTPUT_HANDLE); // For use of SetConsoleTextAttribute()
	ReadConsoleColorsFromConfigFile(); // TODO: Beware of calling the virtual function in the constructor.
}

utility::logging::LoggerWindows::~LoggerWindows()
{
}

void utility::logging::LoggerWindows::Fill(const std::string& strLevel /* = EmptyString */, LogLevel defaultLogLevel /* = NOTICE */)
{
	//std::string str = line.Get<string>("--log", "");
	if (strLevel == "")
	{
		DELOCUST_LOG_UTILITY("Setting new logging level from \"", s_loggingLevelNames[static_cast<int>(GetLevel())], "\" to \"", s_loggingLevelNames[static_cast<int>(defaultLogLevel)], "\".");
		SetLevel(defaultLogLevel);
		return;
	}
	auto i = 0;
	for (auto levelNameItr = s_loggingLevelNames.begin(); levelNameItr != s_loggingLevelNames.end(); ++levelNameItr, ++i)
	{
		if (*levelNameItr == strLevel)
		{
			SetLevel(static_cast<LogLevel>(i));
			return;
		}
	}
	SetLevel(defaultLogLevel);
}

void utility::logging::LoggerWindows::ResetConsoleColor() const
{
	SetConsoleTextAttribute(m_console, 15); // set color back to normal
}

void utility::logging::LoggerWindows::ReadConsoleColorsFromConfigFile()
{
	// TODO: Not yet implemented
	// TODO: Store console colors for each logging level from configuration file
}

void utility::logging::LoggerWindows::SetConsoleColor(LogLevel level) const
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
	case COUNT:
	default:
		ERROR_LOG_UTILITY("Incorrect logging level set: ", level);
		break;
	}
}