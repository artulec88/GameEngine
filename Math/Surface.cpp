#include "stdafx.h"
#include "Surface.h"
#include "ISort.h"
#include "KDTree.h"
//#include "Triangle.h"


Math::Surface::Surface(const Vector2D& surfaceBottomLeftPosition, int surfaceWidth, int surfaceDepth,
	int surfaceHorizontalVerticesCount, int surfaceVerticalVerticesCount, Real* heights) :
	m_position(surfaceBottomLeftPosition),
	m_horizontalVerticesCount(surfaceHorizontalVerticesCount),
	m_verticalVerticesCount(surfaceVerticalVerticesCount),
	m_squareWidth(surfaceWidth / (surfaceHorizontalVerticesCount - 1)),
	m_squareDepth(surfaceDepth / (surfaceVerticalVerticesCount - 1))//,
	//m_heights(heights, heights + (m_horizontalVerticesCount * m_verticalVerticesCount))
{
	for (auto i = 0; i < m_horizontalVerticesCount * m_verticalVerticesCount; ++i)
	{
		m_heights.push_back(heights[i]);
	}
}

Math::Surface::Surface(Vector3D* positions, unsigned int positionsCount) :
	m_position(),
	m_horizontalVerticesCount(0),
	m_verticalVerticesCount(0),
	m_squareWidth(REAL_ZERO),
	m_squareDepth(REAL_ZERO),
	m_heights()
{
	CHECK_CONDITION_MATH(positionsCount > 0, Utility::Logging::ERR, "Cannot intialize the surface with negative number of positions: ", positionsCount);

	// TODO: Implement
	// 1. Find the lowest X and Z and greatest X and Z values within all vertices (minX, minZ, maxX and maxZ respectively). Each of these four values may come from the different position.
	// 2. Set m_position to be equal to the vector [minX, minZ].
	// 3. Save surfaceWidth to be equal to (maxX - minX) and surfaceDepth to be equal to (maxZ - minZ).
	// 4. Interpolate between given positions to create a grid of final positions being laid out uniformly on the XZ surface.

	Real minX = positions[0].x;
	Real minZ = positions[0].z;
	Real maxX = positions[0].x;
	Real maxZ = positions[0].z;
	for (unsigned int i = 1; i < positionsCount; ++i)
	{
		if (positions[i].x < minX)
		{
			minX = positions[i].x;
		}
		else if (positions[i].x > maxX)
		{
			maxX = positions[i].x;
		}
		if (positions[i].z < minZ)
		{
			minZ = positions[i].z;
		}
		else if (positions[i].z > maxZ)
		{
			maxZ = positions[i].z;
		}
	}
	m_position.Set(minX, minZ);
	const Real surfaceWidth = maxX - minX;
	const Real surfaceDepth = maxZ - minZ;

	// Find minimum distance between any two positions that have different Y values.
	Real minDistance = FindMinimumDistanceBetweenPairOfPositions(positions, positionsCount);

	DEBUG_LOG_MATH("Min distance between pair of positions that have different Y values = ", minDistance);

	m_horizontalVerticesCount = Ceil(surfaceWidth / minDistance) + 1;
	m_verticalVerticesCount = Ceil(surfaceDepth / minDistance) + 1;
	m_squareWidth = surfaceWidth / (m_horizontalVerticesCount - 1);
	m_squareDepth = surfaceDepth / (m_verticalVerticesCount - 1);

	DEBUG_LOG_MATH("Horizontal vertices count = ", m_horizontalVerticesCount, ". Vertical vertices count = ", m_verticalVerticesCount);
	DEBUG_LOG_MATH("Square width = ", m_squareWidth, ". Square depth = ", m_squareDepth);

	// Interpolate between points to create a collection of heights.
	KDTree kdTree(positions, positionsCount, 3);
	for (int i = 0; i < m_verticalVerticesCount; ++i)
	{
		for (int j = 0; j < m_horizontalVerticesCount; ++j)
		{
			Vector2D position = m_position + Vector2D(j * m_squareWidth, i * m_squareDepth);
			m_heights.push_back(kdTree.SearchNearestValue(position));
			DELOCUST_LOG_MATH("Height for position [", i, "][", j, "] = ", position, " equals ", m_heights.back());
		}
	}
}

