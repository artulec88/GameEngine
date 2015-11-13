#ifndef __MATH_KD_TREE_H__
#define __MATH_KD_TREE_H__

#include "Math.h"
#include "Vector.h"
#ifdef CALCULATE_MATH_STATS
#include "Statistics.h"
#include "IStatisticsStorage.h"
#endif
#include <string> // for ToString() method

namespace Math
{

class KDTree
{
/* ==================== Static variables and functions begin ==================== */
/* ==================== Static variables and functions end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	MATH_API KDTree(Vector3D* positions, int positionsCount, int numberOfSamples = 1);
	MATH_API ~KDTree(void);
private:
	KDTree(Vector3D* positions, int positionsCount, int numberOfSamples, int depth);
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	/**
	 * \brief Searches the tree to find the numberOfSamples closest tree nodes to the specified position.
	 * Then it interpolates their values based on their distances to the specified position. Finally it returns the interpolated value.
	 * 
	 * \param position position in 2D space
	 * TODO: Add additional parameter to represent the number of closest positions maintained at all times
	 * TODO: Add additional parameter to represent how to interpolate the nodes (average, weighted average, some other interpolation?)
	 */
	MATH_API Real SearchNearestValue(const Vector2D& position) const;
	MATH_API bool IsLeaf() const { return ((m_leftTree == NULL) && (m_rightTree == NULL)); }
	MATH_API bool HasChildren() const { return !IsLeaf(); }

	MATH_API const Vector2D& GetPosition() const { return m_position; }
	MATH_API Real GetValue() const { return m_value; }
	MATH_API std::string ToString() const;
private:
	void BuildTree(Math::Vector3D* positions, int positionsCount, int depth);
	void SearchNearestValue(const Vector2D& position, int depth, Vector3D* minDistancePositions, Real* minDistances) const;
	std::string ToString(int depth) const;
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
private:
	KDTree* m_leftTree;
	KDTree* m_rightTree;
	Vector2D m_position;
	Real m_value;
	int m_numberOfSamples;
	Vector3D* m_minDistancePositions; // only root node should allocate memory for this variable
	Real* m_minDistances; // only root node should allocate memory for this variable
#ifdef CALCULATE_MATH_STATS
	Statistics::ClassStats& m_classStats;
#endif
/* ==================== Non-static member variables end ==================== */

}; /* end class KDTree */

} /* end namespace Math */

#endif /* __MATH_KD_TREE_H__ */