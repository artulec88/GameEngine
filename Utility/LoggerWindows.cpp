#include "stdafx.h"
#include "LoggerWindows.h"
//#include "Command.h"
#include <string>
#include "Time.h"
//#include <Windows.h>

using namespace std;
	
Utility::LoggerWindows::LoggerWindows(FILE *first) :
	ILogger(),
	m_modified(false),
	m_indentDepth(0),
	m_dateTimeFormat("%H:%M:%S")
{
	if (first)
	{
		m_outs.push_back(first);
	}
	m_console = GetStdHandle(STD_OUTPUT_HANDLE); // For use of SetConsoleTextAttribute()
	ReadConsoleColorsFromConfigFile();
}

Utility::LoggerWindows::~LoggerWindows()
{
}

//void Utility::LoggerWindows::operator()(LogLevel level, const char *name, int line, const char *format, ...)
//{
//	if (m_level < level)
//	{
//		return;
//	}
//
//	va_list args;
//	if (name == NULL)
//	{
//		name = "nowhere";
//	}
//	const char *tmp = strrchr(name, '\\');
//	if (tmp != NULL)
//	{
//		name = tmp + 1;
//	}
//	Utility::Timing::Time now = Utility::Timing::Time::Now();
//	std::string date = now.ToDateString();
//
//	//mutex.Lock();
//	//iterate through all the targets of logging
//	for (Outs::iterator o = m_outs.begin(); o != m_outs.end(); ++o)
//	{
//		va_start(args, format);
//		fprintf(*o, "[%s] [%s] %s", LevelNames[level], date.c_str(), name);
//		if (line)
//		{
//			fprintf(*o, "(%d)", line);
//		}
//		fprintf(*o, " : ");
//			
//		vfprintf(*o, format, args);
//		va_end(args);
//		
//		fprintf(*o, "\n");
//		fflush(*o);
//	}
//	//mutex.Unlock();
//}

void Utility::LoggerWindows::Log(LogLevel level, const char *name, int line, const char *format, ...)
{
	if (m_level < level)
	{
		return;
	}

	va_list args;
	if (name == NULL)
	{
		name = "nowhere";
	}
	const char *tmp = strrchr(name, '\\');
	if (tmp != NULL)
	{
		name = tmp + 1;
	}
	Utility::Timing::Time now = Utility::Timing::Time::Now();
	std::string date = now.ToDateString("%H:%M:%S");

	//mutex.Lock();
	//iterate through all the targets of logging
	for (Outs::iterator o = m_outs.begin(); o != m_outs.end(); ++o)
	{
		va_start(args, format);
		SetConsoleColor(level);
		fprintf(*o, "[%s] [%s] %s", LevelNames[level], date.c_str(), name);
		if (line)
		{
			fprintf(*o, "(%d)", line);
		}
		fprintf(*o, " : ");
			
		vfprintf(*o, format, args);
		va_end(args);
		
		fprintf(*o, "\n");
		fflush(*o);
	}
	ResetConsoleColor();
}

void Utility::LoggerWindows::Fill(const std::string& strLevel /* = EmptyString */, LogLevel defaultLogLevel /* = Notice */)
{
	//std::string str = line.Get<string>("--log", "");
	if (strLevel == "")
	{
		DELOCUST_LOG("Setting new logging level from %s to %s", LevelNames[static_cast<int>(GetLevel())], LevelNames[static_cast<int>(defaultLogLevel)]);
		SetLevel(defaultLogLevel);
		return;
	}
	int i = 0;
	while (LevelNames[i])
	{
		if (strcmp(strLevel.c_str(), LevelNames[i]) == 0)
		{
			//stdlog(Delocust, LOGPLACE, "Setting new logging level from %s to %s", LevelNames[static_cast<int>(stdlog.GetLevel())], LevelNames[static_cast<int>(defaultLogLevel)]);
			SetLevel((LogLevel)i);
			return;
		}
		++i;
	}
	SetLevel(defaultLogLevel);
}


void Utility::LoggerWindows::AddFile(const char *name)
{
	//mutex.Lock();
	FILE *file;
	fopen_s(&file, name, "a");
	m_outs.push_back(file);

	//mutex.Unlock();
}

void Utility::LoggerWindows::ResetConsoleColor() const
{
	SetConsoleTextAttribute(m_console, 15); // set color back to normal
}

void Utility::LoggerWindows::ReadConsoleColorsFromConfigFile()
{
	// TODO: Not yet implemented
	// TODO: Store console colors for each logging level from configuration file
}

void Utility::LoggerWindows::SetConsoleColor(LogLevel level) const
{
	switch (level)
	{
	case Critical:
		//SetConsoleTextAttribute(console, FOREGROUND_RED | FOREGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE );
		SetConsoleTextAttribute(m_console, FOREGROUND_RED | FOREGROUND_INTENSITY);
		break;
	case Emergency:
		SetConsoleTextAttribute(m_console, FOREGROUND_RED);
		break;
	case Error:
		SetConsoleTextAttribute(m_console, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		break;
	case Warning:
		SetConsoleTextAttribute(m_console, FOREGROUND_RED | FOREGROUND_GREEN);
		break;
	case Notice:
		SetConsoleTextAttribute(m_console, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		break;
	case Info:
		SetConsoleTextAttribute(m_console, FOREGROUND_GREEN);
		break;
	case Debug:
	case Delocust:
	case DevNull:
		SetConsoleTextAttribute(m_console, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		break;
	default:
		ERROR_LOG_UTILITY("Incorrect logging level set");
		break;
	}
}