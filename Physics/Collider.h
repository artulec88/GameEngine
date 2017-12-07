#ifndef __PHYSICS_COLLIDER_H__
#define __PHYSICS_COLLIDER_H__

namespace math
{
	class IntersectInfo;
} /* end namespace math */

namespace physics
{
	namespace collider_types
	{
		enum ColliderType
		{
			TYPE_SPHERE,
			TYPE_AABB,

			TYPE_SIZE
		};
	} /* end namespace collider_types */

	/// <summary>
	/// The representation of the physical collider object.
	/// </summary>
	class Collider
	{
		/* ==================== Static variables begin ==================== */
		/* ==================== Static variables end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		explicit Collider(collider_types::ColliderType type);
		virtual ~Collider();

		/// <summary> The copy constructor of the collider. </summary>
		/// <param name="collider"> The collider to copy construct from. </param>
		Collider(const Collider& collider) = delete;

		/// <summary> The move constructor of the collider. </summary>
		/// <param name="collider"> The r-value reference of the collider to move construct from. </param>
		Collider(Collider&& collider) = delete;

		/// <summary> The copy assignment operator of the collider. </summary>
		/// <param name="collider"> The collider to copy assign from. </param>
		/// <returns> The reference to the newly copy-assigned <code>Collider</code> object. </returns>
		Collider& operator=(const Collider& collider) = delete;

		/// <summary> The move assignment operator of the collider. </summary>
		/// <param name="collider"> The r-value reference of the collider to move assign from. </param>
		/// <returns> The reference to the newly move-assigned <code>Collider</code> object. </returns>
		Collider& operator=(Collider&& collider) = delete;
	/* ==================== Constructors and destructors end ==================== */

	/* ==================== Non-static, non-virtual member functions begin ==================== */
		collider_types::ColliderType GetType() const { return m_type; }
		virtual math::IntersectInfo Intersect(const Collider& collider) const;
		/* ==================== Non-static, non-virtual member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	private:
		collider_types::ColliderType m_type;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class Collider */

} /* end namespace physics */

#endif // __PHYSICS_COLLIDER_H__