#include "StatsTest.h"
#include "Math\Math.h"

/* ==================== class StatsTest1 begin ==================== */
MathTest::StatsTest1::StatsTest1(void)
#ifdef CALCULATE_TEST_STATS
	: m_classStats(STATS_STORAGE.GetClassStats("StatsTest1"))
#endif
{
}


MathTest::StatsTest1::~StatsTest1(void)
{
}

float MathTest::StatsTest1::Method1()
{
	START_PROFILING;
	// This method is just to test collecting statistics (see classes IStatisticsStorage, Statistics, etc.)
	const int ITERATIONS = 5000000;
	float floatValue = 0.1f;
	for (int i = 0; i < ITERATIONS; ++i)
	{
		floatValue -= static_cast<float>(i / 5);
		floatValue += static_cast<float>(i / 3);
		floatValue -= 1000.0f;
	}
	STOP_PROFILING;
	return floatValue;
}

float MathTest::StatsTest1::Method2()
{
	START_PROFILING;
	// This method is just to test collecting statistics (see classes IStatisticsStorage, Statistics, etc.)
	const int ITERATIONS = 1000000;
	float floatValue = -300.1f;
	for (int i = 0; i < ITERATIONS; ++i)
	{
		floatValue -= static_cast<float>(i / 4);
		floatValue += static_cast<float>(i / 2);
		floatValue -= 1000.0f;
	}
	STOP_PROFILING;
	return floatValue;
}

float MathTest::StatsTest1::Method3()
{
	START_PROFILING;
	// This method is just to test collecting statistics (see classes IStatisticsStorage, Statistics, etc.)
	const int ITERATIONS = 100000;
	float floatValue = -300.1f;
	for (int i = 0; i < ITERATIONS; ++i)
	{
		floatValue -= static_cast<float>(i / 4);
		floatValue += static_cast<float>(i / 2);
		floatValue -= 1000.0f;
		if (i % 6 == 0)
		{
			floatValue += NestedMethod4();
		}
	}
	STOP_PROFILING;
	return floatValue;
}

float MathTest::StatsTest1::NestedMethod4()
{
	START_PROFILING;
	// This method is just to test collecting statistics (see classes IStatisticsStorage, Statistics, etc.)
	const int ITERATIONS = 10000;
	float floatValue = 500.0f;
	for (int i = 0; i < ITERATIONS; ++i)
	{
		floatValue -= static_cast<float>(i / 4);
		floatValue += static_cast<float>(i / 2);
		floatValue -= 1000.0f;
	}
	STOP_PROFILING;
	return floatValue;
}
/* ==================== class StatsTest1 end ==================== */

/* ==================== class StatsTest2 begin ==================== */
MathTest::StatsTest2::StatsTest2(void)
#ifdef CALCULATE_TEST_STATS
	: m_classStats(STATS_STORAGE.GetClassStats("StatsTest2"))
#endif
{
}


MathTest::StatsTest2::~StatsTest2(void)
{
}

float MathTest::StatsTest2::Method1()
{
	START_PROFILING;
	// This method is just to test collecting statistics (see classes IStatisticsStorage, Statistics, etc.)
	const int ITERATIONS = 20000;
	float floatValue = 0.1f;
	for (int i = 0; i < ITERATIONS; ++i)
	{
		if (i % 10 == 0)
		{
			floatValue += Method2();
		}
		if (i % 200 == 0)
		{
			floatValue -= Method3();
		}
		if (i % 1000 == 0)
		{
			floatValue -= NestedMethod4();
		}
		floatValue -= static_cast<float>(i / 5);
		floatValue += static_cast<float>(i / 3);
		floatValue -= 1000.0f;
	}
	STOP_PROFILING;
	return floatValue;
}

float MathTest::StatsTest2::Method2()
{
	START_PROFILING;
	// This method is just to test collecting statistics (see classes IStatisticsStorage, Statistics, etc.)
	const int ITERATIONS = 1000;
	float floatValue = -300.1f;
	for (int i = 0; i < ITERATIONS; ++i)
	{
		floatValue -= static_cast<float>(i / 4);
		floatValue += static_cast<float>(i / 2);
		floatValue -= 1000.0f;
	}
	STOP_PROFILING;
	return floatValue;
}

float MathTest::StatsTest2::Method3()
{
	START_PROFILING;
	// This method is just to test collecting statistics (see classes IStatisticsStorage, Statistics, etc.)
	const int ITERATIONS = 1000;
	float floatValue = -300.1f;
	for (int i = 0; i < ITERATIONS; ++i)
	{
		floatValue -= static_cast<float>(i / 4);
		floatValue += static_cast<float>(i / 2);
		floatValue -= 1000.0f;
		if (i % 5 == 0)
		{
			floatValue += NestedMethod4();
		}
	}
	STOP_PROFILING;
	return floatValue;
}

float MathTest::StatsTest2::NestedMethod4()
{
	START_PROFILING;
	// This method is just to test collecting statistics (see classes IStatisticsStorage, Statistics, etc.)
	const int ITERATIONS = 10000;
	float floatValue = 500.0f;
	for (int i = 0; i < ITERATIONS; ++i)
	{
		floatValue -= static_cast<float>(i / 4);
		floatValue += static_cast<float>(i / 2);
		floatValue -= 1000.0f;
	}
	STOP_PROFILING;
	return floatValue;
}
/* ==================== class StatsTest2 end ==================== */