#pragma once
#include "GameComponent.h"

namespace Rendering
{

class Mesh;
class Material;
class Shader;

// TODO: Inherit from IRenderable interface or use it in the Mesh class instead.
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
	Mesh* m_mesh;
	Material* m_material;
/* ==================== Non-static member variables end ==================== */
}; /* end class MeshRenderer */

} /* end namespace Rendering */