#include "stdafx.h"
#include "InputMapping.h"
#include "InputContext.h"
#include "GameManager.h"

#include "Utility/ILogger.h"
#include "Utility/FileManager.h"
//#include "Utility/IConfig.h"

#include <fstream>

engine::Input::InputMapping::InputMapping() //:
	//m_inputContexts({ {"mainMenuInputContext", } })
{
	m_inputContexts.insert(std::make_pair("mainMenuInputContext", std::make_unique<InputContext>("mainMenuInputContext")));
}

engine::Input::InputMapping::InputMapping(const std::string& configDirectory, const std::string& contextListFileName)
{
	unsigned count;
	std::ifstream inFile(configDirectory + contextListFileName);
	if (!(inFile >> count))
	{
		EMERGENCY_LOG_ENGINE("Failed to read context list file \"", contextListFileName, "\".");
	}
	else
	{
		for (unsigned i = 0; i < count; ++i)
		{
			//Utility::IConfig::CreateConfig()
			const std::string name = utility::FileManager::AttemptRead<std::string>(inFile);
			const std::string fileName = configDirectory + utility::FileManager::AttemptRead<std::string>(inFile);
			INFO_LOG_ENGINE("Input context \"", name, "\" initialized with file \"", fileName, "\".");
			m_inputContexts[name] = std::make_unique<InputContext>(std::ifstream(fileName));
		}
	}
}


engine::Input::InputMapping::~InputMapping()
{
}

void engine::Input::InputMapping::ClearActions()
{
	m_currentMappedInput.m_actions.clear();
}

void engine::Input::InputMapping::ClearRanges()
{
	m_currentMappedInput.m_ranges.clear();
}

void engine::Input::InputMapping::SetRawButtonState(raw_input_keys::RawInputKey button, bool pressed, bool previouslyPressed)
{
	DEBUG_LOG_ENGINE("Key = ", button, ", pressed = ", pressed, ", previously pressed = ", previouslyPressed);
	if (pressed)
	{
		if (!previouslyPressed) // action
		{
			Actions::Action action = MapButtonToAction(button);
			CHECK_CONDITION_RETURN_VOID_ALWAYS_ENGINE(action != Actions::INVALID, utility::logging::WARNING,
				"Invalid action for button ", button, "(pressed = ", pressed, ", previouslyPressed = ", previouslyPressed, ")");
			m_currentMappedInput.m_actions.insert(action);
		}
		else // state
		{
			States::State state = MapButtonToState(button);
			CHECK_CONDITION_RETURN_VOID_ALWAYS_ENGINE(state != States::INVALID, utility::logging::WARNING,
				"Invalid state for button ", button, "(pressed = ", pressed, ", previouslyPressed = ", previouslyPressed, ")");
			m_currentMappedInput.m_states.insert(state);
		}
	}
	else
	{
		MapAndConsumeButton(button);
	}
}

void engine::Input::InputMapping::SetRawAxisValue(RawInputAxes::RawInputAxis axis, math::Real value)
{
	for (std::list<InputContext*>::const_iterator iter = m_activeContexts.begin(); iter != m_activeContexts.end(); ++iter)
	{
		const InputContext* inputContext = *iter;
		Ranges::Range range = inputContext->MapAxisToRange(axis);
		if (range != Ranges::INVALID)
		{
			m_currentMappedInput.m_ranges[range] = inputContext->GetConverter().Convert(range, value * inputContext->GetSensitivity(range));
			break;
		}
	}
}

void engine::Input::InputMapping::Dispatch() const
{
	//MappedInput input = m_currentMappedInput;
	//for (std::multimap<int, InputCallback>::const_iterator iter = m_callbackTable.begin(); iter != m_callbackTable.end(); ++iter)
	//{
	//	(*iter->second)(input);
	//}

	//GameManager::GetGameManager()->Input(m_currentMappedInput.m_actions);
}

void engine::Input::InputMapping::RegisterCallback(InputCallback callback, int priority)
{
	m_callbackTable.insert(std::make_pair(priority, callback));
}

void engine::Input::InputMapping::PushContext(const std::string& name)
{
	std::map<std::string, std::unique_ptr<InputContext>>::iterator iter = m_inputContexts.find(name);
	CHECK_CONDITION_EXIT_ALWAYS_ENGINE(iter != m_inputContexts.end(), utility::logging::EMERGENCY, "Cannot push input context with name \"", name, "\". The input context cannot be found.");
	m_activeContexts.push_front(iter->second.get());
}

void engine::Input::InputMapping::PopContext()
{
	CHECK_CONDITION_EXIT_ALWAYS_ENGINE(!m_activeContexts.empty(), utility::logging::ERR, "Cannot pop input context. No context is currently active.");
	m_activeContexts.pop_front();
}


//
// Helper: map a button to an action in the active context(s)
//
engine::Actions::Action engine::Input::InputMapping::MapButtonToAction(raw_input_keys::RawInputKey button) const
{
	Actions::Action action = Actions::INVALID;
	for (std::list<InputContext*>::const_iterator iter = m_activeContexts.begin(); iter != m_activeContexts.end(); ++iter)
	{
		const InputContext* context = *iter;

		action = context->MapButtonToAction(button);
		if (action != Actions::INVALID)
		{
			break;
		}
	}
	return action;
}

//
// Helper: map a button to a state in the active context(s)
//
engine::States::State engine::Input::InputMapping::MapButtonToState(raw_input_keys::RawInputKey button) const
{
	States::State state = States::INVALID;
	for (std::list<InputContext*>::const_iterator iter = m_activeContexts.begin(); iter != m_activeContexts.end(); ++iter)
	{
		const InputContext* inputContext = *iter;
		state = inputContext->MapButtonToState(button);
		if (state != States::INVALID)
		{
			break;
		}
	}
	return state;
}

void engine::Input::InputMapping::MapAndConsumeButton(raw_input_keys::RawInputKey button)
{
	Actions::Action action = MapButtonToAction(button);
	if (action != Actions::INVALID)
	{
		m_currentMappedInput.ConsumeAction(action);
	}
	States::State state = MapButtonToState(button);
	if (state != States::INVALID)
	{
		m_currentMappedInput.ConsumeState(state);
	}
}
