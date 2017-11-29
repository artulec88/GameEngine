#include "StdAfx.h"
#include "GameNode.h"
#include "GameComponent.h"
//#include "Renderer.h"
#include "CoreEngine.h"
#include "IRenderable.h"
#include "IUpdateable.h"

#include "Utility/ILogger.h"

/* static */ int engine::GameNode::s_gameNodeCount = 0;

engine::GameNode::GameNode() :
	m_id(++GameNode::s_gameNodeCount),
	m_transform(),
	m_physicsObject(nullptr)
{
	DEBUG_LOG_ENGINE("Game node with ID ", m_id, " has been created.");
}


engine::GameNode::~GameNode()
{
	DEBUG_LOG_ENGINE("Game node (ID=", m_id, ") destruction started");
	//DEBUG_LOG_ENGINE("Destroying components started for game node with ID=", m_ID);
	//for (std::vector<GameComponent*>::iterator itr = components.begin(); itr != components.end(); ++itr)
	//{
	//	SAFE_DELETE(*itr);
	//}
	//DEBUG_LOG_ENGINE("Destroying components finished for game node with ID=", m_ID);

	//DEBUG_LOG_ENGINE("Started recursive destruction of game nodes for game node with ID=", m_ID);
	//for (std::vector<GameNode*>::iterator itr = childrenGameNodes.begin(); itr != childrenGameNodes.end(); ++itr)
	//{
	//	if ((*itr) != NULL)
	//	{
	//		DELOCUST_LOG_ENGINE("Destroying child game node started");
	//		delete *itr;
	//		*itr = NULL;
	//		DELOCUST_LOG_ENGINE("Destroying child game node finished");
	//	}
	//}
	//DEBUG_LOG_ENGINE("Recursive destruction of game nodes for game node with ID=", m_ID, " finished");
	//childrenGameNodes.clear();

	for (unsigned int i = 0; i < m_components.size(); ++i)
	{
		SAFE_DELETE(m_components[i]);
	}
	if (!m_components.empty())
	{
		m_components.clear();
	}
	DELOCUST_LOG_ENGINE("Game node (ID=", m_id, ") destruction finished");
}

//Engine::GameNode::GameNode(const GameNode& gameNode) :
//	m_ID(++GameNode::gameNodeCount),
//	m_childrenGameNodes(gameNode.m_childrenGameNodes),
//	m_components(gameNode.m_components),
//	m_renderableComponents(gameNode.m_renderableComponents),
//	m_inputableMouseComponents(gameNode.m_inputableMouseComponents),
//	m_updateableComponents(gameNode.m_updateableComponents),
//	m_transform(gameNode.m_transform),
//	m_physicsObject(gameNode.m_physicsObject),
//	m_actionsToCommands(gameNode.m_actionsToCommands),
//	m_statesToCommands(gameNode.m_statesToCommands)
//{
//}

engine::GameNode::GameNode(GameNode&& gameNode) noexcept:
	m_id(std::move(gameNode.m_id)),
	m_childrenGameNodes(std::move(gameNode.m_childrenGameNodes)),
	m_components(std::move(gameNode.m_components)),
	m_renderableComponents(std::move(gameNode.m_renderableComponents)),
	m_inputableMouseComponents(std::move(gameNode.m_inputableMouseComponents)),
	m_updateableComponents(std::move(gameNode.m_updateableComponents)),
	m_transform(std::move(gameNode.m_transform)),
	m_physicsObject(std::move(gameNode.m_physicsObject)),
	m_actionsToCommands(std::move(gameNode.m_actionsToCommands)),
	m_statesToCommands(std::move(gameNode.m_statesToCommands))
{
	gameNode.m_components.clear();
	for (auto componentItr = m_components.begin(); componentItr != m_components.end(); ++componentItr)
	{
		(*componentItr)->SetParent(this);
	}
}

engine::GameNode& engine::GameNode::operator=(GameNode&& gameNode) noexcept
{
	m_id = std::move(gameNode.m_id);
	m_childrenGameNodes = std::move(gameNode.m_childrenGameNodes);
	m_components = std::move(gameNode.m_components);
	m_renderableComponents = std::move(gameNode.m_renderableComponents);
	m_inputableMouseComponents = std::move(gameNode.m_inputableMouseComponents);
	m_updateableComponents = std::move(gameNode.m_updateableComponents);
	m_transform = std::move(gameNode.m_transform);
	m_physicsObject = std::move(gameNode.m_physicsObject);
	m_actionsToCommands = std::move(gameNode.m_actionsToCommands);
	m_statesToCommands = std::move(gameNode.m_statesToCommands);

	gameNode.m_components.clear();
	for (auto componentItr = m_components.begin(); componentItr != m_components.end(); ++componentItr)
	{
		(*componentItr)->SetParent(this);
	}
	return *this;
}

engine::GameNode engine::GameNode::Clone() const
{
	GameNode cloneGameNode;
	// TODO: Finish the implementation
	return cloneGameNode;
}

void engine::GameNode::CreatePhysicsObject(math::Real mass, const math::Vector3D& linearVelocity)
{
	m_physicsObject = std::make_unique<physics::PhysicsObject>(GetTransform(), mass, linearVelocity);
	engine::CoreEngine::GetCoreEngine()->AddPhysicsObject(m_physicsObject.get());
}

