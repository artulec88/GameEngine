#ifndef __ENGINE_TERRAIN_COLLISION_COMPONENT_H__
#define __ENGINE_TERRAIN_COLLISION_COMPONENT_H__

#include "Engine.h"
#include "GameComponent.h"
#include "IUpdateable.h"

#include "Rendering\Mesh.h"

#include "Math\Vector.h"

#include "Physics\PhysicsObject.h"

namespace Engine
{
	class GameNode;

	class TerrainCollisionComponent : public GameComponent, public IUpdateable
	{
	/* ==================== Static variables and functions begin ==================== */
	private:
		// TODO: Move this class to physics engine. The same static variable is defined in GravityComponent.
		static const Math::Vector3D GRAVITY_ACCELERATION;
	/* ==================== Static variables and functions end ==================== */

	/* ==================== Constructors and destructors begin ==================== */
	public:
		ENGINE_API TerrainCollisionComponent(Physics::PhysicsObject* physicsObject, Rendering::TerrainMesh* terrainMesh);
		ENGINE_API virtual ~TerrainCollisionComponent(void);
	/* ==================== Constructors and destructors end ==================== */

	/* ==================== Non-static member functions begin ==================== */
	public:
		ENGINE_API virtual void Update(Math::Real deltaTime);
	/* ==================== Non-static member functions end ==================== */

	/* ==================== Non-static member variables begin ==================== */
	protected:
		Physics::PhysicsObject* m_physicsObject;
		Rendering::TerrainMesh* m_terrainMesh;
		bool m_isTouchingTerrain;
	/* ==================== Non-static member variables end ==================== */
	}; /* end class TerrainCollisionComponent */

} /* end namespace Engine */

#endif /* __ENGINE_TERRAIN_COLLISION_COMPONENT_H__ */