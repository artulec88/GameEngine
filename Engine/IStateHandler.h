#ifndef __ENGINE_ISTATE_HANDLER_H__
#define __ENGINE_ISTATE_HANDLER_H__

#include "Engine.h"
#include "ActionConstants.h"

namespace engine
{
	/// <summary>
	/// Interface for handlers of specific game states.
	/// </summary>
	class IStateHandler
	{
		/* ==================== Static variables and functions begin ==================== */
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		/// <summary> State handler default constructor. </summary>
		ENGINE_API IStateHandler()
		{
		}

		/// <summary> State handler destructor. </summary>
		ENGINE_API virtual ~IStateHandler()
		{
		}

		/// <summary> State handler copy constructor. </summary>
		/// <param name="iStateHandler"> The reference to state handler to copy construct from. </param>
		IStateHandler(const IStateHandler& iStateHandler) = delete;

		/// <summary> State handler move constructor. </summary>
		/// <param name="iStateHandler"> The r-value reference to state handler to move construct from. </param>
		IStateHandler(IStateHandler&& iStateHandler) = default;

		/// <summary> State handler copy assignment operator. </summary>
		/// <param name="iStateHandler"> The reference to state handler to copy assign from. </param>
		/// <returns> The reference to the newly copy-assigned state handler. </returns>
		IStateHandler& operator=(const IStateHandler& iStateHandler) = delete;

		/// <summary> State handler move assignment operator. </summary>
		/// <param name="iStateHandler"> The r-value reference to state handler to move assign from. </param>
		/// <returns> The reference to the newly move-assigned state handler. </returns>
		IStateHandler& operator=(IStateHandler&& iStateHandler) = default;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
		/// <summary>
		/// Handles the incoming state appropriately.
		/// </summary>
		/// <param name="state"> The state that must be handled. </param>
		ENGINE_API virtual void Handle(states::State state) = 0;
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
		/* ==================== Non-static member variables end ==================== */
	}; /* end class IStateHandler */

} /* end namespace engine */

#endif // __ENGINE_ISTATE_HANDLER_H__