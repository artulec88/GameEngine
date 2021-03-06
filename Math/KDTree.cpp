#include "stdafx.h"
#include "KDTree.h"
#include "ISort.h"
#include "SortingParameters.h"

#include "Utility/ILogger.h"

math::KdTree::KdTree(Vector3D* positions, size_t positionsCount, int numberOfSamples /* = 1 */, int depth /* = 0 */) :
	m_leftTree(nullptr),
	m_rightTree(nullptr),
	m_numberOfSamples(numberOfSamples),
	m_position(REAL_ZERO, REAL_ZERO),
	m_value(REAL_ZERO)
#ifdef PROFILING_MATH_MODULE_ENABLED
	, m_classStats(STATS_STORAGE.GetClassStats("KDTree"))
#endif
{
	START_PROFILING_MATH(true, "");
	CHECK_CONDITION_EXIT_MATH(positions != nullptr, utility::logging::EMERGENCY,
		"Cannot create K-d tree. The positions array is nullptr.");
	CHECK_CONDITION_EXIT_MATH(positionsCount > 0, utility::logging::EMERGENCY,
		"Cannot create K-d tree. The number of positions is not positive (", positionsCount, ").");

	if (depth == 0)
	{
		DEBUG_LOG_MATH("The array of positions consists of ", positionsCount, " entries.");
		std::vector<Vector3D> uniquePositions;
		for (unsigned int i = 0; i < positionsCount; ++i)
		{
			// for positions with equal XZ components we want to store only the one with the greatest Y value.
			auto sameXzVectorFound = false;
			const auto xz = positions[i].GetXz();
			for (auto vecItr = uniquePositions.begin(); vecItr != uniquePositions.end(); ++vecItr)
			{
				if (xz == vecItr->GetXz())
				{
					if (positions[i].y > vecItr->y)
					{
						vecItr->y = positions[i].y;
					}
					sameXzVectorFound = true;
					break;
				}
			}
			if (!sameXzVectorFound)
			{
				uniquePositions.push_back(positions[i]);
			}
		}
		INFO_LOG_MATH("The array consists of ", uniquePositions.size(), " unique entries");

		//for (size_t i = 0; i < uniquePositions.size(); ++i)
		//{
		//	WARNING_LOG_MATH("Unique position[", i, "] = ", uniquePositions[i], ".");
		//}

		BuildTree(uniquePositions.data(), uniquePositions.size(), depth);
	}
	else
	{
		BuildTree(positions, positionsCount, depth);
	}

	STOP_PROFILING_MATH("");
}

math::KdTree::~KdTree()
{
}

void math::KdTree::BuildTree(Vector3D* positions, size_t positionsCount, int depth)
{
	START_PROFILING_MATH(true, "");
	auto sortingKey = depth % 2 == 0 ? sorting::keys::COMPONENT_X : sorting::keys::COMPONENT_Z;
	//DEBUG_LOG_MATH("Before sorting: depth = ", depth);
	//for (int i = 0; i < positionsCount; ++i)
	//{
	//	DEBUG_LOG_MATH("depth = ", depth, ") positions[", i, "] = ", positions[i].ToString());
	//}
	sorting::SortingParametersChain sortingParameters(sortingKey, sorting::orders::ASCENDING);
	sorting::ISort::GetSortingObject(sorting::sorting_algorithms::MERGE_SORT)->Sort(positions, positionsCount, sortingParameters);
	//DEBUG_LOG_MATH("After sorting: depth = ", depth);
	//for (int i = 0; i < positionsCount; ++i)
	//{
	//	DEBUG_LOG_MATH("depth = ", depth, ") positions[", i, "] = ", positions[i].ToString());
	//}

	const auto medianIndex = positionsCount / 2;
	m_position = positions[medianIndex].GetXz();
	m_value = positions[medianIndex].y;

	if (medianIndex > 0)
	{
		m_leftTree = std::make_unique<KdTree>(positions, medianIndex, m_numberOfSamples, depth + 1);
	}
	if (positionsCount - medianIndex - 1 > 0)
	{
		m_rightTree = std::make_unique<KdTree>(&positions[medianIndex + 1], positionsCount - medianIndex - 1, m_numberOfSamples, depth + 1);
	}
	STOP_PROFILING_MATH("");
}

