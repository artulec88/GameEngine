#pragma once

#include "Utility.h"
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <sstream>
//#include <assert.h>
#include <iostream>
#include <list>

namespace Utility
{
	class UTILITY_API Log
	{
	public:
		typedef std::list<FILE *> Outs;
		static const char *LevelNames[];

	protected:
		LogLevel m_level;
		Outs m_outs;
		bool m_modified;
	public:
		Log(FILE *first = stdout);
		~Log();

		LogLevel GetLevel() const;
		void operator()(LogLevel level, const char *name, int line, const char *format, ...);

		void Fill(const std::string& strLevel, LogLevel level);
		void AddFile(const char *name);
	private:
		void SetLevel(LogLevel level);
	};

} /* end namespace Utils */

UTILITY_API extern Utility::Log stdlog;