#pragma once

//#pragma warning(disable:4251)

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <sstream>
#include <iostream>
#include <list>
#include "Utility.h"
#include "ILogger.h"

namespace Utility
{
	class Logger : public ILogger
	{
	typedef std::list<FILE *> Outs;
	/* ==================== Constructors and destructors begin ==================== */
	public:
		Logger(FILE *first = stdout);
		~Logger();
	/* ==================== Constructors and destructors end ==================== */
	
	/* ==================== Non-static member functions begin ==================== */
	public:
		//void operator()(LogLevel level, const char *name, int line, const char *format, ...);
		void Log(LogLevel level, const char *name, int line, const char *format, ...);
		void Fill(const std::string& strLevel, LogLevel level);
		void AddFile(const char *name);
	/* ==================== Non-static member functions end ==================== */

	/* ==================== Non-static member variables begin ==================== */
	protected:
		Outs m_outs;
		bool m_modified;
		int m_indentDepth;
	/* ==================== Non-static member variables end ==================== */
	}; /* end class Logger */

} /* end namespace Utility */