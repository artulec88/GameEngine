#ifndef __ENGINE_IUPDATEABLE_H__
#define __ENGINE_IUPDATEABLE_H__

#include "Engine.h"
#include "Math\Math.h"

namespace Engine
{
	/// <summary>
	/// Interface for objects which can be updated in the game engine.
	/// </summary>
	class IUpdateable
	{
	/* ==================== Constructors and destructors begin ==================== */
	public:
		/// <summary> Creates the updateable object. </summary>
		ENGINE_API IUpdateable()
		{
		}

		/// <summary> Destroys the updateable object. </summary>
		ENGINE_API virtual ~IUpdateable(void)
		{
		}
	/* ==================== Constructors and destructors end ==================== */

	/* ==================== Non-static member functions begin ==================== */
	public:
		/// <summary> Called when the updateable objects should advance its simulation state. </summary>
		/// <param name="elapsedTime"> Time by which to advance the simulation. </param>
		/// <remarks>
		/// In this call, the object should advance its own simulation according to the amount of elapsed time.
		/// </remarks>
		virtual void Update(Math::Real elapsedTime) = 0;
	/* ==================== Non-static member functions end ==================== */

	/* ==================== Non-static member variables begin ==================== */
	/* ==================== Non-static member variables end ==================== */
}; /* end class IUpdateable */

} /* end namespace Engine */

#endif /* __ENGINE_IUPDATEABLE_H__ */