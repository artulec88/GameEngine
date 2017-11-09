#ifndef __MATH_SURFACE_H__
#define __MATH_SURFACE_H__

#include "Math.h"
#include "Vector.h"

#include <vector>

namespace Math
{
	/// <summary>
	/// The mathematical representation of a surface. The vertices on the surface are always laid out uniformly.
	/// </summary>
	class Surface
	{
		/* ==================== Static variables begin ==================== */
		/* ==================== Static variables end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		/// <summary>
		/// The constructor of the surface.
		/// </summary>
		/// <param name="surfaceBottomLeftPosition"> The bottom left position of the surface. </param>
		/// <param name="surfaceWidth"> The width of the whole surface. It is equal to the size of the surface along the X axis. </param>
		/// <param name="surfaceDepth"> The depth of the whole surface. It is equal to the size of the surface along the Z axis. </param>
		/// <param name="surfaceHorizontalVerticesCount"> The number of vertices along the X axis of the surface. </param>
		/// <param name="surfaceVerticalVerticesCount"> The number of vertices along the Z axis of the surface. </param>
		/// <param name="heights">
		/// The pointer to heights values. The array must consist of exactly
		/// <paramref name="surfaceHorizontalVerticesCount"/> * <paramref name="surfaceVerticalVerticesCount"/> elements.
		/// </param>
		MATH_API Surface(const Vector2D& surfaceBottomLeftPosition, int surfaceWidth, int surfaceDepth,
			int surfaceHorizontalVerticesCount, int surfaceVerticalVerticesCount, Real* heights);

		MATH_API Surface(Vector3D* positions, unsigned int positionsCount);

		/// <summary>
		/// Surface copy constructor.
		/// </summary>
		MATH_API constexpr Surface(const Surface& surface) = default;

		/// <summary>
		/// Surface move constructor.
		/// </summary>
		MATH_API constexpr Surface(Surface&& surface) = default;

		/// <summary>
		/// Surface copy assignment operator.
		/// </summary>
		MATH_API Surface& operator=(const Surface& surface) = default;

		/// <summary>
		/// Surface move assignment operator.
		/// </summary>
		MATH_API Surface& operator=(Surface&& surface) = default;

		/// <summary>
		/// The destructor of the surface.
		/// </summary>
		MATH_API ~Surface(void) = default;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static, non-virtual member functions begin ==================== */
	public:
		/// <summary> Returns the number of vertices in the surface along the X axis. </summary>
		/// <returns> The number of vertices in the surface along the X axis. </returns>
		MATH_API int GetHorizontalVerticesCount() const { return m_horizontalVerticesCount; }

		/// <summary> Returns the number of vertices in the surface along the Z axis. </summary>
		/// <returns> The number of vertices in the surface along the Z axis. </returns>
		MATH_API int GetVerticalVerticesCount() const { return m_verticalVerticesCount; }

		/// <summary> Calculates and returns the width of the whole surface. </summary>
		/// <returns> The width of the whole surface. </returns>
		MATH_API Real GetWidth() const { return m_horizontalVerticesCount * m_squareWidth; }

		/// <summary> Calculates and returns the depth of the whole surface. </summary>
		/// <returns> The depth of the whole surface. </returns>
		MATH_API Real GetDepth() const { return m_verticalVerticesCount * m_squareDepth; }

		/// <summary>
		/// </summary>
		/// <param name="x"> The index of the vertex along the X axis. Must be in range [0; m_horizontalVerticesCount). </param>
		/// <param name="z"> The index of the vertex along the Z axis. Must be in range [0; m_verticalVerticesCount). </param>
		/// <returns>
		/// </returns>
		MATH_API Vector3D GetPositionAt(int x, int z) const { return Vector3D(m_position.x + x * m_squareWidth, GetHeightAt(x, z), m_position.y + z * m_squareDepth); }

		/// <summary>
		/// Returns the height at given location specified by the index of the vertex along X and Z axis respectively.
		/// </summary>
		/// <param name="x"> The location along the X axis of the vertex we want to get height of. </param>
		/// <param name="z"> The location along the Z axis of the vertex we want to get height of. </param>
		/// <returns> The height of the exact given location. </returns>
		MATH_API Real GetHeightAt(int x, int z) const;

		/// <summary>
		/// Finds and returns the height of the surface at given location <paramref name="xz"/>.
		/// </summary>
		/// <param name="xz">
		/// The two-dimensional vector representing the location in the world.
		/// The output of the function will represent the height of the surface at that particular location.
		/// </param>
		/// <returns> The height of the surface at location <paramref name="xz"/>. </returns>
		MATH_API Real GetHeightAt(const Vector2D& xz) const { return GetHeightAt(xz.x, xz.y); }

		/// <summary>
		/// Finds and returns the height of the surface at location given by the two specified X and Z components
		/// (<paramref name="x"/> and <paramref name="z"/> respectively).
		/// </summary>
		/// <param name="x">
		/// The X component of the location in the world for which we want to determine the height of the surface.
		/// </param>
		/// <param name="z">
		/// The Z component of the location in the world for which we want to determine the height of the surface.
		/// </param>
		/// <returns>
		/// The height of the surface at location given by the two specified X and Z components
		/// (<paramref name="x"/> and <paramref name="z"/> respectively).
		/// </returns>
		MATH_API Real GetHeightAt(Real x, Real z) const;

	private:
		inline int GetHeightsIndex(int xIndex, int zIndex) const noexcept
		{
			return zIndex * m_horizontalVerticesCount + xIndex;
		}

		/// <summary>
		/// After projecting given array of three-dimensional vertices onto the XZ plane the algorithm finds the smallest distance
		/// between any two positions in the <paramref name="vectors"/> array of vertices.
		/// </summary>
		/// <param name="vectors">
		/// The pointer to array of three-dimensional vectors. They are first projected onto the XZ plane so that the minimum distance
		/// is found between two-dimensional vertices.
		/// </param>
		/// <param name="vectorsCount"> The number of three-dimensional vectors passed in the <paramref name="vectors"/> pointer. </param>
		Real FindMinimumDistanceBetweenPairOfPositions(Vector3D* vectors, unsigned int vectorsCount) const;

		Real FindMinimumDistanceBetweenPairOfPositions(std::vector<Vector2D>& vectorsSortedByX, std::vector<Vector2D>& vectorsSortedByZ) const;

		Real FindMinimumDistanceBetweenPairOfPositionsBruteForce(std::vector<Vector2D>& vectors) const;
		/* ==================== Non-static, non-virtual member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	private:
		/// <summary>
		/// The position of the bottom left position of the surface.
		/// This is the position on the surface with least values for both X and Z components.
		/// </summary>
		Vector2D m_position;

		/// <summary>
		/// Stores the number of vertices along one line on the X axis.
		/// </summary>
		int m_horizontalVerticesCount;

		/// <summary>
		/// Stores the number of vertices along one line on the Z axis.
		/// </summary>
		int m_verticalVerticesCount;

		/// <summary>
		/// The width of the single square on the surface. It is the size of a single square along the X axis.
		/// </summary>
		Real m_squareWidth;

		/// <summary>
		/// The depth of the single square on the surface. It is the size of a single square along the Z axis.
		/// </summary>
		Real m_squareDepth;

		/// <summary>
		/// Stores the heights of the surface.
		/// </summary>
		std::vector<Real> m_heights;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class Surface */

} /* end namespace Math */

#endif /* __MATH_SURFACE_H__ */