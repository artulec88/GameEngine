#ifndef __MATH_TEST_STATS_TESTING_H__
#define __MATH_TEST_STATS_TESTING_H__

#define CALCULATE_TEST_STATS

#ifdef CALCULATE_TEST_STATS
#undef START_PROFILING_STATIC
#undef STOP_PROFILING_STATIC
#undef START_PROFILING
#undef STOP_PROFILING

#define START_PROFILING_STATIC(param) do { s_classStats.StartProfiling(__FUNCTION__##param); } while (0)
#define STOP_PROFILING_STATIC(param) do { s_classStats.StopProfiling(__FUNCTION__##param); } while (0)
#define START_PROFILING(param) do { m_classStats.StartProfiling(__FUNCTION__##param); } while (0)
#define STOP_PROFILING(param) do { m_classStats.StopProfiling(__FUNCTION__##param); } while (0)
#else
#define START_PROFILING_STATIC(param)
#define STOP_PROFILING_STATIC(param)
#define START_PROFILING(param)
#define STOP_PROFILING(param)
#endif

#endif