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
			/* ==================== Constructors and destructors begin ==================== */
		public:
			/// <summary>
			/// Windows-specific logger constructor.
			/// </summary>
			LoggerWindows(const char* fileName = NULL);
			/// <summary>
			/// Windows-specific logger destructor.
			/// </summary>
			~LoggerWindows();
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
		public:
			virtual void Fill(const std::string& strLevel, LogLevel level);
			virtual void ResetConsoleColor() const;
		protected:
			virtual void SetConsoleColor(LogLevel level) const;
			virtual void ReadConsoleColorsFromConfigFile();
		private:
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
		protected:
			int m_indentDepth; // TODO: Compiler warning C4820: '4' bytes padding added after data member m_indentDepth
			const char* m_dateTimeFormat;
			HANDLE m_console;
			bool m_modified; // TODO: Compiler warning C4820: '7' bytes padding added after data member m_modified
			/* ==================== Non-static member variables end ==================== */
		}; /* end class LoggerWindows */
	} /* end namespace Logging */
} /* end namespace Utility */