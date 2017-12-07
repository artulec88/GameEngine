#include "stdafx.h"
#include "Collider.h"
#include "Physics.h"

#include "Math/Sphere.h"
#include "Math/IntersectInfo.h"

#include "Utility/ILogger.h"

physics::Collider::Collider(collider_types::ColliderType type) :
	m_type(type)
{
}

physics::Collider::~Collider()
{
}

math::IntersectInfo physics::Collider::Intersect(const Collider& collider) const
{
	if (collider_types::TYPE_SPHERE == m_type && collider_types::TYPE_SPHERE == collider.GetType() )
	{
		const math::Sphere* self = dynamic_cast<const math::Sphere*>(this);
		if (self == nullptr)
		{
			CRITICAL_LOG_PHYSICS("Casting collider to BoundingSphere pointer failed.");
			exit(EXIT_FAILURE);
		}
		//return self->DoesIntersectSphere(static_cast<math::Sphere>(collider));
	}

	CRITICAL_LOG_PHYSICS("Only bounding spheres collision is currently handled by the physics engine. Other colliders are not yet supported.");
	return math::IntersectInfo(REAL_ONE /* some arbitrary (but positive) value */);
}