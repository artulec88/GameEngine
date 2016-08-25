#ifndef __MATH_TEST_STATS_TESTING_H__
#define __MATH_TEST_STATS_TESTING_H__

#define PROFILING_MATH_TEST_MODULE_ENABLED

#ifdef PROFILING_MATH_TEST_MODULE_ENABLED
#define START_PROFILING_STATIC_MATH_TEST(measureTime, param) START_PROFILING_STATIC("MathTest", measureTime, param)
#define STOP_PROFILING_STATIC_MATH_TEST(param) STOP_PROFILING_STATIC("MathTest", param)
#define START_PROFILING_MATH_TEST(measureTime, param) START_PROFILING("MathTest", measureTime, param)
#define STOP_PROFILING_MATH_TEST(param) STOP_PROFILING("MathTest", param)
#else
#define START_PROFILING_STATIC(measureTime, param)
#define STOP_PROFILING_STATIC(param)
#define START_PROFILING(measureTime, param)
#define STOP_PROFILING(param)
#endif

#endif