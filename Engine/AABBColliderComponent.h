#ifndef __ENGINE_AABB_COLLIDER_COMPONENT_H__
#define __ENGINE_AABB_COLLIDER_COMPONENT_H__

#include "Engine.h"
#include "GameComponent.h"
//#include "IUpdateable.h"

//#include "Math\Math.h"
//#include "Math\Angle.h"
//#include "Math/Vector.h"
#include "Physics\AABBCollider.h"

namespace engine
{

	class AABBColliderComponent : public GameComponent
	{
	/* ==================== Static variables and functions begin ==================== */
	/* ==================== Static variables and functions end ==================== */

	/* ==================== Constructors and destructors begin ==================== */
	public:
		ENGINE_API AABBColliderComponent(const math::Vector3D& bottomLeftPos, const math::Vector3D& topRightPos);
		ENGINE_API virtual ~AABBColliderComponent(void);
		AABBColliderComponent(const AABBColliderComponent& aabbColliderComponent) = delete;
		ENGINE_API AABBColliderComponent(AABBColliderComponent&& aabbColliderComponent) = default;
		AABBColliderComponent& operator=(const AABBColliderComponent& aabbColliderComponent) = delete;
		ENGINE_API AABBColliderComponent& operator=(AABBColliderComponent&& aabbColliderComponent) = default;
	/* ==================== Constructors and destructors end ==================== */

	/* ==================== Non-static member functions begin ==================== */
	public:
		//ENGINE_API virtual void Update(math::Real deltaTime);
	/* ==================== Non-static member functions end ==================== */

	/* ==================== Non-static member variables begin ==================== */
	protected:
		Physics::AABBCollider m_aabbCollider;
	/* ==================== Non-static member variables end ==================== */
	}; /* end class AABBColliderComponent */

} /* end namespace Engine */

#endif /* __ENGINE_AABB_COLLIDER_COMPONENT_H__ */