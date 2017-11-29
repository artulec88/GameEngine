#ifndef __ENGINE_IRANGE_HANDLER_H__
#define __ENGINE_IRANGE_HANDLER_H__

#include "Engine.h"
#include "ActionConstants.h"

#include "Math/Math.h"

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
		/// <summary> Range handler default constructor. </summary>
		ENGINE_API IRangeHandler()
		{
		}

		/// <summary> Range handler destructor. </summary>
		ENGINE_API virtual ~IRangeHandler()
		{
		}

		/// <summary> Range handler copy constructor. </summary>
		/// <param name="iRangeHandler"> The reference to range handler to copy construct from. </param>
		IRangeHandler(const IRangeHandler& iRangeHandler) = delete;

		/// <summary> Range handler move constructor. </summary>
		/// <param name="iRangeHandler"> The r-value reference to range handler to move construct from. </param>
		IRangeHandler(IRangeHandler&& iRangeHandler) = default;

		/// <summary> Range handler copy assignment operator. </summary>
		/// <param name="iRangeHandler"> The reference to range handler to copy assign from. </param>
		/// <returns> The reference to the newly copy-assigned range handler. </returns>
		IRangeHandler& operator=(const IRangeHandler& iRangeHandler) = delete;

		/// <summary> Range handler move assignment operator. </summary>
		/// <param name="iRangeHandler"> The r-value reference to range handler to move assign from. </param>
		/// <returns> The reference to the newly move-assigned range handler. </returns>
		IRangeHandler& operator=(IRangeHandler&& iRangeHandler) = default;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
		/// <summary>
		/// Handles the incoming range appropriately.
		/// </summary>
		/// <param name="range"> The range that must be handled. </param>
		/// <param name="value"> The value associated with the range. </param>
		ENGINE_API virtual void Handle(ranges::Range range, math::Real value) = 0;
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
		/* ==================== Non-static member variables end ==================== */
	}; /* end class IRangeHandler */

} /* end namespace engine */

#endif // __ENGINE_IRANGE_HANDLER_H__