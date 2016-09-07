#include "stdafx.h"
#include "InputContext.h"

#include "Utility\IConfig.h"
#include "Utility\FileManager.h"

#include <sstream>

Engine::Input::InputContext::InputContext(const std::string& inputContextName) :
	m_converter(nullptr)
{
	CRITICAL_LOG_ENGINE("InputContextName = ", inputContextName);
	unsigned int rangesCount = GET_CONFIG_VALUE_ENGINE(inputContextName + "RangesCount", 0);
	for (unsigned int i = 0; i < rangesCount; ++i)
	{
		std::stringstream ss("");
		ss << (i + 1);
		RawInputAxes::RawInputAxis axis = static_cast<RawInputAxes::RawInputAxis>(GET_CONFIG_VALUE_ENGINE(inputContextName + "RawInputAxis_" + ss.str(), 0));
		Ranges::Range range = static_cast<Ranges::Range>(GET_CONFIG_VALUE_ENGINE(inputContextName + "Range_" + ss.str(), 0));
		m_rangesMap[axis] = range;
	}

	unsigned int statesCount = GET_CONFIG_VALUE_ENGINE(inputContextName + "StatesCount", 0);
	for (unsigned int i = 0; i < statesCount; ++i)
	{
		std::stringstream ss("");
		ss << (i + 1);
		RawInputKeys::RawInputKey button = static_cast<RawInputKeys::RawInputKey>(GET_CONFIG_VALUE_ENGINE(inputContextName + "RawInputKey_" + ss.str(), 0));
		States::State state = static_cast<States::State>(GET_CONFIG_VALUE_ENGINE(inputContextName + "State_" + ss.str(), 0));
		m_statesMap[button] = state;
		ERROR_LOG_ENGINE("Button = ", button, " mapped to state = ", state);
	}

	unsigned int actionsCount = GET_CONFIG_VALUE_ENGINE(inputContextName + "ActionsCount", 0);
	for (unsigned int i = 0; i < actionsCount; ++i)
	{
		std::stringstream ss("");
		ss << (i + 1);
		RawInputKeys::RawInputKey button = static_cast<RawInputKeys::RawInputKey>(GET_CONFIG_VALUE_ENGINE(inputContextName + "RawInputKey_" + ss.str(), 0));
		Engine::Actions::Action action = static_cast<Engine::Actions::Action>(GET_CONFIG_VALUE_ENGINE(inputContextName + "Action_" + ss.str(), 0));
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
		Math::Real sensitivity = GET_CONFIG_VALUE_ENGINE(inputContextName + "Sensitivity_" + ss.str(), REAL_ZERO);
		m_sensitivitiesMap[range] = sensitivity;
		ERROR_LOG_ENGINE("Range = ", range, " mapped to sensitivity = ", sensitivity);
	}
}

Engine::Input::InputContext::InputContext(std::ifstream& inputContextFile) :
	m_converter(nullptr)
{
	unsigned int rangesCount = Utility::FileManager::AttemptRead<unsigned>(inputContextFile);
	for (unsigned int i = 0; i < rangesCount; ++i)
	{
		RawInputAxes::RawInputAxis axis = static_cast<RawInputAxes::RawInputAxis>(Utility::FileManager::AttemptRead<unsigned>(inputContextFile));
		Ranges::Range range = static_cast<Ranges::Range>(Utility::FileManager::AttemptRead<unsigned>(inputContextFile));
		m_rangesMap[axis] = range;
	}

	unsigned int statesCount = Utility::FileManager::AttemptRead<unsigned>(inputContextFile);
	for (unsigned int i = 0; i < statesCount; ++i)
	{
		RawInputKeys::RawInputKey button = static_cast<RawInputKeys::RawInputKey>(Utility::FileManager::AttemptRead<unsigned>(inputContextFile));
		States::State state = static_cast<States::State>(Utility::FileManager::AttemptRead<unsigned>(inputContextFile));
		m_statesMap[button] = state;
		ERROR_LOG_ENGINE("Button = ", button, " mapped to state = ", state);
	}

	unsigned int actionsCount = Utility::FileManager::AttemptRead<unsigned>(inputContextFile);
	for (unsigned int i = 0; i < actionsCount; ++i)
	{
		RawInputKeys::RawInputKey button = static_cast<RawInputKeys::RawInputKey>(Utility::FileManager::AttemptRead<unsigned>(inputContextFile));
		Actions::Action action = static_cast<Actions::Action>(Utility::FileManager::AttemptRead<unsigned>(inputContextFile));
		m_actionsMap[button] = action;
		ERROR_LOG_ENGINE("Button = ", button, " mapped to action = ", action);
	}

	m_converter = std::make_unique<InputRangeConverter>(inputContextFile);

	unsigned int sensitivitiesCount = Utility::FileManager::AttemptRead<unsigned>(inputContextFile);
	ERROR_LOG_ENGINE("Sensitivities count = ", sensitivitiesCount);
	for (unsigned int i = 0; i < sensitivitiesCount; ++i)
	{
		Ranges::Range range = static_cast<Ranges::Range>(Utility::FileManager::AttemptRead<unsigned>(inputContextFile));
		Math::Real sensitivity = Utility::FileManager::AttemptRead<Math::Real>(inputContextFile);
		m_sensitivitiesMap[range] = sensitivity;
		ERROR_LOG_ENGINE("Range = ", range, " mapped to sensitivity = ", sensitivity);
	}
}


Engine::Input::InputContext::~InputContext()
{
}

Engine::Actions::Action Engine::Input::InputContext::MapButtonToAction(RawInputKeys::RawInputKey button) const
{
	//for (std::map<RawInputKeys::RawInputKey, Actions::Action>::const_iterator actionsItr = m_actionsMap.begin(); actionsItr != m_actionsMap.end(); ++actionsItr)
	//{
	//	CRITICAL_LOG_ENGINE("")
	//}
	std::map<RawInputKeys::RawInputKey, Actions::Action>::const_iterator actionsItr = m_actionsMap.find(button);
	if (actionsItr == m_actionsMap.end())
	{
		return Actions::INVALID;
	}
	return actionsItr->second;
}

Engine::States::State Engine::Input::InputContext::MapButtonToState(RawInputKeys::RawInputKey button) const
{
	std::map<RawInputKeys::RawInputKey, States::State>::const_iterator statesItr = m_statesMap.find(button);
	if (statesItr == m_statesMap.end())
	{
		return States::INVALID;
	}
	return statesItr->second;
}

Engine::Ranges::Range Engine::Input::InputContext::MapAxisToRange(RawInputAxes::RawInputAxis axis) const
{
	std::map<RawInputAxes::RawInputAxis, Ranges::Range>::const_iterator rangesItr = m_rangesMap.find(axis);
	if (rangesItr == m_rangesMap.end())
	{
		return Ranges::INVALID;
	}
	return rangesItr->second;
}

Math::Real Engine::Input::InputContext::GetSensitivity(Ranges::Range range) const
{
	std::map<Ranges::Range, Math::Real>::const_iterator iter = m_sensitivitiesMap.find(range);
	if (iter == m_sensitivitiesMap.end())
	{
		return REAL_ONE;
	}
	return iter->second;
}
