#pragma once

#include <vector>
#include <string>
#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>

#ifdef UTILITY_EXPORTS
#define UTILITY_API __declspec(dllexport)
#else
#define UTILITY_API __declspec(dllimport)
#endif

#ifdef NDEBUG
#ifdef LOGMODULE
#define LOGPLACE LOGMODULE, 0 
#else
#define LOGPLACE "unknown-module", 0 
#endif
#else
#define LOGPLACE __FILE__, __LINE__
#endif

#ifdef _DEBUG
#include <string.h>
#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__) // For UNIX change '\\' to '/'
#else
#define __FILENAME__ __FILE__
#endif

//#ifdef _DEBUG
#define MEASURE_TIME_ENABLED
//#endif

#ifdef _DEBUG
#define ASSERT(expr) if (expr) { } else { printf("ASSERT in file \"%s\" in line \"%d\"\n", __FILENAME__, __LINE__); /*stdlog(Critical, LOGPLACE, "Error occurred");*/ }
#define SLOW_ASSERTIONS_ENABLED
#ifdef SLOW_ASSERTIONS_ENABLED
#define SLOW_ASSERT(expr) ASSERT(expr)
#else
#define SLOW_ASSERT(expr)
#endif
#else /* _DEBUG */
#define ASSERT(expr)
#define SLOW_ASSERT(expr)
#endif /* _DEBUG */

#define INVALID_VALUE 0xFFFFFFFF

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


	// trim from start
	//void UTILITY_API LeftTrim(const std::string& s, std::string& result);
	std::string& LeftTrim(std::string& s);
	// trim from end
	//void UTILITY_API RightTrim(const std::string& s, std::string& result);
	std::string& RightTrim(std::string& s);
	// trim from both ends
	//void UTILITY_API Trim(const std::string& s, std::string& result);
	std::string& Trim(std::string& s);

	/**
	 * @brief Cut the string into tokens.
	 *
	 * Tokens in str are separated by some number of spaces. 
	 * Tokens are added to the end of tokens vector -- it is not cleared at the beginning.
	 * Notably, if str == "" or "  " there will be no tokens added to vector.
	 * All the spaces are removed.
	 * 
	 * @param str string containing tokens separated by spaces
	 * @param tokens vector to which the tokes are added
	 */
	void UTILITY_API CutToTokens(const std::string& str, std::vector<std::string>& tokens, const char delim);
	void UTILITY_API CutToTokens(const std::string& str, std::vector<std::string>& tokens, const char* delim, int delimCount);
} /* end namespace Utility */