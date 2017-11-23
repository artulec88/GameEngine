#include "stdafx.h"
#include "InputContext.h"

#include "Utility/IConfig.h"
#include "Utility/FileManager.h"

#include <sstream>

engine::Input::InputContext::InputContext(const std::string& inputContextName) :
	m_converter(nullptr)
{
	CRITICAL_LOG_ENGINE("InputContextName = ", inputContextName);
	unsigned int rangesCount = GET_CONFIG_VALUE_ENGINE(inputContextName + "RangesCount", 0);
	for (unsigned int i = 0; i < rangesCount; ++i)
	{
		std::stringstream ss("");
		ss << (i + 1);
		RawInputAxes::RawInputAxis axis = static_cast<RawInputAxes::RawInputAxis>(GET_CONFIG_VALUE_ENGINE(inputContextName + "RawInputAxis_" + ss.str(), static_cast<int>(RawInputAxes::RAW_INPUT_AXIS_MOUSE_X)));
		Ranges::Range range = static_cast<Ranges::Range>(GET_CONFIG_VALUE_ENGINE(inputContextName + "Range_" + ss.str(), static_cast<int>(Ranges::INVALID)));
		m_rangesMap[axis] = range;
		ERROR_LOG_ENGINE("Axis = ", axis, " mapped to range = ", range);
	}

	unsigned int statesCount = GET_CONFIG_VALUE_ENGINE(inputContextName + "StatesCount", 0);
	for (unsigned int i = 0; i < statesCount; ++i)
	{
		std::stringstream ss("");
		ss << (i + 1);
		raw_input_keys::RawInputKey button = static_cast<raw_input_keys::RawInputKey>(GET_CONFIG_VALUE_ENGINE(inputContextName + "RawInputKey_" + ss.str(), 0));
		States::State state = static_cast<States::State>(GET_CONFIG_VALUE_ENGINE(inputContextName + "State_" + ss.str(), 0));
		m_statesMap[button] = state;
		ERROR_LOG_ENGINE("Button = ", button, " mapped to state = ", state);
	}

	unsigned int actionsCount = GET_CONFIG_VALUE_ENGINE(inputContextName + "ActionsCount", 0);
	for (unsigned int i = 0; i < actionsCount; ++i)
	{
		std::stringstream ss("");
		ss << (i + 1);
		raw_input_keys::RawInputKey button = static_cast<raw_input_keys::RawInputKey>(GET_CONFIG_VALUE_ENGINE(inputContextName + "RawInputKey_" + ss.str(), 0));
		engine::Actions::Action action = static_cast<engine::Actions::Action>(GET_CONFIG_VALUE_ENGINE(inputContextName + "Action_" + ss.str(), 0));
		m_actionsMap[button] = action;
		ERROR_LOG_ENGINE("Button = ", button, " mapped to action = ", action);
	}

	m_converter = std::make_unique<InputRangeConverter>(inputContextName);

	unsigned int sensitivitiesCount = GET_CONFIG_VALUE_ENGINE(inputContextName + "SensitivitiesCount", 0);
	ERROR_LOG_ENGINE("Sensitivities count = ", sensitivitiesCount);
	for (unsigned int i = 0; i < sensitivitiesCount; ++i)
	{
		std::stringstream ss("");
		ss << (i + 1);
		Ranges::Range range = static_cast<Ranges::Range>(GET_CONFIG_VALUE_ENGINE(inputContextName + "SensitivityRange_" + ss.str(), 0));
		math::Real sensitivity = GET_CONFIG_VALUE_ENGINE(inputContextName + "Sensitivity_" + ss.str(), REAL_ZERO);
		m_sensitivitiesMap[range] = sensitivity;
		ERROR_LOG_ENGINE("Range = ", range, " mapped to sensitivity = ", sensitivity);
	}
}

