#ifndef __ENGINE_MESH_GROUP_RENDERER_COMPONENT_H__
#define __ENGINE_MESH_GROUP_RENDERER_COMPONENT_H__

#include "Engine.h"
#include "Rendering\GameComponent.h"
#include "Rendering\IRenderable.h"
#include "Rendering\MeshGroupRenderer.h"

namespace Engine
{
	class MeshGroupRendererComponent : public Rendering::GameComponent, public Rendering::IRenderable
	{
	/* ==================== Constructors and destructors begin ==================== */
	public:
		ENGINE_API MeshGroupRendererComponent(Rendering::MeshGroupRenderer* meshGroupRenderer);
		ENGINE_API virtual ~MeshGroupRendererComponent(void);
	/* ==================== Constructors and destructors end ==================== */

	/* ==================== Non-static member functions begin ==================== */
	public:
		virtual void Render(Rendering::Shader* shader, Rendering::Renderer* renderer);
	/* ==================== Non-static member functions end ==================== */

	/* ==================== Non-static member variables begin ==================== */
	protected:
		Rendering::MeshGroupRenderer* m_meshGroupRenderer;
	/* ==================== Non-static member variables end ==================== */
	}; /* end class MeshGroupRendererComponent */

} /* end namespace Engine */

#endif /* __ENGINE_MESH_GROUP_RENDERER_COMPONENT_H__ */