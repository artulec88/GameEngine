#include "stdafx.h"
#include "PhysicsEngine.h"

using namespace Physics;

PhysicsEngine::PhysicsEngine()
{
}

PhysicsEngine::~PhysicsEngine()
{
}

void PhysicsEngine::AddPhysicsObject(const PhysicsObject& physicsObject)
{
	m_physicsObjects.push_back(physicsObject);
}

void PhysicsEngine::Simulate(Math::Real delta)
{
	for (std::vector<PhysicsObject>::iterator physicsObjectItr = m_physicsObjects.begin(); physicsObjectItr != m_physicsObjects.end(); ++physicsObjectItr)
	{
		(*physicsObjectItr).Integrate(delta);
	}
}