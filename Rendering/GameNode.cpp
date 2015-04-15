#include "StdAfx.h"
#include "GameNode.h"
#include "GameComponent.h"
//#include "Renderer.h"
#include "CoreEngine.h"

#include "Utility\ILogger.h"

using namespace Rendering;
using namespace Utility;
using namespace std;

/* static */ int GameNode::gameNodeCount = 0;

GameNode::GameNode(void) :
	m_ID(GameNode::gameNodeCount++)
{
	//LOG(Info, LOGPLACE, "Transform.GetPos() = \"%s\"", transform.GetPos().ToString().c_str());
	//LOG(Info, LOGPLACE, "Transform.GetRot() = \"%s\"", transform.GetRot().ToString().c_str());
	//LOG(Info, LOGPLACE, "Transform.GetScale() = \"%f\"", transform.GetScale());
}


GameNode::~GameNode(void)
{
	LOG(Info, LOGPLACE, "Game node (ID=%d) destruction started", m_ID);
	//LOG(Debug, LOGPLACE, "Destroying components started for game node with ID=%d", this->ID);
	//for (std::vector<GameComponent*>::iterator itr = components.begin(); itr != components.end(); ++itr)
	//{
	//	SAFE_DELETE(*itr);
	//}
	//LOG(Debug, LOGPLACE, "Destroying components finished for game node with ID=%d", this->ID);

	//LOG(Debug, LOGPLACE, "Started recursive destruction of game nodes for game node with ID=%d", this->ID);
	//for (std::vector<GameNode*>::iterator itr = childrenGameNodes.begin(); itr != childrenGameNodes.end(); ++itr)
	//{
	//	if ((*itr) != NULL)
	//	{
	//		LOG(Delocust, LOGPLACE, "Destroying child game node started");
	//		delete *itr;
	//		*itr = NULL;
	//		LOG(Delocust, LOGPLACE, "Destroying child game node finished");
	//	}
	//}
	//LOG(Debug, LOGPLACE, "Recursive destruction of game nodes for game node with ID=%d finished", this->ID);
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
	LOG(Info, LOGPLACE, "Game node (ID=%d) destruction finished", m_ID);
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
	return this;
}

void GameNode::InputAll(Math::Real delta)
{
	Input(delta);

	for (std::vector<GameNode*>::iterator gameNodeItr = m_childrenGameNodes.begin(); gameNodeItr != m_childrenGameNodes.end(); ++gameNodeItr)
	{
		(*gameNodeItr)->InputAll(delta);
	}
}

void GameNode::Input(Math::Real delta)
{
	//transform.Update();

	for (std::vector<GameComponent*>::iterator gameComponentItr = m_components.begin(); gameComponentItr != m_components.end(); ++gameComponentItr)
	{
		(*gameComponentItr)->Input(delta);
	}
}

void GameNode::UpdateAll(Math::Real delta)
{
	Update(delta);

	for (std::vector<GameNode*>::iterator gameNodeItr = m_childrenGameNodes.begin(); gameNodeItr != m_childrenGameNodes.end(); ++gameNodeItr)
	{
		(*gameNodeItr)->UpdateAll(delta);
	}
}

void GameNode::Update(Math::Real delta)
{
	for (std::vector<GameComponent*>::iterator gameComponentItr = m_components.begin(); gameComponentItr != m_components.end(); ++gameComponentItr)
	{
		(*gameComponentItr)->Update(delta);
	}
}

void GameNode::RenderAll(Shader* shader, Renderer* renderer) const
{
	Render(shader, renderer);

	for (std::vector<GameNode*>::const_iterator gameNodeItr = m_childrenGameNodes.begin(); gameNodeItr != m_childrenGameNodes.end(); ++gameNodeItr)
	{
		(*gameNodeItr)->RenderAll(shader, renderer);
	}
}

void GameNode::Render(Shader* shader, Renderer* renderer) const
{
	for (std::vector<GameComponent*>::const_iterator gameComponentItr = m_components.begin(); gameComponentItr != m_components.end(); ++gameComponentItr)
	{
		(*gameComponentItr)->Render(shader, renderer);
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