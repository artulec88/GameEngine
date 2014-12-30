#pragma once

#include <iostream>
#include "Utility.h"
#include <Windows.h>

// TODO: Doesn't work. The #define keyword cannot replace parts of the expression.
#define LOG Utility::ILogger::GetLogger().Log

//#define LOG(level, message) Utility::LOG(level, LOGPLACE, message)

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

class UTILITY_API ILogger
{
/* ==================== Static variables and functions begin ==================== */
protected:
	static ILogger* logger;
	static const char* LevelNames[];
public:
	static ILogger& GetLogger(/* some parameter based on which the logger implementation class is chosen */);
	//static void Log(LogLevel level, const char *name, int line, const char *format, ...);
/* ==================== Static variables and functions end ==================== */

/* ==================== Constructors and destructors begin ==================== */
protected:
	ILogger();
	virtual ~ILogger(void);
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	LogLevel GetLevel() const { return level; }
	virtual void operator()(LogLevel level, const char *name, int line, const char *format, ...) = 0;
	virtual void Log(LogLevel level, const char *name, int line, const char *format, ...) = 0;
	virtual void Fill(const std::string& strLevel, LogLevel level) = 0;
	virtual void AddFile(const char *name) = 0;
	void ResetConsoleColor() const;
protected:
	inline void SetLevel(LogLevel level);
	inline void SetConsoleColor(LogLevel level) const;
	void ReadConsoleColorsFromConfigFile();
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
protected:
	LogLevel level;
	HANDLE console;
	// TODO: Store console colors from configuration file
/* ==================== Non-static member variables end ==================== */
}; /* end class ILogger */

inline void ILogger::SetLevel(LogLevel level)
{
	//mutex.Lock();
	//if ((!m_modified) || (level > this->m_level))
	//{
	//	std::cout << "Logging level set to " << LevelNames[level] << std::endl;
	//	this->m_level = level;
	//}
	//m_modified = true;
	//mutex.Unlock();
	std::cout << "Logging level set to " << LevelNames[level] << std::endl;
	this->level = level;
}

} /* end namespace Utility */
