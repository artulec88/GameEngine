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
		ENGINE_API GravityComponent(Rendering::TerrainMesh const* terrainMesh);
		ENGINE_API virtual ~GravityComponent(void);
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		ENGINE_API virtual void Update(Math::Real deltaTime);
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	protected:
		Rendering::TerrainMesh const* m_terrainMesh;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class GravityComponent */

} /* end namespace Engine */

#endif /* __ENGINE_GRAVITY_COMPONENT_H__ */