#ifndef __MATH_ISTATISTICS_STORAGE_H__
#define __MATH_ISTATISTICS_STORAGE_H__

#include "Math.h"
#include "Statistics.h"
#include <map>
#include <string>

#define STATS_STORAGE Math::Statistics::IStatisticsStorage::GetStatisticsStorage()

namespace Math { namespace Statistics
{
/**
 * TODO: The Math library should not know anything (even class names) about the classes it provides statistics for.
 * Some other solution should be given.
 */
//enum MATH_API StatsClassID
//{
//	CORE_ENGINE = 0,
//	RENDERER
//};

class IStatisticsStorage
{
/* ==================== Static variables and functions begin ==================== */
protected:
	static IStatisticsStorage* statisticsStorage;
public:
	MATH_API static IStatisticsStorage& GetStatisticsStorage();
/* ==================== Static variables and functions end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	IStatisticsStorage();
	~IStatisticsStorage(void);
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	MATH_API ClassStats& GetClassStats(const char* className);

	MATH_API void PrintSimpleReport() const;
	MATH_API void PrintReport(Math::Real totalElapsedSeconds) const;
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
protected:
	std::map<const char*, ClassStats*> m_classStatistics;
/* ==================== Non-static member variables end ==================== */
}; /* end class IStatisticsStorage */

} /* end namespace Statistics */

} /* end namespace Math */

#endif // __MATH_ISTATISTICS_STORAGE_H__