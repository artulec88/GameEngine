#include "StdAfx.h"
#include "ILogger.h"
#include "LoggerWindows.h"
#include <iostream>

/* static */ std::map<std::string, std::unique_ptr<Utility::Logging::ILogger>> Utility::Logging::ILogger::loggers;

/* static */ const char* Utility::Logging::ILogger::LevelNames[] =
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

/* static */ Utility::Logging::ILogger& Utility::Logging::ILogger::GetLogger(const std::string& moduleName)
{
	if (ILogger::loggers.find(moduleName) == ILogger::loggers.end())
	{
		ILogger::loggers[moduleName] = std::make_unique<LoggerWindows>(); // TODO: Create object in accordance to the platform we are currently using.
	}
	return *ILogger::loggers[moduleName];
}

Utility::Logging::ILogger::ILogger(void) :
	m_level(NOTICE)
{
}


Utility::Logging::ILogger::~ILogger(void)
{
}

void Utility::Logging::ILogger::SetLevel(LogLevel level)
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