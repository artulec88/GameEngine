#ifndef __ENGINE_IRENDERABLE_H__
#define __ENGINE_IRENDERABLE_H__

#include "Engine.h"

#include "Rendering\Shader.h"
#include "Rendering\Renderer.h"

#include "Math\Math.h"

namespace Engine
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
		ENGINE_API virtual ~IRenderable(void)
		{
		}
	/* ==================== Constructors and destructors end ==================== */

	/* ==================== Non-static member functions begin ==================== */
	public:
		ENGINE_API virtual void Render(const Rendering::Shader& shader, Rendering::Renderer* renderer) const = 0;
	/* ==================== Non-static member functions end ==================== */

	/* ==================== Non-static member variables begin ==================== */
	/* ==================== Non-static member variables end ==================== */
	}; /* end class IRenderable */

} /* end namespace Engine */

#endif /* __ENGINE_IRENDERABLE_H__ */