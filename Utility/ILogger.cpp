#include "StdAfx.h"
#include "ILogger.h"
#include "LoggerWindows.h"
#include <iostream>

/* static */ std::map<std::string, std::unique_ptr<Utility::Logging::ILogger>> Utility::Logging::ILogger::loggers;

/* static */ const std::array<std::string, Utility::Logging::COUNT> Utility::Logging::ILogger::LOGGING_LEVEL_NAMES =
{
	"Critical",
	"Emergency",
	"Error",
	"Warning",
	"Notice",
	"Info",
	"Debug",
	"Delocust"
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
	std::cout << "Logging level set to " << LOGGING_LEVEL_NAMES[level] << std::endl;
	m_level = level;
}