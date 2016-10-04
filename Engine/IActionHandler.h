#ifndef __ENGINE_IACTION_HANDLER_H__
#define __ENGINE_IACTION_HANDLER_H__

#include "Engine.h"
#include "ActionConstants.h"
#include "InputConstants.h"

namespace Engine
{
	/// <summary>
	/// Interface for handlers of specific game actions.
	/// </summary>
	class IActionHandler
	{
		/* ==================== Static variables and functions begin ==================== */
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		ENGINE_API IActionHandler()
		{
		}
		ENGINE_API virtual ~IActionHandler(void)
		{
		}
		IActionHandler(const IActionHandler& actionHandler) = delete;
		ENGINE_API IActionHandler(IActionHandler&& actionHandler) = default;
		IActionHandler& operator=(const IActionHandler& actionHandler) = delete;
		ENGINE_API IActionHandler& operator=(IActionHandler&& actionHandler) = default;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		///// <summary>
		///// Registers to get notifications about specific actions.
		///// </summary>
		//ENGINE_API virtual void Register() = 0;

		/// <summary>
		/// Handles the incoming action appropriately.
		/// </summary>
		/// <param name="action"> The action that must be handled. </param>
		ENGINE_API virtual void Handle(Actions::Action action) = 0;
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	protected:
		/* ==================== Non-static member variables end ==================== */
	}; /* end class IActionHandler */

} /* end namespace Engine */

#endif // __ENGINE_IACTION_HANDLER_H__