#include "stdafx.h"
#include "KDTree.h"
#include "ISort.h"
#include "SortingParameters.h"
#include <sstream>
#include "Utility\ILogger.h"

Math::KDTree::KDTree(Math::Vector3D* positions, size_t positionsCount, int numberOfSamples /* = 1 */, int depth /* = 0 */) :
	m_leftTree(nullptr),
	m_rightTree(nullptr),
	m_numberOfSamples(numberOfSamples),
	m_position(REAL_ZERO, REAL_ZERO),
	m_value(REAL_ZERO)
#ifdef PROFILING_MATH_MODULE_ENABLED
	,m_classStats(STATS_STORAGE.GetClassStats("KDTree"))
#endif
{
	START_PROFILING_MATH(true, "");
	CHECK_CONDITION_EXIT_MATH(positions != nullptr, Utility::Logging::EMERGENCY, "Cannot transform the positions. The positions array is NULL.");
	BuildTree(positions, positionsCount, depth);
	STOP_PROFILING_MATH("");
}

Math::KDTree::~KDTree(void)
{
}

void Math::KDTree::BuildTree(Math::Vector3D* positions, size_t positionsCount, int depth)
{
	START_PROFILING_MATH(true, "");
	Sorting::SortingKey sortingKey = (depth % 2 == 0) ? Sorting::COMPONENT_X : Sorting::COMPONENT_Z;
	//DEBUG_LOG_MATH("Before sorting: depth = ", depth);
	//for (int i = 0; i < positionsCount; ++i)
	//{
	//	DEBUG_LOG_MATH("depth = ", depth, ") positions[", i, "] = ", positions[i].ToString());
	//}
	Sorting::SortingParametersChain sortingParameters(sortingKey, Sorting::ASCENDING);
	Sorting::ISort::GetSortingObject(Sorting::SortingAlgorithms::MERGE_SORT)->Sort(positions, positionsCount, sortingParameters);
	//DEBUG_LOG_MATH("After sorting: depth = ", depth);
	//for (int i = 0; i < positionsCount; ++i)
	//{
	//	DEBUG_LOG_MATH("depth = ", depth, ") positions[", i, "] = ", positions[i].ToString());
	//}

	const size_t medianIndex = positionsCount / 2;
	m_position = positions[medianIndex].GetXZ();
	m_value = positions[medianIndex].y;

	if (medianIndex > 0)
	{
		m_leftTree = std::make_unique<KDTree>(positions, medianIndex, m_numberOfSamples, depth + 1);
	}
	if (positionsCount - medianIndex - 1 > 0)
	{
		m_rightTree = std::make_unique<KDTree>(&positions[medianIndex + 1], positionsCount - medianIndex - 1, m_numberOfSamples, depth + 1);
	}
	STOP_PROFILING_MATH("");
}

//int numberOfPositionsChecked = 0; // just to measure how many nodes have actually been checked / visited during the search for the nearest positions

Math::Real Math::KDTree::SearchNearestValue(Math::Real posX, Math::Real posZ) const
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
	/* ==================== METHOD #1 begin ==================== */
	if (AlmostEqual(minDistances[0], REAL_ZERO))
	{
		STOP_PROFILING_MATH("");
		return minDistanceValues[0];
	}
	Real sumOfDistances = REAL_ZERO;
	for (int i = 0; i < m_numberOfSamples; ++i)
	{
		sumOfDistances += minDistances[i];
	}
	Real result = REAL_ZERO;
	Real sumOfWeights = REAL_ZERO;
	for (int i = 0; i < m_numberOfSamples; ++i)
	{
		Real weight = sumOfDistances / minDistances[i];
		sumOfWeights += weight;
		result += minDistanceValues[i] * weight;
	}
	result /= sumOfWeights;
	STOP_PROFILING_MATH("");
	return result;
	/* ==================== METHOD #1 end ==================== */
	
	/* ==================== METHOD #2 begin ==================== */
//	if (AlmostEqual(m_minDistances[0], REAL_ZERO))
//	{
//		return m_minDistancePositions[0].GetY();
//	}
//	Real sumOfInversedDistances = REAL_ZERO;
//	for (int i = 0; i < m_numberOfSamples; ++i)
//	{
//		sumOfInversedDistances += REAL_ONE / m_minDistances[i];
//	}
//	Real result = REAL_ZERO;
//	for (int i = 0; i < m_numberOfSamples; ++i)
//	{
//		result += m_minDistancePositions[i].GetY() / m_minDistances[i];
//	}
//	result /= sumOfInversedDistances;
//	return result;
	/* ==================== METHOD #2 end ==================== */
}

