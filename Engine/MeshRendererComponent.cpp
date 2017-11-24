#include "StdAfx.h"
#include "MeshRendererComponent.h"
#include "Rendering/Shader.h"
#include "Rendering/Renderer.h"

engine::MeshRendererComponent::MeshRendererComponent(int meshID, const rendering::Material* material) :
	GameComponent(),
	IRenderable(),
	m_meshID(meshID),
	m_material(material)
{
	CHECK_CONDITION_ENGINE(m_material != NULL, Utility::Logging::WARNING, "The material given to the mesh renderer component is NULL.");
}


engine::MeshRendererComponent::~MeshRendererComponent(void)
{
	SAFE_DELETE(m_material);
}

engine::MeshRendererComponent::MeshRendererComponent(MeshRendererComponent&& meshRendererComponent) :
	GameComponent(std::move(meshRendererComponent)),
	IRenderable(std::move(meshRendererComponent)),
	m_meshID(std::move(meshRendererComponent.m_meshID)),
	m_material(std::move(meshRendererComponent.m_material))
{
	meshRendererComponent.m_material = NULL;
}

engine::MeshRendererComponent& engine::MeshRendererComponent::operator=(MeshRendererComponent&& meshRendererComponent)
{
	GameComponent::operator=(std::move(meshRendererComponent));
	IRenderable::operator=(std::move(meshRendererComponent));
	m_meshID = std::move(meshRendererComponent.m_meshID);
	m_material = std::move(meshRendererComponent.m_material);
	meshRendererComponent.m_material = NULL;
	return *this;
}

void engine::MeshRendererComponent::Render(int shaderID, rendering::Renderer* renderer) const
{
	//CRITICAL_LOG_ENGINE("Rendering mesh started");
	CHECK_CONDITION_EXIT_ENGINE(renderer != NULL, Utility::Logging::CRITICAL, "Rendering a mesh failed. Rendering engine is NULL.");
	CHECK_CONDITION_ENGINE(m_material != NULL, Utility::Logging::WARNING, "Rendering a mesh while the material is NULL.");


	//const math::Transform& transform = GetTransform();
	//renderer->AddRenderCommand(shader, m_mesh, m_material, &transform);
	renderer->Render(m_meshID, m_material, GetTransform(), shaderID);
}
