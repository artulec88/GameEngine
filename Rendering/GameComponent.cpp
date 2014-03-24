#include "StdAfx.h"
#include "GameComponent.h"

using namespace Rendering;

GameComponent::GameComponent(void) :
	parentGameNode(NULL)
{
}


GameComponent::~GameComponent(void)
{
	// what if parentGameNode is still in use by some other staff?
	//if (parentGameNode != NULL)
	//{
	//	delete parentGameNode;
	//	parentGameNode = NULL;
	//}
}

void GameComponent::Input()
{
}

void GameComponent::Update()
{
}

void GameComponent::Render(Shader* shader)
{

}

void GameComponent::SetParent(GameNode* parentGameNode)
{
	this->parentGameNode = parentGameNode;
}

Transform& GameComponent::GetTransform()
{
	return parentGameNode->GetTransform();
}