engine::Input::InputContext::InputContext(std::ifstream& inputContextFile) :
	m_converter(nullptr)
{
	unsigned int rangesCount = utility::FileManager::AttemptRead<unsigned>(inputContextFile);
	for (unsigned int i = 0; i < rangesCount; ++i)
	{
		RawInputAxes::RawInputAxis axis = static_cast<RawInputAxes::RawInputAxis>(utility::FileManager::AttemptRead<unsigned>(inputContextFile));
		Ranges::Range range = static_cast<Ranges::Range>(utility::FileManager::AttemptRead<unsigned>(inputContextFile));
		m_rangesMap[axis] = range;
		DEBUG_LOG_ENGINE("Axis = ", axis, " mapped to range = ", range);
	}

	unsigned int statesCount = utility::FileManager::AttemptRead<unsigned>(inputContextFile);
	for (unsigned int i = 0; i < statesCount; ++i)
	{
		raw_input_keys::RawInputKey button = static_cast<raw_input_keys::RawInputKey>(utility::FileManager::AttemptRead<unsigned>(inputContextFile));
		States::State state = static_cast<States::State>(utility::FileManager::AttemptRead<unsigned>(inputContextFile));
		m_statesMap[button] = state;
		DEBUG_LOG_ENGINE("Button = ", button, " mapped to state = ", state);
	}

	unsigned int actionsCount = utility::FileManager::AttemptRead<unsigned>(inputContextFile);
	for (unsigned int i = 0; i < actionsCount; ++i)
	{
		raw_input_keys::RawInputKey button = static_cast<raw_input_keys::RawInputKey>(utility::FileManager::AttemptRead<unsigned>(inputContextFile));
		Actions::Action action = static_cast<Actions::Action>(utility::FileManager::AttemptRead<unsigned>(inputContextFile));
		m_actionsMap[button] = action;
		DEBUG_LOG_ENGINE("Button = ", button, " mapped to action = ", action);
	}

	m_converter = std::make_unique<InputRangeConverter>(inputContextFile);

	unsigned int sensitivitiesCount = utility::FileManager::AttemptRead<unsigned>(inputContextFile);
	DEBUG_LOG_ENGINE("Sensitivities count = ", sensitivitiesCount);
	for (unsigned int i = 0; i < sensitivitiesCount; ++i)
	{
		Ranges::Range range = static_cast<Ranges::Range>(utility::FileManager::AttemptRead<unsigned>(inputContextFile));
		math::Real sensitivity = utility::FileManager::AttemptRead<math::Real>(inputContextFile);
		m_sensitivitiesMap[range] = sensitivity;
		DEBUG_LOG_ENGINE("Range = ", range, " mapped to sensitivity = ", sensitivity);
	}
}


engine::Input::InputContext::~InputContext()
{
}

engine::Actions::Action engine::Input::InputContext::MapButtonToAction(raw_input_keys::RawInputKey button) const
{
	//for (std::map<raw_input_keys::RawInputKey, Actions::Action>::const_iterator actionsItr = m_actionsMap.begin(); actionsItr != m_actionsMap.end(); ++actionsItr)
	//{
	//	CRITICAL_LOG_ENGINE("")
	//}
	std::map<raw_input_keys::RawInputKey, Actions::Action>::const_iterator actionsItr = m_actionsMap.find(button);
	if (actionsItr == m_actionsMap.end())
	{
		return Actions::INVALID;
	}
	return actionsItr->second;
}

engine::States::State engine::Input::InputContext::MapButtonToState(raw_input_keys::RawInputKey button) const
{
	std::map<raw_input_keys::RawInputKey, States::State>::const_iterator statesItr = m_statesMap.find(button);
	return (statesItr == m_statesMap.end()) ? States::INVALID : statesItr->second;
}

engine::Ranges::Range engine::Input::InputContext::MapAxisToRange(RawInputAxes::RawInputAxis axis) const
{
	std::map<RawInputAxes::RawInputAxis, Ranges::Range>::const_iterator rangesItr = m_rangesMap.find(axis);
	if (rangesItr == m_rangesMap.end())
	{
		return Ranges::INVALID;
	}
	return rangesItr->second;
}

math::Real engine::Input::InputContext::GetSensitivity(Ranges::Range range) const
{
	std::map<Ranges::Range, math::Real>::const_iterator iter = m_sensitivitiesMap.find(range);
	if (iter == m_sensitivitiesMap.end())
	{
		return REAL_ONE;
	}
	return iter->second;
}
