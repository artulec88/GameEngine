#ifndef __ENGINE_IRENDERABLE_H__
#define __ENGINE_IRENDERABLE_H__

#include "Engine.h"

#include "Rendering/Renderer.h"

namespace engine
{
	/// <summary>
	/// Interface for objects which can be rendered in the game engine.
	/// </summary>
	class IRenderable
	{
		/* ==================== Constructors and destructors begin ==================== */
	public:
		/// <summary> Creates the renderable object. </summary>
		ENGINE_API IRenderable()
		{
		}

		/// <summary> Destroys the renderable object. </summary>
		ENGINE_API virtual ~IRenderable()
		{
		}

		/// <summary> Renderable object copy constructor. </summary>
		/// <param name="iRenderable"> The reference to renderable object to copy construct from. </param>
		IRenderable(const IRenderable& iRenderable) = delete;

		/// <summary> Renderable object move constructor. </summary>
		/// <param name="iRenderable"> The r-value reference to renderable object to move construct from. </param>
		IRenderable(IRenderable&& iRenderable) = default;

		/// <summary> Renderable object copy assignment operator. </summary>
		/// <param name="iRenderable"> The reference to renderable object to copy assign from. </param>
		/// <returns> The reference to the newly copy-assigned renderable object. </returns>
		IRenderable& operator=(const IRenderable& iRenderable) = delete;

		/// <summary> Renderable object move assignment operator. </summary>
		/// <param name="iRenderable"> The r-value reference to renderable object to move assign from. </param>
		/// <returns> The reference to the newly move-assigned renderable object. </returns>
		IRenderable& operator=(IRenderable&& iRenderable) = default;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
			/// <summary>
			/// Issues the render call to the specific <paramref name="renderer"/> using the given <paramref name="shader"/>.
			/// </summary>
			/// <param name="shaderID"> The ID of the shader to be used by <paramref name="renderer"/>. </param>
			/// <param name="renderer"> The rendering engine to handle the rendering call. </param>
		ENGINE_API virtual void Render(int shaderId, rendering::Renderer* renderer) const = 0;
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
		/* ==================== Non-static member variables end ==================== */
	}; /* end class IRenderable */

} /* end namespace engine */

#endif /* __ENGINE_IRENDERABLE_H__ */