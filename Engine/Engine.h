#ifndef __ENGINE_ENGINE_H__
#define __ENGINE_ENGINE_H__

#ifdef ENGINE_EXPORTS
#define ENGINE_API __declspec(dllexport)
#else
#define ENGINE_API __declspec(dllimport)
#endif

#include "stdafx.h"

#define CALCULATE_CORE_STATS

#ifndef CALCULATE_CORE_STATS
#define START_PROFILING_STATIC
#define STOP_PROFILING_STATIC
#define START_PROFILING
#define STOP_PROFILING
#endif

namespace ENGINE
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
} /* end namespace Engine */

#endif /* __ENGINE_ENGINE_H__ */