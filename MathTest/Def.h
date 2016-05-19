#ifndef __MATH_TEST_DEF_H__
#define __MATH_TEST_DEF_H__

#define CRITICAL_LOG_MATH_TEST(message, ...) CRITICAL_LOG("MathTest", message, ##__VA_ARGS__)
#define EMERGENCY_LOG_MATH_TEST(message, ...) EMERGENCY_LOG("MathTest", message, ##__VA_ARGS__)
#define ERROR_LOG_MATH_TEST(message, ...) ERROR_LOG("MathTest", message, ##__VA_ARGS__)
#define WARNING_LOG_MATH_TEST(message, ...) WARNING_LOG("MathTest", message, ##__VA_ARGS__)
#define NOTICE_LOG_MATH_TEST(message, ...) NOTICE_LOG("MathTest", message, ##__VA_ARGS__)
#define INFO_LOG_MATH_TEST(message, ...) INFO_LOG("MathTest", message, ##__VA_ARGS__)
#define DEBUG_LOG_MATH_TEST(message, ...) DEBUG_LOG("MathTest", message, ##__VA_ARGS__)
#define DELOCUST_LOG_MATH_TEST(message, ...) DELOCUST_LOG("MathTest", message, ##__VA_ARGS__)

#define CHECK_CONDITION_MATH_TEST(expr, logLevel, message, ...) CHECK_CONDITION(expr, "MathTest", logLevel, message, ##__VA_ARGS__)
#define CHECK_CONDITION_ALWAYS_MATH_TEST(expr, logLevel, message, ...) CHECK_CONDITION_ALWAYS(expr, "MathTest", logLevel, message, ##__VA_ARGS__)
#define CHECK_CONDITION_EXIT_MATH_TEST(expr, logLevel, message, ...) CHECK_CONDITION_EXIT(expr, "MathTest", logLevel, message, ##__VA_ARGS__)
#define CHECK_CONDITION_EXIT_ALWAYS_MATH_TEST(expr, logLevel, message, ...) CHECK_CONDITION_EXIT_ALWAYS(expr, "MathTest", logLevel, message, ##__VA_ARGS__)
#define CHECK_CONDITION_RETURN_VOID_MATH_TEST(expr, logLevel, message, ...) CHECK_CONDITION_RETURN_VOID(expr, "MathTest", logLevel, message, ##__VA_ARGS__)
#define CHECK_CONDITION_RETURN_VOID_ALWAYS_MATH_TEST(expr, logLevel, message, ...) CHECK_CONDITION_RETURN_VOID_ALWAYS(expr, "MathTest", logLevel, message, ##__VA_ARGS__)
#define CHECK_CONDITION_RETURN_MATH_TEST(expr, returnValue, logLevel, message, ...) CHECK_CONDITION_RETURN(expr, returnValue, "MathTest", logLevel, message, ##__VA_ARGS__)
#define CHECK_CONDITION_RETURN_ALWAYS_MATH_TEST(expr, returnValue, logLevel, message, ...) CHECK_CONDITION_RETURN_ALWAYS(expr, returnValue, "MathTest", logLevel, message, ##__VA_ARGS__)

#endif /* __MATH_TEST_DEF_H__ */