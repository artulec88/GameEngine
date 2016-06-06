#pragma once

//#pragma warning(disable:4251)

#include "Utility.h"
#include "ILogger.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <sstream>
#include <iostream>
#include <Windows.h>
#include <list>

namespace Utility
{
	class LoggerWindows : public ILogger
	{
	typedef std::list<FILE *> Outs;
	/* ==================== Constructors and destructors begin ==================== */
	public:
		LoggerWindows(FILE *first = stdout);
		~LoggerWindows();
	/* ==================== Constructors and destructors end ==================== */
	
	/* ==================== Non-static member functions begin ==================== */
	public:
		//void operator()(LogLevel level, const char *name, int line, const char *format, ...);
		void Log(LogLevel level, const char *name, int line, const char *format, ...);
		void Fill(const std::string& strLevel, LogLevel level);
		void AddFile(const char *name);
		virtual void ResetConsoleColor() const;
	protected:
		virtual void SetConsoleColor(LogLevel level) const;
		virtual void ReadConsoleColorsFromConfigFile();
	/* ==================== Non-static member functions end ==================== */

	/* ==================== Non-static member variables begin ==================== */
	protected:
		Outs m_outs;
		bool m_modified;
		int m_indentDepth;
		const char* m_dateTimeFormat;
		HANDLE m_console;
	/* ==================== Non-static member variables end ==================== */
	}; /* end class Logger */

} /* end namespace Utility */