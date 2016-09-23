#include "stdafx.h"
#include "BillboardRendererComponent.h"
#include "Rendering\Shader.h"

Engine::BillboardsRendererComponent::BillboardsRendererComponent(Rendering::BillboardMesh* billboardMesh, Rendering::Material* billboardMaterial) :
	GameComponent(),
	IRenderable(),
	m_billboardMesh(billboardMesh),
	m_billboardMaterial(billboardMaterial)
{
}


Engine::BillboardsRendererComponent::~BillboardsRendererComponent()
{
	SAFE_DELETE(m_billboardMesh);
	SAFE_DELETE(m_billboardMaterial);
}

Engine::BillboardsRendererComponent::BillboardsRendererComponent(BillboardsRendererComponent&& billboardsRendererComponent) :
	GameComponent(std::move(billboardsRendererComponent)),
	IRenderable(std::move(billboardsRendererComponent)),
	m_billboardMesh(billboardsRendererComponent.m_billboardMesh),
	m_billboardMaterial(billboardsRendererComponent.m_billboardMaterial)
{
	billboardsRendererComponent.m_billboardMesh = NULL;
	billboardsRendererComponent.m_billboardMaterial = NULL;
}

Engine::BillboardsRendererComponent& Engine::BillboardsRendererComponent::operator=(BillboardsRendererComponent&& billboardsRendererComponent)
{
	GameComponent::operator=(std::move(billboardsRendererComponent));
	m_billboardMesh = std::move(billboardsRendererComponent.m_billboardMesh);
	m_billboardMaterial = std::move(billboardsRendererComponent.m_billboardMaterial);
	billboardsRendererComponent.m_billboardMesh = NULL;
	billboardsRendererComponent.m_billboardMaterial = NULL;
	return *this;
}

void Engine::BillboardsRendererComponent::Render(const Rendering::Shader* shader, Rendering::Renderer* renderer) const
{
	//CRITICAL_LOG_ENGINE("Rendering billboards started");
	//shader->Bind();
	//shader->UpdateUniforms(GetTransform(), m_billboardMaterial, renderer);
	//m_billboardMesh->Draw();

	renderer->Render(*m_billboardMesh, m_billboardMaterial, GetTransform(), shader);
	//renderer->RenderBillboards(*m_billboardMesh, m_billboardMaterial, shader);
}