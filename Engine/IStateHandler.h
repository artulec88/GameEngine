#ifndef __ENGINE_ISTATE_HANDLER_H__
#define __ENGINE_ISTATE_HANDLER_H__

#include "Engine.h"
#include "ActionConstants.h"
#include "InputConstants.h"

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
		ENGINE_API IStateHandler()
		{
		}
		ENGINE_API virtual ~IStateHandler(void)
		{
		}

		IStateHandler(const IStateHandler& iStateHandler) = delete;
		IStateHandler(IStateHandler&& iStateHandler) = default;
		IStateHandler& operator=(const IStateHandler& iStateHandler) = delete;
		IStateHandler& operator=(IStateHandler&& iStateHandler) = default;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		/// <summary>
		/// Handles the incoming state appropriately.
		/// </summary>
		/// <param name="state"> The state that must be handled. </param>
		ENGINE_API virtual void Handle(States::State state) = 0;
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	protected:
		/* ==================== Non-static member variables end ==================== */
	}; /* end class IStateHandler */

} /* end namespace Engine */

#endif // __ENGINE_ISTATE_HANDLER_H__