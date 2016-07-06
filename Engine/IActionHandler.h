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
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		ENGINE_API virtual void Handle(Actions::Action action) = 0;
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	protected:
		/* ==================== Non-static member variables end ==================== */
	}; /* end class IActionHandler */

} /* end namespace Engine */

#endif // __ENGINE_IACTION_HANDLER_H__