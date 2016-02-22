#ifndef __RENDERING_MESH_GROUP_RENDERER_H__
#define __RENDERING_MESH_GROUP_RENDERER_H__

#include "Rendering.h"
#include "Shader.h"
#include "Renderer.h"
#include "Math\Transform.h"

namespace Rendering
{

	class Mesh;
	class Material;
	class Shader;

	class MeshGroupRenderer
	{
	/* ==================== Constructors and destructors begin ==================== */
	public:
		RENDERING_API MeshGroupRenderer(Math::Transform& transform, Mesh* mesh, Material* material);
		RENDERING_API virtual ~MeshGroupRenderer(void);
	/* ==================== Constructors and destructors end ==================== */

	/* ==================== Non-static member functions begin ==================== */
	public:
		RENDERING_API virtual void Render(Shader* shader, Renderer* renderer);
		Math::Transform& GetTransform() { return m_transform; }
		const Math::Transform& GetTransform() const { return m_transform; }
	/* ==================== Non-static member functions end ==================== */

	/* ==================== Non-static member variables begin ==================== */
	protected:
		Math::Transform& m_transform;
		Mesh* m_mesh;
		Material* m_material;
	/* ==================== Non-static member variables end ==================== */
	}; /* end class MeshGroupRenderer */

} /* end namespace Rendering */

#endif /* __RENDERING_MESH_GROUP_RENDERER_H__ */