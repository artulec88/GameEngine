#include "StdAfx.h"
#include "MeshRendererComponent.h"
#include "Rendering\Shader.h"
#include "Rendering\Renderer.h"

Engine::MeshRendererComponent::MeshRendererComponent(Rendering::MeshRenderer* meshRenderer) :
	GameComponent(),
	IRenderable(),
	m_meshRenderer(meshRenderer)
{
}


Engine::MeshRendererComponent::~MeshRendererComponent(void)
{
	SAFE_DELETE(m_meshRenderer);
}

void Engine::MeshRendererComponent::Render(Rendering::Shader* shader, Rendering::Renderer* renderer) const
{
	CHECK_CONDITION_RETURN_VOID(shader != NULL, Utility::Emergency, "Rendering a mesh failed. Shader instance is NULL.");
	CHECK_CONDITION_EXIT(renderer != NULL, Utility::Critical, "Rendering a mesh failed. Rendering engine is NULL.");
	
	m_meshRenderer->Render(shader, renderer);
}
