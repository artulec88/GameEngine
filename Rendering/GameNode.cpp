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
	ID(GameNode::gameNodeCount++),
	coreEngine(NULL)
{
	//LOG(Info, LOGPLACE, "Transform.GetPos() = \"%s\"", transform.GetPos().ToString().c_str());
	//LOG(Info, LOGPLACE, "Transform.GetRot() = \"%s\"", transform.GetRot().ToString().c_str());
	//LOG(Info, LOGPLACE, "Transform.GetScale() = \"%f\"", transform.GetScale());
}


GameNode::~GameNode(void)
{
	LOG(Debug, LOGPLACE, "Game node destruction started");
	//for (std::vector<GameComponent*>::iterator itr = components.begin(); itr != components.end(); ++itr)
	//{
	//	if ((*itr) != NULL)
	//	{
	//		delete *itr;
	//		*itr = NULL;
	//	}
	//}

	//for (std::vector<GameNode*>::iterator itr = childrenGameNodes.begin(); itr != childrenGameNodes.end(); ++itr)
	//{
	//	if ((*itr) != NULL)
	//	{
	//		LOG(Debug, LOGPLACE, "Destroying child game node started");
	//		delete *itr;
	//		//*itr = NULL;
	//		LOG(Debug, LOGPLACE, "Destroying child game node finished");
	//	}
	//}
	//childrenGameNodes.clear();

	// TODO: Fix dangling pointers
	for (unsigned int i = 0; i < components.size(); ++i)
	{
		if (components[i] != NULL)
		{
			delete components[i];
			components[i] = NULL;
		}
	}
	components.clear();
	for (unsigned int i = 0; i < childrenGameNodes.size(); ++i)
	{
		if (childrenGameNodes[i] != NULL)
		{
			delete childrenGameNodes[i];
			childrenGameNodes[i] = NULL;
		}
	}
	childrenGameNodes.clear();
	LOG(Debug, LOGPLACE, "Game node destroyed");
}

GameNode* GameNode::AddChild(GameNode* child)
{
	childrenGameNodes.push_back(child);
	child->GetTransform().SetParent(&transform);
	child->SetEngine(coreEngine);
	return this;
}

void GameNode::SetEngine(CoreEngine* coreEngine)
{
	if (this->coreEngine == coreEngine)
	{
		LOG(Debug, LOGPLACE, "Core engine already set for the game node");
		return;
	}

	this->coreEngine = coreEngine;
	for (std::vector<GameComponent*>::iterator gameComponentItr = components.begin(); gameComponentItr != components.end(); ++gameComponentItr)
	{
		(*gameComponentItr)->AddToEngine(coreEngine);
	}
	for (std::vector<GameNode*>::iterator gameNodeItr = childrenGameNodes.begin(); gameNodeItr != childrenGameNodes.end(); ++gameNodeItr)
	{
		(*gameNodeItr)->SetEngine(coreEngine);
	}
}

GameNode* GameNode::AddComponent(GameComponent* child)
{
	components.push_back(child);
	child->SetParent(this);
	return this;
}

void GameNode::InputAll(Math::Real delta)
{
	Input(delta);

	for (std::vector<GameNode*>::iterator gameNodeItr = childrenGameNodes.begin(); gameNodeItr != childrenGameNodes.end(); ++gameNodeItr)
	{
		(*gameNodeItr)->InputAll(delta);
	}
}

void GameNode::Input(Math::Real delta)
{
	for (std::vector<GameComponent*>::iterator gameComponentItr = components.begin(); gameComponentItr != components.end(); ++gameComponentItr)
	{
		(*gameComponentItr)->Input(delta);
	}
}

void GameNode::UpdateAll(Math::Real delta)
{
	Update(delta);

	for (std::vector<GameNode*>::iterator gameNodeItr = childrenGameNodes.begin(); gameNodeItr != childrenGameNodes.end(); ++gameNodeItr)
	{
		(*gameNodeItr)->UpdateAll(delta);
	}
}

void GameNode::Update(Math::Real delta)
{
	for (std::vector<GameComponent*>::iterator gameComponentItr = components.begin(); gameComponentItr != components.end(); ++gameComponentItr)
	{
		(*gameComponentItr)->Update(delta);
	}
}

void GameNode::RenderAll(Shader* shader, Renderer* renderer)
{
	Render(shader, renderer);

	for (std::vector<GameNode*>::iterator gameNodeItr = childrenGameNodes.begin(); gameNodeItr != childrenGameNodes.end(); ++gameNodeItr)
	{
		(*gameNodeItr)->RenderAll(shader, renderer);
	}
}

void GameNode::Render(Shader* shader, Renderer* renderer)
{
	for (std::vector<GameComponent*>::iterator gameComponentItr = components.begin(); gameComponentItr != components.end(); ++gameComponentItr)
	{
		(*gameComponentItr)->Render(shader, renderer);
	}
}

std::vector<GameNode*> GameNode::GetAllDescendants() const
{
	std::vector<GameNode*> descendants;
	
	for (std::vector<GameNode*>::const_iterator itr = childrenGameNodes.begin(); itr != childrenGameNodes.end(); ++itr)
	{
		descendants.push_back(*itr);
		std::vector<GameNode*> itrDescendants = (*itr)->GetAllDescendants();
		descendants.insert(descendants.end(), itrDescendants.begin(), itrDescendants.end());
	}

	//descendants.push_back(this);
	return descendants;
}