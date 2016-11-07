#ifndef __RENDERING_TEST_DEF_H__
#define __RENDERING_TEST_DEF_H__

#define CRITICAL_LOG_RENDERING_TEST(...) CRITICAL_LOG("RenderingTest", ##__VA_ARGS__)
#define EMERGENCY_LOG_RENDERING_TEST(...) EMERGENCY_LOG("RenderingTest", ##__VA_ARGS__)
#define ERROR_LOG_RENDERING_TEST(...) ERROR_LOG("RenderingTest", ##__VA_ARGS__)
#define WARNING_LOG_RENDERING_TEST(...) WARNING_LOG("RenderingTest", ##__VA_ARGS__)
#define NOTICE_LOG_RENDERING_TEST(...) NOTICE_LOG("RenderingTest", ##__VA_ARGS__)
#define INFO_LOG_RENDERING_TEST(...) INFO_LOG("RenderingTest", ##__VA_ARGS__)
#define DEBUG_LOG_RENDERING_TEST(...) DEBUG_LOG("RenderingTest", ##__VA_ARGS__)
#define DELOCUST_LOG_RENDERING_TEST(...) DELOCUST_LOG("RenderingTest", ##__VA_ARGS__)

#define CHECK_CONDITION_RENDERING_TEST(expr, logLevel, ...) CHECK_CONDITION(expr, "RenderingTest", logLevel, ##__VA_ARGS__)
#define CHECK_CONDITION_ALWAYS_RENDERING_TEST(expr, logLevel, ...) CHECK_CONDITION_ALWAYS(expr, "RenderingTest", logLevel, ##__VA_ARGS__)
#define CHECK_CONDITION_EXIT_RENDERING_TEST(expr, logLevel, ...) CHECK_CONDITION_EXIT(expr, "RenderingTest", logLevel, ##__VA_ARGS__)
#define CHECK_CONDITION_EXIT_ALWAYS_RENDERING_TEST(expr, logLevel, ...) CHECK_CONDITION_EXIT_ALWAYS(expr, "RenderingTest", logLevel, ##__VA_ARGS__)
#define CHECK_CONDITION_RETURN_VOID_RENDERING_TEST(expr, logLevel, ...) CHECK_CONDITION_RETURN_VOID(expr, "RenderingTest", logLevel, ##__VA_ARGS__)
#define CHECK_CONDITION_RETURN_VOID_ALWAYS_RENDERING_TEST(expr, logLevel, ...) CHECK_CONDITION_RETURN_VOID_ALWAYS(expr, "RenderingTest", logLevel, ##__VA_ARGS__)
#define CHECK_CONDITION_RETURN_RENDERING_TEST(expr, returnValue, logLevel, ...) CHECK_CONDITION_RETURN(expr, returnValue, "RenderingTest", logLevel, ##__VA_ARGS__)
#define CHECK_CONDITION_RETURN_ALWAYS_RENDERING_TEST(expr, returnValue, logLevel, ...) CHECK_CONDITION_RETURN_ALWAYS(expr, returnValue, "RenderingTest", logLevel, ##__VA_ARGS__)

#endif /* __RENDERING_TEST_DEF_H__ */