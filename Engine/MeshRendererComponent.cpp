#include "StdAfx.h"
#include "MeshRendererComponent.h"
#include "Rendering\Shader.h"
#include "Rendering\Renderer.h"

Engine::MeshRendererComponent::MeshRendererComponent(Rendering::Mesh* mesh, Rendering::Material* material) :
	GameComponent(),
	IRenderable(),
	m_mesh(mesh),
	m_material(material)
{
	CHECK_CONDITION_EXIT_ENGINE(m_mesh != NULL, Utility::Logging::ERR, "Cannot create a mesh renderer instance. The specified mesh is NULL.");
	m_mesh->Initialize();
	if (m_material == NULL)
	{
		WARNING_LOG_ENGINE("The material given to the mesh renderer is NULL.");
	}
	//if (m_mesh != NULL)
	//{
	//	m_mesh->AddReference();
	//}
}


Engine::MeshRendererComponent::~MeshRendererComponent(void)
{
	SAFE_DELETE(m_mesh);
	SAFE_DELETE(m_material);
}

void Engine::MeshRendererComponent::Render(const Rendering::Shader& shader, Rendering::Renderer* renderer) const
{
	CHECK_CONDITION_EXIT_ENGINE(renderer != NULL, Utility::Logging::CRITICAL, "Rendering a mesh failed. Rendering engine is NULL.");
	CHECK_CONDITION_ENGINE(m_material != NULL, Utility::Logging::WARNING, "Rendering a mesh while the material is NULL.");


	//const Math::Transform& transform = GetTransform();
	//renderer->AddRenderCommand(shader, m_mesh, m_material, &transform);
	renderer->Render(*m_mesh, m_material, GetTransform(), shader);
}
