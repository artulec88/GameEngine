#pragma once

#include "Math.h"
#include "Vector.h"

namespace Math
{

class KDTreeNode
{
/* ==================== Static variables and functions begin ==================== */
/* ==================== Static variables and functions end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	KDTreeNode(const Math::Vector2D& position, float value);
	~KDTreeNode(void);
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	const Math::Vector2D& GetPosition() const { return position; }
	float GetValue() const { return value; }
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
private:
	Math::Vector2D position;
	float value;
/* ==================== Non-static member variables end ==================== */

}; /* end class KDTreeNode */

} /* end namespace Math */