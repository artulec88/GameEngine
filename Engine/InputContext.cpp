#include "stdafx.h"
#include "InputContext.h"

#include "Utility/IConfig.h"
#include "Utility/FileManager.h"

#include <sstream>
#include <fstream>

engine::input::InputContext::InputContext(const std::string& inputContextName) :
	m_converter(nullptr)
{
	CRITICAL_LOG_ENGINE("InputContextName = ", inputContextName);
	const auto rangesCount = GET_CONFIG_VALUE_ENGINE(inputContextName + "RangesCount", 0);
	for (auto i = 0; i < rangesCount; ++i)
	{
		std::stringstream ss("");
		ss << i + 1;
		const auto axis = static_cast<raw_input_axes::RawInputAxis>(GET_CONFIG_VALUE_ENGINE(inputContextName + "RawInputAxis_" + ss.str(), static_cast<int>(raw_input_axes::RAW_INPUT_AXIS_MOUSE_X)));
		const auto range = static_cast<ranges::Range>(GET_CONFIG_VALUE_ENGINE(inputContextName + "Range_" + ss.str(), static_cast<int>(ranges::INVALID)));
		m_rangesMap[axis] = range;
		ERROR_LOG_ENGINE("Axis = ", axis, " mapped to range = ", range);
	}

	const auto statesCount = GET_CONFIG_VALUE_ENGINE(inputContextName + "StatesCount", 0);
	for (auto i = 0; i < statesCount; ++i)
	{
		std::stringstream ss("");
		ss << i + 1;
		const auto button = static_cast<raw_input_keys::RawInputKey>(GET_CONFIG_VALUE_ENGINE(inputContextName + "RawInputKey_" + ss.str(), 0));
		const auto state = static_cast<states::State>(GET_CONFIG_VALUE_ENGINE(inputContextName + "State_" + ss.str(), 0));
		m_statesMap[button] = state;
		ERROR_LOG_ENGINE("Button = ", button, " mapped to state = ", state);
	}

	const auto actionsCount = GET_CONFIG_VALUE_ENGINE(inputContextName + "ActionsCount", 0);
	for (auto i = 0; i < actionsCount; ++i)
	{
		std::stringstream ss("");
		ss << i + 1;
		const auto button = static_cast<raw_input_keys::RawInputKey>(GET_CONFIG_VALUE_ENGINE(inputContextName + "RawInputKey_" + ss.str(), 0));
		const auto action = static_cast<actions::Action>(GET_CONFIG_VALUE_ENGINE(inputContextName + "Action_" + ss.str(), 0));
		m_actionsMap[button] = action;
		ERROR_LOG_ENGINE("Button = ", button, " mapped to action = ", action);
	}

	m_converter = std::make_unique<InputRangeConverter>(inputContextName);

	const auto sensitivitiesCount = GET_CONFIG_VALUE_ENGINE(inputContextName + "SensitivitiesCount", 0);
	ERROR_LOG_ENGINE("Sensitivities count = ", sensitivitiesCount);
	for (auto i = 0; i < sensitivitiesCount; ++i)
	{
		std::stringstream ss("");
		ss << i + 1;
		const auto range = static_cast<ranges::Range>(GET_CONFIG_VALUE_ENGINE(inputContextName + "SensitivityRange_" + ss.str(), 0));
		const auto sensitivity = GET_CONFIG_VALUE_ENGINE(inputContextName + "Sensitivity_" + ss.str(), REAL_ZERO);
		m_sensitivitiesMap[range] = sensitivity;
		ERROR_LOG_ENGINE("Range = ", range, " mapped to sensitivity = ", sensitivity);
	}
}

engine::input::InputContext::InputContext(std::ifstream& inputContextFile) :
	m_converter(nullptr)
{
	const auto rangesCount = utility::FileManager::AttemptRead<unsigned>(inputContextFile);
	for (auto i = 0; i < rangesCount; ++i)
	{
		const auto axis = static_cast<raw_input_axes::RawInputAxis>(utility::FileManager::AttemptRead<unsigned>(inputContextFile));
		const auto range = static_cast<ranges::Range>(utility::FileManager::AttemptRead<unsigned>(inputContextFile));
		m_rangesMap[axis] = range;
		DEBUG_LOG_ENGINE("Axis = ", axis, " mapped to range = ", range);
	}

	const auto statesCount = utility::FileManager::AttemptRead<unsigned>(inputContextFile);
	for (auto i = 0; i < statesCount; ++i)
	{
		const auto button = static_cast<raw_input_keys::RawInputKey>(utility::FileManager::AttemptRead<unsigned>(inputContextFile));
		const auto state = static_cast<states::State>(utility::FileManager::AttemptRead<unsigned>(inputContextFile));
		m_statesMap[button] = state;
		DEBUG_LOG_ENGINE("Button = ", button, " mapped to state = ", state);
	}

	const auto actionsCount = utility::FileManager::AttemptRead<unsigned>(inputContextFile);
	for (auto i = 0; i < actionsCount; ++i)
	{
		const auto button = static_cast<raw_input_keys::RawInputKey>(utility::FileManager::AttemptRead<unsigned>(inputContextFile));
		const auto action = static_cast<actions::Action>(utility::FileManager::AttemptRead<unsigned>(inputContextFile));
		m_actionsMap[button] = action;
		DEBUG_LOG_ENGINE("Button = ", button, " mapped to action = ", action);
	}

	m_converter = std::make_unique<InputRangeConverter>(inputContextFile);

	const auto sensitivitiesCount = utility::FileManager::AttemptRead<unsigned>(inputContextFile);
	DEBUG_LOG_ENGINE("Sensitivities count = ", sensitivitiesCount);
	for (auto i = 0; i < sensitivitiesCount; ++i)
	{
		const auto range = static_cast<ranges::Range>(utility::FileManager::AttemptRead<unsigned>(inputContextFile));
		const auto sensitivity = utility::FileManager::AttemptRead<math::Real>(inputContextFile);
		m_sensitivitiesMap[range] = sensitivity;
		DEBUG_LOG_ENGINE("Range = ", range, " mapped to sensitivity = ", sensitivity);
	}
}


engine::input::InputContext::~InputContext()
{
}

engine::actions::Action engine::input::InputContext::MapButtonToAction(raw_input_keys::RawInputKey button) const
{
	//for (auto actionsItr = m_actionsMap.begin(); actionsItr != m_actionsMap.end(); ++actionsItr)
	//{
	//	CRITICAL_LOG_ENGINE("")
	//}
	const auto actionsItr = m_actionsMap.find(button);
	return actionsItr == m_actionsMap.end() ? actions::INVALID : actionsItr->second;
}

engine::states::State engine::input::InputContext::MapButtonToState(raw_input_keys::RawInputKey button) const
{
	const auto statesItr = m_statesMap.find(button);
	return statesItr == m_statesMap.end() ? states::INVALID : statesItr->second;
}

engine::ranges::Range engine::input::InputContext::MapAxisToRange(raw_input_axes::RawInputAxis axis) const
{
	const auto rangesItr = m_rangesMap.find(axis);
	return rangesItr == m_rangesMap.end() ? ranges::INVALID : rangesItr->second;
}

math::Real engine::input::InputContext::GetSensitivity(ranges::Range range) const
{
	const auto iter = m_sensitivitiesMap.find(range);
	return iter == m_sensitivitiesMap.end() ? REAL_ONE : iter->second;
}
