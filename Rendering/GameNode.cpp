#include "StdAfx.h"
#include "GameNode.h"
#include "GameComponent.h"

#include "Utility\Log.h"

using namespace Rendering;
using namespace std;

GameNode::GameNode(void)
{
}


GameNode::~GameNode(void)
{
	//for (std::vector<GameComponent*>::iterator itr = components.begin(); itr != components.end(); ++itr)
	//{
	//	if ((*itr) != NULL)
	//	{
	//		delete *itr;
	//		*itr = NULL;
	//	}
	//}
	for (std::vector<GameNode*>::iterator itr = childrenGameNodes.begin(); itr != childrenGameNodes.end(); ++itr)
	{
		if ((*itr) != NULL)
		{
			delete *itr;
			*itr = NULL;
		}
	}
}

GameNode* GameNode::AddChild(GameNode* child)
{
	childrenGameNodes.push_back(child);
	return this;
}

GameNode* GameNode::AddComponent(GameComponent* child)
{
	components.push_back(child);
	return this;
}

void GameNode::Input()
{
	for (std::vector<GameComponent*>::iterator gameComponentItr = components.begin(); gameComponentItr != components.end(); ++gameComponentItr)
	{
		(*gameComponentItr)->Input();
	}
	for (std::vector<GameNode*>::iterator gameNodeItr = childrenGameNodes.begin(); gameNodeItr != childrenGameNodes.end(); ++gameNodeItr)
	{
		(*gameNodeItr)->Input();
	}
}

void GameNode::Update()
{
	for (std::vector<GameComponent*>::iterator gameComponentItr = components.begin(); gameComponentItr != components.end(); ++gameComponentItr)
	{
		(*gameComponentItr)->Update();
	}
	for (std::vector<GameNode*>::iterator gameNodeItr = childrenGameNodes.begin(); gameNodeItr != childrenGameNodes.end(); ++gameNodeItr)
	{
		(*gameNodeItr)->Update();
	}
}

void GameNode::Render(Shader* shader)
{
	for (std::vector<GameComponent*>::iterator gameComponentItr = components.begin(); gameComponentItr != components.end(); ++gameComponentItr)
	{
		(*gameComponentItr)->Render(shader);
	}
	for (std::vector<GameNode*>::iterator gameNodeItr = childrenGameNodes.begin(); gameNodeItr != childrenGameNodes.end(); ++gameNodeItr)
	{
		(*gameNodeItr)->Render(shader);
	}
}