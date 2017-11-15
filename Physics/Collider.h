#ifndef __PHYSICS_COLLIDER_H__
#define __PHYSICS_COLLIDER_H__

#include "Physics.h"
#include "Math\IntersectInfo.h"

namespace Physics
{

	/// <summary>
	/// The representation of the physical collider object.
	/// </summary>
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
		virtual math::IntersectInfo Intersect(const Collider& collider) const;
	/* ==================== Non-static, non-virtual member functions end ==================== */

	/* ==================== Non-static member variables begin ==================== */
	private:
		ColliderType m_type;
	/* ==================== Non-static member variables end ==================== */
	}; /* end class Collider */

} /* end namespace Physics */

#endif // __PHYSICS_COLLIDER_H__