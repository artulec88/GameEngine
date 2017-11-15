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
#define CRITICAL_LOG(moduleName, ...) utility::logging::ILogger::GetLogger(moduleName).Log(utility::logging::CRITICAL, LOGPLACE, ##__VA_ARGS__)
#else
#define CRITICAL_LOG(moduleName, ...)
#endif

#ifdef EMERGENCY_LOGGING_ENABLED
#define EMERGENCY_LOG(moduleName, ...) utility::logging::ILogger::GetLogger(moduleName).Log(utility::logging::EMERGENCY, LOGPLACE, ##__VA_ARGS__)
#else
#define EMERGENCY_LOG(moduleName, ...)
#endif

#ifdef ERROR_LOGGING_ENABLED
#define ERROR_LOG(moduleName, ...) utility::logging::ILogger::GetLogger(moduleName).Log(utility::logging::ERR, LOGPLACE, ##__VA_ARGS__)
#else
#define ERROR_LOG(moduleName, ...)
#endif

#ifdef WARNING_LOGGING_ENABLED
#define WARNING_LOG(moduleName, ...) utility::logging::ILogger::GetLogger(moduleName).Log(utility::logging::WARNING, LOGPLACE, ##__VA_ARGS__)
#else
#define WARNING_LOG(moduleName, ...)
#endif

#ifdef NOTICE_LOGGING_ENABLED
#define NOTICE_LOG(moduleName, ...) utility::logging::ILogger::GetLogger(moduleName).Log(utility::logging::NOTICE, LOGPLACE, ##__VA_ARGS__)
#else
#define NOTICE_LOG(moduleName, ...)
#endif

#ifdef INFO_LOGGING_ENABLED
#define INFO_LOG(moduleName, ...) utility::logging::ILogger::GetLogger(moduleName).Log(utility::logging::INFO, LOGPLACE, ##__VA_ARGS__)
#else
#define INFO_LOG(moduleName, ...)
#endif

#ifdef DEBUG_LOGGING_ENABLED
#define DEBUG_LOG(moduleName, ...) utility::logging::ILogger::GetLogger(moduleName).Log(utility::logging::DEBUG, LOGPLACE, ##__VA_ARGS__)
#else
#define DEBUG_LOG(moduleName, ...)
#endif

#ifdef DELOCUST_LOGGING_ENABLED
#define DELOCUST_LOG(moduleName, ...) utility::logging::ILogger::GetLogger(moduleName).Log(utility::logging::DELOCUST, LOGPLACE, ##__VA_ARGS__)
#else
#define DELOCUST_LOG(moduleName, ...)
#endif

namespace utility
{
	namespace logging
	{
		// TODO: Make this enum scoped.
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
			using Outs = std::vector<std::ofstream>;
			/* ==================== Static variables and functions begin ==================== */
		protected:
			static std::map<std::string, std::unique_ptr<ILogger>> loggers;
			UTILITY_API static const std::array<std::string, COUNT> s_loggingLevelNames;
		public:
			UTILITY_API static ILogger& GetLogger(const std::string& moduleName);
			/* ==================== Static variables and functions end ==================== */

			/* ==================== Constructors and destructors begin ==================== */
			/// <summary> Base logger constructor. </summary>
			/// <param name="fileName">
			/// The name of the file name to be used to store the logs.
			/// May be <code>nullptr</code> to indicate that no external storage is supposed to be used to store the application logs.
			/// </param>
			explicit ILogger(const char* fileName = nullptr);

			/// <summary> Base logger destructor. </summary>
			virtual ~ILogger();

			/// <summary> Base logger copy constructor. </summary>
			/// <param name="logger"> The logger instance to copy construct from. </param>
			ILogger(const ILogger& logger) = delete;
			
			/// <summary> Base logger move constructor. </summary>
			/// <param name="logger"> The r-value reference to the logger instance to move construct from. </param>
			ILogger(ILogger&& logger) = delete;
			
			/// <summary> Base logger copy assignment operator. </summary>
			/// <param name="logger"> The reference to the logger instance to copy assign from. </param>
			/// <returns> The reference to the newly copy-assigned logger instance. </returns>
			ILogger& operator=(const ILogger& logger) = delete;

			/// <summary> Base logger move assignment operator. </summary>
			/// <param name="logger"> The r-value reference to the logger instance to move assign from. </param>
			/// <returns> The reference to the newly move-assigned logger instance. </returns>
			ILogger& operator=(ILogger&& logger) = delete;
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
			/// <summary> Returns the logging level currently set. </summary>
			/// <returns> The logging level. </returns>
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
			void SetLevel(const LogLevel level);
			virtual void SetConsoleColor(LogLevel level) const = 0;
			virtual void ReadConsoleColorsFromConfigFile() = 0;
		private:
			template <typename T, typename... Args>
			void LogRecursive(LogLevel level, const char* file, int line, std::ostringstream& msg, const T& value, const Args&... args)
			{
				msg << value;
				LogRecursive(level, file, line, msg, args...);
			}
			void LogRecursive(const LogLevel level, const char* file, const int line, std::ostringstream& msg)
			{
				//for (Outs::iterator outItr = m_outs.begin(); outItr != m_outs.end(); ++outItr)
				//{
				//	(*outItr) << "[" << LOGGING_LEVEL_NAMES[level] << "] [" << Utility::timing::Time::Now().ToDateString("%H:%M:%S") << "] " <<
				//		file << "(" << line << "): " << msg.str() << std::endl;
				//}
				std::cout << "[" << s_loggingLevelNames[level] << "] [" << timing::DateTime::Now().ToString("%H:%M:%S" /* TODO: Don't use hard-coded format */) << "] " <<
					file << "(" << line << "): " << msg.str() << std::endl;
			}
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
		protected:
			Outs m_outs;
			LogLevel m_level; // TODO: Compiler warning C4820: '4' bytes padding added after data member 'm_level'
			// TODO: Store console colors from configuration file
		/* ==================== Non-static member variables end ==================== */
		}; /* end class ILogger */
	} /* end namespace Logging */
} /* end namespace Utility */

#endif /* __UTILITY_ILOGGER_H__ */