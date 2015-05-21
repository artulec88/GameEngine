#include "stdafx.h"
#include "KDTree.h"
#include "ISort.h"
#include "SortingParameters.h"
#include <sstream>

using namespace Math;

KDTree::KDTree(Math::Vector3D* positions, int positionsCount, int numberOfSamples /* = 1 */) :
	m_leftTree(NULL),
	m_rightTree(NULL),
	m_numberOfSamples(numberOfSamples),
	m_minDistancePositions(NULL),
	m_minDistances(NULL)
#ifdef CALCULATE_MATH_STATS
	,m_classStats(STATS_STORAGE.GetClassStats("KDTree"))
#endif
{
	START_PROFILING;
	m_minDistancePositions = new Vector3D[m_numberOfSamples];
	m_minDistances = new Real[m_numberOfSamples];
	BuildTree(positions, positionsCount, 0);
	STOP_PROFILING;
}

KDTree::KDTree(Math::Vector3D* positions, int positionsCount, int numberOfSamples, int depth) :
	m_leftTree(NULL),
	m_rightTree(NULL),
	m_numberOfSamples(numberOfSamples),
	m_minDistancePositions(NULL),
	m_minDistances(NULL)
#ifdef CALCULATE_MATH_STATS
	,m_classStats(STATS_STORAGE.GetClassStats("KDTree"))
#endif
{
	START_PROFILING;
	//m_minDistancePositions = new Vector3D[m_numberOfSamples];
	//m_minDistances = new Real[m_numberOfSamples];
	BuildTree(positions, positionsCount, depth);
	STOP_PROFILING;
}

KDTree::~KDTree(void)
{
	SAFE_DELETE_JUST_TABLE(m_minDistancePositions);
	SAFE_DELETE_JUST_TABLE(m_minDistances);
	SAFE_DELETE(m_leftTree);
	SAFE_DELETE(m_rightTree);
}

void KDTree::BuildTree(Math::Vector3D* positions, int positionsCount, int depth)
{
	START_PROFILING;
	Sorting::SortingKey sortingKey;
	switch (depth % 2)
	{
	case 0:
		sortingKey = Sorting::COMPONENT_X;
		break;
	case 1:
		sortingKey = Sorting::COMPONENT_Z;
		break;
	default: // shouldn't occur at all
		LOG(Utility::Warning, LOGPLACE, "Cannot determine the sorting key for the process of constructing the k-d tree");
		sortingKey = Sorting::COMPONENT_X;
		break;
	}
	//LOG(Utility::Debug, LOGPLACE, "Before sorting: depth = %d", depth);
	//for (int i = 0; i < positionsCount; ++i)
	//{
	//	LOG(Utility::Debug, LOGPLACE, "depth = %d) positions[%d] = %s", depth, i, positions[i].ToString().c_str());
	//}
	Sorting::SortingParametersChain sortingParameters(sortingKey, Sorting::ASCENDING);
	Sorting::ISort::GetSortingObject(Sorting::MERGE_SORT)->Sort(positions, positionsCount, sortingParameters);
	//LOG(Utility::Debug, LOGPLACE, "After sorting: depth = %d", depth);
	//for (int i = 0; i < positionsCount; ++i)
	//{
	//	LOG(Utility::Debug, LOGPLACE, "depth = %d) positions[%d] = %s", depth, i, positions[i].ToString().c_str());
	//}

	int medianIndex = positionsCount / 2;
	m_position = Vector2D(positions[medianIndex].GetXZ());
	m_value = positions[medianIndex].GetY();

	if (medianIndex > 0)
	{
		m_leftTree = new KDTree(positions, medianIndex, m_numberOfSamples, depth + 1);
	}
	if (positionsCount - medianIndex - 1 > 0)
	{
		m_rightTree = new KDTree(&positions[medianIndex + 1], positionsCount - medianIndex - 1, m_numberOfSamples, depth + 1);
	}
	STOP_PROFILING;
}

//int numberOfPositionsChecked = 0; // just to measure how many nodes have actually been checked / visited during the search for the nearest positions

