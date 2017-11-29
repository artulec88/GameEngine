#ifndef __ENGINE_IUPDATEABLE_H__
#define __ENGINE_IUPDATEABLE_H__

#include "Engine.h"
#include "Math/Math.h"

namespace engine
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
		ENGINE_API virtual ~IUpdateable()
		{
		}

		/// <summary> Updateable object copy constructor. </summary>
		/// <param name="iUpdateable"> The reference to updatable object to copy construct from. </param>
		IUpdateable(const IUpdateable& iUpdateable) = delete;

		/// <summary> Updateable object move constructor. </summary>
		/// <param name="iUpdateable"> The r-value reference to updatable object to move construct from. </param>
		IUpdateable(IUpdateable&& iUpdateable) = default;

		/// <summary> Updateable object copy assignment operator. </summary>
		/// <param name="iUpdateable"> The reference to updatable object to copy assign from. </param>
		/// <returns> The reference to the newly copy-assigned updateable object. </returns>
		IUpdateable& operator=(const IUpdateable& iUpdateable) = delete;

		/// <summary> Updateable object move assignment operator. </summary>
		/// <param name="iUpdateable"> The r-value reference to updatable object to move assign from. </param>
		/// <returns> The reference to the newly move-assigned updateable object. </returns>
		IUpdateable& operator=(IUpdateable&& iUpdateable) = default;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
		/// <summary> Called when the updateable objects should advance its simulation state. </summary>
		/// <param name="elapsedTime"> Time by which to advance the simulation. </param>
		/// <remarks>
		/// In this call, the object should advance its own simulation according to the amount of elapsed time.
		/// </remarks>
		virtual void Update(math::Real elapsedTime) = 0;
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
		/* ==================== Non-static member variables end ==================== */
	}; /* end class IUpdateable */

} /* end namespace engine */

#endif /* __ENGINE_IUPDATEABLE_H__ */