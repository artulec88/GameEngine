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
		return false;
	}
	return currentState->IsInGameTimeCalculationEnabled();
}

void GameStateManager::SetTransition(GameStateTransitioning::GameStateTransition* gameStateTransition)
{
	if (gameStateTransition == NULL)
	{
		LOG(Utility::Warning, LOGPLACE, "There is no need to set game state transition to NULL manually.");
	}
	if (m_gameStateTransition != NULL)
	{
		LOG(Utility::Emergency, LOGPLACE, "Cannot set the game state transition object. Previous transition has not been performed yet.");
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
		LOG(Utility::Notice, LOGPLACE, "Switching the topmost state");
		Switch(m_gameStateTransition->GetGameState(), m_gameStateTransition->GetModalityType());
		break;
	case GameStateTransitioning::PUSH:
		LOG(Utility::Notice, LOGPLACE, "Pushing new game state on the stack");
		Push(m_gameStateTransition->GetGameState(), m_gameStateTransition->GetModalityType());
		break;
	default:
		LOG(Utility::Error, LOGPLACE, "Unknown game state transition type (%d)", m_gameStateTransition->GetTransitionType());
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
	//std::vector<IInputable*> m_exposedInputables;
	//std::vector<IRenderable*> m_exposedRenderables;
	//std::vector<IUpdateable*> m_exposedUpdateables;
}

GameState* DefaultGameStateManager::Peek() const
{
	if (m_activeStates.empty())
	{
		return NULL;
	}
	else
	{
		return m_activeStates.at(m_activeStates.size() - 1).first;
	}
}

void DefaultGameStateManager::Push(GameState* gameState, GameStateModality::ModalityType modalityType /* = GameStateModality::EXCLUSIVE */)
{
	m_activeStates.push_back(std::make_pair(gameState, modalityType));
	
	if (modalityType == GameStateModality::EXCLUSIVE)
	{
		m_exposedInputables.clear();
		m_exposedRenderables.clear();
		m_exposedUpdateables.clear();
	}

	AddToInterfaces(gameState);
	NotifyObscuredStates();
	gameState->Entered();
}

GameState* DefaultGameStateManager::Pop()
{
	if (m_activeStates.empty())
	{
		LOG(Utility::Error, LOGPLACE, "Attempted to pop from an empty game state stack");
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
	//LOG(Utility::Error, LOGPLACE, "Key event started (key=%d, scancode=%d, action=%d, mods=%d)", key, scancode, action, mods);
	if (m_exposedInputables.empty())
	{
		return;
	}
	LOG(Utility::Debug, LOGPLACE, "The INPUT queue has %d elements (key=%d, scancode=%d, action=%d, mods=%d)",
		m_exposedInputables.size(), key, scancode, action, mods);
	for (std::vector<IInputable*>::iterator gameStateItr = m_exposedInputables.begin(); gameStateItr != m_exposedInputables.end(); ++gameStateItr)
	{
		(*gameStateItr)->KeyEvent(key, scancode, action, mods);
	}
	//LOG(Utility::Error, LOGPLACE, "Key event finished (key=%d, scancode=%d, action=%d, mods=%d)", key, scancode, action, mods);
}

void DefaultGameStateManager::Input(Math::Real elapsedTime)
{
	for (std::vector<IInputable*>::iterator gameStateItr = m_exposedInputables.begin(); gameStateItr != m_exposedInputables.end(); ++gameStateItr)
	{
		(*gameStateItr)->Input(elapsedTime, gameNode);
	}
}

void DefaultGameStateManager::Update(Math::Real deltaTime)
{
	for (std::vector<IUpdateable*>::iterator gameStateItr = m_exposedUpdateables.begin(); gameStateItr != m_exposedUpdateables.end(); ++gameStateItr)
	{
		(*gameStateItr)->Update(deltaTime);
	}
}

void DefaultGameStateManager::Render(Renderer* renderer)
{
	for (std::vector<IRenderable*>::iterator gameStateItr = m_exposedRenderables.begin(); gameStateItr != m_exposedRenderables.end(); ++gameStateItr)
	{
		(*gameStateItr)->Render(renderer);
	}
}

void DefaultGameStateManager::AddToInterfaces(GameState* gameState)
{
	IInputable* inputable = dynamic_cast<IInputable*>(gameState);
	if (inputable != NULL)
	{
		m_exposedInputables.push_back(inputable);
	}

	IRenderable* renderable = dynamic_cast<IRenderable*>(gameState);
	if(renderable != NULL)
	{
		m_exposedRenderables.push_back(renderable);
	}
	
	IUpdateable* updateable = dynamic_cast<IUpdateable*>(gameState);
	if(updateable != NULL)
	{
		m_exposedUpdateables.push_back(updateable);
	}
}

void DefaultGameStateManager::RemoveFromInterfaces(GameState* gameState)
{
	IInputable* inputable = dynamic_cast<IInputable*>(gameState);
	if (inputable != NULL)
	{
		m_exposedInputables.pop_back();
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
	LOG(Utility::Info, LOGPLACE, "Clearing game state interface queues");
	m_exposedInputables.clear();
	m_exposedRenderables.clear();
	m_exposedUpdateables.clear();

	if (m_activeStates.empty())
	{
		return;
	}

	// Reverse scan the active states until we hit either the beginning or a Hiding state
	LOG(Utility::Debug, LOGPLACE, "Currently active game states: %d", m_activeStates.size());
	ASSERT(!m_activeStates.empty());
	std::size_t index = m_activeStates.size() - 1;
	while (index > 0)
	{
		if (m_activeStates.at(index).second == GameStateModality::EXCLUSIVE)
		{
			break;
		}
		--index;
	}
	LOG(Utility::Debug, LOGPLACE, "Calculated game state index equals %d", index);
	while (index < m_activeStates.size())
	{
		AddToInterfaces(m_activeStates.at(index++).first);
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
		if (m_activeStates.at(index).second == GameStateModality::EXCLUSIVE)
		{
			break;
		}
		--index;
	}

	// Now go forward (up until the second-to-last state) and notify the obscured states
	LOG(Utility::Debug, LOGPLACE, "Calculated game state index equals %d", index);
	while (index < m_activeStates.size() - 1)
	{
		m_activeStates.at(index++).first->Obscuring();
	}
}

void DefaultGameStateManager::NotifyRevealedStates()
{
	if (m_activeStates.empty())
	{
		return;
	}
	LOG(Utility::Info, LOGPLACE, "Notifying revealed game states");

	// Reverse scan until we hit either the beginning or find the next Hiding state
	std::size_t index = m_activeStates.size() - 1;
	while (index > 0)
	{
		if (m_activeStates.at(index).second == GameStateModality::EXCLUSIVE)
		{
			break;
		}
		--index;
	}

	// Now go forward and notify all revealed state
	LOG(Utility::Info, LOGPLACE, "Calculated game state index equals %d", index);
	while (index < m_activeStates.size())
	{
		m_activeStates.at(index++).first->Revealed();
	}
}
