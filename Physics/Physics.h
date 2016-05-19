#ifndef __PHYSICS_PHYSICS_H__
#define __PHYSICS_PHYSICS_H__

#ifdef PHYSICS_EXPORTS
#define PHYSICS_API __declspec(dllexport)
#else
#define PHYSICS_API __declspec(dllimport)
#endif

#define CALCULATE_PHYSICS_STATS

#define GET_CONFIG_VALUE_PHYSICS(cfgName, defValue) GET_CONFIG_VALUE("Physics", cfgName, defValue)
#define GET_CONFIG_VALUE_STR_PHYSICS(cfgName, defValue) GET_CONFIG_VALUE_STR("Physics", cfgName, defValue)

#define CRITICAL_LOG_PHYSICS(message, ...) CRITICAL_LOG("Physics", message, ##__VA_ARGS__)
#define EMERGENCY_LOG_PHYSICS(message, ...) EMERGENCY_LOG("Physics", message, ##__VA_ARGS__)
#define ERROR_LOG_PHYSICS(message, ...) ERROR_LOG("Physics", message, ##__VA_ARGS__)
#define WARNING_LOG_PHYSICS(message, ...) WARNING_LOG("Physics", message, ##__VA_ARGS__)
#define NOTICE_LOG_PHYSICS(message, ...) NOTICE_LOG("Physics", message, ##__VA_ARGS__)
#define INFO_LOG_PHYSICS(message, ...) INFO_LOG("Physics", message, ##__VA_ARGS__)
#define DEBUG_LOG_PHYSICS(message, ...) DEBUG_LOG("Physics", message, ##__VA_ARGS__)
#define DELOCUST_LOG_PHYSICS(message, ...) DELOCUST_LOG("Physics", message, ##__VA_ARGS__)

#define CHECK_CONDITION_PHYSICS(expr, logLevel, message, ...) CHECK_CONDITION(expr, "Physics", logLevel, message, ##__VA_ARGS__)
#define CHECK_CONDITION_ALWAYS_PHYSICS(expr, logLevel, message, ...) CHECK_CONDITION_ALWAYS(expr, "Physics", logLevel, message, ##__VA_ARGS__)
#define CHECK_CONDITION_EXIT_PHYSICS(expr, logLevel, message, ...) CHECK_CONDITION_EXIT(expr, "Physics", logLevel, message, ##__VA_ARGS__)
#define CHECK_CONDITION_EXIT_ALWAYS_PHYSICS(expr, logLevel, message, ...) CHECK_CONDITION_EXIT_ALWAYS(expr, "Physics", logLevel, message, ##__VA_ARGS__)
#define CHECK_CONDITION_RETURN_VOID_PHYSICS(expr, logLevel, message, ...) CHECK_CONDITION_RETURN_VOID(expr, "Physics", logLevel, message, ##__VA_ARGS__)
#define CHECK_CONDITION_RETURN_VOID_ALWAYS_PHYSICS(expr, logLevel, message, ...) CHECK_CONDITION_RETURN_VOID_ALWAYS(expr, "Physics", logLevel, message, ##__VA_ARGS__)
#define CHECK_CONDITION_RETURN_PHYSICS(expr, returnValue, logLevel, message, ...) CHECK_CONDITION_RETURN(expr, returnValue, "Physics", logLevel, message, ##__VA_ARGS__)
#define CHECK_CONDITION_RETURN_ALWAYS_PHYSICS(expr, returnValue, logLevel, message, ...) CHECK_CONDITION_RETURN_ALWAYS(expr, returnValue, "Physics", logLevel, message, ##__VA_ARGS__)

namespace Physics
{
} /* end namespace Physics */

#endif // __PHYSICS_PHYSICS_H__