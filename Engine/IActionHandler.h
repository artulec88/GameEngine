#ifndef __ENGINE_IACTION_HANDLER_H__
#define __ENGINE_IACTION_HANDLER_H__

#include "Engine.h"
#include "ActionConstants.h"

namespace engine
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
		/// <summary> Action handler default constructor. </summary>
		ENGINE_API IActionHandler()
		{
		}

		/// <summary> Action handler destructor. </summary>
		ENGINE_API virtual ~IActionHandler()
		{
		}

		/// <summary> Action handler copy constructor. </summary>
		/// <param name="actionHandler"> The reference to action handler to copy construct from. </param>
		IActionHandler(const IActionHandler& actionHandler) = delete;

		/// <summary> Action handler move constructor. </summary>
		/// <param name="actionHandler"> The r-value reference to action handler to move construct from. </param>
		ENGINE_API IActionHandler(IActionHandler&& actionHandler) = default;

		/// <summary> Action handler copy assignment operator. </summary>
		/// <param name="actionHandler"> The reference to action handler to copy assign from. </param>
		/// <returns> The reference to the newly copy-assigned action handler. </returns>
		IActionHandler& operator=(const IActionHandler& actionHandler) = delete;

		/// <summary> Action handler move assignment operator. </summary>
		/// <param name="actionHandler"> The r-value reference to action handler to move assign from. </param>
		/// <returns> The reference to the newly move-assigned action handler. </returns>
		ENGINE_API IActionHandler& operator=(IActionHandler&& actionHandler) = default;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
		///// <summary>
		///// Registers to get notifications about specific actions.
		///// </summary>
		//ENGINE_API virtual void Register() = 0;

		/// <summary>
		/// Handles the incoming action appropriately.
		/// </summary>
		/// <param name="action"> The action that must be handled. </param>
		ENGINE_API virtual void Handle(actions::Action action) = 0;
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
		/* ==================== Non-static member variables end ==================== */
	}; /* end class IActionHandler */

} /* end namespace engine */

#endif // __ENGINE_IACTION_HANDLER_H__