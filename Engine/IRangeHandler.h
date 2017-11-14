#ifndef __ENGINE_IRANGE_HANDLER_H__
#define __ENGINE_IRANGE_HANDLER_H__

#include "Engine.h"
#include "ActionConstants.h"
#include "InputConstants.h"

#include "Math\Math.h"

namespace engine
{
	/// <summary>
	/// Interface for handlers of specific game ranges.
	/// </summary>
	class IRangeHandler
	{
		/* ==================== Static variables and functions begin ==================== */
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		ENGINE_API IRangeHandler()
		{
		}
		ENGINE_API virtual ~IRangeHandler(void)
		{
		}

		IRangeHandler(const IRangeHandler& iRangeHandler) = delete;
		IRangeHandler(IRangeHandler&& iRangeHandler) = default;
		IRangeHandler& operator=(const IRangeHandler& iRangeHandler) = delete;
		IRangeHandler& operator=(IRangeHandler&& iRangeHandler) = default;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		/// <summary>
		/// Handles the incoming range appropriately.
		/// </summary>
		/// <param name="range"> The range that must be handled. </param>
		/// <param name="value"> The value associated with the range. </param>
		ENGINE_API virtual void Handle(Ranges::Range range, Math::Real value) = 0;
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	protected:
		/* ==================== Non-static member variables end ==================== */
	}; /* end class IRangeHandler */

} /* end namespace Engine */

#endif // __ENGINE_IRANGE_HANDLER_H__