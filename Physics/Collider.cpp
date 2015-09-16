#include "stdafx.h"
#include "Collider.h"
#include "BoundingSphere.h"
#include "Utility\ILogger.h"

using namespace Physics;

Collider::Collider(ColliderType type) :
	m_type(type)
{
}

Collider::~Collider()
{
}

IntersectInfo Collider::Intersect(const Collider& collider) const
{
	if ( (m_type == TYPE_SPHERE) && (collider.GetType() == TYPE_SPHERE) )
	{
		const BoundingSphere* self = dynamic_cast<const BoundingSphere*>(this);
		if (self == NULL)
		{
			CRITICAL_LOG("Casting collider to BoundingSphere pointer failed.");
			exit(EXIT_FAILURE);
		}
		return self->IntersectBoundingSphere((BoundingSphere&)collider);
	}

	CRITICAL_LOG("Only bounding spheres collision is currently handled by the physics engine. Other colliders are not yet supported.");
	return IntersectInfo(false, REAL_ZERO);
}