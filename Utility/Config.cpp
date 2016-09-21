#include "StdAfx.h"
#include "Config.h"
#include "StringUtility.h"
#include "ILogger.h"
#include <fstream>

//Config::ValuesMap Config::cfgValues;
//Config::ValuesMap Config::cfgNotDefinedValues;
//bool Config::isInitialized = false;

Utility::Config::Config(const std::string& fileName) :
	IConfig(fileName)
{
	LoadFromFile();
}

//Utility::Config::Config(const std::string& fileName)
//{
//	LoadFromFile(fileName);
//}
//

Utility::Config::~Config()
{
	m_cfgValues.clear();
	//m_cfgNotDefinedValues.clear();
}

void Utility::Config::LoadFromFile()
{
	INFO_LOG_UTILITY("Loading configuration from file \"", m_fileName, "\".");
	std::ifstream file(m_fileName);
	CHECK_CONDITION_RETURN_VOID_ALWAYS_UTILITY(file.is_open(), Logging::ERR, "Could not open configuration file \"", m_fileName, "\".");
	m_cfgValues.clear();
	//cfgNotDefinedValues.clear();

	std::string value, line;
	while (!file.eof())
	{
		//file >> name;
		CHECK_CONDITION_EXIT_ALWAYS_UTILITY(!file.fail(), Logging::EMERGENCY, "Error occured in the stream while reading the configuration file ", m_fileName);
		std::getline(file, line);
		if ((line.empty()) || (line[0] == '#')) // ignore comment lines
		{
			continue;
		}

		line = line.substr(0, line.find_first_of('#'));
		StringUtility::RightTrim(line);
		DELOCUST_LOG_UTILITY("Line after trimming = \"", line, "\"");
		std::vector<std::string> tokens;
		StringUtility::CutToTokens(line, tokens, ' ');
		CHECK_CONDITION_UTILITY(tokens[1].compare("=") == 0, Logging::ERR, "Failed when parsing the line \"", line, "\" into tokens. Token[1] is \"", tokens[1], "\" but should be equal to \"=\".");
		value = tokens[2];
		if (tokens.size() > 3)
		{
			for (unsigned int i = 3; i < tokens.size(); ++i)
			{
				if (tokens[i] == "#")
				{
					WARNING_LOG_UTILITY("Comment sign '#' found when it should be already removed");
					break;
				}
				value += " " + tokens[i];
			}
		}
		DEBUG_LOG_UTILITY("Configuration parameter \"", tokens[0], "\" = \"", value, "\"");
		m_cfgValues[tokens[0]] = value;
	}

	file.close();
}

std::string Utility::Config::GetArg(const std::string& name, const std::string& defValue) const
{
	ValuesMap::const_iterator valueMapIt = m_cfgValues.find(name);
	if (valueMapIt == m_cfgValues.end())
	{
		WARNING_LOG_UTILITY("The parameter \"", name, "\" has not been specified. Using default value \"", defValue, "\"");
		//cfgNotDefinedValues[name] = defValue;
		return defValue;
	}

	return valueMapIt->second;
}

//std::string Utility::Config::ReportUndefined()
//{
//	std::stringstream stream;
//	for (ValuesMap::iterator it = cfgNotDefinedValues.begin(); it != cfgNotDefinedValues.end(); ++it)
//	{
//		stream << it->first << " = " << it->second << "\n";
//	}
//
//	return stream.str();
//}