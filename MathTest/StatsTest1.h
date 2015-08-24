#ifndef __MATH_TEST_STATS_TEST_1_H__
#define __MATH_TEST_STATS_TEST_1_H__

#include "Math\Statistics.h"
#include "Math\IStatisticsStorage.h"
#include "StatsTesting.h"

namespace MathTest
{

class StatsTest1
{
public:
	StatsTest1(void);
	~StatsTest1(void);
public:
	float Method1();
	float Method2();

private:
#ifdef CALCULATE_TEST_STATS
	mutable Math::Statistics::ClassStats& m_classStats;
#endif
}; /* end class StatsTest1 */

} /* end namespace MathTest */

#endif