Real KDTree::SearchNearestValue(const Vector2D& position) const
{
	START_PROFILING;
	// The numberOfSamples must be less than the number of nodes. We assume that it is.
	// If we wanted to check that condition we would have to store the number of nodes in the separate member variable.
	if (m_minDistancePositions == NULL)
	{
		LOG(Utility::Emergency, LOGPLACE, "The table for storing the nearest positions is NULL. Creating a new one.");
	}
	if (m_minDistances == NULL)
	{
		LOG(Utility::Emergency, LOGPLACE, "The table for storing the minimum distances is NULL. Creating a new one.");
	}
	for (int i = 0; i < m_numberOfSamples; ++i)
	{
		//minDistancePositions[i] = 
		m_minDistances[i] = REAL_MAX;
	}
	
	//numberOfPositionsChecked = 0;
	SearchNearestValue(position, 0, m_minDistancePositions, m_minDistances);
	//LOG(Utility::Debug, LOGPLACE, "Number of positions checked during the search for the nearest positions = %d", numberOfPositionsChecked);

	//return minDistancePositions[0].GetY(); // return the nearest position's Y component

	// Calculate the weighted average where weights are in inverse proportion to distances
	// See: http://en.wikipedia.org/wiki/Inverse_distance_weighting
	// TODO: Create different strategies (strategy pattern) for weighted average calculation
	// TODO: Measure which method (#1 or #2) is faster
	/* ==================== METHOD #1 begin ==================== */
	if (AlmostEqual(m_minDistances[0], REAL_ZERO))
	{
		STOP_PROFILING;
		return m_minDistancePositions[0].GetY();
	}
	Real sumOfDistances = REAL_ZERO;
	for (int i = 0; i < m_numberOfSamples; ++i)
	{
		sumOfDistances += m_minDistances[i];
	}
	Real result = REAL_ZERO;
	Real sumOfWeights = REAL_ZERO;
	for (int i = 0; i < m_numberOfSamples; ++i)
	{
		Real weight = sumOfDistances / m_minDistances[i];
		sumOfWeights += weight;
		result += m_minDistancePositions[i].GetY() * weight;
	}
	result /= sumOfWeights;
	STOP_PROFILING;
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

void KDTree::SearchNearestValue(const Vector2D& position, int depth, Vector3D* minDistancePositions, Real* minDistances) const
{
	START_PROFILING;
	//++numberOfPositionsChecked;
	LOG(Utility::Debug, LOGPLACE, "Visiting the node with position (%s) and value %.2f", m_position.ToString().c_str(), m_value);
	Real distance = (position - m_position).LengthSquared();
	int j = m_numberOfSamples - 1;
	while ( (j >= 0) && (minDistances[j] > distance) )
	{
		if (j + 1 < m_numberOfSamples)
		{
			minDistances[j + 1] = minDistances[j];
			minDistancePositions[j + 1] = minDistancePositions[j];
		}
		minDistances[j] = distance;
		minDistancePositions[j] = Math::Vector3D(m_position.GetX(), m_value, m_position.GetY());
		--j;
	}
	//for (int i = 0; i < numberOfSamples; ++i)
	//{
	//	LOG(Utility::Info, LOGPLACE, "minDistancePositions[%d] = (%s)\tminDistances[%d] = %.2f", i, minDistancePositions[i].ToString().c_str(), i, minDistances[i]);
	//}
	if (IsLeaf())
	{
		LOG(Utility::Debug, LOGPLACE, "The node with position (%s) and value %.2f is a leaf", m_position.ToString().c_str(), m_value);
		STOP_PROFILING;
		return;
	}
	//switch (depth % 2)
	//{
	//case 0:
	//	sortingKey = COMPONENT_X;
	//	break;
	//case 1:
	//	sortingKey = COMPONENT_Z;
	//	break;
	//default: // shouldn't occur at all
	//	LOG(Utility::Warning, LOGPLACE, "Cannot determine the sorting key for the process of constructing the k-d tree");
	//	sortingKey = COMPONENT_X;
	//	break;
	//}

	Real positionComponentValue = ((depth % 2) == 0) ? position.GetX() : position.GetY();
	Real nodePositionComponentValue = ((depth % 2) == 0) ? m_position.GetX() : m_position.GetY();
	bool searchLeftTreeFirst = positionComponentValue < nodePositionComponentValue; // false means we will search the right tree first

	if (searchLeftTreeFirst)
	{
		if (((m_leftTree != NULL)) && (positionComponentValue - minDistances[m_numberOfSamples - 1] <= nodePositionComponentValue))
		{
			m_leftTree->SearchNearestValue(position, depth + 1, minDistancePositions, minDistances);
		}
		else if (m_leftTree != NULL)
		{
			LOG(Utility::Delocust, LOGPLACE, "Left tree of node (%s) pruned", m_position.ToString().c_str());
		}
		if ((m_rightTree != NULL) && ((positionComponentValue + minDistances[m_numberOfSamples - 1]) > nodePositionComponentValue))
		{
			m_rightTree->SearchNearestValue(position, depth + 1, minDistancePositions, minDistances);
		}
		else if (m_rightTree != NULL)
		{
			LOG(Utility::Delocust, LOGPLACE, "Right tree of node (%s) pruned", m_position.ToString().c_str());
		}
	}
	else
	{
		if ((m_rightTree != NULL) && ((positionComponentValue + minDistances[m_numberOfSamples - 1]) > nodePositionComponentValue))
		{
			m_rightTree->SearchNearestValue(position, depth + 1, minDistancePositions, minDistances);
		}
		else if (m_rightTree != NULL)
		{
			LOG(Utility::Delocust, LOGPLACE, "Right tree of node (%s) pruned", m_position.ToString().c_str());
		}
		if ((m_leftTree != NULL) && ((positionComponentValue - minDistances[m_numberOfSamples - 1]) <= nodePositionComponentValue))
		{
			m_leftTree->SearchNearestValue(position, depth + 1, minDistancePositions, minDistances);
		}
		else if (m_leftTree != NULL)
		{
			LOG(Utility::Delocust, LOGPLACE, "Left tree of node (%s) pruned", m_position.ToString().c_str());
		}
	}
	STOP_PROFILING;
}

std::string KDTree::ToString() const
{
	return ToString(0);
}

std::string KDTree::ToString(int depth) const
{
	std::stringstream s("");
	for (int i = 0; i < depth; ++i)
	{
		s << "  "; // TODO: create a const static field representing the indentation string
	}
	s << "Parent: " << m_position.ToString() << " with value " << m_value << " has following children:" << std::endl;
	for (int i = 0; i < depth + 1; ++i)
	{
		s << "  "; // TODO: create a const static field representing the indentation string
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
		s << "  "; // TODO: create a const static field representing the indentation string
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
