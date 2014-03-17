#pragma once

#include <vector>
#include <string>

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
#define ASSERT(expr) if (expr) { } else { printf("ASSERT in file \"%s\" in line \"%d\"\n", __FILE__, __LINE__); /*stdlog(Critical, LOGPLACE, "Error occurred");*/ }
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
	void CutToTokens(const std::string& str, std::vector<std::string>& tokens);
} /* end namespace Utility */