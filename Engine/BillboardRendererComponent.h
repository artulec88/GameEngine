#ifndef __ENGINE_BILLBOARDS_RENDERER_COMPONENT_H__
#define __ENGINE_BILLBOARDS_RENDERER_COMPONENT_H__

#include "Engine.h"
#include "GameComponent.h"
#include "IRenderable.h"
#include "Rendering\Mesh.h"
#include "Rendering\Material.h"

namespace engine
{

	class BillboardsRendererComponent : public GameComponent, public IRenderable
	{
	/* ==================== Constructors and destructors begin ==================== */
	public:
		ENGINE_API BillboardsRendererComponent(int billboardMeshID, rendering::Material* billboardMaterial);
		ENGINE_API virtual ~BillboardsRendererComponent(void);
		BillboardsRendererComponent(const BillboardsRendererComponent& billboardsRendererComponent) = delete;
		ENGINE_API BillboardsRendererComponent(BillboardsRendererComponent&& billboardsRendererComponent);
		BillboardsRendererComponent& operator=(const BillboardsRendererComponent& billboardsRendererComponent) = delete;
		ENGINE_API BillboardsRendererComponent& operator=(BillboardsRendererComponent&& billboardsRendererComponent);
	/* ==================== Constructors and destructors end ==================== */

	/* ==================== Non-static member functions begin ==================== */
	public:
		virtual void Render(int shaderID, rendering::Renderer* renderer) const override;
	/* ==================== Non-static member functions end ==================== */

	/* ==================== Non-static member variables begin ==================== */
	protected:
		int m_billboardMeshID;
		rendering::Material* m_billboardMaterial;
	/* ==================== Non-static member variables end ==================== */
	}; /* end class BillboardRendererComponent */

} /* end namespace Engine */

#endif /* __ENGINE_BILLBOARDS_RENDERER_COMPONENT_H__ */