#include "stdafx.h"
#include "Logger.h"
//#include "Command.h"
#include <string>
#include "Time.h"

using namespace Utility;
using namespace std;
	
Logger::Logger(FILE *first) :
	ILogger(),
	m_modified(false),
	m_indentDepth(0)
{
	if (first)
	{
		m_outs.push_back(first);
	}
}

Logger::~Logger()
{

}

void Logger::operator()(LogLevel level, const char *name, int line, const char *format, ...)
{
	if (this->level < level)
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
	Utility::Time now = Utility::Time::Now();
	std::string date = now.ToDateString();

	//mutex.Lock();
	//iterate through all the targets of logging
	for (Outs::iterator o = m_outs.begin(); o != m_outs.end(); ++o)
	{
		va_start(args, format);
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
	//mutex.Unlock();
}

void Logger::Log(LogLevel level, const char *name, int line, const char *format, ...)
{
	if (this->level < level)
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
	Utility::Time now = Utility::Time::Now();
	std::string date = now.ToDateString();

	//mutex.Lock();
	//iterate through all the targets of logging
	for (Outs::iterator o = m_outs.begin(); o != m_outs.end(); ++o)
	{
		va_start(args, format);
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
	//mutex.Unlock();
}

void Logger::Fill(const std::string& strLevel /* = EmptyString */, LogLevel defaultLogLevel /* = Notice */)
{
	//std::string str = line.Get<string>("--log", "");
	if (strLevel == "")
	{
		//stdlog(Delocust, LOGPLACE, "Setting new logging level from %s to %s", LevelNames[static_cast<int>(stdlog.GetLevel())], LevelNames[static_cast<int>(defaultLogLevel)]);
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


void Logger::AddFile(const char *name)
{
	//mutex.Lock();
	FILE *file;
	fopen_s(&file, name, "a");
	m_outs.push_back(file);

	//mutex.Unlock();
}
