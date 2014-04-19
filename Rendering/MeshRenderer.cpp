#include "StdAfx.h"
#include "MeshRenderer.h"
#include "Mesh.h"
#include "Material.h"
#include "Shader.h"

using namespace Rendering;

MeshRenderer::MeshRenderer(Mesh* mesh, Material* material) :
	GameComponent(),
	mesh(mesh),
	material(material)
{
	if (this->mesh == NULL)
	{
		stdlog(Utility::Error, LOGPLACE, "Cannot create a MeshRenderer instance. Mesh is NULL");
		exit(EXIT_FAILURE);
	}
	if (this->material == NULL)
	{
		stdlog(Utility::Warning, LOGPLACE, "The material given to the mesh renderer is NULL");
	}
	//if (this->mesh != NULL)
	//{
	//	this->mesh->AddReference();
	//}
}


MeshRenderer::~MeshRenderer(void)
{
	// TODO: Fix dangling pointers
	if (mesh != NULL)
	{
		//mesh->RemoveReference();
		//if (! mesh->IsReferenced())
		//{
		//	delete mesh;
		//	mesh = NULL;
		//}
		delete mesh;
		mesh = NULL;
	}
	if (material != NULL)
	{
		delete material;
		material = NULL;
	}
}

void MeshRenderer::Render(Shader* shader, Renderer* renderer)
{
	if (shader == NULL)
	{
		stdlog(Utility::Emergency, LOGPLACE, "Rendering a mesh failed. Shader instance is NULL");
		return;
	}
	if (renderer == NULL)
	{
		stdlog(Utility::Critical, LOGPLACE, "Rendering a mesh failed. Rendering engine is NULL");
		exit(EXIT_FAILURE);
	}
	if (this->material == NULL)
	{
		stdlog(Utility::Warning, LOGPLACE, "Rendering a mesh failed. Material instance is NULL");
		return;
	}
	shader->Bind();
	shader->UpdateUniforms(GetTransform(), *material, renderer);
	mesh->Draw();
}