#ifndef __MATH_SURFACE_H__
#define __MATH_SURFACE_H__

#include "Math.h"
#include "Vector.h"

namespace Math
{

	/// <summary>
	/// The mathematical representation of the surface.
	/// </summary>
	class Surface
	{
		/* ==================== Static variables begin ==================== */
		/* ==================== Static variables end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		/// <summary>
		/// The constructor of the surface.
		/// </summary>
		MATH_API Surface();

		/// <summary>
		/// Surface copy constructor.
		/// </summary>
		MATH_API Surface(const Surface& surface) = default;

		/// <summary>
		/// Surface move constructor.
		/// </summary>
		MATH_API Surface(Surface&& surface) = default;

		/// <summary>
		/// Surface copy assignment operator.
		/// </summary>
		MATH_API Surface& operator=(const Surface& surface) = default;

		/// <summary>
		/// Surface move assignment operator.
		/// </summary>
		MATH_API Surface& operator=(Surface&& surface) = default;

		/// <summary>
		/// The destructor of the surface.
		/// </summary>
		MATH_API ~Surface(void);
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static, non-virtual member functions begin ==================== */
	public:
		MATH_API Real GetHeightAt(const Vector2D& xz) const { return GetHeightAt(xz.x, xz.y); }
		MATH_API Real GetHeightAt(Real x, Real y) const
		{
			// TODO: Implement this function
			return x + y;
		}
		/* ==================== Non-static, non-virtual member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	private:
		/* ==================== Non-static member variables end ==================== */
	}; /* end class Surface */

} /* end namespace Math */

#endif /* __MATH_SURFACE_H__ */