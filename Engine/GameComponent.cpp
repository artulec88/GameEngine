#include "StdAfx.h"
#include "GameComponent.h"
#include "GameNode.h"

#include "Rendering\Renderer.h"
#include "Rendering\Shader.h"

#include "Utility\ILogger.h"

engine::GameComponent::GameComponent(void) :
	m_parentGameNode(NULL)
{
}


engine::GameComponent::~GameComponent(void)
{
}

//Engine::GameComponent::GameComponent(GameComponent&& gameComponent) :
//	m_parentGameNode(std::move(gameComponent.m_parentGameNode))
//{
//}

//Engine::GameComponent& Engine::GameComponent::operator=(GameComponent&& gameComponent)
//{
//	m_parentGameNode = std::move(gameComponent.m_parentGameNode);
//	return *this;
//}

void engine::GameComponent::SetParent(GameNode* parentGameNode)
{
	CHECK_CONDITION_EXIT_ENGINE(parentGameNode != NULL, Utility::Logging::ERR, "Cannot assign a parent to the game component. Parent game node is NULL.");
	m_parentGameNode = parentGameNode;
}

Math::Transform& engine::GameComponent::GetTransform()
{
	CHECK_CONDITION_EXIT_ENGINE(m_parentGameNode != NULL, Utility::Logging::CRITICAL, "Cannot get transformation for a given component. Parent game node is NULL.");
	return m_parentGameNode->GetTransform();
}

const Math::Transform& engine::GameComponent::GetTransform() const
{
	CHECK_CONDITION_EXIT_ENGINE(m_parentGameNode != NULL, Utility::Logging::CRITICAL, "Cannot get transformation for a given component. Parent game node is NULL.");
	return m_parentGameNode->GetTransform();
}
