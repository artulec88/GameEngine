#pragma once

#ifdef UTILITY_EXPORTS
#define UTILITY_API __declspec(dllexport)
#else
#define UTILITY_API __declspec(dllimport)
#endif

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

} /* end namespace Utility */