//int numberOfPositionsChecked = 0; // just to measure how many nodes have actually been checked / visited during the search for the nearest positions

math::Real math::KdTree::SearchNearestValue(Real posX, Real posZ) const
{
	START_PROFILING_MATH(true, "");
	// The numberOfSamples must be less than the number of nodes. We assume that it is.
	// If we wanted to check that condition we would have to store the number of nodes in the separate member variable.
	std::vector<Real> minDistanceValues;
	std::vector<Real> minDistances;
	minDistanceValues.reserve(m_numberOfSamples);
	minDistances.reserve(m_numberOfSamples);
	minDistances.assign(m_numberOfSamples, REAL_MAX);

	//numberOfPositionsChecked = 0;
	SearchNearestValue(posX, posZ, 0, minDistanceValues, minDistances);
	//DEBUG_LOG_MATH("Number of positions checked during the search for the nearest positions = ", numberOfPositionsChecked);

	//return minDistancePositions[0].GetY(); // return the nearest position's Y component

	// Calculate the weighted average where weights are in inverse proportion to distances
	// See: http://en.wikipedia.org/wiki/Inverse_distance_weighting
	// TODO: Create different strategies (strategy pattern) for weighted average calculation
	// TODO: Measure which method (#1 or #2) is faster

	//for (int i = 0; i < m_numberOfSamples; ++i)
	//{
	//	ERROR_LOG_MATH("minDistances[", i, "] = ", minDistances[i], "; minDistanceValues[", i, "] = ", minDistanceValues[i]);
	//}

	auto result = Interpolate(minDistanceValues, minDistances);
	STOP_PROFILING_MATH("");
	return result;
}

void math::KdTree::SearchNearestValue(Real x, Real z, int depth, std::vector<Real>& minDistanceValues, std::vector<Real>& minDistances) const
{
	START_PROFILING_MATH(true, "");
	//++numberOfPositionsChecked;
	//DELOCUST_LOG_MATH("Visiting the node with position (", m_position.ToString(), ") and value ", m_value);
	auto distance = (x - m_position.x) * (x - m_position.x) + (z - m_position.y) * (z - m_position.y);
	auto j = m_numberOfSamples - 1;
	while (j >= 0 && minDistances[j] > distance)
	{
		if (j + 1 < m_numberOfSamples)
		{
			minDistances[j + 1] = minDistances[j];
			minDistanceValues[j + 1] = minDistanceValues[j];
		}
		minDistances[j] = distance;
		minDistanceValues[j] = m_value;
		--j;
	}
	//for (int i = 0; i < numberOfSamples; ++i)
	//{
	//	INFO_LOG_MATH("minDistancePositions[", i, "] = (", minDistancePositions[i].ToString(), ")\tminDistances[", i, "] = ", minDistances[i]);
	//}
	if (IsLeaf())
	{
		//DELOCUST_LOG_MATH("The node with position (", m_position.ToString(), ") and value ", m_value, " is a leaf");
		STOP_PROFILING_MATH("");
		return;
	}

	const auto positionComponentValue = depth % 2 == 0 ? x : z;
	const auto nodePositionComponentValue = depth % 2 == 0 ? m_position.x : m_position.y;
	const auto searchLeftTreeFirst = positionComponentValue < nodePositionComponentValue; // false means we will search the right tree first

	if (searchLeftTreeFirst)
	{
		SearchNearestValueInLeftTree(x, z, depth, positionComponentValue, nodePositionComponentValue, minDistances, minDistanceValues);
		SearchNearestValueInRightTree(x, z, depth, positionComponentValue, nodePositionComponentValue, minDistances, minDistanceValues);
	}
	else
	{
		SearchNearestValueInRightTree(x, z ,depth, positionComponentValue, nodePositionComponentValue, minDistances, minDistanceValues);
		SearchNearestValueInLeftTree(x, z, depth, positionComponentValue, nodePositionComponentValue, minDistances, minDistanceValues);
	}
	STOP_PROFILING_MATH("");
}

void math::KdTree::SearchNearestValueInLeftTree(Real x, Real z, int depth, const Real positionComponentValue, const Real nodePositionComponentValue, std::vector<Real>& minDistanceValues, std::vector<Real>& minDistances) const
{
	if (m_leftTree != nullptr)
	{
		if (positionComponentValue - minDistances[m_numberOfSamples - 1] <= nodePositionComponentValue)
		{
			m_leftTree->SearchNearestValue(x, z, depth + 1, minDistanceValues, minDistances);
		}
		else
		{
			//DELOCUST_LOG_MATH("Left tree of node (", m_position.ToString(), ") pruned");		
		}
	}
}

