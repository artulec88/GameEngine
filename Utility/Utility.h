#ifndef __UTILITY_H__
#define __UTILITY_H__

#include <vector>
#include <string>
#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>

#ifdef UTILITY_EXPORTS
#define UTILITY_API __declspec(dllexport)
#else
#define UTILITY_API __declspec(dllimport)
#endif

//#ifdef NDEBUG
//#ifdef LOGMODULE
//#define LOGPLACE LOGMODULE, 0 
//#else
//#define LOGPLACE "unknown-module", 0 
//#endif
//#else
//#define LOGPLACE __FILE__, __LINE__
//#endif
#define LOGPLACE __FILE__, __LINE__

#ifdef _DEBUG
#include <string.h>
#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__) // For UNIX change '\\' to '/'
#else
#define __FILENAME__ __FILE__
#endif

//#ifdef _DEBUG
#define MEASURE_TIME_ENABLED
//#endif

#define SAFE_DELETE(p) if(p != nullptr) { delete p; p = nullptr; }
#define SAFE_DELETE_WHOLE_TABLE(p, c) for(int i = 0; i < c; ++i) { if (p[i] != nullptr) { delete p[i]; p[i] = nullptr; } } delete [] p; p = nullptr;
#define SAFE_DELETE_JUST_TABLE(p) if (p != nullptr) { delete [] p; p = nullptr; }

#ifdef _DEBUG
#define CHECK_CONDITION(expr, moduleName, logLevel, ...) do { if (!(expr)) { utility::logging::ILogger::GetLogger(moduleName).Log(logLevel, LOGPLACE, ##__VA_ARGS__); } } while (0)
#define CHECK_CONDITION_ALWAYS(expr, moduleName, logLevel, ...) do { if (!(expr)) { utility::logging::ILogger::GetLogger(moduleName).Log(logLevel, LOGPLACE, ##__VA_ARGS__); } } while (0)
#define CHECK_CONDITION_EXIT(expr, moduleName, logLevel, ...) do { if (!(expr)) { utility::logging::ILogger::GetLogger(moduleName).Log(logLevel, LOGPLACE, ##__VA_ARGS__); exit(EXIT_FAILURE); } } while (0)
#define CHECK_CONDITION_EXIT_ALWAYS(expr, moduleName, logLevel, ...) do { if (!(expr)) { utility::logging::ILogger::GetLogger(moduleName).Log(logLevel, LOGPLACE, ##__VA_ARGS__); exit(EXIT_FAILURE); } } while (0)
#define CHECK_CONDITION_RETURN_VOID(expr, moduleName, logLevel, ...) do { if (!(expr)) { utility::logging::ILogger::GetLogger(moduleName).Log(logLevel, LOGPLACE, ##__VA_ARGS__); return; } } while (0)
#define CHECK_CONDITION_RETURN_VOID_ALWAYS(expr, moduleName, logLevel, ...) do { if (!(expr)) { utility::logging::ILogger::GetLogger(moduleName).Log(logLevel, LOGPLACE, ##__VA_ARGS__); return; } } while (0)
#define CHECK_CONDITION_RETURN(expr, returnValue, moduleName, logLevel, ...) do { if (!(expr)) { utility::logging::ILogger::GetLogger(moduleName).Log(logLevel, LOGPLACE, ##__VA_ARGS__); return returnValue; } } while (0)
#define CHECK_CONDITION_RETURN_ALWAYS(expr, returnValue, moduleName, logLevel, ...) do { if (!(expr)) { utility::logging::ILogger::GetLogger(moduleName).Log(logLevel, LOGPLACE, ##__VA_ARGS__); return returnValue; } } while (0)
#define ASSERT(expr) do { if (!(expr)) { printf("ASSERT in file \"%s\" in line \"%d\"\n", __FILENAME__, __LINE__); } } while (0)
#define SLOW_ASSERTIONS_ENABLED
#ifdef SLOW_ASSERTIONS_ENABLED
#define SLOW_ASSERT(expr) ASSERT(expr)
#else
#define SLOW_ASSERT(expr)
#endif
#else /* _RELEASE */
#define CHECK_CONDITION(expr, moduleName, logLevel, ...)
#define CHECK_CONDITION_ALWAYS(expr, moduleName, logLevel, ...) do { if (!(expr)) { utility::logging::ILogger::GetLogger(moduleName).Log(logLevel, LOGPLACE, ##__VA_ARGS__); } } while (0)
#define CHECK_CONDITION_EXIT(expr, moduleName, logLevel, ...)
#define CHECK_CONDITION_EXIT_ALWAYS(expr, moduleName, logLevel, ...) do { if (!(expr)) { utility::logging::ILogger::GetLogger(moduleName).Log(logLevel, LOGPLACE, ##__VA_ARGS__); exit(EXIT_FAILURE); } } while (0)
#define CHECK_CONDITION_RETURN_VOID(expr, moduleName, logLevel, ...)
#define CHECK_CONDITION_RETURN_VOID_ALWAYS(expr, moduleName, logLevel, ...) do { if (!(expr)) { utility::logging::ILogger::GetLogger(moduleName).Log(logLevel, LOGPLACE, ##__VA_ARGS__); return; } } while (0)
#define CHECK_CONDITION_RETURN(expr, returnValue, moduleName, logLevel, ...)
#define CHECK_CONDITION_RETURN_ALWAYS(expr, returnValue, moduleName, logLevel, ...) do { if (!(expr)) { utility::logging::ILogger::GetLogger(moduleName).Log(logLevel, LOGPLACE, ##__VA_ARGS__); return returnValue; } } while (0)
#define ASSERT(expr)
#define SLOW_ASSERT(expr)
#endif /* _DEBUG */

