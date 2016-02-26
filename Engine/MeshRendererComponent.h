#ifndef __ENGINE_MESH_RENDERER_COMPONENT_H__
#define __ENGINE_MESH_RENDERER_COMPONENT_H__

#include "Engine.h"
#include "GameComponent.h"
#include "IRenderable.h"
#include "Rendering\Mesh.h"
#include "Rendering\Material.h"

namespace Engine
{

	class MeshRendererComponent : public GameComponent, public IRenderable
	{
	/* ==================== Constructors and destructors begin ==================== */
	public:
		ENGINE_API MeshRendererComponent(Rendering::Mesh* mesh, Rendering::Material* material);
		ENGINE_API virtual ~MeshRendererComponent(void);
	/* ==================== Constructors and destructors end ==================== */

	/* ==================== Non-static member functions begin ==================== */
	public:
		virtual void Render(Rendering::Shader* shader, Rendering::Renderer* renderer) const;
	/* ==================== Non-static member functions end ==================== */

	/* ==================== Non-static member variables begin ==================== */
	protected:
		Rendering::Mesh* m_mesh;
		Rendering::Material* m_material;
	/* ==================== Non-static member variables end ==================== */
	}; /* end class MeshRendererComponent */

} /* end namespace Engine */

#endif /* __ENGINE_MESH_RENDERER_COMPONENT_H__ */