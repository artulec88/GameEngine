#ifndef __ENGINE_AABB_COLLIDER_COMPONENT_H__
#define __ENGINE_AABB_COLLIDER_COMPONENT_H__

#include "Engine.h"
#include "GameComponent.h"
//#include "IUpdateable.h"

//#include "Math/Math.h"
//#include "Math/Angle.h"
//#include "Math/Vector.h"
#include "Physics\AABBCollider.h"

namespace engine
{

	class AabbColliderComponent : public GameComponent
	{
	/* ==================== Static variables and functions begin ==================== */
	/* ==================== Static variables and functions end ==================== */

	/* ==================== Constructors and destructors begin ==================== */
	public:
		ENGINE_API AabbColliderComponent(const math::Vector3D& bottomLeftPos, const math::Vector3D& topRightPos);
		ENGINE_API virtual ~AabbColliderComponent();
		AabbColliderComponent(const AabbColliderComponent& aabbColliderComponent) = delete;
		ENGINE_API AabbColliderComponent(AabbColliderComponent&& aabbColliderComponent) = default;
		AabbColliderComponent& operator=(const AabbColliderComponent& aabbColliderComponent) = delete;
		ENGINE_API AabbColliderComponent& operator=(AabbColliderComponent&& aabbColliderComponent) = default;
	/* ==================== Constructors and destructors end ==================== */

	/* ==================== Non-static member functions begin ==================== */
		//ENGINE_API void Update(math::Real deltaTime) override;
	/* ==================== Non-static member functions end ==================== */

	/* ==================== Non-static member variables begin ==================== */
	protected:
		physics::AabbCollider m_aabbCollider;
	/* ==================== Non-static member variables end ==================== */
	}; /* end class AABBColliderComponent */

} /* end namespace engine */

#endif /* __ENGINE_AABB_COLLIDER_COMPONENT_H__ */