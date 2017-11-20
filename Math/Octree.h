#ifndef __MATH_OCTREE_H__
#define __MATH_OCTREE_H__

#include "Math.h"

namespace math {
	namespace space_partitioning
	{
		/// <summary> The representation of the Octree in the system for space partitioning functionality. </summary>
		class Octree
		{
			/* ==================== Static variables and functions begin ==================== */
			// TODO: There should be a global pointer to the root node. Decide whether to define it here as a static field or just declare a variable in one specific place in the application.
			/* ==================== Static variables and functions end ==================== */

			/* ==================== Constructors and destructors begin ==================== */
		public:
			/// <summary> Octree constructor. </summary>
			MATH_API Octree();

			/// <summary> Octree destructor. </summary>
			MATH_API ~Octree();
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
			// TODO: Create a "RayIntersect" method. Read "3D Math Primer for Graphics and Game Development" 16.4 section ("Quadtrees and Octrees").
			MATH_API bool HasChildren() const;
			MATH_API bool IsLeaf() const;
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
		private:
			Octree* m_topNorthWestNode;
			Octree* m_topNorthEastNode;
			Octree* m_topSouthWestNode;
			Octree* m_topSouthEastNode;
			Octree* m_bottomNorthWestNode;
			Octree* m_bottomNorthEastNode;
			Octree* m_bottomSouthWestNode;
			Octree* m_bottomSouthEastNode;

			// TODO: There should be a list of objects stored in the current node
		/* ==================== Non-static member variables end ==================== */
		}; /* end class Octree */

	} /* end namespace space_partitioning */

} /* end namespace math */

#endif /* __MATH_OCTREE_H__ */