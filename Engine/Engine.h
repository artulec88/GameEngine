#ifndef __ENGINE_ENGINE_H__
#define __ENGINE_ENGINE_H__

#ifdef ENGINE_EXPORTS
#define ENGINE_API __declspec(dllexport)
#else
#define ENGINE_API __declspec(dllimport)
#endif

#include "stdafx.h"

#define PROFILING_ENGINE_MODULE_ENABLED

#define GET_CONFIG_VALUE_ENGINE(cfgName, defValue) GET_CONFIG_VALUE("Engine", cfgName, defValue)
#define GET_CONFIG_VALUE_STR_ENGINE(cfgName, defValue) GET_CONFIG_VALUE_STR("Engine", cfgName, defValue)
#define HAS_CONFIG_VALUE_ENGINE(cfgName) HAS_CONFIG_VALUE("Engine", cfgName)

#define CRITICAL_LOG_ENGINE(...) CRITICAL_LOG("Engine", ##__VA_ARGS__)
#define EMERGENCY_LOG_ENGINE(...) EMERGENCY_LOG("Engine", ##__VA_ARGS__)
#define ERROR_LOG_ENGINE(...) ERROR_LOG("Engine", ##__VA_ARGS__)
#define WARNING_LOG_ENGINE(...) WARNING_LOG("Engine", ##__VA_ARGS__)
#define NOTICE_LOG_ENGINE(...) NOTICE_LOG("Engine", ##__VA_ARGS__)
#define INFO_LOG_ENGINE(...) INFO_LOG("Engine", ##__VA_ARGS__)
#define DEBUG_LOG_ENGINE(...) DEBUG_LOG("Engine", ##__VA_ARGS__)
#define DELOCUST_LOG_ENGINE(...) DELOCUST_LOG("Engine", ##__VA_ARGS__)

#ifdef PROFILING_ENGINE_MODULE_ENABLED
#define START_PROFILING_STATIC_ENGINE(measureTime, param) START_PROFILING_STATIC("Engine", measureTime, param)
#define STOP_PROFILING_STATIC_ENGINE(param) STOP_PROFILING_STATIC("Engine", param)
#define START_PROFILING_ENGINE(measureTime, param) START_PROFILING("Engine", measureTime, param)
#define STOP_PROFILING_ENGINE(param) STOP_PROFILING("Engine", param)
#else
#define START_PROFILING_STATIC_ENGINE(measureTime, param)
#define STOP_PROFILING_STATIC_ENGINE(param)
#define START_PROFILING_ENGINE(measureTime, param)
#define STOP_PROFILING_ENGINE(param)
#endif

#define CHECK_CONDITION_ENGINE(expr, logLevel, ...) CHECK_CONDITION(expr, "Engine", logLevel, ##__VA_ARGS__)
#define CHECK_CONDITION_ALWAYS_ENGINE(expr, logLevel, ...) CHECK_CONDITION_ALWAYS(expr, "Engine", logLevel, ##__VA_ARGS__)
#define CHECK_CONDITION_EXIT_ENGINE(expr, logLevel, ...) CHECK_CONDITION_EXIT(expr, "Engine", logLevel, ##__VA_ARGS__)
#define CHECK_CONDITION_EXIT_ALWAYS_ENGINE(expr, logLevel, ...) CHECK_CONDITION_EXIT_ALWAYS(expr, "Engine", logLevel, ##__VA_ARGS__)
#define CHECK_CONDITION_RETURN_VOID_ENGINE(expr, logLevel, ...) CHECK_CONDITION_RETURN_VOID(expr, "Engine", logLevel, ##__VA_ARGS__)
#define CHECK_CONDITION_RETURN_VOID_ALWAYS_ENGINE(expr, logLevel, ...) CHECK_CONDITION_RETURN_VOID_ALWAYS(expr, "Engine", logLevel, ##__VA_ARGS__)
#define CHECK_CONDITION_RETURN_ENGINE(expr, returnValue, logLevel, ...) CHECK_CONDITION_RETURN(expr, returnValue, "Engine", logLevel, ##__VA_ARGS__)
#define CHECK_CONDITION_RETURN_ALWAYS_ENGINE(expr, returnValue, logLevel, ...) CHECK_CONDITION_RETURN_ALWAYS(expr, returnValue, "Engine", logLevel, ##__VA_ARGS__)

namespace engine
{

} /* end namespace engine */

#endif /* __ENGINE_ENGINE_H__ */