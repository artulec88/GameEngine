#include "stdafx.h"
#include "BillboardRendererComponent.h"
#include "Rendering\Shader.h"

engine::BillboardsRendererComponent::BillboardsRendererComponent(int billboardMeshID, Rendering::Material* billboardMaterial) :
	GameComponent(),
	IRenderable(),
	m_billboardMeshID(billboardMeshID),
	m_billboardMaterial(billboardMaterial)
{
}


engine::BillboardsRendererComponent::~BillboardsRendererComponent()
{
	SAFE_DELETE(m_billboardMaterial);
}

engine::BillboardsRendererComponent::BillboardsRendererComponent(BillboardsRendererComponent&& billboardsRendererComponent) :
	GameComponent(std::move(billboardsRendererComponent)),
	IRenderable(std::move(billboardsRendererComponent)),
	m_billboardMeshID(std::move(billboardsRendererComponent.m_billboardMeshID)),
	m_billboardMaterial(std::move(billboardsRendererComponent.m_billboardMaterial))
{
	billboardsRendererComponent.m_billboardMaterial = NULL;
}

engine::BillboardsRendererComponent& engine::BillboardsRendererComponent::operator=(BillboardsRendererComponent&& billboardsRendererComponent)
{
	GameComponent::operator=(std::move(billboardsRendererComponent));
	m_billboardMeshID = std::move(billboardsRendererComponent.m_billboardMeshID);
	m_billboardMaterial = std::move(billboardsRendererComponent.m_billboardMaterial);
	billboardsRendererComponent.m_billboardMaterial = NULL;
	return *this;
}

void engine::BillboardsRendererComponent::Render(int shaderID, Rendering::Renderer* renderer) const
{
	//CRITICAL_LOG_ENGINE("Rendering billboards started");
	//shader->Bind();
	//shader->UpdateUniforms(GetTransform(), m_billboardMaterial, renderer);
	//m_billboardMesh->Draw();

	renderer->Render(m_billboardMeshID, m_billboardMaterial, GetTransform(), shaderID);
	//renderer->RenderBillboards(*m_billboardMesh, m_billboardMaterial, shader);
}