Math::Real Math::Surface::GetHeightAt(Real x, Real z) const
{
	Real surfaceX = x - m_position.x;
	Real surfaceZ = z - m_position.y;
	int gridX = Math::Floor(surfaceX / m_squareWidth);
	int gridZ = Math::Floor(surfaceZ / m_squareDepth);
	if (gridX < 0 || gridX >= m_horizontalVerticesCount || gridZ < 0 || gridZ >= m_verticalVerticesCount)
	{
		return REAL_ZERO;
	}
	Real xCoord = fmod(surfaceX, m_squareWidth) / m_squareWidth;
	if (AlmostEqual(xCoord, REAL_ONE)) // xCoord should never be >= REAL_ONE, but due to floating-point errors this may happen.
	{
		xCoord = REAL_ZERO;
	}
	Real zCoord = fmod(surfaceZ, m_squareDepth) / m_squareDepth;
	if (AlmostEqual(zCoord, REAL_ONE)) // zCoord should never be >= REAL_ONE, but due to floating-point errors this may happen.
	{
		zCoord = REAL_ZERO;
	}
	Real y;
	if (xCoord <= (REAL_ONE - zCoord))
	{
		DEBUG_LOG_MATH("Left triangle indices for position [", x, "; ", z, "] are: ", GetHeightsIndex(gridX, gridZ), "; ", GetHeightsIndex(gridX + 1, gridZ),
			"; ", GetHeightsIndex(gridX, gridZ + 1));
		y = Math::Interpolation::BarycentricInterpolation(0.0f, m_heights[GetHeightsIndex(gridX, gridZ)], 0.0f,
			1.0f, m_heights[GetHeightsIndex(gridX + 1, gridZ)], 0.0f,
			0.0f, m_heights[GetHeightsIndex(gridX, gridZ + 1)], 1.0f,
			xCoord, zCoord);
	}
	else
	{
		DEBUG_LOG_MATH("Right triangle indices for position [", x, "; ", z, "] are: ", GetHeightsIndex(gridX + 1, gridZ), "; ", GetHeightsIndex(gridX + 1, gridZ + 1),
			"; ", GetHeightsIndex(gridX, gridZ + 1));
		y = Math::Interpolation::BarycentricInterpolation(1.0f, m_heights[GetHeightsIndex(gridX + 1, gridZ)], 0.0f,
			1.0f, m_heights[GetHeightsIndex(gridX + 1, gridZ + 1)], 1.0f,
			0.0f, m_heights[GetHeightsIndex(gridX, gridZ + 1)], 1.0f,
			xCoord, zCoord);
	}
	return y;
}

Math::Real Math::Surface::FindMinimumDistanceBetweenPairOfPositions(Vector3D* vectors, unsigned int vectorsCount) const
{
	std::vector<Vector2D> positionsSortedByX;
	for (unsigned int i = 0; i < vectorsCount; ++i)
	{
		positionsSortedByX.push_back(vectors[i].GetXZ());
	}
	std::vector<Vector2D> positionsSortedByZ(positionsSortedByX.begin(), positionsSortedByX.end());

	Sorting::SortingParametersChain sortParamsX(Sorting::Keys::COMPONENT_X, Sorting::Orders::ASCENDING);
	Sorting::SortingParametersChain sortParamsZ(Sorting::Keys::COMPONENT_Y /* (sic!) this is not a mistake- we sort Vector2D objects so we need to use Y component */, Sorting::Orders::ASCENDING);
	Sorting::ISort::GetSortingObject(Sorting::SortingAlgorithms::QUICK_SORT)->Sort(positionsSortedByX.data(), vectorsCount, sortParamsX);
	Sorting::ISort::GetSortingObject(Sorting::SortingAlgorithms::QUICK_SORT)->Sort(positionsSortedByZ.data(), vectorsCount, sortParamsZ);

	//for (unsigned int i = 0; i < positionsSortedByX.size(); ++i)
	//{
	//	ERROR_LOG_MATH("Posititions sorted by X #", i, " = ", positionsSortedByX[i]);
	//}
	//for (unsigned int i = 0; i < positionsSortedByZ.size(); ++i)
	//{
	//	ERROR_LOG_MATH("Posititions sorted by Z #", i, " = ", positionsSortedByZ[i]);
	//}

	// Use recursive function FindMinimumDistanceBetweenPairOfPositions() to find the smallest distance
	return sqrt(FindMinimumDistanceBetweenPairOfPositions(positionsSortedByX, positionsSortedByZ));
}

