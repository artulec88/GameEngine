#ifndef __PHYSICS_PHYSICS_H__
#define __PHYSICS_PHYSICS_H__

#ifdef PHYSICS_EXPORTS
#define PHYSICS_API __declspec(dllexport)
#else
#define PHYSICS_API __declspec(dllimport)
#endif

#define CALCULATE_PHYSICS_STATS

namespace Physics
{
} /* end namespace Physics */

#endif // __PHYSICS_PHYSICS_H__