engine::GameNode* engine::GameNode::AddChild(GameNode* child)
{
	m_childrenGameNodes.push_back(child);
	child->GetTransform().SetParent(&m_transform);
	return this;
}

engine::GameNode* engine::GameNode::AddComponent(GameComponent* child)
{
	m_components.push_back(child);
	child->SetParent(this);
	IRenderable* renderableComponent = dynamic_cast<IRenderable*>(child);
	if (renderableComponent != nullptr)
	{
		m_renderableComponents.push_back(renderableComponent);
	}
	input::IInputableMouse* inputableMouseComponent = dynamic_cast<input::IInputableMouse*>(child);
	if (inputableMouseComponent != nullptr)
	{
		m_inputableMouseComponents.push_back(inputableMouseComponent);
	}
	IUpdateable* updateableComponent = dynamic_cast<IUpdateable*>(child);
	if (updateableComponent != nullptr)
	{
		m_updateableComponents.push_back(updateableComponent);
	}
	return this;
}

//void Engine::GameNode::InputAll(math::Real delta)
//{
//transform.Update();

//for (std::vector<Input::IInputable*>::iterator gameComponentItr = m_inputableComponents.begin(); gameComponentItr != m_inputableComponents.end(); ++gameComponentItr)
//{
//	(*gameComponentItr)->Input(delta);
//}

//for (std::vector<GameNode*>::iterator gameNodeItr = m_childrenGameNodes.begin(); gameNodeItr != m_childrenGameNodes.end(); ++gameNodeItr)
//{
//	(*gameNodeItr)->InputAll(delta);
//}
//}

void engine::GameNode::MouseButtonEvent(int button, int action, int mods)
{
	for (std::vector<input::IInputableMouse*>::iterator gameComponentItr = m_inputableMouseComponents.begin(); gameComponentItr != m_inputableMouseComponents.end(); ++gameComponentItr)
	{
		(*gameComponentItr)->MouseButtonEvent(button, action, mods);
	}

	for (std::vector<GameNode*>::iterator gameNodeItr = m_childrenGameNodes.begin(); gameNodeItr != m_childrenGameNodes.end(); ++gameNodeItr)
	{
		(*gameNodeItr)->MouseButtonEvent(button, action, mods);
	}
}

void engine::GameNode::MousePosEvent(double xPos, double yPos)
{
	for (std::vector<input::IInputableMouse*>::iterator gameComponentItr = m_inputableMouseComponents.begin(); gameComponentItr != m_inputableMouseComponents.end(); ++gameComponentItr)
	{
		(*gameComponentItr)->MousePosEvent(xPos, yPos);
	}

	for (std::vector<GameNode*>::iterator gameNodeItr = m_childrenGameNodes.begin(); gameNodeItr != m_childrenGameNodes.end(); ++gameNodeItr)
	{
		(*gameNodeItr)->MousePosEvent(xPos, yPos);
	}
}

void engine::GameNode::ScrollEvent(double xOffset, double yOffset)
{
	for (std::vector<input::IInputableMouse*>::iterator gameComponentItr = m_inputableMouseComponents.begin(); gameComponentItr != m_inputableMouseComponents.end(); ++gameComponentItr)
	{
		(*gameComponentItr)->ScrollEvent(xOffset, yOffset);
	}

	for (std::vector<GameNode*>::iterator gameNodeItr = m_childrenGameNodes.begin(); gameNodeItr != m_childrenGameNodes.end(); ++gameNodeItr)
	{
		(*gameNodeItr)->ScrollEvent(xOffset, yOffset);
	}
}

void engine::GameNode::Update(math::Real delta)
{
	for (std::vector<IUpdateable*>::iterator gameComponentItr = m_updateableComponents.begin(); gameComponentItr != m_updateableComponents.end(); ++gameComponentItr)
	{
		(*gameComponentItr)->Update(delta);
	}

	for (std::vector<GameNode*>::iterator gameNodeItr = m_childrenGameNodes.begin(); gameNodeItr != m_childrenGameNodes.end(); ++gameNodeItr)
	{
		(*gameNodeItr)->Update(delta);
	}
}

void engine::GameNode::Render(int shaderId, rendering::Renderer* renderer) const
{
	for (auto gameComponentItr = m_renderableComponents.begin(); gameComponentItr != m_renderableComponents.end(); ++gameComponentItr)
	{
		(*gameComponentItr)->Render(shaderId, renderer);
	}

	for (auto gameNodeItr = m_childrenGameNodes.begin(); gameNodeItr != m_childrenGameNodes.end(); ++gameNodeItr)
	{
		(*gameNodeItr)->Render(shaderId, renderer);
	}
}

std::vector<engine::GameNode*> engine::GameNode::GetAllDescendants() const
{
	std::vector<GameNode*> descendants;

	for (auto itr = m_childrenGameNodes.begin(); itr != m_childrenGameNodes.end(); ++itr)
	{
		descendants.push_back(*itr);
		std::vector<GameNode*> itrDescendants = (*itr)->GetAllDescendants();
		descendants.insert(descendants.end(), itrDescendants.begin(), itrDescendants.end());
	}

	//descendants.push_back(this);
	return descendants;
}