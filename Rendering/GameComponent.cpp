#include "StdAfx.h"
#include "GameComponent.h"
#include "Transform.h"
#include "CoreEngine.h"
#include "Renderer.h"
#include "GameNode.h"
#include "Shader.h"
#include "Utility\Log.h"

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

void GameComponent::Input(Math::Real delta)
{
}

void GameComponent::Update(Math::Real delta)
{
}

void GameComponent::Render(Shader* shader, Renderer* renderingEngine)
{
}

void GameComponent::SetParent(GameNode* parentGameNode)
{
	this->parentGameNode = parentGameNode;
}

Transform& GameComponent::GetTransform()
{
	if (parentGameNode == NULL)
	{
		stdlog(Utility::Emergency, LOGPLACE, "Cannot get transformation for a given component. Parent game node is NULL.");
		exit(EXIT_FAILURE);
	}
	return parentGameNode->GetTransform();
}

const Transform& GameComponent::GetTransform() const
{
	if (parentGameNode == NULL)
	{
		stdlog(Utility::Emergency, LOGPLACE, "Cannot get transformation for a given component. Parent game node is NULL.");
		exit(EXIT_FAILURE);
	}
	return parentGameNode->GetTransform();
}

void GameComponent::AddToEngine(CoreEngine* engine)
{

}