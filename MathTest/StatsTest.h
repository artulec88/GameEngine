#ifndef __MATH_TEST_STATS_TEST_H__
#define __MATH_TEST_STATS_TEST_H__

#include "Math\Statistics.h"
#include "Math\StatisticsStorage.h"
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
		void Method1();
		void Method2();
		void Method3();

	private:
		void NestedMethod4();
#ifdef PROFILING_MATH_TEST_MODULE_ENABLED
		Math::Statistics::ClassStats& m_classStats;
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
		void Method1();
		void Method2();
		void Method3();

	private:
		void NestedMethod4();
#ifdef PROFILING_MATH_TEST_MODULE_ENABLED
		Math::Statistics::ClassStats& m_classStats;
#endif
	};
	/* ==================== class StatsTest2 end ==================== */

} /* end namespace MathTest */

#endif /* __MATH_TEST_STATS_TEST_H__ */