void math::KdTree::SearchNearestValueInRightTree(Real x, Real z, int depth, const Real positionComponentValue, const Real nodePositionComponentValue, std::vector<Real>& minDistanceValues, std::vector<Real>& minDistances) const
{
	if (m_rightTree != nullptr)
	{
		if (positionComponentValue + minDistances[m_numberOfSamples - 1] > nodePositionComponentValue)
		{
			m_rightTree->SearchNearestValue(x, z, depth + 1, minDistanceValues, minDistances);
		}
		else
		{
			//DELOCUST_LOG_MATH("Right tree of node (", m_position.ToString(), ") pruned");		
		}
	}
}


math::Real math::KdTree::Interpolate(std::vector<Real>& minDistanceValues, std::vector<Real>& minDistances) const
{
	/* ==================== METHOD #1 (Inverse Distance Weighting- http://www.gitta.info/ContiSpatVar/en/html/Interpolatio_learningObject2.xhtml) begin ==================== */
	//const Real exponent = 4.0f;
	if (AlmostEqual(minDistances[0], REAL_ZERO))
	{
		STOP_PROFILING_MATH("");
		return minDistanceValues[0];
	}
	auto sumOfInversedDistances = REAL_ZERO;
	for (auto i = 0; i < m_numberOfSamples; ++i)
	{
		sumOfInversedDistances += REAL_ONE / minDistances[i];
		//sumOfInversedDistances += REAL_ONE / pow(minDistances[i], exponent);
	}
	auto result = REAL_ZERO;
	for (auto i = 0; i < m_numberOfSamples; ++i)
	{
		result += minDistanceValues[i] / minDistances[i];
		//result += minDistanceValues[i] / pow(minDistances[i], exponent);
	}
	result /= sumOfInversedDistances;
	STOP_PROFILING_MATH("");
	return result;
	/* ==================== METHOD #1 end ==================== */

	/* ==================== METHOD #2 begin ==================== */
	// TODO: Create a polygon of m_numberOfSamples vertices. Then find the barycentric coordinates

	//if (AlmostEqual(minDistances[0], REAL_ZERO))
	//{
	//	STOP_PROFILING_MATH("");
	//	return minDistanceValues[0];
	//}
	//Triangle t(Vector3D(-8, -2, 8), Vector3D(-8, -1, -8), Vector3D(8, 1, 8));
	//t.
	//STOP_PROFILING_MATH("");
	//return result;
	/* ==================== METHOD #2 end ==================== */
}

std::string math::KdTree::ToString() const
{
	return ToString(0);
}

std::string math::KdTree::ToString(int depth) const
{
	constexpr char* indentationString = "  ";
	std::stringstream s("");
	for (auto i = 0; i < depth; ++i)
	{
		s << indentationString;
	}
	s << "Parent: " << m_position << " with value " << m_value << " has following children:" << std::endl;
	for (auto i = 0; i < depth + 1; ++i)
	{
		s << indentationString;
	}
	if (m_leftTree == nullptr)
	{
		//s << "\t1. NULL" << std::endl;
		s << "1. NULL" << std::endl;
	}
	else
	{
		//s << "\t1. " << m_leftTree->GetPosition().ToString() << " with value " << m_leftTree->GetValue() << std::endl;
		s << "1. " << m_leftTree->ToString(depth + 1) << std::endl;
	}
	for (auto i = 0; i < depth + 1; ++i)
	{
		s << indentationString;
	}
	if (m_rightTree == nullptr)
	{
		//s << "\t2. NULL" << std::endl;
		s << "2. NULL" << std::endl;
	}
	else
	{
		s << "2. " << m_rightTree->ToString(depth + 1) << std::endl;
		//s << "\t2. " << m_rightTree->GetPosition().ToString() << " with value " << m_rightTree->GetValue() << std::endl;
	}

	//if (m_leftTree != nullptr)
	//{
	//	s << m_leftTree->ToString();
	//}
	//if (m_rightTree != nullptr)
	//{
	//	s << m_rightTree->ToString();
	//}

	return s.str();
}