void Math::KDTree::SearchNearestValue(Math::Real x, Math::Real z, int depth, std::vector<Real>& minDistanceValues, std::vector<Real>& minDistances) const
{
	START_PROFILING_MATH(true, "");
	//++numberOfPositionsChecked;
	//DELOCUST_LOG_MATH("Visiting the node with position (", m_position.ToString(), ") and value ", m_value);
	Real distance = (x - m_position.x) * (x - m_position.x) + (z - m_position.y) * (z - m_position.y);
	int j = m_numberOfSamples - 1;
	while ( (j >= 0) && (minDistances[j] > distance) )
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

	Real positionComponentValue = ((depth % 2) == 0) ? x : z;
	Real nodePositionComponentValue = ((depth % 2) == 0) ? m_position.x : m_position.y;
	bool searchLeftTreeFirst = positionComponentValue < nodePositionComponentValue; // false means we will search the right tree first

	if (searchLeftTreeFirst)
	{
		if (((m_leftTree != NULL)) && (positionComponentValue - minDistances[m_numberOfSamples - 1] <= nodePositionComponentValue))
		{
			m_leftTree->SearchNearestValue(x, z, depth + 1, minDistanceValues, minDistances);
		}
		else if (m_leftTree != NULL)
		{
			//DELOCUST_LOG_MATH("Left tree of node (", m_position.ToString(), ") pruned");
		}
		if ((m_rightTree != NULL) && ((positionComponentValue + minDistances[m_numberOfSamples - 1]) > nodePositionComponentValue))
		{
			m_rightTree->SearchNearestValue(x, z, depth + 1, minDistanceValues, minDistances);
		}
		else if (m_rightTree != NULL)
		{
			//DELOCUST_LOG_MATH("Right tree of node (", m_position.ToString(), ") pruned");
		}
	}
	else
	{
		if ((m_rightTree != NULL) && ((positionComponentValue + minDistances[m_numberOfSamples - 1]) > nodePositionComponentValue))
		{
			m_rightTree->SearchNearestValue(x, z, depth + 1, minDistanceValues, minDistances);
		}
		else if (m_rightTree != NULL)
		{
			//DELOCUST_LOG_MATH("Right tree of node (", m_position.ToString(), ") pruned");
		}
		if ((m_leftTree != NULL) && ((positionComponentValue - minDistances[m_numberOfSamples - 1]) <= nodePositionComponentValue))
		{
			m_leftTree->SearchNearestValue(x, z, depth + 1, minDistanceValues, minDistances);
		}
		else if (m_leftTree != NULL)
		{
			//DELOCUST_LOG_MATH("Left tree of node (", m_position.ToString(), ") pruned");
		}
	}
	STOP_PROFILING_MATH("");
}

std::string Math::KDTree::ToString() const
{
	return ToString(0);
}

std::string Math::KDTree::ToString(int depth) const
{
	constexpr char* INDENTATION_STRING = "  ";
	std::stringstream s("");
	for (int i = 0; i < depth; ++i)
	{
		s << INDENTATION_STRING;
	}
	s << "Parent: " << m_position << " with value " << m_value << " has following children:" << std::endl;
	for (int i = 0; i < depth + 1; ++i)
	{
		s << INDENTATION_STRING;
	}
	if (m_leftTree == NULL)
	{
		//s << "\t1. NULL" << std::endl;
		s << "1. NULL" << std::endl;
	}
	else
	{
		//s << "\t1. " << m_leftTree->GetPosition().ToString() << " with value " << m_leftTree->GetValue() << std::endl;
		s << "1. " << m_leftTree->ToString(depth + 1) << std::endl;
	}
	for (int i = 0; i < depth + 1; ++i)
	{
		s << INDENTATION_STRING;
	}
	if (m_rightTree == NULL)
	{
		//s << "\t2. NULL" << std::endl;
		s << "2. NULL" << std::endl;
	}
	else
	{
		s << "2. " << m_rightTree->ToString(depth + 1) << std::endl;
		//s << "\t2. " << m_rightTree->GetPosition().ToString() << " with value " << m_rightTree->GetValue() << std::endl;
	}

	//if (m_leftTree != NULL)
	//{
	//	s << m_leftTree->ToString();
	//}
	//if (m_rightTree != NULL)
	//{
	//	s << m_rightTree->ToString();
	//}

	return s.str();	
}
