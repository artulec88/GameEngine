#include "stdafx.h"
#include "PhysicsEngine.h"
#include "Math\IntersectInfo.h"
#include "Utility\IConfig.h"

Physics::PhysicsEngine::PhysicsEngine() :
#ifdef CALCULATE_PHYSICS_STATS
	m_classStats(STATS_STORAGE.GetClassStats("PhysicsEngine"))
#endif
{
	START_PROFILING;
	/* ==================== Initializing physics logger begin ==================== */
	std::string loggingLevel = GET_CONFIG_VALUE_STR_PHYSICS("LoggingLevel", "Info");
	Utility::ILogger::GetLogger("Physics").Fill(loggingLevel, Utility::INFO);
	/* ==================== Initializing physics logger end ==================== */

	NOTICE_LOG_PHYSICS("Physics engine created");
	STOP_PROFILING;
}

Physics::PhysicsEngine::~PhysicsEngine()
{
}

void Physics::PhysicsEngine::AddPhysicsObject(PhysicsObject* physicsObject)
{
	m_physicsObjects.push_back(physicsObject);
}

void Physics::PhysicsEngine::Simulate(Math::Real passedTime)
{
	for (PhysicObjectsContainer::iterator physicsObjectItr = m_physicsObjects.begin(); physicsObjectItr != m_physicsObjects.end(); ++physicsObjectItr)
	{
		(*physicsObjectItr)->Simulate(passedTime);
	}
}

void Physics::PhysicsEngine::HandleCollisions()
{
	// TODO: Sounds like a template design pattern (?)
	// 1. Check which objects collide with each other
	// 2. For objects that collide perform the collision response.

	for (PhysicObjectsContainer::iterator physicsObjectItr = m_physicsObjects.begin(); physicsObjectItr != m_physicsObjects.end(); ++physicsObjectItr)
	{
		for (PhysicObjectsContainer::iterator otherPhysicsObjectItr = physicsObjectItr + 1; otherPhysicsObjectItr != m_physicsObjects.end(); ++otherPhysicsObjectItr)
		{
			Math::IntersectInfo intersectInfo = (*physicsObjectItr)->Intersect(*(*otherPhysicsObjectItr));
			if (intersectInfo.IsIntersecting())
			{
				DEBUG_LOG_PHYSICS("The objects collided"); // TODO: Better log message
				(*physicsObjectItr)->GetLinearVelocity().Negate();
				(*otherPhysicsObjectItr)->GetLinearVelocity().Negate();
			}
		}
	}
}