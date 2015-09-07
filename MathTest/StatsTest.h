#ifndef __MATH_TEST_STATS_TEST_H__
#define __MATH_TEST_STATS_TEST_H__

#include "Math\Statistics.h"
#include "Math\IStatisticsStorage.h"
#include "StatsTesting.h"

namespace MathTest
{

/* ==================== class StatsTest1 begin ==================== */
class StatsTest1
{
public:
	StatsTest1(void);
	~StatsTest1(void);
public:
	float Method1();
	float Method2();
	float Method3();

private:
	float NestedMethod4();
#ifdef CALCULATE_TEST_STATS
	mutable Math::Statistics::ClassStats& m_classStats;
#endif
};
/* ==================== class StatsTest1 end ==================== */

/* ==================== class StatsTest2 begin ==================== */
class StatsTest2
{
public:
	StatsTest2(void);
	~StatsTest2(void);
public:
	float Method1();
	float Method2();
	float Method3();

private:
	float NestedMethod4();
#ifdef CALCULATE_TEST_STATS
	mutable Math::Statistics::ClassStats& m_classStats;
#endif
};
/* ==================== class StatsTest2 end ==================== */

} /* end namespace MathTest */

#endif /* __MATH_TEST_STATS_TEST_H__ */