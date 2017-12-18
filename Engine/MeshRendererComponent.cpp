#include "StdAfx.h"
#include "MeshRendererComponent.h"
#include "Rendering/Renderer.h"

engine::MeshRendererComponent::MeshRendererComponent(int meshId, const rendering::Material* material) :
	GameComponent(),
	IRenderable(),
	m_meshId(meshId),
	m_material(material)
{
	CHECK_CONDITION_ENGINE(m_material != nullptr, utility::logging::WARNING, "The material given to the mesh renderer component is NULL.");
}


engine::MeshRendererComponent::~MeshRendererComponent()
{
	SAFE_DELETE(m_material);
}

engine::MeshRendererComponent::MeshRendererComponent(MeshRendererComponent&& meshRendererComponent) noexcept:
	GameComponent(std::move(meshRendererComponent)),
	IRenderable(std::move(meshRendererComponent)),
	m_meshId(std::move(meshRendererComponent.m_meshId)),
	m_material(std::move(meshRendererComponent.m_material))
{
	meshRendererComponent.m_material = nullptr;
}

engine::MeshRendererComponent& engine::MeshRendererComponent::operator=(MeshRendererComponent&& meshRendererComponent) noexcept
{
	GameComponent::operator=(std::move(meshRendererComponent));
	IRenderable::operator=(std::move(meshRendererComponent));
	m_meshId = std::move(meshRendererComponent.m_meshId);
	m_material = std::move(meshRendererComponent.m_material);
	meshRendererComponent.m_material = nullptr;
	return *this;
}

void engine::MeshRendererComponent::Render(int shaderId, rendering::Renderer* renderer) const
{
	//CRITICAL_LOG_ENGINE("Rendering mesh started");
	CHECK_CONDITION_EXIT_ENGINE(renderer != NULL, utility::logging::CRITICAL, "Rendering a mesh failed. Rendering engine is NULL.");
	CHECK_CONDITION_ENGINE(m_material != NULL, utility::logging::WARNING, "Rendering a mesh while the material is NULL.");


	//const math::Transform& transform = GetTransform();
	//renderer->AddRenderCommand(shader, m_mesh, m_material, &transform);
	renderer->Render(m_meshId, m_material, GetTransform(), shaderId);
}
