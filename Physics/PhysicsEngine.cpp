#include "stdafx.h"
#include "PhysicsEngine.h"
#include "Math/IntersectInfo.h"
#include "Utility/IConfig.h"

physics::PhysicsEngine::PhysicsEngine() :
#ifdef PROFILING_PHYSICS_MODULE_ENABLED
	m_classStats(STATS_STORAGE.GetClassStats("PhysicsEngine"))
#endif
{
	START_PROFILING_PHYSICS(true, "");
	NOTICE_LOG_PHYSICS("Physics engine created");
	STOP_PROFILING_PHYSICS("");
}

physics::PhysicsEngine::~PhysicsEngine()
{
}

void physics::PhysicsEngine::AddPhysicsObject(PhysicsObject* physicsObject)
{
	m_physicsObjects.push_back(physicsObject);
}

void physics::PhysicsEngine::Simulate(math::Real passedTime)
{
	for (auto physicsObjectItr = m_physicsObjects.begin(); physicsObjectItr != m_physicsObjects.end(); ++physicsObjectItr)
	{
		(*physicsObjectItr)->Simulate(passedTime);
	}
}

void physics::PhysicsEngine::HandleCollisions()
{
	// TODO: Sounds like a template design pattern (?)
	// 1. Check which objects collide with each other
	// 2. For objects that collide perform the collision response.

	for (auto physicsObjectItr = m_physicsObjects.begin(); physicsObjectItr != m_physicsObjects.end(); ++physicsObjectItr)
	{
		for (auto otherPhysicsObjectItr = physicsObjectItr + 1; otherPhysicsObjectItr != m_physicsObjects.end(); ++otherPhysicsObjectItr)
		{
			math::IntersectInfo intersectInfo = (*physicsObjectItr)->Intersect(**otherPhysicsObjectItr);
			if (intersectInfo.IsIntersecting())
			{
				DEBUG_LOG_PHYSICS("The objects collided"); // TODO: Better log message
				(*physicsObjectItr)->GetLinearVelocity().Negate();
				(*otherPhysicsObjectItr)->GetLinearVelocity().Negate();
			}
		}
	}
}