#ifndef __CORE_CORE_H__
#define __CORE_CORE_H__

#ifdef CORE_EXPORTS
#define CORE_API __declspec(dllexport)
#else
#define CORE_API __declspec(dllimport)
#endif

#include "stdafx.h"

#define CALCULATE_CORE_STATS

#ifndef CALCULATE_CORE_STATS
#define START_PROFILING_STATIC
#define STOP_PROFILING_STATIC
#define START_PROFILING
#define STOP_PROFILING
#endif

namespace Core
{
	namespace GameTime
	{
		enum Daytime
		{
			NIGHT = 0,
			BEFORE_DAWN,
			SUNRISE,
			DAY,
			SUNSET,
			AFTER_DUSK
		};
	}
} /* end namespace Core */

#endif /* __CORE_CORE_H__ */