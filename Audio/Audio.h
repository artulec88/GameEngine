#ifndef __AUDIO_AUDIO_H__
#define __AUDIO_AUDIO_H__

#ifdef AUDIO_EXPORTS
#define AUDIO_API __declspec(dllexport)
#else
#define AUDIO_API __declspec(dllimport)
#endif

#define PROFILING_AUDIO_MODULE_ENABLED

#define GET_CONFIG_VALUE_AUDIO(cfgName, defValue) GET_CONFIG_VALUE("Audio", cfgName, defValue)
#define GET_CONFIG_VALUE_STR_AUDIO(cfgName, defValue) GET_CONFIG_VALUE_STR("Audio", cfgName, defValue)

#define CRITICAL_LOG_AUDIO(...) CRITICAL_LOG("Audio", ##__VA_ARGS__)
#define EMERGENCY_LOG_AUDIO(...) EMERGENCY_LOG("Audio", ##__VA_ARGS__)
#define ERROR_LOG_AUDIO(...) ERROR_LOG("Audio", ##__VA_ARGS__)
#define WARNING_LOG_AUDIO(...) WARNING_LOG("Audio", ##__VA_ARGS__)
#define NOTICE_LOG_AUDIO(...) NOTICE_LOG("Audio", ##__VA_ARGS__)
#define INFO_LOG_AUDIO(...) INFO_LOG("Audio", ##__VA_ARGS__)
#define DEBUG_LOG_AUDIO(...) DEBUG_LOG("Audio", ##__VA_ARGS__)
#define DELOCUST_LOG_AUDIO(...) DELOCUST_LOG("Audio", ##__VA_ARGS__)

#ifdef PROFILING_AUDIO_MODULE_ENABLED
#define START_PROFILING_STATIC_AUDIO(measureTime, param) START_PROFILING_STATIC("Audio", measureTime, param)
#define STOP_PROFILING_STATIC_AUDIO(param) STOP_PROFILING_STATIC("Audio", param)
#define START_PROFILING_AUDIO(measureTime, param) START_PROFILING("Audio", measureTime, param)
#define STOP_PROFILING_AUDIO(param) STOP_PROFILING("Audio", param)
#else
#define START_PROFILING_STATIC_AUDIO(measureTime, param)
#define STOP_PROFILING_STATIC_AUDIO(param)
#define START_PROFILING_AUDIO(measureTime, param)
#define STOP_PROFILING_AUDIO(param)
#endif

#define CHECK_CONDITION_AUDIO(expr, logLevel, ...) CHECK_CONDITION(expr, "Audio", logLevel, ##__VA_ARGS__)
#define CHECK_CONDITION_ALWAYS_AUDIO(expr, logLevel, ...) CHECK_CONDITION_ALWAYS(expr, "Audio", logLevel, ##__VA_ARGS__)
#define CHECK_CONDITION_EXIT_AUDIO(expr, logLevel, ...) CHECK_CONDITION_EXIT(expr, "Audio", logLevel, ##__VA_ARGS__)
#define CHECK_CONDITION_EXIT_ALWAYS_AUDIO(expr, logLevel, ...) CHECK_CONDITION_EXIT_ALWAYS(expr, "Audio", logLevel, ##__VA_ARGS__)
#define CHECK_CONDITION_RETURN_VOID_AUDIO(expr, logLevel, ...) CHECK_CONDITION_RETURN_VOID(expr, "Audio", logLevel, ##__VA_ARGS__)
#define CHECK_CONDITION_RETURN_VOID_ALWAYS_AUDIO(expr, logLevel, ...) CHECK_CONDITION_RETURN_VOID_ALWAYS(expr, "Audio", logLevel, ##__VA_ARGS__)
#define CHECK_CONDITION_RETURN_AUDIO(expr, returnValue, logLevel, ...) CHECK_CONDITION_RETURN(expr, returnValue, "Audio", logLevel, ##__VA_ARGS__)
#define CHECK_CONDITION_RETURN_ALWAYS_AUDIO(expr, returnValue, logLevel, ...) CHECK_CONDITION_RETURN_ALWAYS(expr, returnValue, "Audio", logLevel, ##__VA_ARGS__)

namespace Audio
{
} /* end namespace Audio */

#endif /* __AUDIO_AUDIO_H__ */