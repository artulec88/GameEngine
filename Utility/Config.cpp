#include "StdAfx.h"
#include "Config.h"
#include <fstream>

using namespace Utility;

//Config::ValuesMap Config::cfgValues;
//Config::ValuesMap Config::cfgNotDefinedValues;
//bool Config::isInitialized = false;

Config::Config() : isInitialized(false)
{
}

//Config::Config(const std::string& fileName)
//{
//	LoadFromFile(fileName);
//}
//
//Config::~Config(void)
//{
//	cfgValues.clear();
//	cfgNotDefinedValues.clear();
//}

/* static */ void Config::LoadFromFile(const std::string& fileName)
{
	std::ifstream file(fileName.c_str());
	if (!file.is_open())
	{
		ERROR_LOG("Could not open configuration file \"%s\"", fileName.c_str());
		return;
	}
	cfgValues.clear();
	//cfgNotDefinedValues.clear();

	std::string name, value, line;
	while (!file.eof())
	{
		file >> name;
		if (file.fail())
		{
			EMERGENCY_LOG("Fail occured in the stream while reading the configuration file");
			exit(EXIT_FAILURE);
		}
		std::getline(file, line);
		if ((name.empty()) || (name[0] == '#')) // ignore comment lines
		{
			continue;
		}

		line = RightTrim(line.substr(0, line.find_first_of('#')));
		DELOCUST_LOG("Line after = \"%s\"", line.c_str());
		std::vector<std::string> tokens;
		CutToTokens(line, tokens, ' ');
		value = tokens[2];
		if (tokens.size() > 3)
		{
			for (unsigned int i = 3; i < tokens.size(); ++i)
			{
				if (tokens[i] == "#")
				{
					WARNING_LOG("Comment sign '#' found when it should be already removed");
					break;
				}
				value += " " + tokens[i];
			}
		}
		DEBUG_LOG("Configuration parameter \"%s\" = \"%s\"", name.c_str(), value.c_str());
		cfgValues[name] = value;
	}

	isInitialized = true;
}

std::string Config::GetArg(const std::string& name, const std::string& defValue) const
{
	ASSERT(isInitialized);
	CHECK_CONDITION_ERROR_EXIT_ALWAYS(isInitialized, "Cannot get the config value for the argument \"%s\". The Config instance is not initialized.", name.c_str());

	ValuesMap::const_iterator valueMapIt = cfgValues.find(name);
	if (valueMapIt == cfgValues.end())
	{
		WARNING_LOG("The parameter \"%s\" has not been specified. Using default value \"%s\"", name.c_str(), defValue.c_str());
		//cfgNotDefinedValues[name] = defValue;
		return defValue;
	}

	return valueMapIt->second;
}

//std::string Config::ReportUndefined()
//{
//	std::stringstream stream;
//	for (ValuesMap::iterator it = cfgNotDefinedValues.begin(); it != cfgNotDefinedValues.end(); ++it)
//	{
//		stream << it->first << " = " << it->second << "\n";
//	}
//
//	return stream.str();
//}