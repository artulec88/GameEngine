#pragma once
#include "GameComponent.h"

namespace Rendering
{

class Mesh;
class Material;
class Shader;

class RENDERING_API MeshRenderer : public GameComponent
{
/* ==================== Constructors and destructors begin ==================== */
public:
	MeshRenderer(Mesh* mesh, Material* material);
	virtual ~MeshRenderer(void);
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	virtual void Render(Shader* shader, Renderer* renderer);
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
protected:
	Mesh* mesh;
	Material* material;
/* ==================== Non-static member variables end ==================== */
}; /* end class MeshRenderer */

} /* end namespace Rendering */