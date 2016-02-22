#ifndef __ENGINE_MESH_RENDERER_COMPONENT_H__
#define __ENGINE_MESH_RENDERER_COMPONENT_H__

#include "Engine.h"
#include "Rendering\GameComponent.h"
#include "Rendering\IRenderable.h"
#include "Rendering\MeshRenderer.h"

namespace Engine
{

	class MeshRendererComponent : public Rendering::GameComponent, public Rendering::IRenderable
	{
	/* ==================== Constructors and destructors begin ==================== */
	public:
		ENGINE_API MeshRendererComponent(Rendering::MeshRenderer* meshRenderer);
		ENGINE_API virtual ~MeshRendererComponent(void);
	/* ==================== Constructors and destructors end ==================== */

	/* ==================== Non-static member functions begin ==================== */
	public:
		virtual void Render(Rendering::Shader* shader, Rendering::Renderer* renderer) const;
	/* ==================== Non-static member functions end ==================== */

	/* ==================== Non-static member variables begin ==================== */
	protected:
		Rendering::MeshRenderer* m_meshRenderer;
	/* ==================== Non-static member variables end ==================== */
	}; /* end class MeshRendererComponent */

} /* end namespace Engine */

#endif /* __ENGINE_MESH_RENDERER_COMPONENT_H__ */