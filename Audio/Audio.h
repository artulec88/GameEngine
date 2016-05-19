#ifndef __AUDIO_AUDIO_H__
#define __AUDIO_AUDIO_H__

#ifdef AUDIO_EXPORTS
#define AUDIO_API __declspec(dllexport)
#else
#define AUDIO_API __declspec(dllimport)
#endif

#define GET_CONFIG_VALUE_AUDIO(cfgName, defValue) GET_CONFIG_VALUE("Audio", cfgName, defValue)
#define GET_CONFIG_VALUE_STR_AUDIO(cfgName, defValue) GET_CONFIG_VALUE_STR("Audio", cfgName, defValue)

#define CRITICAL_LOG_AUDIO(message, ...) CRITICAL_LOG("Audio", message, ##__VA_ARGS__)
#define EMERGENCY_LOG_AUDIO(message, ...) EMERGENCY_LOG("Audio", message, ##__VA_ARGS__)
#define ERROR_LOG_AUDIO(message, ...) ERROR_LOG("Audio", message, ##__VA_ARGS__)
#define WARNING_LOG_AUDIO(message, ...) WARNING_LOG("Audio", message, ##__VA_ARGS__)
#define NOTICE_LOG_AUDIO(message, ...) NOTICE_LOG("Audio", message, ##__VA_ARGS__)
#define INFO_LOG_AUDIO(message, ...) INFO_LOG("Audio", message, ##__VA_ARGS__)
#define DEBUG_LOG_AUDIO(message, ...) DEBUG_LOG("Audio", message, ##__VA_ARGS__)
#define DELOCUST_LOG_AUDIO(message, ...) DELOCUST_LOG("Audio", message, ##__VA_ARGS__)

#define CHECK_CONDITION_AUDIO(expr, logLevel, message, ...) CHECK_CONDITION(expr, "Audio", logLevel, message, ##__VA_ARGS__)
#define CHECK_CONDITION_ALWAYS_AUDIO(expr, logLevel, message, ...) CHECK_CONDITION_ALWAYS(expr, "Audio", logLevel, message, ##__VA_ARGS__)
#define CHECK_CONDITION_EXIT_AUDIO(expr, logLevel, message, ...) CHECK_CONDITION_EXIT(expr, "Audio", logLevel, message, ##__VA_ARGS__)
#define CHECK_CONDITION_EXIT_ALWAYS_AUDIO(expr, logLevel, message, ...) CHECK_CONDITION_EXIT_ALWAYS(expr, "Audio", logLevel, message, ##__VA_ARGS__)
#define CHECK_CONDITION_RETURN_VOID_AUDIO(expr, logLevel, message, ...) CHECK_CONDITION_RETURN_VOID(expr, "Audio", logLevel, message, ##__VA_ARGS__)
#define CHECK_CONDITION_RETURN_VOID_ALWAYS_AUDIO(expr, logLevel, message, ...) CHECK_CONDITION_RETURN_VOID_ALWAYS(expr, "Audio", logLevel, message, ##__VA_ARGS__)
#define CHECK_CONDITION_RETURN_AUDIO(expr, returnValue, logLevel, message, ...) CHECK_CONDITION_RETURN(expr, returnValue, "Audio", logLevel, message, ##__VA_ARGS__)
#define CHECK_CONDITION_RETURN_ALWAYS_AUDIO(expr, returnValue, logLevel, message, ...) CHECK_CONDITION_RETURN_ALWAYS(expr, returnValue, "Audio", logLevel, message, ##__VA_ARGS__)

namespace Audio
{
} /* end namespace Audio */

#endif /* __AUDIO_AUDIO_H__ */