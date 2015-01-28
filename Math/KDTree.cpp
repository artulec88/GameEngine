#include "stdafx.h"
#include "KDTree.h"
#include "ISort.h"
#include "SortingParameters.h"
#include <sstream>

using namespace Math;

KDTree::KDTree(Math::Vector3D* positions, int positionsCount) :
	m_leftTree(NULL),
	m_rightTree(NULL)
{
	BuildTree(positions, positionsCount, 0);
}

KDTree::KDTree(Math::Vector3D* positions, int positionsCount, int depth) :
	m_leftTree(NULL),
	m_rightTree(NULL)
{
	BuildTree(positions, positionsCount, depth);
}

KDTree::~KDTree(void)
{
	SAFE_DELETE(m_leftTree);
	SAFE_DELETE(m_rightTree);
}

void KDTree::BuildTree(Math::Vector3D* positions, int positionsCount, int depth)
{
	SortingKey sortingKey;
	switch (depth % 2)
	{
	case 0:
		sortingKey = COMPONENT_X;
		break;
	case 1:
		sortingKey = COMPONENT_Z;
		break;
	default: // shouldn't occur at all
		LOG(Utility::Warning, LOGPLACE, "Cannot determine the sorting key for the process of constructing the k-d tree");
		sortingKey = COMPONENT_X;
		break;
	}
	LOG(Utility::Notice, LOGPLACE, "Before sorting: depth = %d", depth);
	for (int i = 0; i < positionsCount; ++i)
	{
		LOG(Utility::Info, LOGPLACE, "depth = %d) positions[%d] = %s", depth, i, positions[i].ToString().c_str());
	}
	SortingParametersChain sortingParameters(sortingKey, ASCENDING);
	ISort::GetSortingObject(ISort::MERGE_SORT)->Sort(positions, positionsCount, sortingParameters);
	LOG(Utility::Notice, LOGPLACE, "After sorting: depth = %d", depth);
	for (int i = 0; i < positionsCount; ++i)
	{
		LOG(Utility::Info, LOGPLACE, "depth = %d) positions[%d] = %s", depth, i, positions[i].ToString().c_str());
	}

	int medianIndex = positionsCount / 2;
	m_position = Vector2D(positions[medianIndex].GetXZ());
	m_value = positions[medianIndex].GetY();

	if (medianIndex > 0)
	{
		m_leftTree = new KDTree(positions, medianIndex, depth + 1);
	}
	if (positionsCount - medianIndex - 1 > 0)
	{
		m_rightTree = new KDTree(&positions[medianIndex + 1], positionsCount - medianIndex - 1, depth + 1);
	}
}

Real KDTree::SearchNearestValue(const Vector2D& position, Vector3D& minDistancePosition, Real& minDistance /*, int numberOfSamples */) const
{
	minDistance = REAL_MAX;
	SearchNearestValue(position, 0, minDistancePosition, minDistance);
	return minDistancePosition.GetY();
}

void KDTree::SearchNearestValue(const Vector2D& position, int depth, Vector3D& minDistancePosition, Real& minDistance /*, int numberOfSamples */) const
{
	// TODO: Finish this function
	Real distance = (position - m_position).LengthSquared();
	if (minDistance > distance)
	{
		minDistancePosition = Math::Vector3D(m_position.GetX(), m_value, m_position.GetY());
		minDistance = distance;
		//if (AlmostEqual(minDistance, REAL_ZERO))
		//{
		//	return minDistance;
		//}
	}
	if (HasChildren())
	{
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
			if (((m_leftTree != NULL)) && (positionComponentValue - minDistance <= nodePositionComponentValue))
			{
				m_leftTree->SearchNearestValue(position, depth + 1, minDistancePosition, minDistance);
			}
			else if (m_leftTree != NULL)
			{
				LOG(Utility::Debug, LOGPLACE, "Left tree of node (%s) pruned", m_position.ToString().c_str());
			}
			if ((m_rightTree != NULL) && ((positionComponentValue + minDistance) > nodePositionComponentValue))
			{
				m_rightTree->SearchNearestValue(position, depth + 1, minDistancePosition, minDistance);
			}
			else if (m_rightTree != NULL)
			{
				LOG(Utility::Debug, LOGPLACE, "Right tree of node (%s) pruned", m_position.ToString().c_str());
			}
		}
		else
		{
			if ((m_rightTree != NULL) && ((positionComponentValue + minDistance) > nodePositionComponentValue))
			{
				m_rightTree->SearchNearestValue(position, depth + 1, minDistancePosition, minDistance);
			}
			else if (m_rightTree != NULL)
			{
				LOG(Utility::Debug, LOGPLACE, "Right tree of node (%s) pruned", m_position.ToString().c_str());
			}
			if ((m_leftTree != NULL) && ((positionComponentValue - minDistance) <= nodePositionComponentValue))
			{
				m_leftTree->SearchNearestValue(position, depth + 1, minDistancePosition, minDistance);
			}
			else if (m_leftTree != NULL)
			{
				LOG(Utility::Debug, LOGPLACE, "Left tree of node (%s) pruned", m_position.ToString().c_str());
			}
		}
	}
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
