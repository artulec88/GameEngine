#include "stdafx.h"
#include "InputMapping.h"
#include "InputContext.h"
#include "GameManager.h"

#include "Utility/ILogger.h"
#include "Utility/FileManager.h"
//#include "Utility/IConfig.h"

#include <fstream>

engine::input::InputMapping::InputMapping() //:
	//m_inputContexts({ {"mainMenuInputContext", } })
{
	m_inputContexts.insert(make_pair("mainMenuInputContext", std::make_unique<InputContext>("mainMenuInputContext")));
}

engine::input::InputMapping::InputMapping(const std::string& configDirectory, const std::string& contextListFileName)
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


engine::input::InputMapping::~InputMapping()
{
}

void engine::input::InputMapping::ClearActions()
{
	m_currentMappedInput.m_actions.clear();
}

void engine::input::InputMapping::ClearRanges()
{
	m_currentMappedInput.m_ranges.clear();
}

void engine::input::InputMapping::SetRawButtonState(raw_input_keys::RawInputKey button, bool pressed, bool previouslyPressed)
{
	DEBUG_LOG_ENGINE("Key = ", button, ", pressed = ", pressed, ", previously pressed = ", previouslyPressed);
	if (pressed)
	{
		if (!previouslyPressed) // action
		{
			actions::Action action = MapButtonToAction(button);
			CHECK_CONDITION_RETURN_VOID_ALWAYS_ENGINE(action != actions::INVALID, utility::logging::WARNING,
				"Invalid action for button ", button, "(pressed = ", pressed, ", previouslyPressed = ", previouslyPressed, ")");
			m_currentMappedInput.m_actions.insert(action);
		}
		else // state
		{
			states::State state = MapButtonToState(button);
			CHECK_CONDITION_RETURN_VOID_ALWAYS_ENGINE(state != states::INVALID, utility::logging::WARNING,
				"Invalid state for button ", button, "(pressed = ", pressed, ", previouslyPressed = ", previouslyPressed, ")");
			m_currentMappedInput.m_states.insert(state);
		}
	}
	else
	{
		MapAndConsumeButton(button);
	}
}

void engine::input::InputMapping::SetRawAxisValue(RawInputAxes::RawInputAxis axis, math::Real value)
{
	for (std::list<InputContext*>::const_iterator iter = m_activeContexts.begin(); iter != m_activeContexts.end(); ++iter)
	{
		const InputContext* inputContext = *iter;
		ranges::Range range = inputContext->MapAxisToRange(axis);
		if (range != ranges::INVALID)
		{
			m_currentMappedInput.m_ranges[range] = inputContext->GetConverter().Convert(range, value * inputContext->GetSensitivity(range));
			break;
		}
	}
}

void engine::input::InputMapping::Dispatch() const
{
	//MappedInput input = m_currentMappedInput;
	//for (std::multimap<int, InputCallback>::const_iterator iter = m_callbackTable.begin(); iter != m_callbackTable.end(); ++iter)
	//{
	//	(*iter->second)(input);
	//}

	//GameManager::GetGameManager()->Input(m_currentMappedInput.m_actions);
}

void engine::input::InputMapping::RegisterCallback(InputCallback callback, int priority)
{
	m_callbackTable.insert(std::make_pair(priority, callback));
}

void engine::input::InputMapping::PushContext(const std::string& name)
{
	std::map<std::string, std::unique_ptr<InputContext>>::iterator iter = m_inputContexts.find(name);
	CHECK_CONDITION_EXIT_ALWAYS_ENGINE(iter != m_inputContexts.end(), utility::logging::EMERGENCY, "Cannot push input context with name \"", name, "\". The input context cannot be found.");
	m_activeContexts.push_front(iter->second.get());
}

void engine::input::InputMapping::PopContext()
{
	CHECK_CONDITION_EXIT_ALWAYS_ENGINE(!m_activeContexts.empty(), utility::logging::ERR, "Cannot pop input context. No context is currently active.");
	m_activeContexts.pop_front();
}


//
// Helper: map a button to an action in the active context(s)
//
engine::actions::Action engine::input::InputMapping::MapButtonToAction(raw_input_keys::RawInputKey button) const
{
	actions::Action action = actions::INVALID;
	for (std::list<InputContext*>::const_iterator iter = m_activeContexts.begin(); iter != m_activeContexts.end(); ++iter)
	{
		const InputContext* context = *iter;

		action = context->MapButtonToAction(button);
		if (action != actions::INVALID)
		{
			break;
		}
	}
	return action;
}

//
// Helper: map a button to a state in the active context(s)
//
engine::states::State engine::input::InputMapping::MapButtonToState(raw_input_keys::RawInputKey button) const
{
	states::State state = states::INVALID;
	for (std::list<InputContext*>::const_iterator iter = m_activeContexts.begin(); iter != m_activeContexts.end(); ++iter)
	{
		const InputContext* inputContext = *iter;
		state = inputContext->MapButtonToState(button);
		if (state != states::INVALID)
		{
			break;
		}
	}
	return state;
}

void engine::input::InputMapping::MapAndConsumeButton(raw_input_keys::RawInputKey button)
{
	actions::Action action = MapButtonToAction(button);
	if (action != actions::INVALID)
	{
		m_currentMappedInput.ConsumeAction(action);
	}
	states::State state = MapButtonToState(button);
	if (state != states::INVALID)
	{
		m_currentMappedInput.ConsumeState(state);
	}
}
