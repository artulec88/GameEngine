#include "StdAfx.h"
#include "IConfig.h"
#include "Config.h"

/* static */ std::map<std::string, std::unique_ptr<utility::IConfig>> utility::IConfig::configurations;

/* static */ void utility::IConfig::CreateConfig(const std::string& moduleName, const std::string& configFileName)
{
	DEBUG_LOG_UTILITY("Reading configuration for module \"", moduleName, "\" from file \"", configFileName, "\".");
	CHECK_CONDITION_RETURN_VOID_UTILITY(configurations.find(moduleName) == configurations.end(), Logging::ERR,
		"Configuration for \"", configFileName, "\" module \"", moduleName, "\" has already been created");
	configurations[moduleName] = std::make_unique<Config>();
	configurations[moduleName]->LoadFromFile(configFileName);
}

/* static */ const utility::IConfig& utility::IConfig::GetConfig(const std::string& moduleName)
{
	const std::map<std::string, std::unique_ptr<IConfig>>::iterator confItr = configurations.find(moduleName);
	CHECK_CONDITION_UTILITY(confItr != configurations.end(), Utility::Logging::ERR, "There is no configuration registered for module \"", moduleName, "\"");
	return *confItr->second;
}

utility::IConfig::IConfig()
{
}

utility::IConfig::~IConfig()
{
}
