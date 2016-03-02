#ifndef __ENGINE_MESH_GROUP_RENDERER_COMPONENT_H__
#define __ENGINE_MESH_GROUP_RENDERER_COMPONENT_H__

#include "Engine.h"
#include "GameComponent.h"
#include "IRenderable.h"
#include "Rendering\Mesh.h"
#include "Rendering\Material.h"

namespace Engine
{
	class MeshGroupRendererComponent : public GameComponent, public IRenderable
	{
	/* ==================== Constructors and destructors begin ==================== */
	public:
		ENGINE_API MeshGroupRendererComponent(Rendering::Mesh* mesh, Rendering::Material* material);
		ENGINE_API virtual ~MeshGroupRendererComponent(void);
	/* ==================== Constructors and destructors end ==================== */

	/* ==================== Non-static member functions begin ==================== */
	public:
		virtual void Render(const Rendering::Shader* shader, Rendering::Renderer* renderer);
	/* ==================== Non-static member functions end ==================== */

	/* ==================== Non-static member variables begin ==================== */
	protected:
		Rendering::Mesh* m_mesh;
		Rendering::Material* m_material;
	/* ==================== Non-static member variables end ==================== */
	}; /* end class MeshGroupRendererComponent */

} /* end namespace Engine */

#endif /* __ENGINE_MESH_GROUP_RENDERER_COMPONENT_H__ */