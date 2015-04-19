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
	if (m_mesh == NULL)
	{
		LOG(Utility::Error, LOGPLACE, "Cannot create a MeshRenderer instance. Mesh is NULL.");
		exit(EXIT_FAILURE);
	}
	m_mesh->Initialize();
	if (m_material == NULL)
	{
		LOG(Utility::Warning, LOGPLACE, "The material given to the mesh renderer is NULL.");
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
	if (shader == NULL)
	{
		LOG(Utility::Emergency, LOGPLACE, "Rendering a mesh failed. Shader instance is NULL.");
		return;
	}
	if (renderer == NULL)
	{
		LOG(Utility::Critical, LOGPLACE, "Rendering a mesh failed. Rendering engine is NULL.");
		exit(EXIT_FAILURE);
	}
	if (m_material == NULL)
	{
		LOG(Utility::Warning, LOGPLACE, "Rendering a mesh failed. Material instance is NULL.");
		return;
	}
	shader->Bind();
	shader->UpdateUniforms(GetTransform(), *m_material, renderer);
	m_mesh->Draw();
}
