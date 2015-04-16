#ifndef __MATH_STATISTICS_H__
#define __MATH_STATISTICS_H__

#include "Math.h"
#include <map>
#include <vector>
#include "Utility\ILogger.h"

namespace Math { namespace Statistics
{

enum MATH_API StatsID
{
	SPF = 0
};

template <typename T>
class Stats
{
/* ==================== Constructors and destructors begin ==================== */
public:
	MATH_API Stats();
	
	MATH_API ~Stats(void);
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	MATH_API void Push(StatsID statsID, T sample);
	MATH_API int Size() const;
	MATH_API int Size(StatsID statsID) const;

	MATH_API T CalculateMean(StatsID statsID) const;
	MATH_API T CalculateMedian(StatsID statsID);
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
private:
	std::map<StatsID, std::vector<T>> m_samples;
/* ==================== Non-static member variables end ==================== */
}; /* end class Stats */

} /* end namespace Statistics */

} /* end namespace Math */

#endif // __MATH_STATISTICS_H__