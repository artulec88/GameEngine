#ifndef __MATH_UTMOST_TIME_SAMPLES_H__
#define __MATH_UTMOST_TIME_SAMPLES_H__

#include "Math.h"
#include "Utility\Time.h"
#include <string>

namespace Math { namespace Statistics
{

/// <summary>The class responsible for maintaining the list of utmost observations in the time samples.</summary>
/// <remarks>
///	For a given time samples: 0.1s, 0.2s, 0.5s, 1.0s, 2.0s, 5.0s, 10.0s, 20.0s, 50.0s and number of samples equal to 3
/// the object will store 3 minimum and 3 maximum samples. In this case it will be (0.1s, 0.2s, 0.5s) for minimum observations and (10.0s, 20.0s, 50.0s) for maximum observations.
/// </remarks>
class UtmostTimeSamples
{
/* ==================== Static variables and functions begin ==================== */
private:
	static const int NUMBER_OF_SAMPLES = 3;
/* ==================== Static variables and functions end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	MATH_API UtmostTimeSamples();
	MATH_API ~UtmostTimeSamples();
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	MATH_API void ProcessTime(const Utility::Timing::TimeSpan& timeSpan);
	MATH_API std::string ToString();
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
private:
	Utility::Timing::TimeSpan m_minTime[NUMBER_OF_SAMPLES];
	Utility::Timing::TimeSpan m_maxTime[NUMBER_OF_SAMPLES];
	/* ==================== Non-static member variables end ==================== */
}; /* end class UtmostTimeSamples */

} /* end namespace Statistics */

} /* end namespace Math */

#endif /* __MATH_UTMOST_TIME_SAMPLES_H__ */