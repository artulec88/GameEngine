#include "StdAfx.h"
#include "MeshRenderer.h"
#include "Mesh.h"
#include "Material.h"
#include "Shader.h"

using namespace Rendering;

MeshRenderer::MeshRenderer(Mesh* mesh, Material* material) :
	GameComponent(),
	m_mesh(mesh),
	m_material(material)
{
	CHECK_CONDITION_EXIT(m_mesh != NULL, Utility::Error, "Cannot create a mesh renderer instance. The specified mesh is NULL.");
	m_mesh->Initialize();
	if (m_material == NULL)
	{
		WARNING_LOG("The material given to the mesh renderer is NULL.");
	}
	//if (m_mesh != NULL)
	//{
	//	m_mesh->AddReference();
	//}
}


MeshRenderer::~MeshRenderer(void)
{
	// TODO: Fix dangling pointers
	SAFE_DELETE(m_mesh);
	SAFE_DELETE(m_material);
}

void MeshRenderer::Render(Shader* shader, Renderer* renderer)
{
	CHECK_CONDITION_RETURN_VOID(shader != NULL, Utility::Emergency, "Rendering a mesh failed. Shader instance is NULL.");
	CHECK_CONDITION_EXIT(renderer != NULL, Utility::Critical, "Rendering a mesh failed. Rendering engine is NULL.");
	CHECK_CONDITION(m_material != NULL, Utility::Warning, "Rendering a mesh while the material is NULL.");
	shader->Bind();
	shader->UpdateUniforms(GetTransform(), m_material, renderer);
	m_mesh->Draw();
}
