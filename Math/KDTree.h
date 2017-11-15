#ifndef __MATH_KD_TREE_H__
#define __MATH_KD_TREE_H__

#include "Math.h"
#include "Vector.h"
#ifdef PROFILING_MATH_MODULE_ENABLED
#include "Statistics.h"
#include "StatisticsStorage.h"
#endif

namespace math
{
	/// <summary>
	/// The implementation of the k-d tree structure. A k-d tree is a space-partitioning data structure for organizing points in a k-dimensional space.
	/// A k-d trees are a useful data structure for such applications, such as searches involving a multidimensional search key (e.g. range searches and nearest neighbour searches).
	/// A k-d tree is a special case of a binary space partitioning tree (BST).
	/// See https://en.wikipedia.org/wiki/K-d_tree for details.
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
		/// <param name="positions">
		/// The 3D positions in the world that we want our k-d tree to operate on. The number of positions must equal <paramref name="positionsCount"/>.
		/// </param>
		/// <param name="positionsCount">The number of positions in the world.</param>
		/// <param name="numberOfSamples">
		/// The number of positions we want to take into account when calculating the nearest neighbour Y component value.
		/// Default value is 1, which means that, once found, the nearest position's Y component is returned.
		/// </param>
		/// <param name="depth">
		/// Specifies the level of hierarchy this particular k-d tree resides on.
		/// The k-d tree is a hierarchical structure. The root of this structure is at depth level 0.
		/// </param>
		MATH_API KDTree(Vector3D* positions, size_t positionsCount, int numberOfSamples = 1, int depth = 0);

		/// <summary>
		/// k-d tree destructor.
		/// </summary>
		MATH_API ~KDTree();

		/// <summary> K-d tree copy constructor. </summary>
		/// <param name="kdTree"> The k-d tree to copy construct from. </param>
		MATH_API KDTree(const KDTree& kdTree) = delete;

		/// <summary> K-d tree move constructor. </summary>
		/// <param name="kdTree"> The k-d tree to move construct from. </param>
		MATH_API KDTree(KDTree&& kdTree) = delete;

		/// <summary> K-d tree copy assignment operator. </summary>
		/// <param name="kdTree"> The k-d tree to copy assign from. </param>
		/// <returns> The reference to the newly copy-assigned k-d tree. </returns>
		MATH_API KDTree& operator=(const KDTree& kdTree) = delete;

		/// <summary> K-d tree move assignment operator. </summary>
		/// <param name="kdTree"> The k-d tree to move assign from. </param>
		/// <returns> The reference to the newly move-assigned k-d tree. </returns>
		MATH_API KDTree& operator=(KDTree&& kdTree) = delete;
	/* ==================== Constructors and destructors end ==================== */

	/* ==================== Non-static member functions begin ==================== */
		/// <summary>
		/// Searches the tree to find specific number of (<see cref="m_numberOfSamples"/>) closest tree nodes to the given <paramref name="position"/>.
		/// Then it interpolates their values based on their distances to the specified <paramref name="position"/> and returns the result.
		/// See also : http://en.wikipedia.org/wiki/Nearest_neighbor_search.
		/// </summary>
		/// <param name="position">The position for which we want to determine the nearest value.</param>
		/// <returns>The interpolated nearest value for the given <paramref name="position"/>.</returns>
		MATH_API Real SearchNearestValue(const Vector2D& position) const
		{
			return SearchNearestValue(position.x, position.y);
		}

		/// <summary>
		/// Searches the tree to find specific number of (<see cref="m_numberOfSamples"/>) closest tree nodes to the given <paramref name="position"/>.
		/// Then it interpolates their values based on their distances to the specified <paramref name="position"/> and returns the result.
		/// See also : http://en.wikipedia.org/wiki/Nearest_neighbor_search.
		/// </summary>
		/// <param name="posX">The X component of the position for which we want to determine the nearest value.</param>
		/// <param name="posZ">The Z component of the position for which we want to determine the nearest value.</param>
		/// <returns>The interpolated nearest value for the given <paramref name="position"/>.</returns>
		MATH_API Real SearchNearestValue(Real posX, Real posZ) const;

		/// <summary>
		/// Returns <code>true</code> if current node is a leaf in a tree hierarchy. If current node has left or right child <code>false</code> is returned.
		/// </summary>
		/// <returns><code>true</code> if current node is a leaf and <code>false</code> otherwise.</returns>
		MATH_API inline bool IsLeaf() const { return ((m_leftTree == nullptr) && (m_rightTree == nullptr)); }

		/// <summary>
		/// Returns <code>true</code> if current node is not a leaf in a tree hierarchy. In all other cases <code>false</code> is returned.
		/// </summary>
		/// <returns><code>true</code> if current node is not a leaf and <code>false</code> otherwise.</returns>
		MATH_API inline bool HasChildren() const { return !IsLeaf(); }

		MATH_API const Vector2D& GetPosition() const { return m_position; }
		MATH_API Real GetValue() const { return m_value; }
		MATH_API std::string ToString() const;
		//friend std::ostream& operator<<(std::ostream& out, const Vector2D& vector)
		//{
		//	out << std::setprecision(4) << "(x=" << vector.m_x << "; y=" << vector.m_y << ")";
		//	return out;
		//}
	private:
		void BuildTree(Vector3D* positions, size_t positionsCount, int depth);
		void SearchNearestValue(const Vector2D& position, int depth, std::vector<Real>& minDistanceValues, std::vector<Real>& minDistances) const
		{
			SearchNearestValue(position.x, position.y, depth, minDistanceValues, minDistances);
		}
		void SearchNearestValue(Real x, Real z, int depth, std::vector<Real>& minDistanceValues, std::vector<Real>& minDistances) const;
		
		/// <summary>
		/// See: http://www.gitta.info/ContiSpatVar/en/html/Interpolatio_learningObject2.xhtml,
		/// https://en.wikipedia.org/wiki/Multivariate_interpolation, https://en.wikipedia.org/wiki/Inverse_distance_weighting.
		/// </summary>
		Real Interpolate(std::vector<Real>& minDistanceValues, std::vector<Real>& minDistances) const;
		std::string ToString(int depth) const;
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
		std::unique_ptr<KDTree> m_leftTree;
		std::unique_ptr<KDTree> m_rightTree;
		int m_numberOfSamples;
		Vector2D m_position;
		Real m_value;
#ifdef PROFILING_MATH_MODULE_ENABLED
		statistics::ClassStats& m_classStats;
#endif
		/* ==================== Non-static member variables end ==================== */

	}; /* end class KDTree */

} /* end namespace math */

#endif /* __MATH_KD_TREE_H__ */