#ifndef __MATH_BSP_TREE_H__
#define __MATH_BSP_TREE_H__

#include "Math.h"

namespace math {
	namespace space_partitioning
	{
		/// <summary>
		/// The representation of the Binary Space Partitioning Tree in the system for space partitioning functionality. BSP is a tree where each node has two children.
		/// The children are separated by a dividing plance. In a BSP we may use planes that are arbitrarily oriented (not necesserily axially aligned).
		/// Just like in a octree, a node overlaps its parent and children.
		/// </summary>
		/// <remarks>
		/// Using BSP tree once they have been constructed is relatively easy. The trick is deciding where to place the dividing planes.
		/// In BSP trees we have considerably more flexibility than we do with octrees.
		/// </remarks>
		class BSPTree
		{
			/* ==================== Static variables and functions begin ==================== */
			// TODO: There should be a global pointer to the root node. Decide whether to define it here as a static field or just declare a variable in one specific place in the application.
			/* ==================== Static variables and functions end ==================== */

			/* ==================== Constructors and destructors begin ==================== */
		public:
			MATH_API BSPTree();
			MATH_API ~BSPTree();
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
			// TODO: Create a "RayIntersect" method. Read "3D Math Primer for Graphics and Game Development" 16.5 section ("BSP trees").
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
			// TODO: There should be a list of objects stored in the current node
			/* ==================== Non-static member variables end ==================== */
		}; /* end class BSPTree */
	} /* end namespace space_partitioning */
} /* end namespace math */

#endif /* __MATH_BSP_TREE_H__ */