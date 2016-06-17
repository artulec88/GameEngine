#include "stdafx.h"
#include "InputContext.h"

#include "Utility\FileManager.h"

Engine::Input::InputContext::InputContext(const std::string& inputContextFileName) :
	m_converter(nullptr)
{
	std::ifstream inFile(inputContextFileName.c_str());

	unsigned rangesCount = Utility::FileManager::AttemptRead<unsigned>(inFile);
	for (unsigned i = 0; i < rangesCount; ++i)
	{
		RawInputAxes::RawInputAxis axis = static_cast<RawInputAxes::RawInputAxis>(Utility::FileManager::AttemptRead<unsigned>(inFile));
		Ranges::Range range = static_cast<Ranges::Range>(Utility::FileManager::AttemptRead<unsigned>(inFile));
		m_rangesMap[axis] = range;
	}

	unsigned statesCount = Utility::FileManager::AttemptRead<unsigned>(inFile);
	for (unsigned i = 0; i < statesCount; ++i)
	{
		RawInputKeys::RawInputKey button = static_cast<RawInputKeys::RawInputKey>(Utility::FileManager::AttemptRead<unsigned>(inFile));
		States::State state = static_cast<States::State>(Utility::FileManager::AttemptRead<unsigned>(inFile));
		m_statesMap[button] = state;
	}

	unsigned actionsCount = Utility::FileManager::AttemptRead<unsigned>(inFile);
	for (unsigned i = 0; i < actionsCount; ++i)
	{
		RawInputKeys::RawInputKey button = static_cast<RawInputKeys::RawInputKey>(Utility::FileManager::AttemptRead<unsigned>(inFile));
		Actions::Action action = static_cast<Actions::Action>(Utility::FileManager::AttemptRead<unsigned>(inFile));
		m_actionsMap[button] = action;
		DEBUG_LOG_ENGINE("Button = %d, action = %d", button, action);
	}

	m_converter = std::make_unique<InputRangeConverter>(inFile);

	unsigned sensitivitiesCount = Utility::FileManager::AttemptRead<unsigned>(inFile);
	for (unsigned i = 0; i < sensitivitiesCount; ++i)
	{
		Ranges::Range range = static_cast<Ranges::Range>(Utility::FileManager::AttemptRead<unsigned>(inFile));
		Math::Real sensitivity = Utility::FileManager::AttemptRead<Math::Real>(inFile);
		m_sensitivitiesMap[range] = sensitivity;
	}
}


Engine::Input::InputContext::~InputContext()
{
}

Engine::Input::Actions::Action Engine::Input::InputContext::MapButtonToAction(RawInputKeys::RawInputKey button) const
{
	std::map<RawInputKeys::RawInputKey, Actions::Action>::const_iterator actionsItr = m_actionsMap.find(button);
	if (actionsItr == m_actionsMap.end())
	{
		return Actions::ACTION_INVALID;
	}
	return actionsItr->second;
}

Engine::Input::States::State Engine::Input::InputContext::MapButtonToState(RawInputKeys::RawInputKey button) const
{
	std::map<RawInputKeys::RawInputKey, States::State>::const_iterator statesItr = m_statesMap.find(button);
	if (statesItr == m_statesMap.end())
	{
		return States::STATE_INVALID;
	}
	return statesItr->second;
}

Engine::Input::Ranges::Range Engine::Input::InputContext::MapAxisToRange(RawInputAxes::RawInputAxis axis) const
{
	std::map<RawInputAxes::RawInputAxis, Ranges::Range>::const_iterator rangesItr = m_rangesMap.find(axis);
	if (rangesItr == m_rangesMap.end())
	{
		return Ranges::RANGE_INVALID;
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