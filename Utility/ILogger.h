#ifndef __UTILITY_ILOGGER_H__
#define __UTILITY_ILOGGER_H__

#include "Utility.h"
#include "Time.h"
#include <map>
#include <vector>
#include <string>
#include <memory>
#include <iostream>
#include <sstream>
#include <list>
#include <array>

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
#define CRITICAL_LOG(moduleName, ...) Utility::Logging::ILogger::GetLogger(moduleName).Log(Utility::Logging::CRITICAL, LOGPLACE, ##__VA_ARGS__)
#else
#define CRITICAL_LOG(moduleName, ...)
#endif

#ifdef EMERGENCY_LOGGING_ENABLED
#define EMERGENCY_LOG(moduleName, ...) Utility::Logging::ILogger::GetLogger(moduleName).Log(Utility::Logging::EMERGENCY, LOGPLACE, ##__VA_ARGS__)
#else
#define EMERGENCY_LOG(moduleName, ...)
#endif

#ifdef ERROR_LOGGING_ENABLED
#define ERROR_LOG(moduleName, ...) Utility::Logging::ILogger::GetLogger(moduleName).Log(Utility::Logging::ERR, LOGPLACE, ##__VA_ARGS__)
#else
#define ERROR_LOG(moduleName, ...)
#endif

#ifdef WARNING_LOGGING_ENABLED
#define WARNING_LOG(moduleName, ...) Utility::Logging::ILogger::GetLogger(moduleName).Log(Utility::Logging::WARNING, LOGPLACE, ##__VA_ARGS__)
#else
#define WARNING_LOG(moduleName, ...)
#endif

#ifdef NOTICE_LOGGING_ENABLED
#define NOTICE_LOG(moduleName, ...) Utility::Logging::ILogger::GetLogger(moduleName).Log(Utility::Logging::NOTICE, LOGPLACE, ##__VA_ARGS__)
#else
#define NOTICE_LOG(moduleName, ...)
#endif

#ifdef INFO_LOGGING_ENABLED
#define INFO_LOG(moduleName, ...) Utility::Logging::ILogger::GetLogger(moduleName).Log(Utility::Logging::INFO, LOGPLACE, ##__VA_ARGS__)
#else
#define INFO_LOG(moduleName, ...)
#endif

#ifdef DEBUG_LOGGING_ENABLED
#define DEBUG_LOG(moduleName, ...) Utility::Logging::ILogger::GetLogger(moduleName).Log(Utility::Logging::DEBUG, LOGPLACE, ##__VA_ARGS__)
#else
#define DEBUG_LOG(moduleName, ...)
#endif

#ifdef DELOCUST_LOGGING_ENABLED
#define DELOCUST_LOG(moduleName, ...) Utility::Logging::ILogger::GetLogger(moduleName).Log(Utility::Logging::DELOCUST, LOGPLACE, ##__VA_ARGS__)
#else
#define DELOCUST_LOG(moduleName, ...)
#endif

namespace Utility
{
	namespace Logging
	{
		enum LogLevel
		{
			CRITICAL = 0,
			EMERGENCY,
			ERR,
			WARNING,
			NOTICE,
			INFO,
			DEBUG,
			DELOCUST,
			COUNT
		}; /* end enum LogLevel */

		class ILogger
		{
			typedef std::vector<std::ofstream> Outs;
			/* ==================== Static variables and functions begin ==================== */
		protected:
			static std::map<std::string, std::unique_ptr<ILogger>> loggers;
			UTILITY_API static const std::array<std::string, LogLevel::COUNT> LOGGING_LEVEL_NAMES;
		public:
			UTILITY_API static ILogger& GetLogger(const std::string& moduleName);
			/* ==================== Static variables and functions end ==================== */

			/* ==================== Constructors and destructors begin ==================== */
		public:
			/// <summary>
			/// Base logger constructor.
			/// </summary>
			ILogger(const char* fileName = NULL);

			/// <summary>
			/// Base logger destructor.
			/// </summary>
			virtual ~ILogger(void);
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
		public:
			UTILITY_API LogLevel GetLevel() const { return m_level; }
			template <typename... Args>
			void Log(LogLevel level, const char* file, int line, const Args&... args)
			{
				if (m_level < level)
				{
					return;
				}
				SetConsoleColor(level);
				std::ostringstream msg;
				LogRecursive(level, file, line, msg, args...);
				ResetConsoleColor();
			}
			UTILITY_API virtual void Fill(const std::string& strLevel, LogLevel level) = 0;
			UTILITY_API virtual void ResetConsoleColor() const = 0;
			//UTILITY_API void AddStream(std::ostream* stream);
			UTILITY_API void AddStream(const char* fileName);
		protected:
			void SetLevel(LogLevel level);
			virtual void SetConsoleColor(LogLevel level) const = 0;
			virtual void ReadConsoleColorsFromConfigFile() = 0;
		private:
			template <typename T, typename... Args>
			void LogRecursive(LogLevel level, const char* file, int line, std::ostringstream& msg, T value, const Args&... args)
			{
				msg << value;
				LogRecursive(level, file, line, msg, args...);
			}
			void LogRecursive(LogLevel level, const char* file, int line, std::ostringstream& msg)
			{
				//for (Outs::iterator outItr = m_outs.begin(); outItr != m_outs.end(); ++outItr)
				//{
				//	(*outItr) << "[" << LOGGING_LEVEL_NAMES[level] << "] [" << Utility::Timing::Time::Now().ToDateString("%H:%M:%S") << "] " <<
				//		file << "(" << line << "): " << msg.str() << std::endl;
				//}
				std::cout << "[" << LOGGING_LEVEL_NAMES[level] << "] [" << Utility::Timing::DateTime::Now().ToString("%H:%M:%S" /* TODO: Don't use hard-coded format */) << "] " <<
					file << "(" << line << "): " << msg.str() << std::endl;
			}
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
		protected:
			LogLevel m_level;
			Outs m_outs;
			// TODO: Store console colors from configuration file
		/* ==================== Non-static member variables end ==================== */
		}; /* end class ILogger */
	} /* end namespace Logging */
} /* end namespace Utility */

#endif /* __UTILITY_ILOGGER_H__ */