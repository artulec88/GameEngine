#ifndef __MATH_OCTREE_H__
#define __MATH_OCTREE_H__

#include "Math.h"

namespace Math { namespace SpacePartitioning
{
	/// <summary> The representation of the Octree in the system for space partitioning functionality. </summary>
	class Octree
	{
		/* ==================== Static variables and functions begin ==================== */
		// TODO: There should be a global pointer to the root node. Decide whether to define it here as a static field or just declare a variable in one specific place in the application.
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		MATH_API Octree();
		MATH_API ~Octree();
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		// TODO: Create a "RayIntersect" method. Read "3D Math Primer for Graphics and Game Development" 16.4 section ("Quadtrees and Octrees").
		MATH_API bool HasChildren() const;
		MATH_API bool IsLeaf() const;
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	private:
		Octree* topNorthWestNode;
		Octree* topNorthEastNode;
		Octree* topSouthWestNode;
		Octree* topSouthEastNode;
		Octree* bottomNorthWestNode;
		Octree* bottomNorthEastNode;
		Octree* bottomSouthWestNode;
		Octree* bottomSouthEastNode;

		// TODO: There should be a list of objects stored in the current node
	/* ==================== Non-static member variables end ==================== */
	}; /* end class Octree */

} /* end namespace SpacePartitioning */

} /* end namespace Math */

#endif /* __MATH_OCTREE_H__ */