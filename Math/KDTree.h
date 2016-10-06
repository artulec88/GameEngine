#ifndef __MATH_KD_TREE_H__
#define __MATH_KD_TREE_H__

#include "Math.h"
#include "Vector.h"
#ifdef PROFILING_MATH_MODULE_ENABLED
#include "Statistics.h"
#include "StatisticsStorage.h"
#endif
#include <string> // for ToString() method

namespace Math
{

/// <summary>
/// The implementation of the k-d tree structure.
/// </summary>
class KDTree
{
/* ==================== Static variables and functions begin ==================== */
/* ==================== Static variables and functions end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	/// <summary>
	/// Constructs the k-d tree for the specified 3D positions in the world (<paramref name="positions"/>).
	/// </summary>
	/// <param name="positions">The 3D positions in the world that we want our k-d tree to operate on.</param>
	/// <param name="positionsCount">The number of positions in the world.</param>
	/// <param name="numberOfSamples">The number of positions we want to take into account when calculating the nearest neighbour Y component value.
	/// Default value is 1, which means that, once found, the nearest position's Y component will be returned.</param>
	/// <param name="depth">Specified the level of hierarchy this particular k-d tree resides on.
	/// The k-d tree is a hierarchical structure. The root of this structure is at depth level 0. </param>
	MATH_API KDTree(Vector3D* positions, size_t positionsCount, int numberOfSamples = 1, int depth = 0);
	/// <summary>
	/// k-d tree destructor.
	/// </summary>
	MATH_API ~KDTree(void);
protected:
	//KDTree(const KDTree& kdTree) = delete;
	//KDTree& operator=(const KDTree& kdTree) = delete;
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	/// <summary>
	/// Searches the tree to find specific number of (<see cref="m_numberOfSamples"/>) closest tree nodes to the given <paramref name="position"/>.
	/// Then it interpolates their values based on their distances to the specified <paramref name="position"/> and returns the result.
	/// </summary>
	/// <param name="position">The position for which we want to determine the nearest value.</param>
	/// <returns>The interpolated nearest value for the given <paramref name="position"/>.</returns>
	MATH_API Real SearchNearestValue(const Vector2D& position) const;

	/// <summary>
	/// Searches the tree to find specific number of (<see cref="m_numberOfSamples"/>) closest tree nodes to the given <paramref name="position"/>.
	/// Then it interpolates their values based on their distances to the specified <paramref name="position"/> and returns the result.
	/// </summary>
	/// <param name="posX">The X component of the position for which we want to determine the nearest value.</param>
	/// <param name="posZ">The Z component of the position for which we want to determine the nearest value.</param>
	/// <returns>The interpolated nearest value for the given <paramref name="position"/>.</returns>
	MATH_API Real SearchNearestValue(Math::Real posX, Math::Real posZ) const;

	/// <summary>
	/// Returns <code>true</code> if current node is a leaf in a tree hierarchy. If current node has left or right child <code>false</code> is returned.
	/// </summary>
	/// <returns><code>true</code> if current node is a leaf and <code>false</code> otherwise.</returns>
	MATH_API bool IsLeaf() const { return ((m_leftTree == NULL) && (m_rightTree == NULL)); }

	/// <summary>
	/// Returns <code>true</code> if current node is not a leaf in a tree hierarchy. In all other cases <code>false</code> is returned.
	/// </summary>
	/// <returns><code>true</code> if current node is not a leaf and <code>false</code> otherwise.</returns>
	MATH_API bool HasChildren() const { return !IsLeaf(); }

	MATH_API const Vector2D& GetPosition() const { return m_position; }
	MATH_API Real GetValue() const { return m_value; }
	MATH_API std::string ToString() const;
private:
	void BuildTree(Math::Vector3D* positions, size_t positionsCount, int depth);
	void SearchNearestValue(const Vector2D& position, int depth, std::vector<Real>& minDistanceValues, std::vector<Real>& minDistances) const;
	void SearchNearestValue(Math::Real x, Math::Real z, int depth, std::vector<Real>& minDistanceValues, std::vector<Real>& minDistances) const;
	std::string ToString(int depth) const;
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
private:
	std::unique_ptr<KDTree> m_leftTree;
	std::unique_ptr<KDTree> m_rightTree;
	int m_numberOfSamples;
	Vector2D m_position;
	Real m_value;
#ifdef PROFILING_MATH_MODULE_ENABLED
	Statistics::ClassStats& m_classStats;
#endif
/* ==================== Non-static member variables end ==================== */

}; /* end class KDTree */

} /* end namespace Math */

#endif /* __MATH_KD_TREE_H__ */