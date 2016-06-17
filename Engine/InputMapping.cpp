#include "stdafx.h"
#include "InputMapping.h"
#include "InputContext.h"

#include "Utility\ILogger.h"
#include "Utility\FileManager.h"

#include <fstream>


Engine::Input::InputMapping::InputMapping(const std::string& contextListFileName)
{
	unsigned count;
	std::ifstream inFile(contextListFileName);
	if (!(inFile >> count))
	{
		EMERGENCY_LOG_ENGINE("Failed to read context list file \"%s\".", contextListFileName.c_str());
	}
	else
	{
		for (unsigned i = 0; i < count; ++i)
		{
			std::string name = Utility::FileManager::AttemptRead<std::string>(inFile);
			std::string file = Utility::FileManager::AttemptRead<std::string>(inFile);
			m_inputContexts[name] = std::make_unique<InputContext>(file);
		}
	}
}


Engine::Input::InputMapping::~InputMapping()
{
}

void Engine::Input::InputMapping::ClearActions()
{
	m_currentMappedInput.m_actions.clear();
}

void Engine::Input::InputMapping::ClearRanges()
{
	m_currentMappedInput.m_ranges.clear();
}

void Engine::Input::InputMapping::SetRawButtonState(RawInputKeys::RawInputKey button, bool pressed, bool previouslyPressed)
{
	if (pressed)
	{
		if (!previouslyPressed) // action
		{
			Actions::Action action = MapButtonToAction(button);
			if (action != Actions::ACTION_INVALID)
			{
				m_currentMappedInput.m_actions.insert(action);
			}
			else
			{
				WARNING_LOG_ENGINE("Invalid action");
			}
		}
		else // state
		{
			States::State state = MapButtonToState(button);
			if (state != States::STATE_INVALID)
			{
				m_currentMappedInput.m_states.insert(state);
			}
		}
	}
	else
	{
		MapAndConsumeButton(button);
	}
}

void Engine::Input::InputMapping::SetRawAxisValue(RawInputAxes::RawInputAxis axis, Math::Real value)
{
	for (std::list<InputContext*>::const_iterator iter = m_activeContexts.begin(); iter != m_activeContexts.end(); ++iter)
	{
		const InputContext* inputContext = *iter;
		Ranges::Range range = inputContext->MapAxisToRange(axis);
		if (range != Ranges::RANGE_INVALID)
		{
			m_currentMappedInput.m_ranges[range] = inputContext->GetConverter().Convert(range, value * inputContext->GetSensitivity(range));
			break;
		}
	}
}

void Engine::Input::InputMapping::Dispatch() const
{
	MappedInput input = m_currentMappedInput;
	for (std::multimap<int, InputCallback>::const_iterator iter = m_callbackTable.begin(); iter != m_callbackTable.end(); ++iter)
	{
		(*iter->second)(input);
	}
}

//
// Add a callback to the dispatch table
//
void Engine::Input::InputMapping::RegisterCallback(InputCallback callback, int priority)
{
	m_callbackTable.insert(std::make_pair(priority, callback));
}


//
// Push an active input context onto the stack
//
void Engine::Input::InputMapping::PushContext(const std::string& name)
{
	std::map<std::string, std::unique_ptr<InputContext>>::iterator iter = m_inputContexts.find(name);
	CHECK_CONDITION_EXIT_ALWAYS_ENGINE(iter != m_inputContexts.end(), Utility::Emergency, "Invalid input context pushed");
	m_activeContexts.push_front(iter->second.get());
}

//
// Pop the current input context off the stack
//
void Engine::Input::InputMapping::PopContext()
{
	CHECK_CONDITION_EXIT_ALWAYS_ENGINE(!m_activeContexts.empty(), Utility::Error, "Cannot pop input context. No context is currently active.");
	m_activeContexts.pop_front();
}


//
// Helper: map a button to an action in the active context(s)
//
Engine::Input::Actions::Action Engine::Input::InputMapping::MapButtonToAction(RawInputKeys::RawInputKey button) const
{
	Actions::Action action = Actions::ACTION_INVALID;
	for (std::list<InputContext*>::const_iterator iter = m_activeContexts.begin(); iter != m_activeContexts.end(); ++iter)
	{
		const InputContext* context = *iter;

		action = context->MapButtonToAction(button);
		if (action != Actions::ACTION_INVALID)
		{
			break;
		}
	}
	return action;
}

//
// Helper: map a button to a state in the active context(s)
//
Engine::Input::States::State Engine::Input::InputMapping::MapButtonToState(RawInputKeys::RawInputKey button) const
{
	States::State state = States::STATE_INVALID;
	for (std::list<InputContext*>::const_iterator iter = m_activeContexts.begin(); iter != m_activeContexts.end(); ++iter)
	{
		const InputContext* inputContext = *iter;
		state = inputContext->MapButtonToState(button);
		if (state != States::STATE_INVALID)
		{
			break;
		}
	}
	return state;
}

//
// Helper: eat all input mapped to a given button
//
void Engine::Input::InputMapping::MapAndConsumeButton(RawInputKeys::RawInputKey button)
{
	Actions::Action action = MapButtonToAction(button);
	if (action != Actions::ACTION_INVALID)
	{
		m_currentMappedInput.ConsumeAction(action);
	}
	States::State state = MapButtonToState(button);
	if (state != States::STATE_INVALID)
	{
		m_currentMappedInput.ConsumeState(state);
	}
}

