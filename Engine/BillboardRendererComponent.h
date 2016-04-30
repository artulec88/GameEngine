#ifndef __ENGINE_BILLBOARDS_RENDERER_COMPONENT_H__
#define __ENGINE_BILLBOARDS_RENDERER_COMPONENT_H__

#include "Engine.h"
#include "GameComponent.h"
#include "IRenderable.h"
#include "Rendering\Mesh.h"
#include "Rendering\Material.h"

namespace Engine
{

	class BillboardsRendererComponent : public GameComponent, public IRenderable
	{
	/* ==================== Constructors and destructors begin ==================== */
	public:
		ENGINE_API BillboardsRendererComponent(Rendering::BillboardMesh* billboardMesh, Rendering::Material* billboardMaterial);
		ENGINE_API virtual ~BillboardsRendererComponent(void);
	/* ==================== Constructors and destructors end ==================== */

	/* ==================== Non-static member functions begin ==================== */
	public:
		virtual void Render(const Rendering::Shader* shader, Rendering::Renderer* renderer) const;
	/* ==================== Non-static member functions end ==================== */

	/* ==================== Non-static member variables begin ==================== */
	protected:
		Rendering::BillboardMesh* m_billboardMesh;
		Rendering::Material* m_billboardMaterial;
	/* ==================== Non-static member variables end ==================== */
	}; /* end class BillboardRendererComponent */

} /* end namespace Engine */

#endif /* __ENGINE_BILLBOARDS_RENDERER_COMPONENT_H__ */