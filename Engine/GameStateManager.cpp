#include "StdAfx.h"
#include "GameStateManager.h"
#include "Utility\ILogger.h"


Engine::GameStateManager::GameStateManager() :
	m_gameStateTransition(NULL)
{
}


Engine::GameStateManager::~GameStateManager(void)
{
	SAFE_DELETE(m_gameStateTransition);
}

void Engine::GameStateManager::SetTransition(GameStateTransitioning::GameStateTransition* gameStateTransition)
{
	if (gameStateTransition == NULL)
	{
		WARNING_LOG_ENGINE("There is no need to set game state transition to NULL manually.");
	}
	if (m_gameStateTransition != NULL)
	{
		EMERGENCY_LOG_ENGINE("Cannot set the game state transition object. Previous transition has not been performed yet.");
	}
	m_gameStateTransition = gameStateTransition;
}

void Engine::GameStateManager::PerformStateTransition()
{
	if (m_gameStateTransition == NULL) // No pending state transition
	{
		return;
	}
	
	switch (m_gameStateTransition->GetTransitionType())
	{
	case GameStateTransitioning::SWITCH:
		NOTICE_LOG_ENGINE("Switching the topmost state");
		Switch(m_gameStateTransition->GetGameState(), m_gameStateTransition->GetModalityType());
		break;
	case GameStateTransitioning::PUSH:
		NOTICE_LOG_ENGINE("Pushing new game state on the stack");
		Push(m_gameStateTransition->GetGameState(), m_gameStateTransition->GetModalityType());
		break;
	default:
		ERROR_LOG_ENGINE("Unknown game state transition type (", m_gameStateTransition->GetTransitionType(), ")");
	}
	//SAFE_DELETE(m_gameStateTransition);
	m_gameStateTransition = NULL;
}

Engine::GameState* Engine::GameStateManager::Switch(GameState* gameState, GameStateModality::ModalityType modality /* = GameStateModality::EXCLUSIVE */)
{
	GameState* currentState = Peek();
	if (currentState != NULL)
	{
		Pop();
	}

	Push(gameState, modality);
	return currentState;
}

Engine::DefaultGameStateManager::DefaultGameStateManager() :
	GameStateManager()
{
}

Engine::DefaultGameStateManager::~DefaultGameStateManager()
{
	//for (std::vector<GameStateModalityTypePair>::iterator activeStateItr = m_activeStates.begin(); activeStateItr != m_activeStates.end(); ++activeStateItr)
	//{
	//	SAFE_DELETE(activeStateItr->first);
	//}

	//std::vector<GameStateModalityTypePair> m_activeStates;
	//std::vector<Input::IInputableMouse*> m_exposedInputablesMouse;
	//std::vector<IRenderable*> m_exposedRenderables;
	//std::vector<IUpdateable*> m_exposedUpdateables;
}

void Engine::DefaultGameStateManager::ClearAllIntefaceLists()
{
	m_exposedInputablesMouse.clear();
	m_exposedUpdateables.clear();
}

Engine::GameState* Engine::DefaultGameStateManager::Peek() const
{
	return (m_activeStates.empty()) ? NULL : m_activeStates.back().first;
}

void Engine::DefaultGameStateManager::Push(GameState* gameState, GameStateModality::ModalityType modalityType /* = GameStateModality::EXCLUSIVE */)
{
	m_activeStates.push_back(std::make_pair(gameState, modalityType));
	
	if (modalityType == GameStateModality::EXCLUSIVE)
	{
		ClearAllIntefaceLists();
	}

	AddToInterfaces(gameState);
	NotifyObscuredStates();
	gameState->Entered();
}

Engine::GameState* Engine::DefaultGameStateManager::Pop()
{
	if (m_activeStates.empty())
	{
		ERROR_LOG_ENGINE("Attempted to pop from an empty game state stack");
		return NULL;
	}
	GameStateModalityTypePair poppedPair = m_activeStates.back();
	poppedPair.first->Leaving();
	m_activeStates.pop_back();

	if (poppedPair.second == GameStateModality::EXCLUSIVE)
	{
		RebuildInterfaceQueues();
	}
	else
	{
		RemoveFromInterfaces(poppedPair.first);
	}

	NotifyRevealedStates();

	return poppedPair.first;
}

