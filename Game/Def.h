#ifndef __GAME_DEF_H__
#define __GAME_DEF_H__

#define GET_CONFIG_VALUE_GAME(cfgName, defValue) GET_CONFIG_VALUE("Game", cfgName, defValue)
#define GET_CONFIG_VALUE_STR_GAME(cfgName, defValue) GET_CONFIG_VALUE_STR("Game", cfgName, defValue)

#define CRITICAL_LOG_GAME(message, ...) CRITICAL_LOG("Game", message, ##__VA_ARGS__)
#define EMERGENCY_LOG_GAME(message, ...) EMERGENCY_LOG("Game", message, ##__VA_ARGS__)
#define ERROR_LOG_GAME(message, ...) ERROR_LOG("Game", message, ##__VA_ARGS__)
#define WARNING_LOG_GAME(message, ...) WARNING_LOG("Game", message, ##__VA_ARGS__)
#define NOTICE_LOG_GAME(message, ...) NOTICE_LOG("Game", message, ##__VA_ARGS__)
#define INFO_LOG_GAME(message, ...) INFO_LOG("Game", message, ##__VA_ARGS__)
#define DEBUG_LOG_GAME(message, ...) DEBUG_LOG("Game", message, ##__VA_ARGS__)
#define DELOCUST_LOG_GAME(message, ...) DELOCUST_LOG("Game", message, ##__VA_ARGS__)

#define CHECK_CONDITION_GAME(expr, logLevel, message, ...) CHECK_CONDITION(expr, "Game", logLevel, message, ##__VA_ARGS__)
#define CHECK_CONDITION_ALWAYS_GAME(expr, logLevel, message, ...) CHECK_CONDITION_ALWAYS(expr, "Game", logLevel, message, ##__VA_ARGS__)
#define CHECK_CONDITION_EXIT_GAME(expr, logLevel, message, ...) CHECK_CONDITION_EXIT(expr, "Game", logLevel, message, ##__VA_ARGS__)
#define CHECK_CONDITION_EXIT_ALWAYS_GAME(expr, logLevel, message, ...) CHECK_CONDITION_EXIT_ALWAYS(expr, "Game", logLevel, message, ##__VA_ARGS__)
#define CHECK_CONDITION_RETURN_VOID_GAME(expr, logLevel, message, ...) CHECK_CONDITION_RETURN_VOID(expr, "Game", logLevel, message, ##__VA_ARGS__)
#define CHECK_CONDITION_RETURN_VOID_ALWAYS_GAME(expr, logLevel, message, ...) CHECK_CONDITION_RETURN_VOID_ALWAYS(expr, "Game", logLevel, message, ##__VA_ARGS__)
#define CHECK_CONDITION_RETURN_GAME(expr, returnValue, logLevel, message, ...) CHECK_CONDITION_RETURN(expr, returnValue, "Game", logLevel, message, ##__VA_ARGS__)
#define CHECK_CONDITION_RETURN_ALWAYS_GAME(expr, returnValue, logLevel, message, ...) CHECK_CONDITION_RETURN_ALWAYS(expr, returnValue, "Game", logLevel, message, ##__VA_ARGS__)

#endif /* __GAME_DEF_H__ */
