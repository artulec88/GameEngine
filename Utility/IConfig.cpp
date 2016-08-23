#include "StdAfx.h"
#include "IConfig.h"
#include "Config.h"

/* static */ std::map<std::string, std::unique_ptr<Utility::IConfig>> Utility::IConfig::configurations;

/* static */ void Utility::IConfig::CreateConfig(const std::string& moduleName, const std::string& configFileName)
{
	CHECK_CONDITION_RETURN_VOID_UTILITY(configurations.find(moduleName) != configurations.end(), Logging::ERR,
		"Configuration for \"", configFileName, "\" module \"", moduleName, "\" has already been created");
	configurations[moduleName] = std::make_unique<Config>(configFileName);
}

/* static */ const Utility::IConfig& Utility::IConfig::GetConfig(const std::string& moduleName)
{
	std::map<std::string, std::unique_ptr<IConfig>>::iterator confItr = configurations.find(moduleName);
	CHECK_CONDITION_UTILITY(confItr != configurations.end(), Utility::Logging::ERR, "There is no configuration registered for module \"", moduleName, "\"");
	return *confItr->second;
}

Utility::IConfig::IConfig(const std::string& fileName) :
	m_fileName(fileName)
{
	//LoadFromFile();
}

Utility::IConfig::~IConfig()
{
}
