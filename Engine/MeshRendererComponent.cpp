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

Engine::MeshRendererComponent::MeshRendererComponent(MeshRendererComponent&& meshRendererComponent) :
	GameComponent(std::move(meshRendererComponent)),
	IRenderable(std::move(meshRendererComponent)),
	m_mesh(std::move(meshRendererComponent.m_mesh)),
	m_material(std::move(meshRendererComponent.m_material))
{
	meshRendererComponent.m_mesh = NULL;
	meshRendererComponent.m_material = NULL;
}

Engine::MeshRendererComponent& Engine::MeshRendererComponent::operator=(MeshRendererComponent&& meshRendererComponent)
{
	GameComponent::operator=(std::move(meshRendererComponent));
	IRenderable::operator=(std::move(meshRendererComponent));
	m_mesh = std::move(meshRendererComponent.m_mesh);
	m_material = std::move(meshRendererComponent.m_material);
	meshRendererComponent.m_mesh = NULL;
	meshRendererComponent.m_material = NULL;
	return *this;
}

void Engine::MeshRendererComponent::Render(const Rendering::Shader* shader, Rendering::Renderer* renderer) const
{
	//CRITICAL_LOG_ENGINE("Rendering mesh started");
	CHECK_CONDITION_EXIT_ENGINE(renderer != NULL, Utility::Logging::CRITICAL, "Rendering a mesh failed. Rendering engine is NULL.");
	CHECK_CONDITION_ENGINE(m_material != NULL, Utility::Logging::WARNING, "Rendering a mesh while the material is NULL.");


	//const Math::Transform& transform = GetTransform();
	//renderer->AddRenderCommand(shader, m_mesh, m_material, &transform);
	renderer->Render(*m_mesh, m_material, GetTransform(), shader);
}
