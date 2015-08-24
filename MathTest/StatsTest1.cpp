#include "StatsTest1.h"
#include "Math\Math.h"

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