Math::Real Math::Surface::FindMinimumDistanceBetweenPairOfPositions(std::vector<Vector2D>& vectorsSortedByX, std::vector<Vector2D>& vectorsSortedByZ) const
{
	CHECK_CONDITION_MATH(vectorsSortedByX.size() == vectorsSortedByZ.size(), Utility::Logging::ERR,
		"Error when trying to find the minimum distance between any two positions. The number of positions in two collections are not equal (",
		vectorsSortedByX.size(), " != ", vectorsSortedByZ.size(), ").");
	if (vectorsSortedByX.size() <= 3) // If there are 2 or 3 points, then use brute force
	{
		return FindMinimumDistanceBetweenPairOfPositionsBruteForce(vectorsSortedByX);
	}
	return FindMinimumDistanceBetweenPairOfPositionsBruteForce(vectorsSortedByX);

	// Find the middle point
	unsigned int midIndex = vectorsSortedByX.size() / 2;
	Vector2D midPoint = vectorsSortedByX[midIndex];


	// Divide points in y sorted array around the vertical line.
	// Assumption: All x coordinates are distinct.
	std::vector<Vector2D> positionsZLeft;   // y sorted points on left of vertical line
	positionsZLeft.reserve(midIndex + 1);
	std::vector<Vector2D> positionsZRight;  // y sorted points on right of vertical line
	positionsZRight.reserve(vectorsSortedByZ.size() - midIndex - 1);
	for (auto vectorItr = vectorsSortedByZ.begin(); vectorItr != vectorsSortedByZ.end(); ++vectorItr)
	{
		if (vectorItr->x <= midPoint.x)
		{
			positionsZLeft.push_back(*vectorItr);
		}
		else
		{
			positionsZRight.push_back(*vectorItr);
		}
	}

	// Consider the vertical line passing through the middle point
	// calculate the smallest distance dl on left of middle point and
	// dr on right side
	Real dl = 3.0f; //FindMinimumDistanceBetweenPairOfPositions(vectorsSortedByX, positionsZLeft, midIndex);
	Real dr = 3.0f; //FindMinimumDistanceBetweenPairOfPositions(vectorsSortedByX + midIndex, positionsZRight, n - mid);
	Real minDistance = (dl < dr) ? dl : dr; // Find the smaller of two distances

	// Build an array strip[] that contains points close (closer than d)
	// to the line passing through the middle point
	std::vector<Vector2D> strip;
	int j = 0;
	for (int i = 0; i < vectorsSortedByX.size(); i++)
	{
		if (abs(vectorsSortedByZ[i].x - midPoint.x) < minDistance)
		{
			strip.push_back(vectorsSortedByZ[i]);
		}
	}

	// Find the closest points in strip.  Return the minimum of d and closest
	// distance is strip[]
	//return min(d, stripClosest(strip, j, d));
	return minDistance;
}

Math::Real Math::Surface::FindMinimumDistanceBetweenPairOfPositionsBruteForce(std::vector<Vector2D>& vectors) const
{
	Real minDistance = REAL_MAX;
	for (auto vectorItr = vectors.begin(); vectorItr != vectors.end(); ++vectorItr)
	{
		for (auto vectorItr2 = vectorItr + 1; vectorItr2 != vectors.end(); ++vectorItr2)
		{
			Real distance = ((*vectorItr2) - (*vectorItr)).LengthSquared();
			if (distance < minDistance)
			{
				minDistance = distance;
			}
		}
	}
	return minDistance;
}