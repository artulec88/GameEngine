#include "stdafx.h"
#include "CommandLineMapper.h"
#include "Time.h"

//#define LOGMODULE "Command"
#include "ILogger.h"

utility::CommandLineMapper::CommandLineMapper(const int argc, char* argv[]) : ICommandLineMapper()
{
	DELOCUST_LOG_UTILITY("CommandLineMapper constructor");
	CHECK_CONDITION_UTILITY(argc >= 1, Utility::Logging::ERR, "Incorrect number of program parameters specified (", argc, ")");
	CHECK_CONDITION_UTILITY(argv != nullptr, Utility::Logging::ERR, "Incorrect program parameters specified.");
	// We specifically ignore the argv[0] argument, because it's just the name of the program.
	for (auto i = 1; i < argc - 1; i += 2)
	{
		m_argsToValuesMap.insert(make_pair(std::string(argv[i]), std::string(argv[i + 1])));
	}
}

utility::CommandLineMapper::~CommandLineMapper()
{
	DELOCUST_LOG_UTILITY("CommandLineMapper object gets destroyed");
	m_argsToValuesMap.clear();
}

const std::string& utility::CommandLineMapper::Get(const std::string& opt, const std::string& defaultValue) const
{
	const auto argToValueItr = m_argsToValuesMap.find(opt);
	return argToValueItr != m_argsToValuesMap.end() ? argToValueItr->second : defaultValue;
}
