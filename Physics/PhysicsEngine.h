#ifndef __PHYSICS_PHYSICS_ENGINE_H__
#define __PHYSICS_PHYSICS_ENGINE_H__

#include "Physics.h"
#include "PhysicsObject.h"

#ifdef PROFILING_PHYSICS_MODULE_ENABLED
#include "Math/Statistics.h"
#include "Math/StatisticsStorage.h"
#endif

#include <vector>

namespace physics
{

	/// <summary>
	/// The physics engine.
	/// </summary>
	class PhysicsEngine
	{
		using PhysicObjectsContainer = std::vector<PhysicsObject*>;
		/* ==================== Static variables begin ==================== */
		/* ==================== Static variables end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		/// <summary> Physics engine constructor. </summary>
		PHYSICS_API PhysicsEngine();

		/// <summary> Physics engine destructor. </summary>
		PHYSICS_API ~PhysicsEngine();

		/// <summary> The copy constructor of the physics engine. </summary>
		/// <param name="physicsEngine"> The physics engine to copy construct from. </param>
		PhysicsEngine(const PhysicsEngine& physicsEngine) = delete;

		/// <summary> The move constructor of the physics engine. </summary>
		/// <param name="physicsEngine"> The r-value reference of the physics engine to move construct from. </param>
		PhysicsEngine(PhysicsEngine&& physicsEngine) = delete;

		/// <summary> The copy assignment operator of the physics engine. </summary>
		/// <param name="physicsEngine"> The physics engine to copy assign from. </param>
		/// <returns> The reference to the newly copy-assigned physics engine. </returns>
		PhysicsEngine& operator=(const PhysicsEngine& physicsEngine) = delete;

		/// <summary> The move assignment operator of the physics engine. </summary>
		/// <param name="physicsEngine"> The r-value reference of the physics engine to move assign from. </param>
		/// <returns> The reference to the newly move-assigned physics engine. </returns>
		PhysicsEngine& operator=(PhysicsEngine&& physicsEngine) = delete;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static, non-virtual member functions begin ==================== */
			/// <summary> Adds physics object for the physics simulation process. </summary>
			/// <param name="physicsObject"> The physics object to be added to the physics engine. </param>
		PHYSICS_API void AddPhysicsObject(PhysicsObject* physicsObject);

		/// <summary>
		/// Simulates the physical behavior of all physical objects in the engine for a certain period of time.
		/// It does not take object interaction into account, so if two objects run into each other, they will pass through each other.
		/// If object interaction is desired, collision detection and response must be performed afterwards.
		/// </summary>
		/// <param name="passedTime">The amount of time to simulate the physics world.</param>
		PHYSICS_API void Simulate(math::Real passedTime);

		PHYSICS_API void HandleCollisions();

		/// <summary>
		/// Gives the number of physical objects being simulated by the physics engine.
		/// </summary>
		/// <returns> Number of physics object that we simulate. </returns>
		PHYSICS_API unsigned int GetPhysicsObjectsCount() const { return static_cast<unsigned int>(m_physicsObjects.size()); }
		/* ==================== Non-static, non-virtual member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	private:
		PhysicObjectsContainer m_physicsObjects;
#ifdef PROFILING_PHYSICS_MODULE_ENABLED
		math::statistics::ClassStats& m_classStats;
#endif
		/* ==================== Non-static member variables end ==================== */
	}; /* end class PhysicsEngine */

} /* end namespace physics */

#endif // __PHYSICS_PHYSICS_ENGINE_H__