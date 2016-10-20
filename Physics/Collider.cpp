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

Math::IntersectInfo Physics::Collider::Intersect(const Collider& collider) const
{
	if ( (m_type == TYPE_SPHERE) && (collider.GetType() == TYPE_SPHERE) )
	{
		const Math::Sphere* self = dynamic_cast<const Math::Sphere*>(this);
		if (self == nullptr)
		{
			CRITICAL_LOG_PHYSICS("Casting collider to BoundingSphere pointer failed.");
			exit(EXIT_FAILURE);
		}
		return self->DoesIntersectSphere((Math::Sphere&)collider);
	}

	CRITICAL_LOG_PHYSICS("Only bounding spheres collision is currently handled by the physics engine. Other colliders are not yet supported.");
	return Math::IntersectInfo(false, REAL_ZERO);
}