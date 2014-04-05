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
	shader->Bind();
	shader->UpdateUniforms(GetTransform(), *material, renderer);
	mesh->Draw();
}