void Engine::DefaultGameStateManager::ScrollEvent(double xOffset, double yOffset)
{
	if (m_exposedInputablesMouse.empty())
	{
		return;
	}
	DEBUG_LOG_ENGINE("The MOUSE INPUT queue has ", m_exposedInputablesMouse.size(), " elements(xOffset = ", xOffset, "; yOffset = ", yOffset, ")");
	for (std::vector<Input::IInputableMouse*>::iterator gameStateItr = m_exposedInputablesMouse.begin(); gameStateItr != m_exposedInputablesMouse.end(); ++gameStateItr)
	{
		(*gameStateItr)->ScrollEvent(xOffset, yOffset);
	}
}

void Engine::DefaultGameStateManager::MouseButtonEvent(int button, int action, int mods)
{
	if (m_exposedInputablesMouse.empty())
	{
		DEBUG_LOG_ENGINE("The MOUSE INPUT queue is empty (button=", button, ", action=", action, ", mods=", mods, ")");
		return;
	}
	//DEBUG_LOG_ENGINE("The MOUSE INPUT queue has ", m_exposedInputablesMouse.size(), " elements (button=", button, ", action=", action, ", mods=", mods, ")");
	for (std::vector<Input::IInputableMouse*>::iterator gameStateItr = m_exposedInputablesMouse.begin(); gameStateItr != m_exposedInputablesMouse.end(); ++gameStateItr)
	{
		(*gameStateItr)->MouseButtonEvent(button, action, mods);
	}
}

void Engine::DefaultGameStateManager::MousePosEvent(double xPos, double yPos)
{
	CHECK_CONDITION_RETURN_VOID_ENGINE(!m_exposedInputablesMouse.empty(), Utility::Logging::DEBUG, "The MOUSE INPUT queue is empty (xPos=", xPos, ", yPos=", yPos, ")");
	//DEBUG_LOG_ENGINE("The MOUSE INPUT queue has ", m_exposedInputablesMouse.size(), " elements (button=", button, ", action=", action, ", mods=", mods, ")");
	for (std::vector<Input::IInputableMouse*>::iterator gameStateItr = m_exposedInputablesMouse.begin(); gameStateItr != m_exposedInputablesMouse.end(); ++gameStateItr)
	{
		(*gameStateItr)->MousePosEvent(xPos, yPos);
	}
}

void Engine::DefaultGameStateManager::Update(Math::Real deltaTime)
{
	for (std::vector<IUpdateable*>::iterator gameStateItr = m_exposedUpdateables.begin(); gameStateItr != m_exposedUpdateables.end(); ++gameStateItr)
	{
		(*gameStateItr)->Update(deltaTime);
	}
}

void Engine::DefaultGameStateManager::Render(Rendering::Renderer* renderer) const
{
	for (std::vector<GameStateModalityTypePair>::const_iterator gameStateItr = m_activeStates.begin(); gameStateItr != m_activeStates.end(); ++gameStateItr)
	{
		gameStateItr->first->Render(renderer);
	}
}

void Engine::DefaultGameStateManager::AddToInterfaces(GameState* gameState)
{
	DEBUG_LOG_ENGINE("Adding to interfaces started");
	Input::IInputableMouse* inputableMouse = dynamic_cast<Input::IInputableMouse*>(gameState);
	if (inputableMouse != NULL)
	{
		DEBUG_LOG_ENGINE("Adding to MOUSE INPUT interface");
		m_exposedInputablesMouse.push_back(inputableMouse);
	}
	
	IUpdateable* updateable = dynamic_cast<IUpdateable*>(gameState);
	if(updateable != NULL)
	{
		DEBUG_LOG_ENGINE("Adding to UPDATE interface");
		m_exposedUpdateables.push_back(updateable);
	}
	DELOCUST_LOG_ENGINE("Adding to interfaces finished");
}

