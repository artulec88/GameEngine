#include "stdafx.h"
#include "InputRangeConverter.h"

#include "Utility\ILogger.h"
#include "Utility\FileManager.h"


Engine::Input::InputRangeConverter::InputRangeConverter(std::ifstream& inFileStream)
{
	if (!inFileStream)
	{
		EMERGENCY_LOG_ENGINE("Invalid file provided to RangeConverter constructor");
	}

	unsigned convertersCount = Utility::FileManager::AttemptRead<unsigned>(inFileStream);
	for (unsigned i = 0; i < convertersCount; ++i)
	{
		Ranges::Range range;
		Converter converter;

		range = static_cast<Ranges::Range>(Utility::FileManager::AttemptRead<unsigned>(inFileStream));
		converter.m_minInput = Utility::FileManager::AttemptRead<Math::Real>(inFileStream);
		converter.m_maxInput = Utility::FileManager::AttemptRead<Math::Real>(inFileStream);
		converter.m_minOutput = Utility::FileManager::AttemptRead<Math::Real>(inFileStream);
		converter.m_maxOutput = Utility::FileManager::AttemptRead<Math::Real>(inFileStream);

		CHECK_CONDITION_EXIT_ALWAYS_ENGINE(converter.m_maxInput >= converter.m_minInput, Utility::Error,
			"Invalid input range. Max input range (%.2f) must not be less than min input range (%.2f)",
			converter.m_maxInput, converter.m_minInput);
		CHECK_CONDITION_EXIT_ALWAYS_ENGINE(converter.m_maxOutput >= converter.m_minOutput, Utility::Error,
			"Invalid output range. Max output range (%.2f) must not be less than min output range (%.2f)",
			converter.m_maxOutput, converter.m_minOutput);

		m_convertersMap.insert(std::make_pair(range, converter));
	}
}


Engine::Input::InputRangeConverter::~InputRangeConverter()
{
}
