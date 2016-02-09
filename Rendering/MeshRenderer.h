#ifndef __RENDERING_MESH_RENDERER_H__
#define __RENDERING_MESH_RENDERER_H__

#include "GameComponent.h"
#include "IRenderable.h"

namespace Rendering
{

class Mesh;
class Material;
class Shader;

// TODO: Inherit from IRenderable interface or use it in the Mesh class instead.
class MeshRenderer : public GameComponent, public IRenderable
{
/* ==================== Constructors and destructors begin ==================== */
public:
	RENDERING_API MeshRenderer(Mesh* mesh, Material* material);
	RENDERING_API virtual ~MeshRenderer(void);
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	RENDERING_API virtual void Render(Shader* shader, Renderer* renderer);
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
protected:
	Mesh* m_mesh;
	Material* m_material;
/* ==================== Non-static member variables end ==================== */
}; /* end class MeshRenderer */

} /* end namespace Rendering */

#endif /* __RENDERING_MESH_RENDERER_H__ */