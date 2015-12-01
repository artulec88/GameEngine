#include "StdAfx.h"
#include "GameComponent.h"
#include "Transform.h"
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

void GameComponent::Input(Math::Real delta)
{
	// TODO: Empty definitions wastes CPU time.
}

void GameComponent::Update(Math::Real delta)
{
	// TODO: Empty definitions wastes CPU time.
}

void GameComponent::Render(Shader* shader, Renderer* renderingEngine)
{
	// TODO: Empty definitions wastes CPU time.
}

void GameComponent::SetParent(GameNode* parentGameNode)
{
	CHECK_CONDITION_EXIT(parentGameNode != NULL, Utility::Error, "Cannot assign a parent to the game component. Parent game node is NULL.");
	m_parentGameNode = parentGameNode;
}

Transform& GameComponent::GetTransform()
{
	CHECK_CONDITION_EXIT(m_parentGameNode != NULL, Utility::Emergency, "Cannot get transformation for a given component. Parent game node is NULL.");
	return m_parentGameNode->GetTransform();
}

const Transform& GameComponent::GetTransform() const
{
	CHECK_CONDITION_EXIT(m_parentGameNode != NULL, Utility::Emergency, "Cannot get transformation for a given component. Parent game node is NULL.");
	return m_parentGameNode->GetTransform();
}
