#include "StdAfx.h"
#include "GameStateManager.h"
#include "Utility\ILogger.h"

using namespace Rendering;

GameStateManager::GameStateManager() :
	m_gameStateTransition(NULL)
{
}


GameStateManager::~GameStateManager(void)
{
	SAFE_DELETE(m_gameStateTransition);
}

bool GameStateManager::IsInGameTimeCalculationEnabled() const
{
	GameState* currentState = Peek();
	if (currentState == NULL)
	{
		WARNING_LOG("Cannot determine whether in-game time calculation should be performed. No state currently active.");
		return false;
	}
	return currentState->IsInGameTimeCalculationEnabled();
}

void GameStateManager::SetTransition(GameStateTransitioning::GameStateTransition* gameStateTransition)
{
	if (gameStateTransition == NULL)
	{
		WARNING_LOG("There is no need to set game state transition to NULL manually.");
	}
	if (m_gameStateTransition != NULL)
	{
		EMERGENCY_LOG("Cannot set the game state transition object. Previous transition has not been performed yet.");
	}
	m_gameStateTransition = gameStateTransition;
}

void GameStateManager::PerformStateTransition()
{
	if (m_gameStateTransition == NULL) // No pending state transition
	{
		return;
	}
	
	switch (m_gameStateTransition->GetTransitionType())
	{
	case GameStateTransitioning::SWITCH:
		NOTICE_LOG("Switching the topmost state");
		Switch(m_gameStateTransition->GetGameState(), m_gameStateTransition->GetModalityType());
		break;
	case GameStateTransitioning::PUSH:
		NOTICE_LOG("Pushing new game state on the stack");
		Push(m_gameStateTransition->GetGameState(), m_gameStateTransition->GetModalityType());
		break;
	default:
		ERROR_LOG("Unknown game state transition type (%d)", m_gameStateTransition->GetTransitionType());
	}
	//SAFE_DELETE(m_gameStateTransition);
	m_gameStateTransition = NULL;
}

GameState* GameStateManager::Switch(GameState* gameState, GameStateModality::ModalityType modality /* = GameStateModality::EXCLUSIVE */)
{
	GameState* currentState = Peek();
	if (currentState != NULL)
	{
		Pop();
	}

	Push(gameState, modality);
	return currentState;
}

DefaultGameStateManager::DefaultGameStateManager() :
	GameStateManager()
{
}

DefaultGameStateManager::~DefaultGameStateManager()
{
	for (std::vector<GameStateModalityTypePair>::iterator activeStateItr = m_activeStates.begin(); activeStateItr != m_activeStates.end(); ++activeStateItr)
	{
		SAFE_DELETE(activeStateItr->first);
	}
	//std::vector<GameStateModalityTypePair> m_activeStates;
	//std::vector<Input::IInputableKeyboard*> m_exposedInputablesKeyboard;
	//std::vector<Input::IInputableMouse*> m_exposedInputablesMouse;
	//std::vector<IRenderable*> m_exposedRenderables;
	//std::vector<IUpdateable*> m_exposedUpdateables;
}

void DefaultGameStateManager::ClearAllIntefaceLists()
{
	m_exposedInputables.clear();
	m_exposedInputablesKeyboard.clear();
	m_exposedInputablesMouse.clear();
	m_exposedRenderables.clear();
	m_exposedUpdateables.clear();
}

GameState* DefaultGameStateManager::Peek() const
{
	return (m_activeStates.empty()) ? NULL : m_activeStates.back().first;
}

void DefaultGameStateManager::Push(GameState* gameState, GameStateModality::ModalityType modalityType /* = GameStateModality::EXCLUSIVE */)
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

