#include "stdafx.h"
#include "InputMapping.h"
#include "InputContext.h"
#include "GameManager.h"

#include "Utility\ILogger.h"
#include "Utility\FileManager.h"
//#include "Utility\IConfig.h"

#include <fstream>

Engine::Input::InputMapping::InputMapping() //:
	//m_inputContexts({ {"mainMenuInputContext", } })
{
	m_inputContexts.insert(std::make_pair("mainMenuInputContext", std::make_unique<InputContext>("mainMenuInputContext")));
}

Engine::Input::InputMapping::InputMapping(const std::string& configDirectory, const std::string& contextListFileName)
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
			const std::string name = Utility::FileManager::AttemptRead<std::string>(inFile);
			const std::string fileName = configDirectory + Utility::FileManager::AttemptRead<std::string>(inFile);
			INFO_LOG_ENGINE("Input context \"", name, "\" initialized with file \"", fileName, "\".");
			m_inputContexts[name] = std::make_unique<InputContext>(std::ifstream(fileName));
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
	DEBUG_LOG_ENGINE("Key = ", button, ", pressed = ", pressed, ", previously pressed = ", previouslyPressed);
	if (pressed)
	{
		if (!previouslyPressed) // action
		{
			Actions::Action action = MapButtonToAction(button);
			CHECK_CONDITION_RETURN_VOID_ALWAYS_ENGINE(action != Actions::INVALID, Utility::Logging::WARNING,
				"Invalid action for button ", button, "(pressed = ", pressed, ", previouslyPressed = ", previouslyPressed, ")");
			m_currentMappedInput.m_actions.insert(action);
		}
		else // state
		{
			States::State state = MapButtonToState(button);
			CHECK_CONDITION_RETURN_VOID_ALWAYS_ENGINE(state != States::INVALID, Utility::Logging::WARNING,
				"Invalid state for button ", button, "(pressed = ", pressed, ", previouslyPressed = ", previouslyPressed, ")");
			m_currentMappedInput.m_states.insert(state);
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
		if (range != Ranges::INVALID)
		{
			m_currentMappedInput.m_ranges[range] = inputContext->GetConverter().Convert(range, value * inputContext->GetSensitivity(range));
			break;
		}
	}
}

void Engine::Input::InputMapping::Dispatch() const
{
	//MappedInput input = m_currentMappedInput;
	//for (std::multimap<int, InputCallback>::const_iterator iter = m_callbackTable.begin(); iter != m_callbackTable.end(); ++iter)
	//{
	//	(*iter->second)(input);
	//}

	//GameManager::GetGameManager()->Input(m_currentMappedInput.m_actions);
}

void Engine::Input::InputMapping::RegisterCallback(InputCallback callback, int priority)
{
	m_callbackTable.insert(std::make_pair(priority, callback));
}

void Engine::Input::InputMapping::PushContext(const std::string& name)
{
	std::map<std::string, std::unique_ptr<InputContext>>::iterator iter = m_inputContexts.find(name);
	CHECK_CONDITION_EXIT_ALWAYS_ENGINE(iter != m_inputContexts.end(), Utility::Logging::EMERGENCY, "Cannot push input context with name \"", name, "\". The input context cannot be found.");
	m_activeContexts.push_front(iter->second.get());
}

void Engine::Input::InputMapping::PopContext()
{
	CHECK_CONDITION_EXIT_ALWAYS_ENGINE(!m_activeContexts.empty(), Utility::Logging::ERR, "Cannot pop input context. No context is currently active.");
	m_activeContexts.pop_front();
}


//
// Helper: map a button to an action in the active context(s)
//
Engine::Actions::Action Engine::Input::InputMapping::MapButtonToAction(RawInputKeys::RawInputKey button) const
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
Engine::States::State Engine::Input::InputMapping::MapButtonToState(RawInputKeys::RawInputKey button) const
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

void Engine::Input::InputMapping::MapAndConsumeButton(RawInputKeys::RawInputKey button)
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