#define GET_CONFIG_VALUE_UTILITY(cfgName, defValue) GET_CONFIG_VALUE("Utility", cfgName, defValue)
#define GET_CONFIG_VALUE_STR_UTILITY(cfgName, defValue) GET_CONFIG_VALUE_STR("Utility", cfgName, defValue)

#define CRITICAL_LOG_UTILITY(...) CRITICAL_LOG("Utility", ##__VA_ARGS__)
#define EMERGENCY_LOG_UTILITY(...) EMERGENCY_LOG("Utility", ##__VA_ARGS__)
#define ERROR_LOG_UTILITY(...) ERROR_LOG("Utility", ##__VA_ARGS__)
#define WARNING_LOG_UTILITY(...) WARNING_LOG("Utility", ##__VA_ARGS__)
#define NOTICE_LOG_UTILITY(...) NOTICE_LOG("Utility", ##__VA_ARGS__)
#define INFO_LOG_UTILITY(...) INFO_LOG("Utility", ##__VA_ARGS__)
#define DEBUG_LOG_UTILITY(...) DEBUG_LOG("Utility", ##__VA_ARGS__)
#define DELOCUST_LOG_UTILITY(...) DELOCUST_LOG("Utility", ##__VA_ARGS__)

#define CHECK_CONDITION_UTILITY(expr, logLevel, ...) CHECK_CONDITION(expr, "Utility", logLevel, ##__VA_ARGS__)
#define CHECK_CONDITION_ALWAYS_UTILITY(expr, logLevel, ...) CHECK_CONDITION_ALWAYS(expr, "Utility", logLevel, ##__VA_ARGS__)
#define CHECK_CONDITION_EXIT_UTILITY(expr, logLevel, ...) CHECK_CONDITION_EXIT(expr, "Utility", logLevel, ##__VA_ARGS__)
#define CHECK_CONDITION_EXIT_ALWAYS_UTILITY(expr, logLevel, ...) CHECK_CONDITION_EXIT_ALWAYS(expr, "Utility", logLevel, ##__VA_ARGS__)
#define CHECK_CONDITION_RETURN_VOID_UTILITY(expr, logLevel, ...) CHECK_CONDITION_RETURN_VOID(expr, "Utility", logLevel, ##__VA_ARGS__)
#define CHECK_CONDITION_RETURN_VOID_ALWAYS_UTILITY(expr, logLevel, ...) CHECK_CONDITION_RETURN_VOID_ALWAYS(expr, "Utility", logLevel, ##__VA_ARGS__)
#define CHECK_CONDITION_RETURN_UTILITY(expr, returnValue, logLevel, ...) CHECK_CONDITION_RETURN(expr, returnValue, "Utility", logLevel, ##__VA_ARGS__)
#define CHECK_CONDITION_RETURN_ALWAYS_UTILITY(expr, returnValue, logLevel, ...) CHECK_CONDITION_RETURN_ALWAYS(expr, returnValue, "Utility", logLevel, ##__VA_ARGS__)

namespace utility
{
	//template<typename T> Min(T value1, T value2)
	//{
	//	return (value1 < value2) ? value1 : value2;
	//}

	//template<typename T> Max(T value1, T value2)
	//{
	//	return (value1 > value2) ? value1 : value2;
	//}
} /* end namespace Utility */

#endif /* __UTILITY_H__ */