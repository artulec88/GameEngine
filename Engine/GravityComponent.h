#ifndef __ENGINE_GRAVITY_COMPONENT_H__
#define __ENGINE_GRAVITY_COMPONENT_H__

#include "Engine.h"
#include "GameComponent.h"
#include "IUpdateable.h"

#include "Rendering\Mesh.h"

#include "Math\Vector.h"

namespace Engine
{
	class GameNode;

	class GravityComponent : public GameComponent, public IUpdateable
	{
		/* ==================== Static variables and functions begin ==================== */
	private:
		static const Math::Vector3D GRAVITY_ACCELERATION;
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		ENGINE_API GravityComponent(const Rendering::TerrainMesh* terrainMesh);
		ENGINE_API virtual ~GravityComponent(void);
		GravityComponent(const GravityComponent& gravityComponent) = delete;
		GravityComponent(GravityComponent&& gravityComponent);
		GravityComponent& operator=(const GravityComponent& gravityComponent) = delete;
		GravityComponent& operator=(GravityComponent&& gravityComponent);
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		ENGINE_API virtual void Update(Math::Real deltaTime);
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	protected:
		const Rendering::TerrainMesh* m_terrainMesh;
		Math::Real m_lastX, m_lastZ, m_lastHeight;
		Math::Real m_heightAdjustment;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class GravityComponent */

} /* end namespace Engine */

#endif /* __ENGINE_GRAVITY_COMPONENT_H__ */