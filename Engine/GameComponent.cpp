#include "StdAfx.h"
#include "GameComponent.h"
#include "GameNode.h"

#include "Rendering\Renderer.h"
#include "Rendering\Shader.h"

#include "Utility\ILogger.h"

Engine::GameComponent::GameComponent(void) :
	m_parentGameNode(NULL)
{
}


Engine::GameComponent::~GameComponent(void)
{
	// what if parentGameNode is still in use by some other stuff?
	//if (parentGameNode != NULL)
	//{
	//	delete parentGameNode;
	//	parentGameNode = NULL;
	//}
}

void Engine::GameComponent::SetParent(GameNode* parentGameNode)
{
	CHECK_CONDITION_EXIT_ENGINE(parentGameNode != NULL, Utility::ERR, "Cannot assign a parent to the game component. Parent game node is NULL.");
	m_parentGameNode = parentGameNode;
}

Math::Transform& Engine::GameComponent::GetTransform()
{
	CHECK_CONDITION_EXIT_ENGINE(m_parentGameNode != NULL, Utility::EMERGENCY, "Cannot get transformation for a given component. Parent game node is NULL.");
	return m_parentGameNode->GetTransform();
}

const Math::Transform& Engine::GameComponent::GetTransform() const
{
	CHECK_CONDITION_EXIT_ENGINE(m_parentGameNode != NULL, Utility::EMERGENCY, "Cannot get transformation for a given component. Parent game node is NULL.");
	return m_parentGameNode->GetTransform();
}
