#ifndef __ENGINE_ENGINE_H__
#define __ENGINE_ENGINE_H__

#ifdef ENGINE_EXPORTS
#define ENGINE_API __declspec(dllexport)
#else
#define ENGINE_API __declspec(dllimport)
#endif

#include "stdafx.h"

#define CALCULATE_CORE_STATS

#ifndef CALCULATE_CORE_STATS
#define START_PROFILING_STATIC
#define STOP_PROFILING_STATIC
#define START_PROFILING
#define STOP_PROFILING
#endif

#define GET_CONFIG_VALUE_ENGINE(cfgName, defValue) GET_CONFIG_VALUE("Engine", cfgName, defValue)
#define GET_CONFIG_VALUE_STR_ENGINE(cfgName, defValue) GET_CONFIG_VALUE_STR("Engine", cfgName, defValue)

#define CRITICAL_LOG_ENGINE(message, ...) CRITICAL_LOG("Engine", message, ##__VA_ARGS__)
#define EMERGENCY_LOG_ENGINE(message, ...) EMERGENCY_LOG("Engine", message, ##__VA_ARGS__)
#define ERROR_LOG_ENGINE(message, ...) ERROR_LOG("Engine", message, ##__VA_ARGS__)
#define WARNING_LOG_ENGINE(message, ...) WARNING_LOG("Engine", message, ##__VA_ARGS__)
#define NOTICE_LOG_ENGINE(message, ...) NOTICE_LOG("Engine", message, ##__VA_ARGS__)
#define INFO_LOG_ENGINE(message, ...) INFO_LOG("Engine", message, ##__VA_ARGS__)
#define DEBUG_LOG_ENGINE(message, ...) DEBUG_LOG("Engine", message, ##__VA_ARGS__)
#define DELOCUST_LOG_ENGINE(message, ...) DELOCUST_LOG("Engine", message, ##__VA_ARGS__)

#define CHECK_CONDITION_ENGINE(expr, logLevel, message, ...) CHECK_CONDITION(expr, "Engine", logLevel, message, ##__VA_ARGS__)
#define CHECK_CONDITION_ALWAYS_ENGINE(expr, logLevel, message, ...) CHECK_CONDITION_ALWAYS(expr, "Engine", logLevel, message, ##__VA_ARGS__)
#define CHECK_CONDITION_EXIT_ENGINE(expr, logLevel, message, ...) CHECK_CONDITION_EXIT(expr, "Engine", logLevel, message, ##__VA_ARGS__)
#define CHECK_CONDITION_EXIT_ALWAYS_ENGINE(expr, logLevel, message, ...) CHECK_CONDITION_EXIT_ALWAYS(expr, "Engine", logLevel, message, ##__VA_ARGS__)
#define CHECK_CONDITION_RETURN_VOID_ENGINE(expr, logLevel, message, ...) CHECK_CONDITION_RETURN_VOID(expr, "Engine", logLevel, message, ##__VA_ARGS__)
#define CHECK_CONDITION_RETURN_VOID_ALWAYS_ENGINE(expr, logLevel, message, ...) CHECK_CONDITION_RETURN_VOID_ALWAYS(expr, "Engine", logLevel, message, ##__VA_ARGS__)
#define CHECK_CONDITION_RETURN_ENGINE(expr, returnValue, logLevel, message, ...) CHECK_CONDITION_RETURN(expr, returnValue, "Engine", logLevel, message, ##__VA_ARGS__)
#define CHECK_CONDITION_RETURN_ALWAYS_ENGINE(expr, returnValue, logLevel, message, ...) CHECK_CONDITION_RETURN_ALWAYS(expr, returnValue, "Engine", logLevel, message, ##__VA_ARGS__)

namespace Engine
{

} /* end namespace Engine */

#endif /* __ENGINE_ENGINE_H__ */