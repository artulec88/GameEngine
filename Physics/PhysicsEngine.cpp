#include "stdafx.h"
#include "PhysicsEngine.h"
#include "Utility\Utility.h"

Physics::PhysicsEngine::PhysicsEngine() :
#ifdef CALCULATE_PHYSICS_STATS
	m_classStats(STATS_STORAGE.GetClassStats("PhysicsEngine"))
#endif
{
	START_PROFILING;
	NOTICE_LOG("Creating Physics engine instance started");
	NOTICE_LOG("Creating Physics engine instance finished");
	STOP_PROFILING;
}

Physics::PhysicsEngine::~PhysicsEngine()
{
}

void Physics::PhysicsEngine::AddPhysicsObject(const PhysicsObject& physicsObject)
{
	m_physicsObjects.push_back(physicsObject);
}

void Physics::PhysicsEngine::Simulate(Math::Real delta)
{
	for (std::vector<PhysicsObject>::iterator physicsObjectItr = m_physicsObjects.begin(); physicsObjectItr != m_physicsObjects.end(); ++physicsObjectItr)
	{
		(*physicsObjectItr).Integrate(delta);
	}
}