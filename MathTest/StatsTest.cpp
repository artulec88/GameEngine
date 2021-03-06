#include "StatsTest.h"
#include "Math/Math.h"

#include <thread>

/* ==================== class StatsTest1 begin ==================== */
math_test::StatsTest1::StatsTest1()
#ifdef PROFILING_MATH_TEST_MODULE_ENABLED
	: m_classStats(STATS_STORAGE.GetClassStats("StatsTest1"))
#endif
{
}


math_test::StatsTest1::~StatsTest1()
{
}

void math_test::StatsTest1::Method1()
{
	START_PROFILING_MATH_TEST(true, "");
	// This method is just to test collecting statistics (see classes StatisticsStorage, Statistics, etc.)
	std::this_thread::sleep_for(std::chrono::milliseconds(200));
	STOP_PROFILING_MATH_TEST("");
}

void math_test::StatsTest1::Method2()
{
	START_PROFILING_MATH_TEST(true, "");
	// This method is just to test collecting statistics (see classes StatisticsStorage, Statistics, etc.)
	std::this_thread::sleep_for(std::chrono::milliseconds(30));
	STOP_PROFILING_MATH_TEST("");
}

void math_test::StatsTest1::Method3()
{
	START_PROFILING_MATH_TEST(true, "");
	// This method is just to test collecting statistics (see classes StatisticsStorage, Statistics, etc.)
	std::this_thread::sleep_for(std::chrono::milliseconds(2));
	const int ITERATIONS = 10;
	for (int i = 0; i < ITERATIONS; ++i)
	{
		NestedMethod4();
	}
	STOP_PROFILING_MATH_TEST("");
}

void math_test::StatsTest1::NestedMethod4()
{
	START_PROFILING_MATH_TEST(true, "");
	// This method is just to test collecting statistics (see classes StatisticsStorage, Statistics, etc.)
	std::this_thread::sleep_for(std::chrono::microseconds(3000));
	STOP_PROFILING_MATH_TEST("");
}
/* ==================== class StatsTest1 end ==================== */

/* ==================== class StatsTest2 begin ==================== */
math_test::StatsTest2::StatsTest2()
#ifdef PROFILING_MATH_TEST_MODULE_ENABLED
	: m_classStats(STATS_STORAGE.GetClassStats("StatsTest2"))
#endif
{
}


math_test::StatsTest2::~StatsTest2()
{
}

void math_test::StatsTest2::Method1()
{
	START_PROFILING_MATH_TEST(true, "");
	// This method is just to test collecting statistics (see classes StatisticsStorage, Statistics, etc.)
	const int ITERATIONS = 200;
	std::this_thread::sleep_for(std::chrono::microseconds(200));
	for (int i = 0; i < ITERATIONS; ++i)
	{
		if (i % 10 == 0)
		{
			Method2();
		}
		if (i % 200 == 0)
		{
			Method3();
		}
		if (i % 1000 == 0)
		{
			NestedMethod4();
		}
	}
	STOP_PROFILING_MATH_TEST("");
}

void math_test::StatsTest2::Method2()
{
	START_PROFILING_MATH_TEST(true, "");
	// This method is just to test collecting statistics (see classes StatisticsStorage, Statistics, etc.)
	std::this_thread::sleep_for(std::chrono::microseconds(200));
	STOP_PROFILING_MATH_TEST("");
}

void math_test::StatsTest2::Method3()
{
	START_PROFILING_MATH_TEST(true, "");
	// This method is just to test collecting statistics (see classes StatisticsStorage, Statistics, etc.)
	std::this_thread::sleep_for(std::chrono::microseconds(10));
	const int ITERATIONS = 10;
	for (int i = 0; i < ITERATIONS; ++i)
	{
		NestedMethod4();
	}
	STOP_PROFILING_MATH_TEST("");
}

void math_test::StatsTest2::NestedMethod4()
{
	START_PROFILING_MATH_TEST(true, "");
	// This method is just to test collecting statistics (see classes StatisticsStorage, Statistics, etc.)
	std::this_thread::sleep_for(std::chrono::microseconds(200));
	STOP_PROFILING_MATH_TEST("");
}
/* ==================== class StatsTest2 end ==================== */