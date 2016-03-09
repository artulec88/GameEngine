#pragma once

#ifdef PHYSICS_EXPORTS
#define PHYSICS_API __declspec(dllexport)
#else
#define PHYSICS_API __declspec(dllimport)
#endif

#define CALCULATE_PHYSICS_STATS

namespace Physics
{
} /* end namespace Physics */