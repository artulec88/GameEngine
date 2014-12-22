#pragma once

#include "Physics.h"
#include "IntersectInfo.h"

namespace Physics
{

/**
 * The collider
 */
class Collider
{
public:
enum ColliderType
{
	TYPE_SPHERE,
	TYPE_AABB,

	TYPE_SIZE
};
private:

/* ==================== Static variables begin ==================== */
/* ==================== Static variables end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	Collider(ColliderType type);
	virtual ~Collider(void);
private:
	Collider(const Collider& collider) {} // don't implement
	void operator=(const Collider& collider) {} // don't implement
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static, non-virtual member functions begin ==================== */
public:
	inline ColliderType GetType() const { return m_type; }
	virtual IntersectInfo Intersect(const Collider& collider) const;
/* ==================== Non-static, non-virtual member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
private:
	ColliderType m_type;
/* ==================== Non-static member variables end ==================== */
}; /* end class Collider */

} /* end namespace Physics */