void Engine::DefaultGameStateManager::RemoveFromInterfaces(GameState* gameState)
{
	Input::IInputableMouse* inputableMouse = dynamic_cast<Input::IInputableMouse*>(gameState);
	if (inputableMouse != NULL)
	{
		m_exposedInputablesMouse.push_back(inputableMouse);
	}

	IUpdateable* updateable = dynamic_cast<IUpdateable*>(gameState);
	if (updateable != NULL)
	{
		m_exposedUpdateables.pop_back();
	}
}

void Engine::DefaultGameStateManager::RebuildInterfaceQueues()
{
	INFO_LOG_ENGINE("Clearing game state interface queues");
	ClearAllIntefaceLists();

	if (m_activeStates.empty())
	{
		return;
	}

	// Reverse scan the active states until we hit either the beginning or a Hiding state
	DEBUG_LOG_ENGINE("Currently active game states: ", m_activeStates.size());
	CHECK_CONDITION_EXIT_ALWAYS_ENGINE(!m_activeStates.empty(), Utility::Logging::EMERGENCY, "No active game state is present in the game at the moment.");
	std::size_t index = m_activeStates.size() - 1;
	while (index > 0)
	{
		if (m_activeStates[index].second == GameStateModality::EXCLUSIVE)
		{
			break;
		}
		--index;
	}
	DEBUG_LOG_ENGINE("Calculated game state index equals ", index);
	while (index < m_activeStates.size())
	{
		AddToInterfaces(m_activeStates[index++].first);
	}
}

void Engine::DefaultGameStateManager::NotifyObscuredStates()
{
	if (m_activeStates.size() < 2)
	{
		return;
	}

	// Reverse scan until we hit either the beginning or find the next Hiding state
	std::size_t index = m_activeStates.size() - 2;
	while (index > 0)
	{
		if (m_activeStates[index].second == GameStateModality::EXCLUSIVE)
		{
			break;
		}
		--index;
	}

	// Now go forward (up until the second-to-last state) and notify the obscured states
	DEBUG_LOG_ENGINE("Calculated game state index equals ", index);
	while (index < m_activeStates.size() - 1)
	{
		m_activeStates[index++].first->Obscuring();
	}
}

void Engine::DefaultGameStateManager::NotifyRevealedStates()
{
	if (m_activeStates.empty())
	{
		return;
	}
	INFO_LOG_ENGINE("Notifying revealed game states");

	// Reverse scan until we hit either the beginning or find the next Hiding state
	std::size_t index = m_activeStates.size() - 1;
	while (index > 0)
	{
		if (m_activeStates[index].second == GameStateModality::EXCLUSIVE)
		{
			break;
		}
		--index;
	}

	// Now go forward and notify all revealed state
	INFO_LOG_ENGINE("Calculated game state index equals ", index);
	while (index < m_activeStates.size())
	{
		m_activeStates[index++].first->Revealed();
	}
}

void Engine::DefaultGameStateManager::Handle(Engine::Actions::Action action)
{
	for (std::vector<GameStateModalityTypePair>::iterator activeStateItr = m_activeStates.begin(); activeStateItr != m_activeStates.end(); ++activeStateItr)
	{
		activeStateItr->first->Handle(action);
	}
}

void Engine::DefaultGameStateManager::Handle(Engine::States::State state)
{
	for (std::vector<GameStateModalityTypePair>::iterator activeStateItr = m_activeStates.begin(); activeStateItr != m_activeStates.end(); ++activeStateItr)
	{
		activeStateItr->first->Handle(state);
	}
}

void Engine::DefaultGameStateManager::Handle(Engine::Ranges::Range range, Math::Real value)
{
	for (std::vector<GameStateModalityTypePair>::iterator activeStateItr = m_activeStates.begin(); activeStateItr != m_activeStates.end(); ++activeStateItr)
	{
		activeStateItr->first->Handle(range, value);
	}
}