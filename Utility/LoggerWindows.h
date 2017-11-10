#pragma once

//#pragma warning(disable:4251)

#include "Utility.h"
#include "ILogger.h"
#include <string>
#include <Windows.h>

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
			explicit LoggerWindows(const char* fileName = nullptr);
			/// <summary>
			/// Windows-specific logger destructor.
			/// </summary>
			virtual ~LoggerWindows();

			/// <summary> Windows logger copy constructor. </summary>
			LoggerWindows(const LoggerWindows& loggerWindows) = delete;
			/// <summary> Windows logger move constructor. </summary>
			LoggerWindows(LoggerWindows&& loggerWindows) = delete;
			/// <summary> Windows logger copy assignment operator. </summary>
			LoggerWindows& operator=(const LoggerWindows& loggerWindows) = delete;
			/// <summary> Windows logger move assignment operator. </summary>
			LoggerWindows& operator=(LoggerWindows&& loggerWindows) = delete;
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
		public:
			virtual void Fill(const std::string& strLevel, LogLevel level) override;
			virtual void ResetConsoleColor() const override;
		protected:
			virtual void SetConsoleColor(LogLevel level) const override;
			virtual void ReadConsoleColorsFromConfigFile() override;
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