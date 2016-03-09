#pragma once

#include "Physics.h"
#include "PhysicsObject.h"

#ifdef CALCULATE_PHYSICS_STATS
#include "Math\Statistics.h"
#include "Math\IStatisticsStorage.h"
#endif

#include <vector>

namespace Physics
{

	/// <summary>
	/// The physics engine.
	/// </summary>
	class PhysicsEngine
	{
	/* ==================== Static variables begin ==================== */
	/* ==================== Static variables end ==================== */

	/* ==================== Constructors and destructors begin ==================== */
	public:
		PHYSICS_API PhysicsEngine();
		PHYSICS_API ~PhysicsEngine(void);
	private:
		PhysicsEngine(const PhysicsEngine& physicsEngine);
		void operator=(const PhysicsEngine& physicsEngine);
	/* ==================== Constructors and destructors end ==================== */

	/* ==================== Non-static, non-virtual member functions begin ==================== */
	public:
		PHYSICS_API void AddPhysicsObject(const PhysicsObject& physicsObject);
		PHYSICS_API void Simulate(Math::Real delta);
		PHYSICS_API unsigned int GetPhysicsObjectsCount() const { return static_cast<unsigned int>(m_physicsObjects.size()); }
	/* ==================== Non-static, non-virtual member functions end ==================== */

	/* ==================== Non-static member variables begin ==================== */
	private:
		std::vector<PhysicsObject> m_physicsObjects;
#ifdef CALCULATE_PHYSICS_STATS
		Math::Statistics::ClassStats& m_classStats;
#endif
	/* ==================== Non-static member variables end ==================== */
	}; /* end class PhysicsEngine */

} /* end namespace Physics */