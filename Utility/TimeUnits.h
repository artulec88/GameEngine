#ifndef __UTILITY_TIMING_TIME_UNITS_H__
#define __UTILITY_TIMING_TIME_UNITS_H__

namespace utility
{
	namespace timing
	{
		// TODO: Make this enum scoped.
		/// <summary> Possible time units. </summary>
		enum TimeUnit
		{
			HOUR = 0,
			MINUTE,
			SECOND,
			MILLISECOND,
			MICROSECOND,
			NANOSECOND,
			TIME_UNITS_COUNT, // the number of all possible date-time units
		}; /* end enum TimeUnit */
	} /* end namespace timing */
} /* end namespace utility */

#endif /* __UTILITY_TIMING_TIME_UNITS_H__ */