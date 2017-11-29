#ifndef __ENGINE_GRAVITY_COMPONENT_H__
#define __ENGINE_GRAVITY_COMPONENT_H__

#include "Engine.h"
#include "GameComponent.h"
#include "IUpdateable.h"

#include "Rendering/Terrain.h"

#include "Math/Vector.h"

namespace engine
{
	class GameNode;

	class GravityComponent : public GameComponent, public IUpdateable
	{
		/* ==================== Static variables and functions begin ==================== */
		// TODO: Don't hard-code any value! Ever!
		// TODO: This variable should be moved into the Physics engine and be accessible from anywhere (similarily as stdlog is accessible).
		static constexpr math::Vector3D GRAVITY_ACCELERATION{ 0.0f, -9.8f, 0.0f };
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		ENGINE_API explicit GravityComponent(const rendering::Terrain* terrain);
		ENGINE_API virtual ~GravityComponent();
		GravityComponent(const GravityComponent& gravityComponent) = delete;
		GravityComponent(GravityComponent&& gravityComponent) noexcept;
		GravityComponent& operator=(const GravityComponent& gravityComponent) = delete;
		GravityComponent& operator=(GravityComponent&& gravityComponent) noexcept;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
		ENGINE_API void Update(math::Real deltaTime) override;
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	protected:
		const rendering::Terrain* m_terrain;
		math::Real m_lastX, m_lastZ, m_lastHeight;
		math::Real m_heightAdjustment;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class GravityComponent */

} /* end namespace engine */

#endif /* __ENGINE_GRAVITY_COMPONENT_H__ */