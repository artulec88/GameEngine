#include "stdafx.h"
#include "BillboardRendererComponent.h"

engine::BillboardsRendererComponent::BillboardsRendererComponent(int billboardMeshId, rendering::Material* billboardMaterial) :
	GameComponent(),
	IRenderable(),
	m_billboardMeshId(billboardMeshId),
	m_billboardMaterial(billboardMaterial)
{
}


engine::BillboardsRendererComponent::~BillboardsRendererComponent()
{
	SAFE_DELETE(m_billboardMaterial);
}

engine::BillboardsRendererComponent::BillboardsRendererComponent(BillboardsRendererComponent&& billboardsRendererComponent) noexcept:
	GameComponent(std::move(billboardsRendererComponent)),
	IRenderable(std::move(billboardsRendererComponent)),
	m_billboardMeshId(std::move(billboardsRendererComponent.m_billboardMeshId)),
	m_billboardMaterial(std::move(billboardsRendererComponent.m_billboardMaterial))
{
	billboardsRendererComponent.m_billboardMaterial = nullptr;
}

engine::BillboardsRendererComponent& engine::BillboardsRendererComponent::operator=(BillboardsRendererComponent&& billboardsRendererComponent) noexcept
{
	GameComponent::operator=(std::move(billboardsRendererComponent));
	m_billboardMeshId = std::move(billboardsRendererComponent.m_billboardMeshId);
	m_billboardMaterial = std::move(billboardsRendererComponent.m_billboardMaterial);
	billboardsRendererComponent.m_billboardMaterial = nullptr;
	return *this;
}

void engine::BillboardsRendererComponent::Render(int shaderId, rendering::Renderer* renderer) const
{
	//CRITICAL_LOG_ENGINE("Rendering billboards started");
	//shader->Bind();
	//shader->UpdateUniforms(GetTransform(), m_billboardMaterial, renderer);
	//m_billboardMesh->Draw();

	renderer->Render(m_billboardMeshId, m_billboardMaterial, GetTransform(), shaderId);
	//renderer->RenderBillboards(*m_billboardMesh, m_billboardMaterial, shader);
}