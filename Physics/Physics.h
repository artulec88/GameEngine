#ifndef __PHYSICS_PHYSICS_H__
#define __PHYSICS_PHYSICS_H__

#ifdef PHYSICS_EXPORTS
#define PHYSICS_API __declspec(dllexport)
#else
#define PHYSICS_API __declspec(dllimport)
#endif

#define GET_CONFIG_VALUE_PHYSICS(cfgName, defValue) GET_CONFIG_VALUE("Physics", cfgName, defValue)
#define GET_CONFIG_VALUE_STR_PHYSICS(cfgName, defValue) GET_CONFIG_VALUE_STR("Physics", cfgName, defValue)
#define HAS_CONFIG_VALUE_PHYSICS(cfgName) HAS_CONFIG_VALUE("Physics", cfgName)

#define CRITICAL_LOG_PHYSICS(...) CRITICAL_LOG("Physics", ##__VA_ARGS__)
#define EMERGENCY_LOG_PHYSICS(...) EMERGENCY_LOG("Physics", ##__VA_ARGS__)
#define ERROR_LOG_PHYSICS(...) ERROR_LOG("Physics", ##__VA_ARGS__)
#define WARNING_LOG_PHYSICS(...) WARNING_LOG("Physics", ##__VA_ARGS__)
#define NOTICE_LOG_PHYSICS(...) NOTICE_LOG("Physics", ##__VA_ARGS__)
#define INFO_LOG_PHYSICS(...) INFO_LOG("Physics", ##__VA_ARGS__)
#define DEBUG_LOG_PHYSICS(...) DEBUG_LOG("Physics", ##__VA_ARGS__)
#define DELOCUST_LOG_PHYSICS(...) DELOCUST_LOG("Physics", ##__VA_ARGS__)

#define PROFILING_PHYSICS_MODULE_ENABLED
#ifdef PROFILING_PHYSICS_MODULE_ENABLED
#define START_PROFILING_STATIC_PHYSICS(measureTime, param) START_PROFILING_STATIC("Physics", measureTime, param)
#define STOP_PROFILING_STATIC_PHYSICS(param) STOP_PROFILING_STATIC("Physics", param)
#define START_PROFILING_PHYSICS(measureTime, param) START_PROFILING("Physics", measureTime, param)
#define STOP_PROFILING_PHYSICS(param) STOP_PROFILING("Physics", param)
#else
#define START_PROFILING_STATIC_PHYSICS(measureTime, param)
#define STOP_PROFILING_STATIC_PHYSICS(param)
#define START_PROFILING_PHYSICS(measureTime, param)
#define STOP_PROFILING_PHYSICS(param)
#endif

#define CHECK_CONDITION_PHYSICS(expr, logLevel, ...) CHECK_CONDITION(expr, "Physics", logLevel, ##__VA_ARGS__)
#define CHECK_CONDITION_ALWAYS_PHYSICS(expr, logLevel, ...) CHECK_CONDITION_ALWAYS(expr, "Physics", logLevel, ##__VA_ARGS__)
#define CHECK_CONDITION_EXIT_PHYSICS(expr, logLevel, ...) CHECK_CONDITION_EXIT(expr, "Physics", logLevel, ##__VA_ARGS__)
#define CHECK_CONDITION_EXIT_ALWAYS_PHYSICS(expr, logLevel, ...) CHECK_CONDITION_EXIT_ALWAYS(expr, "Physics", logLevel, ##__VA_ARGS__)
#define CHECK_CONDITION_RETURN_VOID_PHYSICS(expr, logLevel, ...) CHECK_CONDITION_RETURN_VOID(expr, "Physics", logLevel, ##__VA_ARGS__)
#define CHECK_CONDITION_RETURN_VOID_ALWAYS_PHYSICS(expr, logLevel, ...) CHECK_CONDITION_RETURN_VOID_ALWAYS(expr, "Physics", logLevel, ##__VA_ARGS__)
#define CHECK_CONDITION_RETURN_PHYSICS(expr, returnValue, logLevel, ...) CHECK_CONDITION_RETURN(expr, returnValue, "Physics", logLevel, ##__VA_ARGS__)
#define CHECK_CONDITION_RETURN_ALWAYS_PHYSICS(expr, returnValue, logLevel, ...) CHECK_CONDITION_RETURN_ALWAYS(expr, returnValue, "Physics", logLevel, ##__VA_ARGS__)

namespace Physics
{
} /* end namespace Physics */

#endif // __PHYSICS_PHYSICS_H__