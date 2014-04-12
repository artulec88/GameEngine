#include "StdAfx.h"
#include "GameNode.h"
#include "GameComponent.h"
//#include "Renderer.h"
#include "CoreEngine.h"

#include "Utility\Log.h"

using namespace Rendering;
using namespace Utility;
using namespace std;

/* static */ int GameNode::gameNodeCount = 0;

GameNode::GameNode(void) :
	ID(GameNode::gameNodeCount++),
	coreEngine(NULL)
{
	//stdlog(Info, LOGPLACE, "Transform.GetPos() = \"%s\"", transform.GetPos().ToString().c_str());
	//stdlog(Info, LOGPLACE, "Transform.GetRot() = \"%s\"", transform.GetRot().ToString().c_str());
	//stdlog(Info, LOGPLACE, "Transform.GetScale() = \"%f\"", transform.GetScale());
}


GameNode::~GameNode(void)
{
	stdlog(Debug, LOGPLACE, "Game node destruction started");
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
	//		stdlog(Debug, LOGPLACE, "Destroying child game node started");
	//		delete *itr;
	//		//*itr = NULL;
	//		stdlog(Debug, LOGPLACE, "Destroying child game node finished");
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
	stdlog(Debug, LOGPLACE, "Game node destroyed");
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
		stdlog(Debug, LOGPLACE, "Core engine already set for the game node");
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

void GameNode::Input(Math::Real delta)
{
	for (std::vector<GameComponent*>::iterator gameComponentItr = components.begin(); gameComponentItr != components.end(); ++gameComponentItr)
	{
		(*gameComponentItr)->Input(delta);
	}
	for (std::vector<GameNode*>::iterator gameNodeItr = childrenGameNodes.begin(); gameNodeItr != childrenGameNodes.end(); ++gameNodeItr)
	{
		(*gameNodeItr)->Input(delta);
	}
}

void GameNode::Update(Math::Real delta)
{
	if (this->ID == 3)
	{
		//stdlog(Debug, LOGPLACE, "1) transform=\"%s\"", GetTransform().GetRot().ToString().c_str());
		for (std::vector<GameComponent*>::iterator gameComponentItr = components.begin(); gameComponentItr != components.end(); ++gameComponentItr)
		{
			(*gameComponentItr)->Update(delta);
		}
		for (std::vector<GameNode*>::iterator gameNodeItr = childrenGameNodes.begin(); gameNodeItr != childrenGameNodes.end(); ++gameNodeItr)
		{
			(*gameNodeItr)->Update(delta);
		}
		return;
	}
	for (std::vector<GameComponent*>::iterator gameComponentItr = components.begin(); gameComponentItr != components.end(); ++gameComponentItr)
	{
		(*gameComponentItr)->Update(delta);
	}
	for (std::vector<GameNode*>::iterator gameNodeItr = childrenGameNodes.begin(); gameNodeItr != childrenGameNodes.end(); ++gameNodeItr)
	{
		(*gameNodeItr)->Update(delta);
	}
}

void GameNode::Render(Shader* shader, Renderer* renderer)
{
	for (std::vector<GameComponent*>::iterator gameComponentItr = components.begin(); gameComponentItr != components.end(); ++gameComponentItr)
	{
		(*gameComponentItr)->Render(shader, renderer);
	}
	for (std::vector<GameNode*>::iterator gameNodeItr = childrenGameNodes.begin(); gameNodeItr != childrenGameNodes.end(); ++gameNodeItr)
	{
		(*gameNodeItr)->Render(shader, renderer);
	}
}