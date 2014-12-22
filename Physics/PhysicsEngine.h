#pragma once

#include "Physics.h"
#include "PhysicsObject.h"
#include <vector>

namespace Physics
{

/**
 * The physics engine
 */
class PhysicsEngine
{
/* ==================== Static variables begin ==================== */
/* ==================== Static variables end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	PhysicsEngine();
	~PhysicsEngine(void);
private:
	PhysicsEngine(const PhysicsEngine& physicsEngine) {} // don't implement
	void operator=(const PhysicsEngine& physicsEngine) {} // don't implement
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static, non-virtual member functions begin ==================== */
public:
	void AddPhysicsObject(const PhysicsObject& physicsObject);
	void Simulate(Math::Real delta);
	unsigned int GetPhysicsObjectsCount() const { return static_cast<unsigned int>(m_physicsObjects.size()); }
/* ==================== Non-static, non-virtual member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
private:
	std::vector<PhysicsObject> m_physicsObjects;
/* ==================== Non-static member variables end ==================== */
}; /* end class PhysicsEngine */

} /* end namespace Physics */