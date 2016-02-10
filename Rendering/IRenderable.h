#ifndef __RENDERING_IRENDERABLE_H__
#define __RENDERING_IRENDERABLE_H__

#include "Rendering.h"
#include "Math\Math.h"

namespace Rendering
{
	class Shader;
	class Renderer;

	/// <summary>
	/// Interface for objects which can be rendered in the game engine.
	/// </summary>
	class IRenderable
	{
	/* ==================== Constructors and destructors begin ==================== */
	public:
		/// <summary> Creates the renderable object. </summary>
		RENDERING_API IRenderable()
		{
		}

		/// <summary> Destroys the renderable object. </summary>
		RENDERING_API virtual ~IRenderable(void)
		{
		}
	/* ==================== Constructors and destructors end ==================== */

	/* ==================== Non-static member functions begin ==================== */
	public:
		virtual void Render(Shader* shader, Renderer* renderer) const = 0;
	/* ==================== Non-static member functions end ==================== */

	/* ==================== Non-static member variables begin ==================== */
	/* ==================== Non-static member variables end ==================== */
	}; /* end class IRenderable */

} /* end namespace Rendering */

#endif /* __RENDERING_IRENDERABLE_H__ */