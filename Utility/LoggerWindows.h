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
	namespace Logging
	{
		class LoggerWindows : public ILogger
		{
			typedef std::list<FILE *> Outs;
			/* ==================== Constructors and destructors begin ==================== */
		public:
			/// <summary>
			/// Windows-specific logger constructor.
			/// </summary>
			LoggerWindows(FILE *first = stdout);
			/// <summary>
			/// Windows-specific logger destructor.
			/// </summary>
			~LoggerWindows();
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
		public:
			virtual void Fill(const std::string& strLevel, LogLevel level);
			virtual void AddFile(const char *name);
			virtual void ResetConsoleColor() const;
		protected:
			virtual void SetConsoleColor(LogLevel level) const;
			virtual void ReadConsoleColorsFromConfigFile();
		private:
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
		protected:
			Outs m_outs;
			bool m_modified;
			int m_indentDepth;
			const char* m_dateTimeFormat;
			HANDLE m_console;
			/* ==================== Non-static member variables end ==================== */
		}; /* end class LoggerWindows */
	} /* end namespace Logging */
} /* end namespace Utility */