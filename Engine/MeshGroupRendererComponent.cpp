#include "StdAfx.h"
#include "MeshGroupRendererComponent.h"

Engine::MeshGroupRendererComponent::MeshGroupRendererComponent(Rendering::MeshGroupRenderer* meshGroupRenderer) :
	Rendering::GameComponent(),
	Rendering::IRenderable(),
	m_meshGroupRenderer(meshGroupRenderer)
{
}


Engine::MeshGroupRendererComponent::~MeshGroupRendererComponent(void)
{
	SAFE_DELETE(m_meshGroupRenderer);
}

void Engine::MeshGroupRendererComponent::Render(Rendering::Shader* shader, Rendering::Renderer* renderer)
{
	CHECK_CONDITION_RETURN_VOID(shader != NULL, Utility::Emergency, "Rendering a mesh failed. Shader instance is NULL.");
	CHECK_CONDITION_EXIT(renderer != NULL, Utility::Critical, "Rendering a mesh failed. Rendering engine is NULL.");

	m_meshGroupRenderer->Render(shader, renderer);
}
