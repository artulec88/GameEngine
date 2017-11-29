#include "stdafx.h"
#include "InputRangeConverter.h"

#include "Utility/ILogger.h"
#include "Utility/IConfig.h"
#include "Utility/FileManager.h"

#include <fstream>

engine::input::InputRangeConverter::InputRangeConverter(const std::string& inputContextName)
{
	const auto convertersCount = GET_CONFIG_VALUE_ENGINE(inputContextName + "ConvertersCount", 0);
	for (auto i = 0; i < convertersCount; ++i)
	{
		std::stringstream ss("");
		ss << convertersCount + 1;

		const auto range = static_cast<ranges::Range>(GET_CONFIG_VALUE_ENGINE(inputContextName + "Range_" + ss.str(), 0));
		Converter converter(GET_CONFIG_VALUE_ENGINE(inputContextName + "ConverterMinInput_" + ss.str(), 0.0f), GET_CONFIG_VALUE_ENGINE(inputContextName + "ConverterMaxInput_" + ss.str(), 0.0f),
			GET_CONFIG_VALUE_ENGINE(inputContextName + "ConverterMinOutput_" + ss.str(), 0.0f), GET_CONFIG_VALUE_ENGINE(inputContextName + "ConverterMaxOutput_" + ss.str(), 0.0f));
		CHECK_CONDITION_EXIT_ALWAYS_ENGINE(converter.maxInput >= converter.minInput, utility::logging::ERR,
			"Invalid input range. Max input range (", converter.maxInput, ") must not be less than min input range (", converter.minInput, ")");
		CHECK_CONDITION_EXIT_ALWAYS_ENGINE(converter.maxOutput >= converter.minOutput, utility::logging::ERR,
			"Invalid output range. Max output range (", converter.maxOutput, ") must not be less than min output range (", converter.minOutput, ")");
		m_convertersMap.insert(std::make_pair(range, converter));
	}
}

engine::input::InputRangeConverter::InputRangeConverter(std::ifstream& inFileStream)
{
	if (!inFileStream)
	{
		EMERGENCY_LOG_ENGINE("Invalid file provided to RangeConverter constructor");
	}

	const auto convertersCount = utility::FileManager::AttemptRead<unsigned>(inFileStream);
	for (auto i = 0; i < convertersCount; ++i)
	{
		ranges::Range range;
		Converter converter;

		range = static_cast<ranges::Range>(utility::FileManager::AttemptRead<unsigned>(inFileStream));
		converter.minInput = utility::FileManager::AttemptRead<math::Real>(inFileStream);
		converter.maxInput = utility::FileManager::AttemptRead<math::Real>(inFileStream);
		converter.minOutput = utility::FileManager::AttemptRead<math::Real>(inFileStream);
		converter.maxOutput = utility::FileManager::AttemptRead<math::Real>(inFileStream);

		CHECK_CONDITION_EXIT_ALWAYS_ENGINE(converter.maxInput >= converter.minInput, utility::logging::ERR,
			"Invalid input range. Max input range (", converter.maxInput, ") must not be less than min input range (", converter.minInput, ")");
		CHECK_CONDITION_EXIT_ALWAYS_ENGINE(converter.maxOutput >= converter.minOutput, utility::logging::ERR,
			"Invalid output range. Max output range (", converter.maxOutput, ") must not be less than min output range (", converter.minOutput, ")");

		m_convertersMap.insert(std::make_pair(range, converter));
	}
}


engine::input::InputRangeConverter::~InputRangeConverter()
{
}
