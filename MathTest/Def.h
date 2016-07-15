#ifndef __MATH_TEST_DEF_H__
#define __MATH_TEST_DEF_H__

#define CRITICAL_LOG_MATH_TEST(...) CRITICAL_LOG("MathTest", ##__VA_ARGS__)
#define EMERGENCY_LOG_MATH_TEST(...) EMERGENCY_LOG("MathTest", ##__VA_ARGS__)
#define ERROR_LOG_MATH_TEST(...) ERROR_LOG("MathTest", ##__VA_ARGS__)
#define WARNING_LOG_MATH_TEST(...) WARNING_LOG("MathTest", ##__VA_ARGS__)
#define NOTICE_LOG_MATH_TEST(...) NOTICE_LOG("MathTest", ##__VA_ARGS__)
#define INFO_LOG_MATH_TEST(...) INFO_LOG("MathTest", ##__VA_ARGS__)
#define DEBUG_LOG_MATH_TEST(...) DEBUG_LOG("MathTest", ##__VA_ARGS__)
#define DELOCUST_LOG_MATH_TEST(...) DELOCUST_LOG("MathTest", ##__VA_ARGS__)

#define CHECK_CONDITION_MATH_TEST(expr, logLevel, ...) CHECK_CONDITION(expr, "MathTest", logLevel, ##__VA_ARGS__)
#define CHECK_CONDITION_ALWAYS_MATH_TEST(expr, logLevel, ...) CHECK_CONDITION_ALWAYS(expr, "MathTest", logLevel, ##__VA_ARGS__)
#define CHECK_CONDITION_EXIT_MATH_TEST(expr, logLevel, ...) CHECK_CONDITION_EXIT(expr, "MathTest", logLevel, ##__VA_ARGS__)
#define CHECK_CONDITION_EXIT_ALWAYS_MATH_TEST(expr, logLevel, ...) CHECK_CONDITION_EXIT_ALWAYS(expr, "MathTest", logLevel, ##__VA_ARGS__)
#define CHECK_CONDITION_RETURN_VOID_MATH_TEST(expr, logLevel, ...) CHECK_CONDITION_RETURN_VOID(expr, "MathTest", logLevel, ##__VA_ARGS__)
#define CHECK_CONDITION_RETURN_VOID_ALWAYS_MATH_TEST(expr, logLevel, ...) CHECK_CONDITION_RETURN_VOID_ALWAYS(expr, "MathTest", logLevel, ##__VA_ARGS__)
#define CHECK_CONDITION_RETURN_MATH_TEST(expr, returnValue, logLevel, ...) CHECK_CONDITION_RETURN(expr, returnValue, "MathTest", logLevel, ##__VA_ARGS__)
#define CHECK_CONDITION_RETURN_ALWAYS_MATH_TEST(expr, returnValue, logLevel, ...) CHECK_CONDITION_RETURN_ALWAYS(expr, returnValue, "MathTest", logLevel, ##__VA_ARGS__)

#endif /* __MATH_TEST_DEF_H__ */