#pragma once
#include "GameComponent.h"
#include "Mesh.h"
#include "Material.h"
#include "Shader.h"

namespace Rendering
{

class RENDERING_API MeshRenderer : public GameComponent
{
/* ==================== Constructors and destructors begin ==================== */
public:
	MeshRenderer(Mesh* mesh, Material* material);
	virtual ~MeshRenderer(void);
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	virtual void Render(Shader* shader);
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
protected:
	Mesh* mesh;
	Material* material;
/* ==================== Non-static member variables end ==================== */
}; /* end class MeshRenderer */

} /* end namespace Rendering */