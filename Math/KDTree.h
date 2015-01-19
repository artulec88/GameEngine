#pragma once

#include "Math.h"
#include <vector>
#include "KDTreeNode.h"

namespace Math
{

class KDTree
{
/* ==================== Static variables and functions begin ==================== */
/* ==================== Static variables and functions end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	KDTree(Math::Vector3D* positions, int positionsCount);
	~KDTree(void);
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
private:
	void BuildTree(Math::Vector3D* positions, int positionsCount);
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
private:
	KDTreeNode* treeNode;
/* ==================== Non-static member variables end ==================== */

}; /* end class KDTree */

} /* end namespace Math */