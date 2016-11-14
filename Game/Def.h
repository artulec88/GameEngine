#ifndef __GAME_DEF_H__
#define __GAME_DEF_H__

#define GET_CONFIG_VALUE_GAME(cfgName, defValue) GET_CONFIG_VALUE("Game", cfgName, defValue)
#define GET_CONFIG_VALUE_STR_GAME(cfgName, defValue) GET_CONFIG_VALUE_STR("Game", cfgName, defValue)
#define HAS_CONFIG_VALUE_GAME(cfgName) HAS_CONFIG_VALUE("Game", cfgName)

#define CRITICAL_LOG_GAME(...) CRITICAL_LOG("Game", ##__VA_ARGS__)
#define EMERGENCY_LOG_GAME(...) EMERGENCY_LOG("Game", ##__VA_ARGS__)
#define ERROR_LOG_GAME(...) ERROR_LOG("Game", ##__VA_ARGS__)
#define WARNING_LOG_GAME(...) WARNING_LOG("Game", ##__VA_ARGS__)
#define NOTICE_LOG_GAME(...) NOTICE_LOG("Game", ##__VA_ARGS__)
#define INFO_LOG_GAME(...) INFO_LOG("Game", ##__VA_ARGS__)
#define DEBUG_LOG_GAME(...) DEBUG_LOG("Game", ##__VA_ARGS__)
#define DELOCUST_LOG_GAME(...) DELOCUST_LOG("Game", ##__VA_ARGS__)

#define PROFILING_GAME_MODULE_ENABLED
#ifdef PROFILING_GAME_MODULE_ENABLED
#define START_PROFILING_STATIC_GAME(measureTime, param) START_PROFILING_STATIC("Game", measureTime, param)
#define STOP_PROFILING_STATIC_GAME(param) STOP_PROFILING_STATIC("Game", param)
#define START_PROFILING_GAME(measureTime, param) START_PROFILING("Game", measureTime, param)
#define STOP_PROFILING_GAME(param) STOP_PROFILING("Game", param)
#else
#define START_PROFILING_STATIC_GAME(measureTime, param)
#define STOP_PROFILING_STATIC_GAME(param)
#define START_PROFILING_GAME(measureTime, param)
#define STOP_PROFILING_GAME(param)
#endif

#define CHECK_CONDITION_GAME(expr, logLevel, ...) CHECK_CONDITION(expr, "Game", logLevel, ##__VA_ARGS__)
#define CHECK_CONDITION_ALWAYS_GAME(expr, logLevel, ...) CHECK_CONDITION_ALWAYS(expr, "Game", logLevel, ##__VA_ARGS__)
#define CHECK_CONDITION_EXIT_GAME(expr, logLevel, ...) CHECK_CONDITION_EXIT(expr, "Game", logLevel, ##__VA_ARGS__)
#define CHECK_CONDITION_EXIT_ALWAYS_GAME(expr, logLevel, ...) CHECK_CONDITION_EXIT_ALWAYS(expr, "Game", logLevel, ##__VA_ARGS__)
#define CHECK_CONDITION_RETURN_VOID_GAME(expr, logLevel, ...) CHECK_CONDITION_RETURN_VOID(expr, "Game", logLevel, ##__VA_ARGS__)
#define CHECK_CONDITION_RETURN_VOID_ALWAYS_GAME(expr, logLevel, ...) CHECK_CONDITION_RETURN_VOID_ALWAYS(expr, "Game", logLevel, ##__VA_ARGS__)
#define CHECK_CONDITION_RETURN_GAME(expr, returnValue, logLevel, ...) CHECK_CONDITION_RETURN(expr, returnValue, "Game", logLevel, ##__VA_ARGS__)
#define CHECK_CONDITION_RETURN_ALWAYS_GAME(expr, returnValue, logLevel, ...) CHECK_CONDITION_RETURN_ALWAYS(expr, returnValue, "Game", logLevel, ##__VA_ARGS__)

#endif /* __GAME_DEF_H__ */
