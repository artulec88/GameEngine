#include "StdAfx.h"
#include "Config.h"
#include "StringUtility.h"
#include <fstream>

using namespace Utility;

//Config::ValuesMap Config::cfgValues;
//Config::ValuesMap Config::cfgNotDefinedValues;
//bool Config::isInitialized = false;

Config::Config(const std::string& fileName) :
	IConfig(fileName)
{
	LoadFromFile();
}

//Config::Config(const std::string& fileName)
//{
//	LoadFromFile(fileName);
//}
//

Config::~Config()
{
	m_cfgValues.clear();
	//m_cfgNotDefinedValues.clear();
}

void Config::LoadFromFile()
{
	std::ifstream file(m_fileName.c_str());
	if (!file.is_open())
	{
		ERROR_LOG("Could not open configuration file \"%s\"", m_fileName.c_str());
		return;
	}
	m_cfgValues.clear();
	//cfgNotDefinedValues.clear();

	std::string value, line;
	while (!file.eof())
	{
		//file >> name;
		if (file.fail())
		{
			EMERGENCY_LOG("Error occured in the stream while reading the configuration file \"%s\"", m_fileName.c_str());
			exit(EXIT_FAILURE);
		}
		std::getline(file, line);
		if ((line.empty()) || (line[0] == '#')) // ignore comment lines
		{
			continue;
		}

		line = line.substr(0, line.find_first_of('#'));
		StringUtility::RightTrim(line);
		DELOCUST_LOG("Line after trimming = \"%s\"", line.c_str());
		std::vector<std::string> tokens;
		StringUtility::CutToTokens(line, tokens, ' ');
		CHECK_CONDITION(tokens[1].compare("=") == 0, Utility::Error, "Failed when parsing the line \"%s\" into tokens. Token[1] is \"%s\" but should be equal to \"=\".",
			line.c_str(), tokens[1].c_str());
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
		DEBUG_LOG_UTILITY("Configuration parameter \"%s\" = \"%s\"", tokens[0].c_str(), value.c_str());
		m_cfgValues[tokens[0]] = value;
	}

	file.close();
}

std::string Config::GetArg(const std::string& name, const std::string& defValue) const
{
	ValuesMap::const_iterator valueMapIt = m_cfgValues.find(name);
	if (valueMapIt == m_cfgValues.end())
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