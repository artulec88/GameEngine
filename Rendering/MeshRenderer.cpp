#include "StdAfx.h"
#include "MeshRenderer.h"
#include "BasicShader.h"

using namespace Rendering;

MeshRenderer::MeshRenderer(Mesh* mesh, Material* material) :
	GameComponent(),
	mesh(mesh),
	material(material)
{
}


MeshRenderer::~MeshRenderer(void)
{
	if (mesh != NULL)
	{
		delete mesh;
		mesh = NULL;
	}
	if (material != NULL)
	{
		delete material;
		material = NULL;
	}
}

void MeshRenderer::Render(Shader* shader)
{
	shader->Bind();
	shader->UpdateUniforms(GetTransform().GetTransformation(), GetTransform().GetProjectedTransformation(), *material);
	mesh->Draw();
}