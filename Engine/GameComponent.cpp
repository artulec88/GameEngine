#include "StdAfx.h"
#include "GameComponent.h"
#include "GameNode.h"

#include "Rendering/Renderer.h"

#include "Utility/ILogger.h"

engine::GameComponent::GameComponent() :
	m_parentGameNode(nullptr)
{
}


engine::GameComponent::~GameComponent()
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
	CHECK_CONDITION_EXIT_ENGINE(parentGameNode != nullptr, utility::logging::ERR,
		"Cannot assign a parent to the game component. Parent game node is NULL.");
	m_parentGameNode = parentGameNode;
}

math::Transform& engine::GameComponent::GetTransform()
{
	CHECK_CONDITION_EXIT_ENGINE(m_parentGameNode != nullptr, utility::logging::CRITICAL,
		"Cannot get transformation for a given component. Parent game node is NULL.");
	return m_parentGameNode->GetTransform();
}

const math::Transform& engine::GameComponent::GetTransform() const
{
	CHECK_CONDITION_EXIT_ENGINE(m_parentGameNode != nullptr, utility::logging::CRITICAL,
		"Cannot get transformation for a given component. Parent game node is NULL.");
	return m_parentGameNode->GetTransform();
}
