#include "StdAfx.h"
#include "ILogger.h"
#include "LoggerWindows.h"
#include <fstream>

/* static */ std::map<std::string, std::unique_ptr<utility::logging::ILogger>> utility::logging::ILogger::loggers;

/* static */ const std::array<std::string, utility::logging::COUNT> utility::logging::ILogger::LOGGING_LEVEL_NAMES =
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

/* static */ utility::logging::ILogger& utility::logging::ILogger::GetLogger(const std::string& moduleName)
{
	if (ILogger::loggers.find(moduleName) == ILogger::loggers.end())
	{
		ILogger::loggers[moduleName] = std::make_unique<LoggerWindows>(); // TODO: Create object in accordance to the platform we are currently using.
	}
	return *ILogger::loggers[moduleName];
}

utility::logging::ILogger::ILogger(const char* fileName /* = nullptr */) :
	m_level(NOTICE)
{
	if (fileName != nullptr)
	{
		AddStream(fileName);
	}
}


utility::logging::ILogger::~ILogger()
{
	for (Outs::iterator outItr = m_outs.begin(); outItr != m_outs.end(); ++outItr)
	{
		(*outItr).close();
	}
}

void utility::logging::ILogger::SetLevel(const LogLevel level)
{
	//mutex.Lock();
	//if ((!m_modified) || (level > m_level))
	//{
	//	std::cout << "Logging level set to " << LevelNames[level] << std::endl;
	//	m_level = level;
	//}
	//m_modified = true;
	//mutex.Unlock();
	//std::cout << "Logging level set to " << LOGGING_LEVEL_NAMES[level] << std::endl;
	m_level = level;
}

void utility::logging::ILogger::AddStream(const char* fileName)
{
	WARNING_LOG_UTILITY("Logging to files doesn't work. This function call will be ignored.");
	return;
	//m_outs.push_back(std::move(std::ofstream(fileName, std::ios::out)));
}