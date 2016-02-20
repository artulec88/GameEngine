#include "StdAfx.h"
#include "GameComponent.h"
#include "CoreEngine.h"
#include "Renderer.h"
#include "GameNode.h"
#include "Shader.h"
#include "Utility\ILogger.h"

using namespace Rendering;

GameComponent::GameComponent(void) :
	m_parentGameNode(NULL)
{
}


GameComponent::~GameComponent(void)
{
	// what if parentGameNode is still in use by some other stuff?
	//if (parentGameNode != NULL)
	//{
	//	delete parentGameNode;
	//	parentGameNode = NULL;
	//}
}

void GameComponent::SetParent(GameNode* parentGameNode)
{
	CHECK_CONDITION_EXIT(parentGameNode != NULL, Utility::Error, "Cannot assign a parent to the game component. Parent game node is NULL.");
	m_parentGameNode = parentGameNode;
}

Math::Transform& GameComponent::GetTransform()
{
	CHECK_CONDITION_EXIT(m_parentGameNode != NULL, Utility::Emergency, "Cannot get transformation for a given component. Parent game node is NULL.");
	return m_parentGameNode->GetTransform();
}

const Math::Transform& GameComponent::GetTransform() const
{
	CHECK_CONDITION_EXIT(m_parentGameNode != NULL, Utility::Emergency, "Cannot get transformation for a given component. Parent game node is NULL.");
	return m_parentGameNode->GetTransform();
}
