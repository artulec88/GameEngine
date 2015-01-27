#pragma once

#include "Math.h"
#include "Vector.h"
#include <string>

namespace Math
{

class KDTree
{
/* ==================== Static variables and functions begin ==================== */
/* ==================== Static variables and functions end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	MATH_API KDTree(Vector3D* positions, int positionsCount);
	MATH_API ~KDTree(void);
private:
	KDTree(Vector3D* positions, int positionsCount, int depth);
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	/**
	 * \brief Searches the tree to find the numberOfSamples closest tree nodes to the specified position.
	 * Then it interpolates their values based on their distances to the specified position. Finally it returns the interpolated value.
	 * 
	 * TODO: Add additional parameter to represent how to interpolate the nodes (average, weighted average, some other interpolation?)
	 */
	MATH_API Real SearchNearestValue(const Vector3D& position, int numberOfSamples = 1) const;
	MATH_API const Vector2D& GetPosition() const { return m_position; }
	MATH_API Real GetValue() const { return m_value; }
	MATH_API std::string ToString() const;
private:
	void BuildTree(Math::Vector3D* positions, int positionsCount, int depth);
	std::string ToString(int depth) const;
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
private:
	KDTree* m_leftTree;
	KDTree* m_rightTree;
	Vector2D m_position;
	Real m_value;
/* ==================== Non-static member variables end ==================== */

}; /* end class KDTree */

} /* end namespace Math */
