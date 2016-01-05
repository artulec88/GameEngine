#pragma once

#include "Utility.h"
#include <Windows.h>

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
#define CRITICAL_LOG(message, ...) Utility::ILogger::GetLogger().Log(Utility::Critical, LOGPLACE, message, ##__VA_ARGS__)
#else
#define CRITICAL_LOG(message, ...)
#endif

#ifdef EMERGENCY_LOGGING_ENABLED
#define EMERGENCY_LOG(message, ...) Utility::ILogger::GetLogger().Log(Utility::Emergency, LOGPLACE, message, ##__VA_ARGS__)
#else
#define EMERGENCY_LOG(message, ...)
#endif

#ifdef ERROR_LOGGING_ENABLED
#define ERROR_LOG(message, ...) Utility::ILogger::GetLogger().Log(Utility::Error, LOGPLACE, message, ##__VA_ARGS__)
#else
#define ERROR_LOG(message, ...)
#endif

#ifdef WARNING_LOGGING_ENABLED
#define WARNING_LOG(message, ...) Utility::ILogger::GetLogger().Log(Utility::Warning, LOGPLACE, message, ##__VA_ARGS__)
#else
#define WARNING_LOG(message, ...)
#endif

#ifdef NOTICE_LOGGING_ENABLED
#define NOTICE_LOG(message, ...) Utility::ILogger::GetLogger().Log(Utility::Notice, LOGPLACE, message, ##__VA_ARGS__)
#else
#define NOTICE_LOG(message, ...)
#endif

#ifdef INFO_LOGGING_ENABLED
#define INFO_LOG(message, ...) Utility::ILogger::GetLogger().Log(Utility::Info, LOGPLACE, message, ##__VA_ARGS__)
#else
#define INFO_LOG(message, ...)
#endif

#ifdef DEBUG_LOGGING_ENABLED
#define DEBUG_LOG(message, ...) Utility::ILogger::GetLogger().Log(Utility::Debug, LOGPLACE, message, ##__VA_ARGS__)
#else
#define DEBUG_LOG(message, ...)
#endif

#ifdef DELOCUST_LOGGING_ENABLED
#define DELOCUST_LOG(message, ...) Utility::ILogger::GetLogger().Log(Utility::Delocust, LOGPLACE, message, ##__VA_ARGS__)
#else
#define DELOCUST_LOG(message, ...)
#endif

namespace Utility
{

enum LogLevel
{
	Critical = 0,
	Emergency = 1,
	Error = 2,
	Warning = 3,
	Notice = 4,
	Info = 5,
	Debug = 6,
	Delocust = 7,

	DevNull = 100
}; /* end enum LogLevel */

class ILogger
{
/* ==================== Static variables and functions begin ==================== */
protected:
	static ILogger* logger;
	static const char* LevelNames[];
public:
	UTILITY_API static ILogger& GetLogger(/* some parameter based on which the logger implementation class is chosen */);
	//static void Log(LogLevel level, const char *name, int line, const char *format, ...);
/* ==================== Static variables and functions end ==================== */

/* ==================== Constructors and destructors begin ==================== */
protected:
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
	UTILITY_API void ResetConsoleColor() const;
protected:
	void SetLevel(LogLevel level);
	void SetConsoleColor(LogLevel level) const;
	void ReadConsoleColorsFromConfigFile();
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
protected:
	LogLevel m_level;
	HANDLE m_console; // only in Windows platform
	// TODO: Store console colors from configuration file
/* ==================== Non-static member variables end ==================== */
}; /* end class ILogger */

} /* end namespace Utility */
