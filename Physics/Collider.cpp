#include "stdafx.h"
#include "Collider.h"
#include "Math\Sphere.h"
#include "Utility\ILogger.h"

Physics::Collider::Collider(ColliderType type) :
	m_type(type)
{
}

Physics::Collider::~Collider()
{
}

math::IntersectInfo Physics::Collider::Intersect(const Collider& collider) const
{
	if ( (m_type == TYPE_SPHERE) && (collider.GetType() == TYPE_SPHERE) )
	{
		const math::Sphere* self = dynamic_cast<const math::Sphere*>(this);
		if (self == nullptr)
		{
			CRITICAL_LOG_PHYSICS("Casting collider to BoundingSphere pointer failed.");
			exit(EXIT_FAILURE);
		}
		return self->DoesIntersectSphere((math::Sphere&)collider);
	}

	CRITICAL_LOG_PHYSICS("Only bounding spheres collision is currently handled by the physics engine. Other colliders are not yet supported.");
	return math::IntersectInfo(REAL_ONE /* some arbitrary (but positive) value */);
}