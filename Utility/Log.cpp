#include "stdafx.h"
#include "Log.h"
//#include "CommandLine.h"
#include <string>
#include "Time.h"

using namespace Utility;
using namespace std;

const char *Log::LevelNames[] = 
{
	"critical",
	"emergency",
	"error",
	"warning",
	"notice",
	"info",
	"debug",
	"delocust",
	NULL
};

Log stdlog;
	
Log::Log(FILE *first) :
	m_level(Notice),
	m_modified(false)
{
	if (first)
	{
		m_outs.push_back(first);
	}
}

Log::~Log()
{

}

void Log::operator()(LogLevel level, const char *name, int line, const char *format, ...)
{
	if (this->m_level < level)
	{
		return;
	}

	va_list args;
	if (name == NULL)
	{
		name = "nowhere";
	}
	const char *tmp = strrchr(name, '/');
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

LogLevel Log::GetLevel() const
{
	return this->m_level;
}

void Log::SetLevel(LogLevel level)
{
	//mutex.Lock();
	if ((!m_modified) || (level > this->m_level))
	{
		this->m_level = level;
	}
	m_modified = true;
	//mutex.Unlock();
}
			

void Log::Fill(const std::string& strLevel /* = EmptyString */, LogLevel defaultLogLevel /* = Notice */)
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
}


void Log::AddFile(const char *name)
{
	//mutex.Lock();
	FILE *file;
	fopen_s(&file, name, "a");
	m_outs.push_back(file);

	//mutex.Unlock();
}