GameState* DefaultGameStateManager::Pop()
{
	if (m_activeStates.empty())
	{
		ERROR_LOG("Attempted to pop from an empty game state stack");
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

void DefaultGameStateManager::KeyEvent(int key, int scancode, int action, int mods)
{
	//ERROR_LOG("Key event started (key=%d, scancode=%d, action=%d, mods=%d)", key, scancode, action, mods);
	if (m_exposedInputablesKeyboard.empty())
	{
		return;
	}
	DEBUG_LOG("The KEYBOARD INPUT queue has %d elements (key=%d, scancode=%d, action=%d, mods=%d)",
		m_exposedInputablesKeyboard.size(), key, scancode, action, mods);
	for (std::vector<Input::IInputableKeyboard*>::iterator gameStateItr = m_exposedInputablesKeyboard.begin(); gameStateItr != m_exposedInputablesKeyboard.end(); ++gameStateItr)
	{
		(*gameStateItr)->KeyEvent(key, scancode, action, mods);
	}
	//ERROR_LOG("Key event finished (key=%d, scancode=%d, action=%d, mods=%d)", key, scancode, action, mods);
}

void DefaultGameStateManager::MouseButtonEvent(int button, int action, int mods)
{
	if (m_exposedInputablesMouse.empty())
	{
		DEBUG_LOG("The MOUSE INPUT queue is empty (button=%d, action=%d, mods=%d)", button, action, mods);
		return;
	}
	//DEBUG_LOG("The MOUSE INPUT queue has %d elements (button=%d, action=%d, mods=%d)", m_exposedInputablesMouse.size(), button, action, mods);
	for (std::vector<Input::IInputableMouse*>::iterator gameStateItr = m_exposedInputablesMouse.begin(); gameStateItr != m_exposedInputablesMouse.end(); ++gameStateItr)
	{
		(*gameStateItr)->MouseButtonEvent(button, action, mods);
	}
}

void DefaultGameStateManager::MousePosEvent(double xPos, double yPos)
{
	if (m_exposedInputablesMouse.empty())
	{
		DEBUG_LOG("The MOUSE INPUT queue is empty (xPos=%.2f, yPos=%.2f)", xPos, yPos);
		return;
	}
	//DEBUG_LOG("The MOUSE INPUT queue has %d elements (button=%d, action=%d, mods=%d)", m_exposedInputablesMouse.size(), button, action, mods);
	for (std::vector<Input::IInputableMouse*>::iterator gameStateItr = m_exposedInputablesMouse.begin(); gameStateItr != m_exposedInputablesMouse.end(); ++gameStateItr)
	{
		(*gameStateItr)->MousePosEvent(xPos, yPos);
	}
}

void DefaultGameStateManager::Input(Math::Real elapsedTime)
{
	for (std::vector<Input::IInputable*>::iterator gameStateItr = m_exposedInputables.begin(); gameStateItr != m_exposedInputables.end(); ++gameStateItr)
	{
		(*gameStateItr)->Input(elapsedTime);
	}
}

void DefaultGameStateManager::Update(Math::Real deltaTime)
{
	for (std::vector<IUpdateable*>::iterator gameStateItr = m_exposedUpdateables.begin(); gameStateItr != m_exposedUpdateables.end(); ++gameStateItr)
	{
		(*gameStateItr)->Update(deltaTime);
	}
}

void DefaultGameStateManager::Render(Shader* shader, Renderer* renderer)
{
	for (std::vector<IRenderable*>::iterator gameStateItr = m_exposedRenderables.begin(); gameStateItr != m_exposedRenderables.end(); ++gameStateItr)
	{
		(*gameStateItr)->Render(shader, renderer);
	}
}

void DefaultGameStateManager::AddToInterfaces(GameState* gameState)
{
	DEBUG_LOG("Adding to interfaces started");
	Input::IInputable* inputable = dynamic_cast<Input::IInputable*>(gameState);
	if (inputable != NULL)
	{
		DEBUG_LOG("Adding to INPUT interface");
		m_exposedInputables.push_back(inputable);
		Input::IInputableKeyboard* inputableKeyboard = dynamic_cast<Input::IInputableKeyboard*>(gameState);
		if (inputableKeyboard != NULL)
		{
			DEBUG_LOG("Adding to KEYBOARD INPUT interface");
			m_exposedInputablesKeyboard.push_back(inputableKeyboard);
		}
		Input::IInputableMouse* inputableMouse = dynamic_cast<Input::IInputableMouse*>(gameState);
		if (inputableMouse != NULL)
		{
			DEBUG_LOG("Adding to MOUSE INPUT interface");
			m_exposedInputablesMouse.push_back(inputableMouse);
		}
	}

	IRenderable* renderable = dynamic_cast<IRenderable*>(gameState);
	if(renderable != NULL)
	{
		DEBUG_LOG("Adding to RENDER interface");
		m_exposedRenderables.push_back(renderable);
	}
	
	IUpdateable* updateable = dynamic_cast<IUpdateable*>(gameState);
	if(updateable != NULL)
	{
		DEBUG_LOG("Adding to UPDATE interface");
		m_exposedUpdateables.push_back(updateable);
	}
	DELOCUST_LOG("Adding to interfaces finished");
}

void DefaultGameStateManager::RemoveFromInterfaces(GameState* gameState)
{
	Input::IInputable* inputable = dynamic_cast<Input::IInputable*>(gameState);
	if (inputable != NULL)
	{
		m_exposedInputables.push_back(inputable);
		Input::IInputableKeyboard* inputableKeyboard = dynamic_cast<Input::IInputableKeyboard*>(gameState);
		if (inputableKeyboard != NULL)
		{
			m_exposedInputablesKeyboard.push_back(inputableKeyboard);
		}
		Input::IInputableMouse* inputableMouse = dynamic_cast<Input::IInputableMouse*>(gameState);
		if (inputableMouse != NULL)
		{
			m_exposedInputablesMouse.push_back(inputableMouse);
		}
	}

	IRenderable* renderable = dynamic_cast<IRenderable*>(gameState);
	if (renderable != NULL)
	{
		m_exposedRenderables.pop_back();
	}

	IUpdateable* updateable = dynamic_cast<IUpdateable*>(gameState);
	if (updateable != NULL)
	{
		m_exposedUpdateables.pop_back();
	}
}

void DefaultGameStateManager::RebuildInterfaceQueues()
{
	INFO_LOG("Clearing game state interface queues");
	ClearAllIntefaceLists();

	if (m_activeStates.empty())
	{
		return;
	}

	// Reverse scan the active states until we hit either the beginning or a Hiding state
	DEBUG_LOG("Currently active game states: %d", m_activeStates.size());
	CHECK_CONDITION_EXIT_ALWAYS(!m_activeStates.empty(), Utility::Emergency, "No active game state is present in the game at the moment.");
	std::size_t index = m_activeStates.size() - 1;
	while (index > 0)
	{
		if (m_activeStates[index].second == GameStateModality::EXCLUSIVE)
		{
			break;
		}
		--index;
	}
	DEBUG_LOG("Calculated game state index equals %d", index);
	while (index < m_activeStates.size())
	{
		AddToInterfaces(m_activeStates[index++].first);
	}
}

void DefaultGameStateManager::NotifyObscuredStates()
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
	DEBUG_LOG("Calculated game state index equals %d", index);
	while (index < m_activeStates.size() - 1)
	{
		m_activeStates[index++].first->Obscuring();
	}
}

void DefaultGameStateManager::NotifyRevealedStates()
{
	if (m_activeStates.empty())
	{
		return;
	}
	INFO_LOG("Notifying revealed game states");

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
	INFO_LOG("Calculated game state index equals %d", index);
	while (index < m_activeStates.size())
	{
		m_activeStates[index++].first->Revealed();
	}
}
