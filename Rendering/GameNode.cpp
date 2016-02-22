#include "StdAfx.h"
#include "GameNode.h"
#include "GameComponent.h"
//#include "Renderer.h"
//#include "CoreEngine.h"
#include "IRenderable.h"
#include "IInputable.h"
#include "IUpdateable.h"

#include "Utility\ILogger.h"

using namespace Rendering;
using namespace Utility;
using namespace std;

/* static */ int GameNode::gameNodeCount = 0;

GameNode::GameNode(void) :
	m_ID(++GameNode::gameNodeCount)
{
	//INFO_LOG("Transform.GetPos() = \"%s\"", transform.GetPos().ToString().c_str());
	//INFO_LOG("Transform.GetRot() = \"%s\"", transform.GetRot().ToString().c_str());
	//INFO_LOG("Transform.GetScale() = \"%f\"", transform.GetScale());
}


GameNode::~GameNode(void)
{
	DEBUG_LOG("Game node (ID=%d) destruction started", m_ID);
	//DEBUG_LOG("Destroying components started for game node with ID=%d", this->ID);
	//for (std::vector<GameComponent*>::iterator itr = components.begin(); itr != components.end(); ++itr)
	//{
	//	SAFE_DELETE(*itr);
	//}
	//DEBUG_LOG("Destroying components finished for game node with ID=%d", this->ID);

	//DEBUG_LOG("Started recursive destruction of game nodes for game node with ID=%d", this->ID);
	//for (std::vector<GameNode*>::iterator itr = childrenGameNodes.begin(); itr != childrenGameNodes.end(); ++itr)
	//{
	//	if ((*itr) != NULL)
	//	{
	//		DELOCUST_LOG("Destroying child game node started");
	//		delete *itr;
	//		*itr = NULL;
	//		DELOCUST_LOG("Destroying child game node finished");
	//	}
	//}
	//DEBUG_LOG("Recursive destruction of game nodes for game node with ID=%d finished", this->ID);
	//childrenGameNodes.clear();

	for (unsigned int i = 0; i < m_components.size(); ++i)
	{
		SAFE_DELETE(m_components[i]);
	}
	m_components.clear();
	for (unsigned int i = 0; i < m_childrenGameNodes.size(); ++i)
	{
		SAFE_DELETE(m_childrenGameNodes[i]);
	}
	m_childrenGameNodes.clear();
	DEBUG_LOG("Game node (ID=%d) destruction finished", m_ID);
}

GameNode* GameNode::AddChild(GameNode* child)
{
	m_childrenGameNodes.push_back(child);
	child->GetTransform().SetParent(&m_transform);
	return this;
}

GameNode* GameNode::AddComponent(GameComponent* child)
{
	m_components.push_back(child);
	child->SetParent(this);
	IRenderable* renderableComponent = dynamic_cast<IRenderable*>(child);
	if (renderableComponent != NULL)
	{
		m_renderableComponents.push_back(renderableComponent);
	}
	Input::IInputableKeyboard* inputableKeyboardComponent = dynamic_cast<Input::IInputableKeyboard*>(child);
	if (inputableKeyboardComponent != NULL)
	{
		m_inputableKeyboardComponents.push_back(inputableKeyboardComponent);
	}
	Input::IInputableMouse* inputableMouseComponent = dynamic_cast<Input::IInputableMouse*>(child);
	if (inputableMouseComponent != NULL)
	{
		m_inputableMouseComponents.push_back(inputableMouseComponent);
	}
	IUpdateable* updateableComponent = dynamic_cast<IUpdateable*>(child);
	if (updateableComponent != NULL)
	{
		m_updateableComponents.push_back(updateableComponent);
	}
	return this;
}

//void GameNode::InputAll(Math::Real delta)
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

void GameNode::KeyEvent(int key, int scancode, int action, int mods)
{
	for (std::vector<Input::IInputableKeyboard*>::iterator gameComponentItr = m_inputableKeyboardComponents.begin(); gameComponentItr != m_inputableKeyboardComponents.end(); ++gameComponentItr)
	{
		(*gameComponentItr)->KeyEvent(key, scancode, action, mods);
	}

	for (std::vector<GameNode*>::iterator gameNodeItr = m_childrenGameNodes.begin(); gameNodeItr != m_childrenGameNodes.end(); ++gameNodeItr)
	{
		(*gameNodeItr)->KeyEvent(key, scancode, action, mods);
	}
}

void GameNode::MouseButtonEvent(int button, int action, int mods)
{
	for (std::vector<Input::IInputableMouse*>::iterator gameComponentItr = m_inputableMouseComponents.begin(); gameComponentItr != m_inputableMouseComponents.end(); ++gameComponentItr)
	{
		(*gameComponentItr)->MouseButtonEvent(button, action, mods);
	}

	for (std::vector<GameNode*>::iterator gameNodeItr = m_childrenGameNodes.begin(); gameNodeItr != m_childrenGameNodes.end(); ++gameNodeItr)
	{
		(*gameNodeItr)->MouseButtonEvent(button, action, mods);
	}
}

void GameNode::MousePosEvent(double xPos, double yPos)
{
	for (std::vector<Input::IInputableMouse*>::iterator gameComponentItr = m_inputableMouseComponents.begin(); gameComponentItr != m_inputableMouseComponents.end(); ++gameComponentItr)
	{
		(*gameComponentItr)->MousePosEvent(xPos, yPos);
	}

	for (std::vector<GameNode*>::iterator gameNodeItr = m_childrenGameNodes.begin(); gameNodeItr != m_childrenGameNodes.end(); ++gameNodeItr)
	{
		(*gameNodeItr)->MousePosEvent(xPos, yPos);
	}
}

void GameNode::ScrollEvent(double xOffset, double yOffset)
{
	for (std::vector<Input::IInputableMouse*>::iterator gameComponentItr = m_inputableMouseComponents.begin(); gameComponentItr != m_inputableMouseComponents.end(); ++gameComponentItr)
	{
		(*gameComponentItr)->ScrollEvent(xOffset, yOffset);
	}

	for (std::vector<GameNode*>::iterator gameNodeItr = m_childrenGameNodes.begin(); gameNodeItr != m_childrenGameNodes.end(); ++gameNodeItr)
	{
		(*gameNodeItr)->ScrollEvent(xOffset, yOffset);
	}
}

void GameNode::Update(Math::Real delta)
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

void GameNode::Render(Shader* shader, Renderer* renderer) const
{
	for (std::vector<IRenderable*>::const_iterator gameComponentItr = m_renderableComponents.begin(); gameComponentItr != m_renderableComponents.end(); ++gameComponentItr)
	{
		(*gameComponentItr)->Render(shader, renderer);
	}

	for (std::vector<GameNode*>::const_iterator gameNodeItr = m_childrenGameNodes.begin(); gameNodeItr != m_childrenGameNodes.end(); ++gameNodeItr)
	{
		(*gameNodeItr)->Render(shader, renderer);
	}
}

std::vector<GameNode*> GameNode::GetAllDescendants() const
{
	std::vector<GameNode*> descendants;
	
	for (std::vector<GameNode*>::const_iterator itr = m_childrenGameNodes.begin(); itr != m_childrenGameNodes.end(); ++itr)
	{
		descendants.push_back(*itr);
		std::vector<GameNode*> itrDescendants = (*itr)->GetAllDescendants();
		descendants.insert(descendants.end(), itrDescendants.begin(), itrDescendants.end());
	}

	//descendants.push_back(this);
	return descendants;
}