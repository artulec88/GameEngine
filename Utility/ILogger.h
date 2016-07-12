#ifndef __UTILITY_ILOGGER_H__
#define __UTILITY_ILOGGER_H__

#include "Utility.h"
#include <map>
#include <string>
#include <memory>

#define LOGGING_ENABLED
#define CRITICAL_LOGGING_ENABLED
#define EMERGENCY_LOGGING_ENABLED
#define ERROR_LOGGING_ENABLED
#define WARNING_LOGGING_ENABLED
#define NOTICE_LOGGING_ENABLED
#define INFO_LOGGING_ENABLED
#define DEBUG_LOGGING_ENABLED
#define DELOCUST_LOGGING_ENABLED

#ifndef LOGGING_ENABLED
#undef CRITICAL_LOGGING_ENABLED
#undef EMERGENCY_LOGGING_ENABLED
#undef ERROR_LOGGING_ENABLED
#undef WARNING_LOGGING_ENABLED
#undef NOTICE_LOGGING_ENABLED
#undef INFO_LOGGING_ENABLED
#undef DEBUG_LOGGING_ENABLED
#undef DELOCUST_LOGGING_ENABLED
#endif

#ifndef CRITICAL_LOGGING_ENABLED
#undef EMERGENCY_LOGGING_ENABLED
#undef ERROR_LOGGING_ENABLED
#undef WARNING_LOGGING_ENABLED
#undef NOTICE_LOGGING_ENABLED
#undef INFO_LOGGING_ENABLED
#undef DEBUG_LOGGING_ENABLED
#undef DELOCUST_LOGGING_ENABLED
#endif

#ifndef EMERGENCY_LOGGING_ENABLED
#undef ERROR_LOGGING_ENABLED
#undef WARNING_LOGGING_ENABLED
#undef NOTICE_LOGGING_ENABLED
#undef INFO_LOGGING_ENABLED
#undef DEBUG_LOGGING_ENABLED
#undef DELOCUST_LOGGING_ENABLED
#endif

#ifndef ERROR_LOGGING_ENABLED
#undef WARNING_LOGGING_ENABLED
#undef NOTICE_LOGGING_ENABLED
#undef INFO_LOGGING_ENABLED
#undef DEBUG_LOGGING_ENABLED
#undef DELOCUST_LOGGING_ENABLED
#endif

#ifndef WARNING_LOGGING_ENABLED
#undef NOTICE_LOGGING_ENABLED
#undef INFO_LOGGING_ENABLED
#undef DEBUG_LOGGING_ENABLED
#undef DELOCUST_LOGGING_ENABLED
#endif

#ifndef NOTICE_LOGGING_ENABLED
#undef INFO_LOGGING_ENABLED
#undef DEBUG_LOGGING_ENABLED
#undef DELOCUST_LOGGING_ENABLED
#endif

#ifndef INFO_LOGGING_ENABLED
#undef DEBUG_LOGGING_ENABLED
#undef DELOCUST_LOGGING_ENABLED
#endif

#ifndef DEBUG_LOGGING_ENABLED
#undef DELOCUST_LOGGING_ENABLED
#endif

#ifdef CRITICAL_LOGGING_ENABLED
#define CRITICAL_LOG(moduleName, message, ...) Utility::Logging::ILogger::GetLogger(moduleName).Log(Utility::Logging::CRITICAL, LOGPLACE, message, ##__VA_ARGS__)
#else
#define CRITICAL_LOG(moduleName, message, ...)
#endif

#ifdef EMERGENCY_LOGGING_ENABLED
#define EMERGENCY_LOG(moduleName, message, ...) Utility::Logging::ILogger::GetLogger(moduleName).Log(Utility::Logging::EMERGENCY, LOGPLACE, message, ##__VA_ARGS__)
#else
#define EMERGENCY_LOG(moduleName, message, ...)
#endif

#ifdef ERROR_LOGGING_ENABLED
#define ERROR_LOG(moduleName, message, ...) Utility::Logging::ILogger::GetLogger(moduleName).Log(Utility::Logging::ERR, LOGPLACE, message, ##__VA_ARGS__)
#else
#define ERROR_LOG(moduleName, message, ...)
#endif

#ifdef WARNING_LOGGING_ENABLED
#define WARNING_LOG(moduleName, message, ...) Utility::Logging::ILogger::GetLogger(moduleName).Log(Utility::Logging::WARNING, LOGPLACE, message, ##__VA_ARGS__)
#else
#define WARNING_LOG(moduleName, message, ...)
#endif

#ifdef NOTICE_LOGGING_ENABLED
#define NOTICE_LOG(moduleName, message, ...) Utility::Logging::ILogger::GetLogger(moduleName).Log(Utility::Logging::NOTICE, LOGPLACE, message, ##__VA_ARGS__)
#else
#define NOTICE_LOG(moduleName, message, ...)
#endif

#ifdef INFO_LOGGING_ENABLED
#define INFO_LOG(moduleName, message, ...) Utility::Logging::ILogger::GetLogger(moduleName).Log(Utility::Logging::INFO, LOGPLACE, message, ##__VA_ARGS__)
#else
#define INFO_LOG(moduleName, message, ...)
#endif

#ifdef DEBUG_LOGGING_ENABLED
#define DEBUG_LOG(moduleName, message, ...) Utility::Logging::ILogger::GetLogger(moduleName).Log(Utility::Logging::DEBUG, LOGPLACE, message, ##__VA_ARGS__)
#else
#define DEBUG_LOG(moduleName, message, ...)
#endif

#ifdef DELOCUST_LOGGING_ENABLED
#define DELOCUST_LOG(moduleName, message, ...) Utility::Logging::ILogger::GetLogger(moduleName).Log(Utility::Logging::DELOCUST, LOGPLACE, message, ##__VA_ARGS__)
#else
#define DELOCUST_LOG(moduleName, message, ...)
#endif

namespace Utility
{
	namespace Logging
	{
		enum LogLevel
		{
			CRITICAL = 0,
			EMERGENCY = 1,
			ERR = 2,
			WARNING = 3,
			NOTICE = 4,
			INFO = 5,
			DEBUG = 6,
			DELOCUST = 7,

			DEV_NULL = 100
		}; /* end enum LogLevel */

		class ILogger
		{
			/* ==================== Static variables and functions begin ==================== */
		protected:
			static std::map<std::string, std::unique_ptr<ILogger>> loggers;
			static const char* LevelNames[];
		public:
			UTILITY_API static ILogger& GetLogger(const std::string& moduleName);
			/* ==================== Static variables and functions end ==================== */

			/* ==================== Constructors and destructors begin ==================== */
		public:
			ILogger();
			virtual ~ILogger(void);
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
		public:
			LogLevel GetLevel() const { return m_level; }
			//virtual void operator()(LogLevel level, const char *name, int line, const char *format, ...) = 0;
			UTILITY_API virtual void Log(LogLevel level, const char *name, int line, const char *format, ...) = 0;
			virtual void Fill(const std::string& strLevel, LogLevel level) = 0;
			virtual void AddFile(const char *name) = 0;
			UTILITY_API virtual void ResetConsoleColor() const = 0;
		protected:
			void SetLevel(LogLevel level);
			virtual void SetConsoleColor(LogLevel level) const = 0;
			virtual void ReadConsoleColorsFromConfigFile() = 0;
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
		protected:
			LogLevel m_level;
			// TODO: Store console colors from configuration file
		/* ==================== Non-static member variables end ==================== */
		}; /* end class ILogger */
	} /* end namespace Logging */
} /* end namespace Utility */

#endif /* __UTILITY_ILOGGER_H__ */