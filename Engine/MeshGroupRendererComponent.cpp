#include "StdAfx.h"
#include "MeshGroupRendererComponent.h"

Engine::MeshGroupRendererComponent::MeshGroupRendererComponent(Rendering::Mesh* mesh, Rendering::Material* material) :
	GameComponent(),
	IRenderable(),
	m_mesh(mesh),
	m_material(material)
{
	CHECK_CONDITION_EXIT_ENGINE(m_mesh != NULL, Utility::ERR, "Cannot create a mesh renderer instance. The specified mesh is NULL.");
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


Engine::MeshGroupRendererComponent::~MeshGroupRendererComponent(void)
{
	SAFE_DELETE(m_mesh);
	SAFE_DELETE(m_material);
}

void Engine::MeshGroupRendererComponent::Render(const Rendering::Shader* shader, Rendering::Renderer* renderer)
{
	CHECK_CONDITION_RETURN_VOID_ENGINE(shader != NULL, Utility::EMERGENCY, "Rendering a mesh failed. Shader instance is NULL.");
	CHECK_CONDITION_EXIT_ENGINE(renderer != NULL, Utility::CRITICAL, "Rendering a mesh failed. Rendering engine is NULL.");
	CHECK_CONDITION_ENGINE(m_material != NULL, Utility::WARNING, "Rendering a mesh while the material is NULL.");
	//shader->Bind();
	//shader->UpdateUniforms(GetTransform(), m_material, renderer);
	//m_mesh->Draw();

	const Math::Transform& transform = GetTransform();
	//renderer->AddRenderCommand(shader, m_mesh, m_material, &transform);
}
