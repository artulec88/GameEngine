#pragma once

//#pragma warning(disable:4251)

#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <sstream>
//#include <assert.h>
#include <iostream>
#include <list>
#include "Utility.h"
#include "ILogger.h"

namespace Utility
{
	class Logger : public ILogger
	{
	typedef std::list<FILE *> Outs;
	protected:
		Outs m_outs;
		bool m_modified;
		int m_indentDepth;
	public:
		Logger(FILE *first = stdout);
		~Logger();

		void operator()(LogLevel level, const char *name, int line, const char *format, ...);
		void Log(LogLevel level, const char *name, int line, const char *format, ...);
		void Fill(const std::string& strLevel, LogLevel level);
		void AddFile(const char *name);
	}; /* end class Logger */

} /* end namespace Utility */