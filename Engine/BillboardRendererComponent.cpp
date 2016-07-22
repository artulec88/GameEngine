#include "stdafx.h"
#include "BillboardRendererComponent.h"
#include "Rendering\Shader.h"

Engine::BillboardsRendererComponent::BillboardsRendererComponent(Rendering::BillboardMesh* billboardMesh, Rendering::Material* billboardMaterial) :
	m_billboardMesh(billboardMesh),
	m_billboardMaterial(billboardMaterial)
{
}


Engine::BillboardsRendererComponent::~BillboardsRendererComponent()
{
	SAFE_DELETE(m_billboardMesh);
	SAFE_DELETE(m_billboardMaterial);
}

void Engine::BillboardsRendererComponent::Render(const Rendering::Shader& shader, Rendering::Renderer* renderer) const
{
	//shader->Bind();
	//shader->UpdateUniforms(GetTransform(), m_billboardMaterial, renderer);
	//m_billboardMesh->Draw();

	renderer->Render(*m_billboardMesh, m_billboardMaterial, GetTransform(), shader);
	//renderer->RenderBillboards(*m_billboardMesh, m_billboardMaterial, shader);
}