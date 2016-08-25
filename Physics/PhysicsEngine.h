#ifndef __PHYSICS_PHYSICS_ENGINE_H__
#define __PHYSICS_PHYSICS_ENGINE_H__

#include "Physics.h"
#include "PhysicsObject.h"

#ifdef PROFILING_PHYSICS_MODULE_ENABLED
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
		/// <summary>
		/// Adds physics object for the physics simulation process.
		/// </summary>
		PHYSICS_API void AddPhysicsObject(PhysicsObject* physicsObject);

		/// <summary>
		/// Simulates the physical behavior of all physical objects in the engine for a certain period of time.
		/// It does not take object interaction into account, so if two objects run into each other, they will pass through each other.
		/// If object interaction is desired, collision detection and response must be performed afterwards.
		/// </summary>
		/// <param name="passedTime">The amount of time to simulate the physics world.</param>
		PHYSICS_API void Simulate(Math::Real passedTime);

		PHYSICS_API void HandleCollisions();

		/// <summary>
		/// Gives the number of physical objects being simulated by the physics engine.
		/// </summary>
		/// <returns> Number of physics object that we simulate. </returns>
		PHYSICS_API unsigned int GetPhysicsObjectsCount() const { return static_cast<unsigned int>(m_physicsObjects.size()); }
	/* ==================== Non-static, non-virtual member functions end ==================== */

	/* ==================== Non-static member variables begin ==================== */
	private:
		typedef std::vector<PhysicsObject*> PhysicObjectsContainer;
		PhysicObjectsContainer m_physicsObjects;
#ifdef PROFILING_PHYSICS_MODULE_ENABLED
		Math::Statistics::ClassStats& m_classStats;
#endif
	/* ==================== Non-static member variables end ==================== */
	}; /* end class PhysicsEngine */

} /* end namespace Physics */

#endif // __PHYSICS_PHYSICS_ENGINE_H__