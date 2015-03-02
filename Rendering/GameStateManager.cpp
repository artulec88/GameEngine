#include "StdAfx.h"
#include "GameStateManager.h"
#include "Utility\ILogger.h"

using namespace Rendering;

GameStateManager::GameStateManager()
{
}


GameStateManager::~GameStateManager(void)
{
}

GameState* GameStateManager::Switch(GameState* gameState, Modality::ModalityType modality /* = Modality::EXCLUSIVE */)
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
	while (!m_activeStates.empty())
	{
		Pop();
	}
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

void DefaultGameStateManager::Push(GameState* gameState, Modality::ModalityType modalityType /* = Modality::EXCLUSIVE */)
{
	m_activeStates.push_back(std::make_pair(gameState, modalityType));
	
	if (modalityType == Modality::EXCLUSIVE)
	{
		m_exposedInputables.clear();
		m_exposedRenderables.clear();
		m_exposedUpdateables.clear();
	}

	AddToRenderablesOrUpdateables(gameState);
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

	if (poppedPair.second == Modality::EXCLUSIVE)
	{
		RebuildRenderableAndUpdateableQueues();
	}
	else
	{
		RemoveFromRenderablesOrUpdateables(poppedPair.first);
	}

	NotifyRevealedStates();

	return poppedPair.first;
}

void DefaultGameStateManager::Input(Math::Real elapsedTime, GameNode& gameNode)
{
	for (std::vector<IInputable*>::iterator gameStateItr = m_exposedInputables.begin(); gameStateItr != m_exposedInputables.end(); ++gameStateItr)
	{
		(*gameStateItr)->Input(elapsedTime, gameNode);
	}
}

void DefaultGameStateManager::Update(Math::Real deltaTime, const GameNode& gameNode)
{
	for (std::vector<IUpdateable*>::iterator gameStateItr = m_exposedUpdateables.begin(); gameStateItr != m_exposedUpdateables.end(); ++gameStateItr)
	{
		(*gameStateItr)->Update(deltaTime, gameNode);
	}
}

void DefaultGameStateManager::Render(Renderer* renderer, const GameNode& gameNode)
{
	//renderer->Render(gameNode);
	for (std::vector<IRenderable*>::iterator gameStateItr = m_exposedRenderables.begin(); gameStateItr != m_exposedRenderables.end(); ++gameStateItr)
	{
		(*gameStateItr)->Render(renderer, gameNode);
	}
}

void DefaultGameStateManager::AddToRenderablesOrUpdateables(GameState* gameState)
{
	IRenderable *renderable = dynamic_cast<IRenderable*>(gameState);
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

void DefaultGameStateManager::RemoveFromRenderablesOrUpdateables(GameState* gameState)
{
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

void DefaultGameStateManager::RebuildRenderableAndUpdateableQueues()
{
	m_exposedUpdateables.clear();
	m_exposedRenderables.clear();

	if (m_activeStates.empty())
	{
		return;
	}

	// Reverse scan the active states until we hit either the beginning or a Hiding state
	std::size_t index = m_activeStates.size() - 1;
	while (index > 0)
	{
		if (m_activeStates.at(index).second == Modality::EXCLUSIVE)
		{
			break;
		}
		--index;
	}

	while (index < m_activeStates.size())
	{
		AddToRenderablesOrUpdateables(m_activeStates.at(index).first);
		++index;
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
		if (m_activeStates.at(index).second == Modality::EXCLUSIVE)
		{
			break;
		}
		--index;
	}

	// Now go forward (up until the second-to-last state) and notify the obscured states
	while (index < m_activeStates.size() - 1)
	{
		m_activeStates.at(index).first->Obscuring();
	}
}

void DefaultGameStateManager::NotifyRevealedStates()
{
	if (m_activeStates.empty())
	{
		return;
	}

	// Reverse scan until we hit either the beginning or find the next Hiding state
	std::size_t index = m_activeStates.size() - 1;
	while (index > 0)
	{
		if (m_activeStates.at(index).second == Modality::EXCLUSIVE)
		{
			break;
		}
		--index;
	}

	// Now go forward and notify all revealed state
	while (index < m_activeStates.size())
	{
		m_activeStates.at(index).first->Revealed();
	}
}