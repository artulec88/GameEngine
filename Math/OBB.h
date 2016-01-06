#ifndef __MATH_OBB_H__
#define __MATH_OBB_H__

#include "Math.h"
#include "Vector.h"

namespace Math
{

class IntersectInfo;

/// <summary>
/// The mathematical representation of the oriented bounding box (OBB).
/// An oriented bounding box (OBB) is a 3D cuboid that has been oriented so as to align in some logical way
/// with the object it bounds. Usually an OBB aligns with the local-space axes of the object.
/// Hence it acts like an AABB in local space, although it may not necessarily align with the world space axes.
/// </summary>
/// <remarks>
/// See section 4.6.5 "Oriented Bounding Boxes (OBB)" in "Game Engine Architecture" by Jason Gregory.
/// </remarks>
class OBB
{
/* ==================== Static variables begin ==================== */
/* ==================== Static variables end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	/// <summary>
	/// The constructor of the oriented bounding box.
	/// </summary>
	MATH_API OBB();

	/// <summary>
	/// The destructor of the axis-aligned bounding box.
	/// </summary>
	MATH_API ~OBB(void);
private:
	//OBB(const OBB& obb) {} // don't implement
	//void operator=(const OBB& obb) {} // don't implement
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static, non-virtual member functions begin ==================== */
public:
	// TODO: Create getters and some more functions here.
/* ==================== Non-static, non-virtual member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
private:
	// TODO: Create private variables here.
/* ==================== Non-static member variables end ==================== */
}; /* end class OBB */

} /* end namespace Math */

#endif /* __MATH_OBB_H__ */