#ifndef __ENGINE_MESH_RENDERER_COMPONENT_H__
#define __ENGINE_MESH_RENDERER_COMPONENT_H__

#include "Engine.h"
#include "GameComponent.h"
#include "IRenderable.h"
#include "Rendering\Mesh.h"
#include "Rendering\Material.h"

namespace engine
{
	/// <summary>
	/// Mesh rendering component.
	/// </summary>
	class MeshRendererComponent : public GameComponent, public IRenderable
	{
		/* ==================== Constructors and destructors begin ==================== */
	public:
		/// <summary> Mesh rendering component constructor. </summary>
		/// <param name="meshID"> The ID of the mesh this game component will render. </param>
		/// <param name="material"> The pointer to the material which will be applied on the mesh during rendering. </param>
		ENGINE_API MeshRendererComponent(int meshID, const Rendering::Material* material);

		/// <summary> Mesh rendering component destructor. </summary>
		ENGINE_API virtual ~MeshRendererComponent(void);

		/// <summary> Mesh rendering component copy constructor. </summary>
		/// <param name="meshRendererComponent"> The mesh rendering component to copy construct from. </param>
		MeshRendererComponent(const MeshRendererComponent& meshRendererComponent) = delete;

		/// <summary> Mesh rendering component move constructor. </summary>
		/// <param name="meshRendererComponent"> The mesh rendering component to move construct from. </param>
		MeshRendererComponent(MeshRendererComponent&& meshRendererComponent);

		/// <summary> Mesh rendering component copy assignment operator. </summary>
		/// <param name="meshRendererComponent"> The mesh rendering component to copy assign from. </param>
		/// <returns> The reference to the newly copy-assigned mesh rendering component. </returns>
		MeshRendererComponent& operator=(const MeshRendererComponent& meshRendererComponent) = delete;

		/// <summary> Mesh rendering component move assignment operator. </summary>
		/// <param name="meshRendererComponent"> The mesh rendering component to move assign from. </param>
		/// <returns> The reference to the newly move-assigned mesh rendering component. </returns>
		MeshRendererComponent& operator=(MeshRendererComponent&& meshRendererComponent);
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		virtual void Render(int shaderID, Rendering::Renderer* renderer) const override;
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	protected:
		int m_meshID;
		const Rendering::Material* m_material;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class MeshRendererComponent */

} /* end namespace Engine */

#endif /* __ENGINE_MESH_RENDERER_COMPONENT_H__ */